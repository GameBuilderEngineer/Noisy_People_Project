//===================================================================================================================================
//【Fader.h】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/09/20
// [更新日]2019/11/01
//===================================================================================================================================

//===================================================================================================================================
//【インクルード】
//===================================================================================================================================
#include "Fader.h"
#include "ShaderLoader.h"
#include "Direct3D9.h"
#include "ImguiManager.h"

//===================================================================================================================================
//【グローバル変数】
//===================================================================================================================================
Fader* pointerFader = NULL;

//===================================================================================================================================
//【外部参照】
//===================================================================================================================================
Fader* getFader() {
	return pointerFader;
}

//===================================================================================================================================
//【コンストラクタ】
//===================================================================================================================================
Fader::Fader()
{
	pointerFader = this;

	device			= getDevice();
	inTimer			= 0.0f;
	outTimer		= 0.0f;
	waitTimer		= 0.0f;
	playTimer		= 0.0f;
	shaderRate		= 0.0f;

	shaderState		= NULL;
	targetTexture	= NULL;
	textureZBuffer	= NULL;
	surface			= NULL;

	//テクスチャオブジェクトの作成
	D3DXCreateTexture(getDevice(), WINDOW_WIDTH, WINDOW_HEIGHT, 1,
		D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &targetTexture);
	//サーフェースの取得
	targetTexture->GetSurfaceLevel(0, &surface);

	//テクスチャをレンダリングターゲットする際のZバッファを作成
	D3DSURFACE_DESC desc;
	targetTexture->GetLevelDesc(0, &desc);
	getDevice()->CreateDepthStencilSurface(desc.Width, desc.Height,
		D3DFMT_D24S8, D3DMULTISAMPLE_NONE, 0, true, &textureZBuffer, NULL);

	//フェード描画頂点構造体
	point[0] = {-1,1,0,0,0 };
	point[1] = {1,1,0,1,0 };
	point[2] = {-1,-1,0,0,1 };
	point[3] = {1,-1,0,1,1 };

	processing = false;
}

//===================================================================================================================================
//【デストラクタ】
//===================================================================================================================================
Fader::~Fader()
{
	SAFE_RELEASE(surface);
}

//===================================================================================================================================
//【更新】
//===================================================================================================================================
void Fader::update(float frameTime)
{
	this->frameTime = frameTime;
	switch (state)
	{
	case faderNS::FADE_IN:
		fadeIn();
		break;
	case faderNS::FADE_OUT:
		fadeOut();
		break;
	case faderNS::FADE_PLAY:
		play();
		break;
	case faderNS::FADE_WAIT:
		wait();
		break;
	}
}

//===================================================================================================================================
//【フェードイン処理】
//===================================================================================================================================
void Fader::fadeIn()
{
	inTimer += frameTime;
	shaderRate = inTimer / limitInTime;
	//フェードイン終了
	if (inTimer > limitInTime)
	{
		state		= faderNS::FADE_WAIT;//待機状態へ
		shaderRate	= 1.0f;
		waitTimer	= 0.0f;
	}
}

//===================================================================================================================================
//【フェードアウト処理】
//===================================================================================================================================
void Fader::fadeOut()
{
	outTimer += frameTime;
	shaderRate = 1.0f - (outTimer / limitInTime);
	//フェードアウト終了
	if (outTimer > limitOutTime)
	{
		state		= faderNS::FADE_PLAY;//再生状態へ
		shaderRate	= 0.0f;
		playTimer	= 0.0f;
	}
}

//===================================================================================================================================
//【再生処理】
//===================================================================================================================================
void Fader::play()
{
	playTimer += frameTime;
	shaderRate = 0.0f;
	if (playTimer > limitPlayTime)
	{
		state = faderNS::FADE_IN;//フェードイン状態へ
		shaderRate = 0.0f;
		inTimer = 0.0f;
	}
}

//===================================================================================================================================
//【待機処理】
//===================================================================================================================================
void Fader::wait()
{
	waitTimer	+= frameTime;
	shaderRate	= 1.0f;
	processing	= false;
}

//===================================================================================================================================
//【描画】
//===================================================================================================================================
void Fader::render()
{
	if (shaderState == NULL)return;
	if (!processing)return;

	//getDirect3D9()->setRenderBackBuffer(0);
	//device->SetRenderState(D3DRS_LIGHTING, false);
	device->SetFVF(D3DFVF_XYZ | D3DFVF_TEX1);
	D3DSURFACE_DESC desc;
	targetTexture->GetLevelDesc(0, &desc);

	shaderState->setValue(desc);
	shaderState->begin(0);
	shaderState->updateValue(shaderRate);

	device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, point, sizeof(FADE_VERTEX));

	shaderState->end();
}

//===================================================================================================================================
//【テクスチャへ描画対象を切り替える】
//===================================================================================================================================
void Fader::setRenderTexture()
{
	getDevice()->SetRenderTarget(0, surface);
	getDevice()->SetDepthStencilSurface(textureZBuffer);
}

//===================================================================================================================================
//【シェーダの設定切り替え】
//===================================================================================================================================
void Fader::setShader(int shaderType)
{
	SAFE_DELETE(shaderState);
	switch (shaderType)
	{
	case faderNS::SHADER_TYPE::NORMAL:
		shaderState = new faderNS::NormalShader(targetTexture);
		break;
	case faderNS::SHADER_TYPE::BLUR:
		shaderState = new faderNS::BlurShader(4, targetTexture);
		break;
	}
}

//===================================================================================================================================
//【開始】
//===================================================================================================================================
void Fader::start()
{
	if (processing)return;//処理中なら処理しない
	processing		= true;
	state			= faderNS::FADE_OUT;
	outTimer		= 0.0f;
	limitOutTime	= shaderState->outTime;
	limitInTime		= shaderState->inTime;
	limitPlayTime	= shaderState->playTime;
	limitWaitTime	= shaderState->waitTime;
}

//===================================================================================================================================
//【再生中】
//===================================================================================================================================
bool Fader::nowPlaying()
{
	return (outTimer > limitOutTime) && (playTimer < limitPlayTime);
}

//===================================================================================================================================
//【処理中】
//===================================================================================================================================
bool Fader::nowProcessing()
{
	return processing;
}


//===================================================================================================================================
//【ImGUIへの出力】
//===================================================================================================================================
#ifdef _DEBUG
void Fader::outputGUI()
{
	switch (state)
	{
	case faderNS::FADE_IN:	ImGui::Text("state:FADE_IN");	break;
	case faderNS::FADE_OUT:	ImGui::Text("state:FADE_OUT");	break;
	case faderNS::FADE_PLAY:ImGui::Text("state:FADE_PLAY");	break;
	case faderNS::FADE_WAIT:ImGui::Text("state:FADE_WAIT");	break;
	}
	ImGui::Text("inTimer = %.02f/%.02f", inTimer,limitInTime);
	ImGui::Text("outTimer = %.02f/%.02f", outTimer,limitOutTime);
	ImGui::Text("playTimer = %.02f/%.02f", playTimer,limitPlayTime);
	ImGui::Text("waitTimer = %.02f/%.02f", waitTimer,limitWaitTime);

	//ImGui::SliderFloat("alpha", &alpha, 0, 255);												//透過値
	//ImGui::SliderFloat4("quaternion", quaternion, limitBottom, limitTop);			//回転
	//ImGui::SliderFloat3("speed", speed, limitBottom, limitTop);						//速度
	//ImGui::Checkbox("onGravity", &onGravity);												//重力有効化フラグ

}
#endif // _DEBUG
