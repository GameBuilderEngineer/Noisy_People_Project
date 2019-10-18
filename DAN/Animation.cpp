//============================================================================================================================================
// Document
//============================================================================================================================================
// Animation.cpp
// HAL東京 GP-12A-332 09 亀岡竣介
// 2019/09/03
//============================================================================================================================================
#include "Animation.h"
//============================================================================================================================================
// Using Declaration
// using宣言
//============================================================================================================================================
using namespace animationNS;
//============================================================================================================================================
// initialize
// 初期化
//============================================================================================================================================
HRESULT initialize(Animation* _animation, LPCSTR _setName, int _setNo)
{
	_animation->animationController->GetAnimationSetByName(_setName, &_animation->animationManager[_setNo].animationSet);

	_animation->animationManager[_setNo].animationSetName = _animation->animationManager[_setNo].animationSet->GetName();

	return S_OK;
}
//============================================================================================================================================
// release
// 解放
//============================================================================================================================================
void release(Animation* _animation)
{
	SAFE_DELETE_ARRAY(_animation->animationManager);
	SAFE_RELEASE(_animation->animationController);
	SAFE_DELETE(_animation->allocateHierarchy);
	//SAFE_DELETE(_animation);

	return;
}
//============================================================================================================================================
// update
// 更新
//============================================================================================================================================
void update(Animation* _animation, float _time)
{
	if (!_animation->flag.animationOn) { return; }

	CallBackAnimation callBackAnimation;	//	コールバックハンドル

	// ハンドルにポインタを代入
	callBackAnimation.animation = _animation;
	callBackAnimation.setName = _animation->animationManager[_animation->animationID.current].animationSetName;

	// 合成中かの判定
	_animation->animationManager[_animation->animationID.current].weightTime += _time;

	// 合成中
	if ((_animation->animationManager[_animation->animationID.current].weightTime) <= (_animation->animationManager[_animation->animationID.current].shiftTime))
	{
		// ウェイトを算出
		float weight = _animation->animationManager[_animation->animationID.current].weightTime / _animation->animationManager[_animation->animationID.current].shiftTime;

		// ウェイトを登録
		_animation->animationController->SetTrackWeight(0, weight);			// 現在のアニメーション
		_animation->animationController->SetTrackWeight(1, (1 - weight));	// 過去のアニメーション
	}
	// 合成終了
	else
	{
		// 通常アニメーションの"Track0"のウェイトを最大値に設定
		_animation->animationController->SetTrackWeight(0, 1.0f);	// 現在のアニメーション
		_animation->animationController->SetTrackEnable(1, false);	// 過去のアニメーションを無効化
	}

	// 時間を更新
	_animation->animationController->AdvanceTime(_time, &callBackAnimation);	// もし現在がコールバック時間なら、"AdvanceTime"関数内でコールバックが実行

	return;
}
//============================================================================================================================================
// updateFrameMatrix
// 更新 - フレーム内の、変更された代替正方形座
//============================================================================================================================================
void updateFrameMatrix(LPDIRECT3DDEVICE9* _device, LPD3DXFRAME _baseFrame, LPD3DXMATRIX _parentMatrix)
{
	D3DXFrameDerived* frame = (D3DXFrameDerived*)_baseFrame;	//	フレームポインタ

	if ((_baseFrame == NULL) || (_parentMatrix == NULL)) { return; }

	// このボーンは累積計算を行うために、親スケルトンのオフセット方程式マトリクスに対して相対的
	D3DXMatrixMultiply(&frame->combinedTransformationMatrix, &frame->TransformationMatrix, _parentMatrix);

	// 兄弟ボーンの更新
	updateFrameMatrix(_device, frame->pFrameSibling, _parentMatrix);

	// 子ボーンの更新
	updateFrameMatrix(_device, frame->pFrameFirstChild, &frame->combinedTransformationMatrix);

	return;
}
//============================================================================================================================================
// render
// 描画
//============================================================================================================================================
void render(LPDIRECT3DDEVICE9 _device, Animation* _animation, LPD3DXMATRIX _worldMatrix)
{
	updateFrameMatrix(&_device, _animation->rootFrame, _worldMatrix);

	renderFrame(_device, _animation, _animation->rootFrame);

	return;
}
//============================================================================================================================================
// renderFrame
// 描画 - フレーム( ボーン )
//============================================================================================================================================
void renderFrame(LPDIRECT3DDEVICE9 _device, Animation* _animation, LPD3DXFRAME _frame)
{
	LPD3DXMESHCONTAINER meshContainer = NULL;	//	メッシュコンテナ

	if (_frame == NULL) { return; }

	// メッシュコンテナの取得
	meshContainer = _frame->pMeshContainer;

	// メッシュコンテナの描画
	while (meshContainer != NULL)
	{
		// スキンメッシュの描画
		renderMeshContainer(_device, meshContainer, _frame);

		// 全てのメッシュコンテナを検索
		meshContainer = meshContainer->pNextMeshContainer;
	}

	// 兄弟フレームの描画
	renderFrame(_device, _animation, _frame->pFrameSibling);

	// 子フレームの描画
	renderFrame(_device, _animation, _frame->pFrameFirstChild);

	return;
}
//============================================================================================================================================
// renderMeshContainer
// 描画 - メッシュコンテナ( スキンメッシュ )
//============================================================================================================================================
void renderMeshContainer(LPDIRECT3DDEVICE9 _device, LPD3DXMESHCONTAINER _baseMeshContainer, LPD3DXFRAME _baseFrame)
{
	D3DCAPS9 caps;																				//	キャップ
	D3DXMATRIXA16 matrix;																		//	ローカルマトリクス
	UINT matrixIndex = NULL;																	//	マトリクスインデックス
	UINT blendNumber = NULL;																	//	ブレンド数
	D3DXMeshContainerDerived* meshContainer = (D3DXMeshContainerDerived*)_baseMeshContainer;	//	メッシュコンテナポインタ
	D3DXFrameDerived* frame = (D3DXFrameDerived*)_baseFrame;									//	フレームポインタ

	// キャップの調査
	_device->GetDeviceCaps(&caps);

	// スキニングの最初のチェック
	if (meshContainer->pSkinInfo != NULL)
	{
		LPD3DXBONECOMBINATION boneCombination = NULL;	//	ボーンの組み合わせ
		DWORD attributeIndexPast = UNUSED32;			//	アトリビュートインデックス( 過去 )

		boneCombination = reinterpret_cast<LPD3DXBONECOMBINATION>(meshContainer->boneCombinationBuffer->GetBufferPointer());

		// デバイスのデフォルトの頂点処理( 通常はHW )を使用して描画
		for (UINT i = 0; i < meshContainer->boneMax; i++)
		{
			blendNumber = 0;

			for (DWORD j = 0; j < meshContainer->boneWeightMax; ++j)
			{
				if (boneCombination[i].BoneId[j] != UINT_MAX)
				{
					blendNumber = j;
				}
			}

			if (caps.MaxVertexBlendMatrices >= (blendNumber + 1))
			{
				// 最初に、現在のブレンドウェイトセットのワールドマトリクスを計算し、ブレンド数の正確なカウントを取得
				for (DWORD j = 0; j < meshContainer->boneWeightMax; ++j)
				{
					matrixIndex = boneCombination[i].BoneId[j];

					if (matrixIndex != UINT_MAX)
					{
						D3DXMatrixMultiply(&matrix, &meshContainer->boneOffsetMatrix[matrixIndex], meshContainer->boneMatrix[matrixIndex]);
						_device->SetTransform(D3DTS_WORLDMATRIX(j), &matrix);
					}
				}

				// レンダリングステートの設定
				_device->SetRenderState(D3DRS_VERTEXBLEND, blendNumber);

				// 部分集合に使用されているマテリアルを検索
				if ((attributeIndexPast != boneCombination[i].AttribId) || (attributeIndexPast == UNUSED32))
				{
					_device->SetMaterial(&meshContainer->pMaterials[boneCombination[i].AttribId].MatD3D);
					_device->SetTexture(0, meshContainer->texture[boneCombination[i].AttribId]);

					attributeIndexPast = boneCombination[i].AttribId;
				}
				// 正しい材料と行列がロードされたので、サブセットを描画
				meshContainer->MeshData.pMesh->DrawSubset(i);
			}
		}
		// レンダリングステートの設定
		_device->SetRenderState(D3DRS_VERTEXBLEND, 0);
	}
	// 標準メッシュ、材料特性を設定した後に描画
	else
	{
		_device->SetTransform(D3DTS_WORLD, &frame->combinedTransformationMatrix);

		for (UINT i = 0; i < meshContainer->NumMaterials; i++)
		{
			_device->SetMaterial(&meshContainer->pMaterials[i].MatD3D);
			_device->SetTexture(0, meshContainer->texture[i]);

			meshContainer->MeshData.pMesh->DrawSubset(i);
		}
	}

	return;
}
//============================================================================================================================================
// switching
// アニメーションの切り替え
//============================================================================================================================================
void switching(Animation* _animation, UINT _animationId)
{
	D3DXTRACK_DESC trackDescription;	//	トラックの能力

	_animation->flag.animationPlayEnd = false;
	_animation->keyFrameCount = 0;

	// 指定のアニメーションIDの存在を調査
	if (_animationId > (UINT)_animation->animationSetMax) { return; }

	// 指定のアニメーションIDが、現在のアニメーションIDと同じであれば、更新不要のため終了
	if (_animation->animationID.current == _animationId) { return; }

	// 現在のアニメーションセットの設定値を取得
	_animation->animationController->GetTrackDesc(0, &trackDescription);

	// 現在のアニメーションをトラック１に移行し、トラックの設定値も移行
	_animation->animationController->SetTrackAnimationSet(1, _animation->animationManager[_animation->animationID.current].animationSet);
	_animation->animationController->SetTrackDesc(1, &trackDescription);
	_animation->animationController->SetTrackSpeed(1, 0.0f);

	// 新しいアニメーションセットをトラック０に設定
	_animation->animationController->SetTrackAnimationSet(0, _animation->animationManager[_animationId].animationSet);

	// トラック０のスピードの設定
	switchingSpeed(_animation);

	// トラック０の位置を、開始位置に設定
	_animation->animationController->SetTrackPosition(0, 0.0f);

	// トラックの合成を許可
	_animation->animationController->SetTrackEnable(0, true);
	_animation->animationController->SetTrackEnable(1, true);

	// ウェイト時間を初期化
	_animation->animationManager[_animationId].weightTime = 0.0f;
	_animation->animationController->ResetTime();

	// 現在のアニメーション番号を切り替え
	_animation->animationID.current = _animationId;

	return;
}
//============================================================================================================================================
// switchingSpeed
// アニメーションの速度切り替え
//============================================================================================================================================
void switchingSpeed(Animation* _animation)
{
	_animation->animationController->SetTrackSpeed(0, _animation->animationSpeed);

	return;
}
//============================================================================================================================================
// createObject
// アニメーションオブジェクトを作成
//============================================================================================================================================
Animation* createObject(void)
{
	//Animation* object = (Animation*)calloc(1, sizeof(Animation));	//	オブジェクトポインタ
	Animation* object = new Animation;	//	オブジェクトポインタ
	ZeroMemory(object, sizeof(Animation));

	object->initialize = initialize;
	object->release = release;
	object->update = update;
	object->render = render;
	object->switching = switching;
	object->setShiftTime = setShiftTime;

	return object;
}
//============================================================================================================================================
// loadXFile
// Xファイルの読み込み
//============================================================================================================================================
HRESULT loadXFile(LPDIRECT3DDEVICE9 _device, Animation* _animation, LPCTSTR _fileName)
{
	_animation->allocateHierarchy = new AllocateHierarchy(_device);

	if (FAILED(D3DXLoadMeshHierarchyFromX
	(
		_fileName,
		D3DXMESH_MANAGED,
		_device,
		_animation->allocateHierarchy,
		NULL,
		&_animation->rootFrame,
		&_animation->animationController
	)))
	{
		// エラーメッセージの出力
		MSG("Xファイルの読み込みに失敗しました");
	}

	setBoneMatrix(_animation->rootFrame, _animation->rootFrame);

	_animation->flag.animationPlayEnd = false;

	_animation->animationSetMax = _animation->animationController->GetMaxNumAnimationSets();

	// Xファイル内のアニメーションセットの数に基づいて、アニメーションセットメンバ変数に、配列を動的に割り当てる
	if (FAILED(_animation->animationManager = new AnimationManager[_animation->animationSetMax]))
	//if (FAILED(_animation->animationManager = (AnimationManager*)malloc(sizeof(AnimationManager)*_animation->animationSetMax)))
	{
		// エラーメッセージの出力
		MSG("Xファイルの読み込みに失敗しました");
	}

	return S_OK;
}
//============================================================================================================================================
// searchBoneFrame
// 特定のボーンフレームを調査
//============================================================================================================================================
D3DXFrameDerived* searchBoneFrame(Animation* _animation, const char* _boneName, D3DXFRAME* _frame)
{
	D3DXFrameDerived* frame = NULL;	//	フレーム

	if (_frame == NULL) { return NULL; }

	if (_frame->Name != NULL && strcmp(_frame->Name, _boneName) == 0)
	{
		frame = (D3DXFrameDerived*)_frame;

		return frame;
	}

	if (_frame->pFrameSibling != NULL)
	{
		frame = searchBoneFrame(_animation, _boneName, _frame->pFrameSibling);

		if (frame != NULL && strcmp(frame->Name, _boneName) == 0) { return frame; }
	}

	if (_frame->pFrameFirstChild != NULL)
	{
		frame = searchBoneFrame(_animation, _boneName, _frame->pFrameFirstChild);

		if (frame != NULL && strcmp(frame->Name, _boneName) == 0) { return frame; }
	}

	return NULL;
}
//============================================================================================================================================
// setShiftTime
// 動作開始に必要な時間を設定
//============================================================================================================================================
void setShiftTime(Animation* _animation, UINT _animationId, float _interval)
{
	// 指定のアニメーションIDの存在を調査
	if (_animationId > (UINT)_animation->animationSetMax) { return; }

	// シフト時間を登録
	_animation->animationManager[_animationId].shiftTime = _interval;

	return;
}
//============================================================================================================================================
// setBoneMatrix
// 各レベルフレームの組み合わせ変換行列を設定
//============================================================================================================================================
HRESULT setBoneMatrix(LPD3DXFRAME _baseFrame, LPD3DXFRAME _rootFrame)
{
	if (_baseFrame->pMeshContainer != NULL)
	{
		D3DXFrameDerived* frame = NULL;																		//	フレーム
		D3DXMeshContainerDerived* meshContainer = (D3DXMeshContainerDerived*)_baseFrame->pMeshContainer;	//	メッシュコンテナ

		// スキンメッシュが存在する場合は、ボーンマトリクスを設定
		if (meshContainer->pSkinInfo != NULL)
		{
			UINT boneCount = meshContainer->pSkinInfo->GetNumBones();	//	ボーンカウント

			meshContainer->boneMatrix = new D3DXMATRIX*[boneCount];

			for (UINT i = 0; i < boneCount; i++)
			{
				frame = (D3DXFrameDerived*)D3DXFrameFind(_rootFrame, meshContainer->pSkinInfo->GetBoneName(i));

				if (frame == NULL) { return E_FAIL; }

				meshContainer->boneMatrix[i] = &frame->combinedTransformationMatrix;
			}
		}
	}

	if (_baseFrame->pFrameSibling != NULL)
	{
		setBoneMatrix(_baseFrame->pFrameSibling, _rootFrame);
	}

	if (_baseFrame->pFrameFirstChild != NULL)
	{
		setBoneMatrix(_baseFrame->pFrameFirstChild, _rootFrame);
	}

	return S_OK;
}
//============================================================================================================================================
// setCallBackKeyFrame
// アニメーション中断イベント( キーフレーム )を設置
//============================================================================================================================================
HRESULT setCallBackKeyFrame(Animation* _animation, LPCSTR _setName)
{
	// 設置するコールバックキーの数
	const UINT callBackNumber = 2;								//	２つめのコールバック
	D3DXKEY_CALLBACK key[callBackNumber] = { NULL,NULL };		//	キーコールバック
	ID3DXBuffer* compressedInformation = 0;						//	圧縮情報
	double timeScaleCoefficient = NULL;							//	タイムスケール係数
	ID3DXKeyframedAnimationSet* animationSetKeyFramed = 0;		//	キーフレームを設置するアニメーションセット
	ID3DXCompressedAnimationSet* animationSetCompressed = 0;	//	キーフレームを設置後の新しいアニメーションセット

	_animation->animationController->GetAnimationSetByName(_setName, (ID3DXAnimationSet**)&animationSetKeyFramed);

	// 圧縮
	animationSetKeyFramed->Compress(D3DXCOMPRESS_DEFAULT, 0.5f, 0, &compressedInformation);

	timeScaleCoefficient = animationSetKeyFramed->GetSourceTicksPerSecond();	//	GetSourceTicksPerSecond関数："D3DXCreateKeyFrameInterpolator"に渡すタイムスケール係数を返す：この値は、ローカルタイムをグローバル( アプリケーション )タイムにスケーリングするために使用する

	key[0].Time = (float)(animationSetKeyFramed->GetPeriod() * (90.0f / 125.0f) * timeScaleCoefficient);

	// アニメーションの最後にコールバックを実行するように、２番目のコールバックキーを設定
	key[1].Time = (float)(animationSetKeyFramed->GetPeriod() * 0.9f * timeScaleCoefficient);

	D3DXCreateCompressedAnimationSet
	(
		animationSetKeyFramed->GetName(),
		animationSetKeyFramed->GetSourceTicksPerSecond(),
		animationSetKeyFramed->GetPlaybackType(),
		compressedInformation,
		callBackNumber,
		key,
		&animationSetCompressed
	);

	// 圧縮情報の解放
	SAFE_RELEASE(compressedInformation);

	// 過去の( 圧縮されていない )アニメーションセットを削除
	_animation->animationController->UnregisterAnimationSet(animationSetKeyFramed);
	SAFE_RELEASE(animationSetKeyFramed);

	// 新しい( 圧縮された )アニメーションセットを追加
	_animation->animationController->RegisterAnimationSet(animationSetCompressed);
	SAFE_RELEASE(animationSetCompressed);

	return S_OK;
}
//============================================================================================================================================
// getBoneMatrix
// 特定のボーンマトリクスを取得
//============================================================================================================================================
D3DXMATRIX getBoneMatrix(Animation* _animation, const char* _boneName)
{
	D3DXFrameDerived* frame = searchBoneFrame(_animation, _boneName, _animation->rootFrame);	//	フレーム

	// ボーンを発見成功
	if ((frame != NULL) && (frame->Name != NULL) && (strcmp(frame->Name, _boneName) == 0))
	{
		return frame->combinedTransformationMatrix;
	}

	// ボーンを発見失敗
	D3DXMATRIX matrix;	//	マトリクス
	D3DXMatrixIdentity(&matrix);

	return matrix;
}
//============================================================================================================================================
// HandleCallback
// アニメーション再生中の、中断コールバック
//============================================================================================================================================
HRESULT CallBackAnimation::HandleCallback(UINT Track, LPVOID pCallbackData)
{
	UINT storageTrack = Track;			//	警告回避用
	LPVOID storageData = pCallbackData;	//	警告回避用

	storageTrack = NULL;
	storageData = NULL;

	//if (animation->keyFrameCount == 0)
	//{
	//	animation->flag.moveStop = true;
	//}
	//else if (animation->keyFrameCount == 1)
	//{
	//	animation->flag.animationPlayEnd = true;

	//	return D3D_OK;
	//}

	if (animation->keyFrameCount == 1)
	{
		animation->flag.animationPlayEnd = true;

		return D3D_OK;
	}

	animation->keyFrameCount++;

	return D3D_OK;
}