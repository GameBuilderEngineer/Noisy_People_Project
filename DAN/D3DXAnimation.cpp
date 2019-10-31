//=============================================================================
//
// アニメーション処理 [D3DXAnimation.cpp]
// Author : HAL東京 周子皓
//
//=============================================================================
#include "D3DXAnimation.h"
#include "Direct3D9.h"
#include "Light.h"
#include "ActionEventSeting.h"


//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPD3DXEFFECT    SkinMeshEffect = NULL;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
//// アニメーション中断イベントのキーを設置する
//HRESULT SetupCallbackKeys(KEYDATA *Keydata, LPCSTR SetName, int CallbacksNum, const char *Owner);
//// キーデータを設置する
//void AddKeydata(float Time, int KeyType);


//=============================================================================
// ボーンマトリクス間の関係を設置する
//=============================================================================
HRESULT SetupBoneMatrixPointers(LPD3DXFRAME pFrameBase, LPD3DXFRAME pFrameRoot)
{
	if (pFrameBase->pMeshContainer != NULL)
	{
		D3DXFRAME_DERIVED* pFrame = NULL;
		D3DXMESHCONTAINER_DERIVED* pMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pFrameBase->pMeshContainer;

		// if there is a skinmesh, then setup the bone matrices
		if (pMeshContainer->pSkinInfo != NULL)
		{
			UINT cBones = pMeshContainer->pSkinInfo->GetNumBones();
			pMeshContainer->ppBoneMatrixPtrs = new D3DXMATRIX*[cBones];
			for (UINT iBone = 0; iBone < cBones; iBone++)
			{
				pFrame = (D3DXFRAME_DERIVED*)D3DXFrameFind(pFrameRoot, pMeshContainer->pSkinInfo->GetBoneName(iBone));
				if (pFrame == NULL) return E_FAIL;

				pMeshContainer->ppBoneMatrixPtrs[iBone] = &pFrame->CombinedTransformationMatrix;
			}
		}
	}

	if (pFrameBase->pFrameSibling != NULL)
	{
		if (FAILED(SetupBoneMatrixPointers(pFrameBase->pFrameSibling, pFrameRoot)))
			return E_FAIL;
	}

	if (pFrameBase->pFrameFirstChild != NULL)
	{
		if (FAILED(SetupBoneMatrixPointers(pFrameBase->pFrameFirstChild, pFrameRoot)))
			return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
// フレイム(ボーン)を描画する
//=============================================================================
void DrawFrame(D3DXANIMATION* Animation, LPD3DXFRAME pFrame, bool DrawShadowMap)
{
	LPDIRECT3DDEVICE9 pDevice = getDevice();
	LPD3DXMESHCONTAINER pMeshContainer;

	if (pFrame == NULL)
	{
		return;
	}

	// メッシュを取得する
	pMeshContainer = pFrame->pMeshContainer;

	while (pMeshContainer != NULL)
	{
		// メッシュを描画する
		DrawMeshContainer(Animation, pMeshContainer, pFrame, DrawShadowMap);
		// 次のメッシュを探す
		pMeshContainer = pMeshContainer->pNextMeshContainer;
	}

	// 兄弟フレイムを描画する
	DrawFrame(Animation, pFrame->pFrameSibling, DrawShadowMap);
	// 子フレイムを描画する
	DrawFrame(Animation, pFrame->pFrameFirstChild, DrawShadowMap);
}

//=============================================================================
// メッシュを描画する
//=============================================================================
void DrawMeshContainer(D3DXANIMATION* Animation, LPD3DXMESHCONTAINER pMeshContainerBase, LPD3DXFRAME pFrameBase, bool DrawShadowMap)
{

	D3DXMESHCONTAINER_DERIVED* pMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pMeshContainerBase;
	D3DXFRAME_DERIVED* pFrame = (D3DXFRAME_DERIVED*)pFrameBase;
	LPDIRECT3DDEVICE9 pDevice = getDevice();
	UINT iMaterial;
	UINT NumBlend;
	UINT iAttrib;
	DWORD AttribIdPrev;
	LPD3DXBONECOMBINATION pBoneComb;
	UINT iPaletteEntry;

	UINT iMatrixIndex;
	D3DXMATRIXA16 matTemp;
	D3DCAPS9 d3dCaps;
	pDevice->GetDeviceCaps(&d3dCaps);

	// first check for skinning
	if (pMeshContainer->pSkinInfo != NULL)
	{
		AttribIdPrev = UNUSED32;
		pBoneComb = reinterpret_cast<LPD3DXBONECOMBINATION>(pMeshContainer->pBoneCombinationBuf->GetBufferPointer());

		// Draw using default vtx processing of the device (typically HW)
		for (iAttrib = 0; iAttrib < pMeshContainer->BoneNum; iAttrib++)
		{
			NumBlend = 0;
			for (DWORD i = 0; i < pMeshContainer->BoneWeightNum; ++i)
			{
				if (pBoneComb[iAttrib].BoneId[i] != UINT_MAX)
				{
					NumBlend = i;
				}
			}

			if (d3dCaps.MaxVertexBlendMatrices >= NumBlend + 1)
			{
				// first calculate the world matrices for the current set of blend weights and get the accurate count of the number of blends
				for (DWORD i = 0; i < pMeshContainer->BoneWeightNum; ++i)
				{
					iMatrixIndex = pBoneComb[iAttrib].BoneId[i];
					if (iMatrixIndex != UINT_MAX)
					{
						D3DXMatrixMultiply(&matTemp, &pMeshContainer->pBoneOffsetMatrices[iMatrixIndex],
							pMeshContainer->ppBoneMatrixPtrs[iMatrixIndex]);
						pDevice->SetTransform(D3DTS_WORLDMATRIX(i), &matTemp);
					}
				}

				pDevice->SetRenderState(D3DRS_VERTEXBLEND, NumBlend);

				// lookup the material used for this subset of faces
				if ((AttribIdPrev != pBoneComb[iAttrib].AttribId) || (AttribIdPrev == UNUSED32))
				{
					pDevice->SetMaterial(&pMeshContainer->pMaterials[pBoneComb[iAttrib].AttribId].MatD3D);
					pDevice->SetTexture(0, pMeshContainer->ppTextures[pBoneComb[iAttrib].AttribId]);
					AttribIdPrev = pBoneComb[iAttrib].AttribId;
				}

				// draw the subset now that the correct material and matrices are loaded
				pMeshContainer->MeshData.pMesh->DrawSubset(iAttrib);
			}
		}
		pDevice->SetRenderState(D3DRS_VERTEXBLEND, 0);
	}
	else  // standard mesh, just draw it after setting material properties
	{
		pDevice->SetTransform(D3DTS_WORLD, &pFrame->CombinedTransformationMatrix);

		for (iMaterial = 0; iMaterial < pMeshContainer->NumMaterials; iMaterial++)
		{
			pDevice->SetMaterial(&pMeshContainer->pMaterials[iMaterial].MatD3D);
			pDevice->SetTexture(0, pMeshContainer->ppTextures[iMaterial]);
			pMeshContainer->MeshData.pMesh->DrawSubset(iMaterial);
		}
	}
}

//=============================================================================
// x Fileを読み込む
//=============================================================================
HRESULT Load_xFile(D3DXANIMATION* D3DXAnimation, LPCTSTR filename, const char* ErrorSrc)
{
	char Message[256];
	LPDIRECT3DDEVICE9 pDevice = getDevice();
	D3DXAnimation->AllocateHier = new CAllocateHierarchy();
	setRootDirectory();
	if (
		FAILED(D3DXLoadMeshHierarchyFromX(filename,
			D3DXMESH_MANAGED,
			pDevice,
			D3DXAnimation->AllocateHier,
			NULL,
			&D3DXAnimation->FrameRoot,
			&D3DXAnimation->AnimController))
		)
	{
		sprintf_s(Message, "Load %s Model Failed！", ErrorSrc);
		MessageBox(0, Message, "Error", 0);
		return E_FAIL;
	}

	if (FAILED(SetupBoneMatrixPointers(D3DXAnimation->FrameRoot, D3DXAnimation->FrameRoot)))
	{
		return E_FAIL;
	}

	D3DXAnimation->MotionEnd = false;
	D3DXAnimation->AnimeSetNum = D3DXAnimation->AnimController->GetMaxNumAnimationSets();
	// メモリを配る
	D3DXAnimation->AnimData = (ANIMATIONDATA*)calloc(D3DXAnimation->AnimeSetNum, sizeof(ANIMATIONDATA));
	if (D3DXAnimation->AnimData == NULL)
	{

		sprintf_s(Message, "Alloc %s AnimationData Memory Failed！", ErrorSrc);
		MessageBox(0, Message, "Error", 0);
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
// 特定なボーンのマトリックスを取得
//=============================================================================
D3DXMATRIX GetBoneMatrix(D3DXANIMATION* Animation, const char* BoneName)
{
	char Message[64];
	D3DXFRAME_DERIVED* pFrame = SearchBoneFrame(Animation, BoneName, Animation->FrameRoot);

	// ボーンが見つかれば
	if (pFrame != NULL && pFrame->Name != NULL && strcmp(pFrame->Name, BoneName) == 0)
	{
		return pFrame->CombinedTransformationMatrix;
	}
	// ボーンが見つからなければ
	else
	{
		sprintf(Message, "Find %s Bone Failed！", BoneName);
		MessageBox(0, Message, "Error", 0);
		D3DXMATRIX TmpMatrix;
		D3DXMatrixIdentity(&TmpMatrix);
		return TmpMatrix;
	}
}

//=============================================================================
// 特定なボーンを探す
//=============================================================================
D3DXFRAME_DERIVED* SearchBoneFrame(D3DXANIMATION* Animation, const char* BoneName, D3DXFRAME* Frame)
{
	D3DXFRAME_DERIVED* pFrame = NULL;

	if (Frame == NULL)
	{
		return NULL;
	}

	if (Frame->Name != NULL && strcmp(Frame->Name, BoneName) == 0)
	{
		pFrame = (D3DXFRAME_DERIVED*)Frame;
		return pFrame;
	}

	if (Frame->pFrameSibling != NULL)
	{
		pFrame = SearchBoneFrame(Animation, BoneName, Frame->pFrameSibling);
		if (pFrame != NULL && strcmp(pFrame->Name, BoneName) == 0)
		{
			return pFrame;
		}
	}

	if (Frame->pFrameFirstChild != NULL)
	{
		pFrame = SearchBoneFrame(Animation, BoneName, Frame->pFrameFirstChild);
		if (pFrame != NULL && strcmp(pFrame->Name, BoneName) == 0)
		{
			return pFrame;
		}
	}

	return NULL;
}

////=============================================================================
//// CallbackKeysを初期化(プレイヤー)
////=============================================================================
//HRESULT InitCallbackKeys_Player(void)
//{
//	//// 設置するキーの数
//	//PLAYER *Player = GetPlayerAdr();
//	//int SetNum = Player->Animation->AnimeSetNum;
//	//float AnimLoopTime = 0.0f;
//
//	//for (int Set_No = 0; Set_No < SetNum; Set_No++)
//	//{
//	//	// キーデータをクリアする
//	//	memset(&Keydata[0], 0, sizeof(KEYDATA) * Keydata_Max);
//	//	Keydata_No = 0;
//
//	//	switch (Set_No)
//	//	{
//	//	case Player_Idle:
//	//		break;
//	//	case Player_Attack:
//	//		AddKeydata(0.01f, PlayerAttackStart);  //アニメの時間によってイベントを発生します
//	//		AddKeydata(0.3f, PlayerIsAttackNext);
//	//		AddKeydata(0.45f, PlayerIsAttackNext1);
//	//		AddKeydata(0.9f, PlayerAttackEnd);
//	//		break;
//	//	case Player_Defend:
//	//		AddKeydata(0.95f, MotionEnd);
//	//		break;
//	//	case Player_Die:
//	//		AddKeydata(0.95f, PlayerDeath);
//	//		break;
//	//	case Player_Run:
//	//		AddKeydata(0.95f, MotionEnd);
//	//		break;
//	//	case Player_Walk:
//	//		AddKeydata(0.95f, MotionEnd);
//	//		break;
//	//	case Player_NoDefendImpact:
//	//		AddKeydata(0.01f, PlayerIsDamage);
//	//		AddKeydata(0.94f, PlayerIsDamageEnd);
//	//		AddKeydata(0.95f, MotionEnd);
//	//		break;
//	//	case Player_DefendImpact:
//	//		AddKeydata(0.99f, MotionEnd);
//	//	case Player_Roll:
//	//		AddKeydata(0.05f, PlayerRollStart);
//	//		AddKeydata(0.75f, PlayerRollEnd);
//	//		break;
//	//	default:
//	//		continue;
//	//	}
//
//	//	if (FAILED(
//	//		SetupCallbackKeys(&Keydata[0], Player->Animation->AnimData[Set_No].SetName,
//	//			Keydata_No, "Player")))
//	//	{
//	//		return E_FAIL;
//	//	}
//
//	//}
//
//
//	return S_OK;
//}
//

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitAnimation(D3DXANIMATION* Animation, int Set_No)
{
	char Message[64];

	if (FAILED(Animation->AnimController->GetAnimationSetByName(
		Animation->AnimData[Set_No].SetName, &Animation->AnimData[Set_No].AnimSet)))
	{
		sprintf(Message, "Can't find %s AnimationSet！", Animation->AnimData[Set_No].SetName);
		MessageBox(0, Message, "Error", 0);
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitAnimation(D3DXANIMATION* Animation)
{
	SAFE_RELEASE(SkinMeshEffect);
	D3DXFrameDestroy(Animation->FrameRoot, Animation->AllocateHier);
	SAFE_FREE(Animation->AnimData);
	SAFE_RELEASE(Animation->AnimController);
	SAFE_DELETE(Animation->AllocateHier);
	SAFE_FREE(Animation);

	return;
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateAnimation(D3DXANIMATION* Animation, float Time)
{
	AnimCallBackHandler CallBackHandler;

	CallBackHandler.AnimPointer = Animation;

	// 合成中か否かを判定
	Animation->AnimData[Animation->CurrentAnimID].CurWeightTime += Time;

	if (Animation->AnimData[Animation->CurrentAnimID].CurWeightTime <= Animation->AnimData[Animation->CurrentAnimID].ShiftTime)
	{
		// 合成中。ウェイトを算出
		float Weight = Animation->AnimData[Animation->CurrentAnimID].CurWeightTime / Animation->AnimData[Animation->CurrentAnimID].ShiftTime;
		// ウェイトを登録
		Animation->AnimController->SetTrackWeight(0, Weight);       // 現在のアニメーション
		Animation->AnimController->SetTrackWeight(1, 1 - Weight);   // 前のアニメーション
	}
	else
	{
		// 合成終了中。通常アニメーションをするTrack0のウェイトを最大値に
		Animation->AnimController->SetTrackWeight(0, 1.0f);			// 現在のアニメーション
		Animation->AnimController->SetTrackEnable(1, false);		// 前のアニメーションを無効にする
		Animation->MotionBlendOver = true;
	}


	// 時間を更新
	Animation->AnimController->AdvanceTime(Time, &CallBackHandler);

	return;
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawAnimation(D3DXANIMATION* Animation, LPD3DXMATRIX WorldMatrix, bool DrawShadowMap)
{
	LPDIRECT3DDEVICE9 pDevice = getDevice();

	if (DrawShadowMap == false)
	{
		UpdateFrameMatrices(Animation->FrameRoot, WorldMatrix);
	}

	DrawFrame(Animation, Animation->FrameRoot, DrawShadowMap);

	return;
}

//=============================================================================
// アニメーションを切り替え
//=============================================================================
void ChangeAnimation(D3DXANIMATION* Animation, UINT AnimID, float PlaySpeed, bool ResetMotion)
{
	// トラックの能力
	D3DXTRACK_DESC TD;

	// 指定のアニメーションIDの存在をチェック
	if (AnimID > (UINT)Animation->AnimeSetNum)
	{
		MessageBox(0, "AnimationSet Don't Exist", "Error", 0);
		return;
	}

	// 異なるアニメーションであるかをチェック
	// 更新する必要がない
	if (Animation->CurrentAnimID == AnimID)
	{
		if (ResetMotion == true)
		{
			Animation->AnimController->ResetTime();
			Animation->AnimController->SetTrackPosition(0, 0.0f);
		}
		return;
	}

	Animation->MotionEnd = false;
	Animation->StartMove = false;
	Animation->MotionBlendOver = false;
	Animation->Cancelable = true;

	// 現在のアニメーションセットの設定値を取得
	Animation->AnimController->GetTrackDesc(0, &TD);

	// 今のアニメーションをトラック1に移行し
	// トラックの設定値も移行
	Animation->AnimController->SetTrackAnimationSet(1, Animation->AnimData[Animation->CurrentAnimID].AnimSet);
	Animation->AnimController->SetTrackDesc(1, &TD);
	Animation->AnimController->SetTrackSpeed(1, 0.0f);

	// 新しいアニメーションセットをトラック0に設定
	Animation->AnimController->SetTrackAnimationSet(0, Animation->AnimData[AnimID].AnimSet);

	// トラック0のスピードの設定
	Animation->AnimController->SetTrackSpeed(0, PlaySpeed);

	// トラック0の位置は最初からに設定
	Animation->AnimController->SetTrackPosition(0, 0.0f);

	// トラックの合成を許可
	Animation->AnimController->SetTrackEnable(0, true);
	Animation->AnimController->SetTrackEnable(1, true);

	// ウェイト時間を初期化
	Animation->AnimData[AnimID].CurWeightTime = 0.0f;
	Animation->AnimController->ResetTime();

	// 現在のアニメーション番号を切り替え
	Animation->PreviousAnimID = Animation->CurrentAnimID;
	Animation->CurrentAnimID = AnimID;

	return;
}

