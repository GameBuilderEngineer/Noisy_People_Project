//===================================================================================================================================
//【EffekseerManager.cpp】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/10/18
// [更新日]2019/10/21
//===================================================================================================================================

//===================================================================================================================================
//【インクルード】
//===================================================================================================================================
#include "EffekseerManager.h"
#include "Direct3D9.h"

//===================================================================================================================================
//【using宣言】
//===================================================================================================================================
using namespace effekseerNS;

//===================================================================================================================================
//【グローバル変数】
//===================================================================================================================================
static EffekseerManager* pointerEffekseerManager[MANAGER_NUM];

static int count = 0;

//===================================================================================================================================
//【コンストラクタ】
//===================================================================================================================================
EffekseerManager::EffekseerManager()
{
	pointerEffekseerManager[count] = this;
	count++;

	manager		= NULL;
	renderer	= NULL;
#if(XADUIO2_STATE)
	sound		= NULL;	
	xa2			= NULL;
	xa2Master	= NULL;
#endif

	fileName[BLOW]				= { L"blow.efk" };
	fileName[MUZZLE]			= { L"muzzle.efk" };
	fileName[DIGIT_TREE]		= { L"Digit_Tree.efk" };
	fileName[DIGIT_TREE_SELECT]	= { L"Digit_Tree_Select_Zoffset+1.efk" };
	fileName[DIGIT_TREE_RAID]	= { L"Digit_Tree_Select.efk" };
	fileName[DAC]				= { L"DAC.efk" };
	fileName[GREENING]			= { L"Greening.efk" };
	fileName[DIGIT_MODE]		= { L"Digit_mode.efk" };
	fileName[DROP_ITEM]			= { L"Drop_Item.efk" };

	instanceList = new LinkedList<::effekseerNS::Instance*>;
}

//===================================================================================================================================
//【デストラクタ】
//===================================================================================================================================
EffekseerManager::~EffekseerManager()
{
	uninitialize();
}

//===================================================================================================================================
//【初期化】
//===================================================================================================================================
void EffekseerManager::initialize()
{
#if(XADUIO2_STATE)
	// XAudio2の初期化を行う
	XAudio2Create(&xa2);
	xa2->CreateMasteringVoice(&xa2Master);
#endif

	// 描画用インスタンスの生成
	renderer = ::EffekseerRendererDX9::Renderer::Create(getDevice(), 20000);

	// エフェクト管理用インスタンスの生成
	manager = ::Effekseer::Manager::Create(20000);

	// 描画用インスタンスから描画機能を設定
	manager->SetSpriteRenderer(renderer->CreateSpriteRenderer());
	manager->SetRibbonRenderer(renderer->CreateRibbonRenderer());
	manager->SetRingRenderer(renderer->CreateRingRenderer());
	manager->SetTrackRenderer(renderer->CreateTrackRenderer());
	manager->SetModelRenderer(renderer->CreateModelRenderer());

	// 描画用インスタンスからテクスチャの読込機能を設定
	// 独自拡張可能、現在はファイルから読み込んでいる。
	manager->SetTextureLoader(renderer->CreateTextureLoader());
	manager->SetModelLoader(renderer->CreateModelLoader());

#if(XADUIO2_STATE)
	// 音再生用インスタンスの生成
	sound = ::EffekseerSound::Sound::Create(xa2, 16, 16);
	// 音再生用インスタンスから再生機能を指定
	manager->SetSoundPlayer(sound->CreateSoundPlayer());
	// 音再生用インスタンスからサウンドデータの読込機能を設定
	// 独自拡張可能、現在はファイルから読み込んでいる。
	manager->SetSoundLoader(sound->CreateSoundLoader());
#endif


	//座標系の指定(LH：左手系）
	manager->SetCoordinateSystem(Effekseer::CoordinateSystem::LH);

	// 視点位置を確定
	position = ::Effekseer::Vector3D(10.0f, 5.0f, 20.0f);

	// カリングを行う範囲を設定
	//manager->CreateCullingWorld(10000.0f, 10000.0f, 10000.0f, 6);

	//エフェクシアーディレクトリへカレント
	setEffekseerDirectory();

	// エフェクトの読込
	for (int i = 0; i < MAX_EFFEKSEER; i++)
	{
		effect[i] = Effekseer::Effect::Create(manager, (const EFK_CHAR*)fileName[i]);
	}

}

//===================================================================================================================================
//【レンダラーの投影行列を設定】
//===================================================================================================================================
void EffekseerManager::setProjectionMatrix(float fov, float windowWidth, float windowHeight, float zn, float zf)
{
	renderer->SetProjectionMatrix(
		::Effekseer::Matrix44().PerspectiveFovLH(fov, (float)windowWidth / (float)windowHeight, zn, zf));
}

//===================================================================================================================================
//【レンダラーのカメラ行列を設定】
//===================================================================================================================================
void EffekseerManager::setCameraMatrix(D3DXVECTOR3 position,D3DXVECTOR3 eye,D3DXVECTOR3 up)
{
	this->position	= ::Effekseer::Vector3D(position.x, position.y, position.z);
	this->eye		= ::Effekseer::Vector3D(eye.x, eye.y, eye.z);
	this->up		= ::Effekseer::Vector3D(up.x, up.y, up.z);

	renderer->SetCameraMatrix(
		::Effekseer::Matrix44().LookAtLH(this->position, this->eye, this->up));
}

//===================================================================================================================================
//【再生】
//===================================================================================================================================
effekseerNS::Instance* EffekseerManager::play(effekseerNS::Instance* instance)
{
	instance->handle = manager->Play(
		effect[instance->effectNo], 
		instance->position.x,
		instance->position.y,
		instance->position.z);
	instanceList->insertFront(instance);
	instance->nodePointer = instanceList->getFrontNode();
	return instance;
}

//===================================================================================================================================
//【一時停止/再度再生：全て】
//===================================================================================================================================
void EffekseerManager::pause(bool flag)
{
	manager->SetPausedToAllEffects(flag);
}

//===================================================================================================================================
//【一時停止/再度再生：1ハンドル】
//===================================================================================================================================
void EffekseerManager::pause(::Effekseer::Handle handle,bool flag )
{
	manager->SetPaused(handle, flag);
}

//===================================================================================================================================
//【全停止】
//===================================================================================================================================
void EffekseerManager::stop()
{
	manager->StopAllEffects();
}

//===================================================================================================================================
//【停止：1ハンドル】
//===================================================================================================================================
void EffekseerManager::stop(::Effekseer::Handle handle)
{
	manager->StopEffect(handle);
}

//===================================================================================================================================
//【停止：１インスタンス】
//===================================================================================================================================
void EffekseerManager::stop(::effekseerNS::Instance* instance)
{
	manager->StopEffect(instance->handle);
	instanceList->remove(instance->nodePointer);
}

//===================================================================================================================================
//【終了処理】
//===================================================================================================================================
void EffekseerManager::uninitialize()
{
	manager->StopAllEffects();
	for (int i = 0; i < MAX_EFFEKSEER;i++)
	{
		// エフェクトの破棄
		ES_SAFE_RELEASE(effect[i]);
		// エフェクトの停止
	}

	// 先にエフェクト管理用インスタンスを破棄
	manager->Destroy();


	// 次に描画用インスタンスを破棄
	renderer->Destroy();

#if(XADUIO2_STATE)
	// 次に音再生用インスタンスを破棄
	sound->Destroy();
	// XAudio2の解放
	if (xa2Master != NULL)
	{
		xa2Master->DestroyVoice();
		xa2Master = NULL;
	}
	ES_SAFE_RELEASE(xa2);
#endif

	for (int i = 0;i <instanceList->nodeNum; i++)
	{
		SAFE_DELETE(*instanceList->getValue(i));
	}
	instanceList->terminate();
	SAFE_DELETE(instanceList);
}

//===================================================================================================================================
//【更新】
//===================================================================================================================================
void EffekseerManager::update()
{
	//リスト更新
	instanceList->listUpdate();

	//削除処理
	int nodeNum = instanceList->nodeNum;
	for (int i = 0; i < nodeNum; i++)
	{
		::Effekseer::Handle handle = (*instanceList->getValue(i))->handle;
		if (!manager->Exists(handle))
		{
			SAFE_DELETE((*instanceList->getValue(i)));
			instanceList->remove(instanceList->getNode(i));
		}
	}

	//リスト更新
	instanceList->listUpdate();

	// エフェクトの更新処理を行う
	for (int i = 0; i < instanceList->nodeNum; i++)
	{
		(*instanceList->getValue(i))->update();
	}

	// エフェクトの更新処理を行う
	manager->Update();
}

//===================================================================================================================================
//【描画開始】
//===================================================================================================================================
void EffekseerManager::begin()
{
	renderer->BeginRendering();
}

//===================================================================================================================================
//【描画開始】
//===================================================================================================================================
void EffekseerManager::end()
{
	renderer->EndRendering();
}

//===================================================================================================================================
//【描画】
//===================================================================================================================================
void EffekseerManager::render()
{
	//manager->CalcCulling(renderer->GetCameraProjectionMatrix(), false);//カリングの計算
	begin();
	manager->Draw();
	end();
}

//===================================================================================================================================
//【外部参照：再生】
//===================================================================================================================================
effekseerNS::Instance*  effekseerNS::play(int no, effekseerNS::Instance* instance)
{
	return pointerEffekseerManager[no]->play(instance);
}

//===================================================================================================================================
//【外部参照：全停止】
//===================================================================================================================================
void effekseerNS::stop(int no)
{
	pointerEffekseerManager[no]->stop();
}

//===================================================================================================================================
//【外部参照：ハンドル別停止】
//===================================================================================================================================
void effekseerNS::stop(int no, ::Effekseer::Handle handle)
{
	pointerEffekseerManager[no]->stop(handle);
}

//===================================================================================================================================
//【外部参照：インスタンス別停止】
//===================================================================================================================================
void effekseerNS::stop(int no, ::effekseerNS::Instance* instance)
{
	pointerEffekseerManager[no]->stop(instance);
}

//===================================================================================================================================
//【外部参照：プロジェクション行列設定】
//===================================================================================================================================
void effekseerNS::setProjectionMatrix(int no, float fov, float windowWidth, float windowHeight, float zn, float zf)
{
	pointerEffekseerManager[no]->setProjectionMatrix(fov, windowWidth, windowHeight, zn, zf);
}

//===================================================================================================================================
//【外部参照：カメラ(ビュー)行列設定】
//===================================================================================================================================
void effekseerNS::setCameraMatrix(int no, D3DXVECTOR3 position, D3DXVECTOR3 eye, D3DXVECTOR3 up)
{
	pointerEffekseerManager[no]->setCameraMatrix(position, eye, up);
}

//===================================================================================================================================
//【外部参照：描画】
//===================================================================================================================================
void effekseerNS::render(int no)
{
	pointerEffekseerManager[no]->render();
}

//===================================================================================================================================
//【外部参照：一時停止】
//===================================================================================================================================
void effekseerNS::pause(int no, bool flag)
{
	pointerEffekseerManager[no]->pause(flag);
}

//===================================================================================================================================
//【外部参照：一時停止】
//===================================================================================================================================
void effekseerNS::pause(int no, ::Effekseer::Handle handle, bool flag)
{
	pointerEffekseerManager[no]->pause(handle, flag);
}

//===================================================================================================================================
//【外部参照：マネージャー取得】
//===================================================================================================================================
EffekseerManager* getEffekseerManager(int no)
{
	return pointerEffekseerManager[no];
}

//===================================================================================================================================
//【インスタンス：更新】
//===================================================================================================================================
void effekseerNS::Instance::update()
{
	::Effekseer::Manager*	manager	= pointerEffekseerManager[managerNo]->manager;
	position	 += speed;
	rotation	 += deltaRadian;
	scale		 += deltaScale;
	manager->SetLocation(handle, position.x,position.y,position.z);
	manager->SetRotation(handle,rotation.x,rotation.y,rotation.z);
	manager->SetScale(handle,scale.x,scale.y,scale.z);
}

//===================================================================================================================================
//【インスタンス：更新】
//===================================================================================================================================
void effekseerNS::Instance::setShown(bool shown)
{
	::Effekseer::Manager*	manager = pointerEffekseerManager[managerNo]->manager;
	manager->SetShown(handle, shown);
}
