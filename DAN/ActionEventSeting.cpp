//=============================================================================
//
// アニメーション処理 [D3DXAnimation.cpp]
// Author : HAL東京 周子皓
//
//=============================================================================

#include "ActionEventSeting.h"
#include "movep.h"



ACTIONEVENTSETING ActionEventSeting;

//=============================================================================
// キーデータを設置する
//=============================================================================
void AddKeydata(float Time, int KeyType)
{
	if (ActionEventSeting.Keydata_No > Keydata_Max)
	{
		MessageBox(0, "Keydata_Max is too samll", "Error", 0);
		return;
	}
	else
	{
		ActionEventSeting.Keydata[ActionEventSeting.Keydata_No] = { Time,KeyType };
		ActionEventSeting.Keydata_No++;
	}

	return;
}


//=============================================================================
// アニメーション中断イベントのキーを設置する
//=============================================================================
HRESULT SetupCallbackKeys(KEYDATA *Keydata, LPCSTR SetName, int CallbacksNum, const char *Owner)
{
	D3DXANIMATION *Animation = NULL;
	// Keyframeを設置する予定のAnimationSet
	ID3DXKeyframedAnimationSet* AnimSetTemp = NULL;
	// Keyframeを設置した新しいAnimationSet
	ID3DXCompressedAnimationSet* CompressedAnimSet = NULL;
	// 圧縮された資料
	ID3DXBuffer* CompressedInfo = NULL;
	// エラーメッセージ	
	char Message[256] = { NULL };
	double Ticks = 0.0;
	double Period = 0.0;
	// 設置するCallback Keys
	D3DXKEY_CALLBACK *Keys = NULL;


	//Sample of SetupCallbackKeys
	MOVEP *MoveP = GetMovePAdr();
	if (strcmp(Owner, "MoveP") == 0)
	{
		Animation = MoveP->Animation;
		if (FAILED(Animation->AnimController->GetAnimationSetByName(SetName, (ID3DXAnimationSet**)&AnimSetTemp)))
		{
			sprintf(Message, "Setup Callbacks in %s AnimationSet Failed！", SetName);
			goto FunctionExit;

		}
	}
	else
	{
		sprintf(Message, "Can't find this AnimationSet Owner");
		goto FunctionExit;
	}


	// 圧縮資料を渡す
	AnimSetTemp->Compress(D3DXCOMPRESS_DEFAULT, 0.5f, 0, &CompressedInfo);

	// 1秒のキーフレイムの数を返す(1秒 = 4800)
	Ticks = AnimSetTemp->GetSourceTicksPerSecond();
	// アニメーションのループ時間
	Period = AnimSetTemp->GetPeriod();

	// メモリを配る
	Keys = (D3DXKEY_CALLBACK*)calloc(CallbacksNum, sizeof(D3DXKEY_CALLBACK));
	if (Keys == NULL)
	{
		sprintf(Message, "Alloc Keys in %s AnimationSet Failed！", SetName);
		goto FunctionExit;
	}

	// Callback Keysを設置する
	for (int i = 0; i < CallbacksNum; i++)
	{
		if (Keydata[i].KeyType == NoEvent)
		{
			sprintf(Message, "There is NoEvent Keydata in %s AnimationSet！", SetName);
			goto FunctionExit;
		}
		Keys[i].Time = (float)(Period * Ticks * Keydata[i].Time);
		Keys[i].pCallbackData = (void*)Keydata[i].KeyType;
	}

	// 圧縮するアニメーションセットを作成する
	D3DXCreateCompressedAnimationSet(AnimSetTemp->GetName(),
		AnimSetTemp->GetSourceTicksPerSecond(),
		AnimSetTemp->GetPlaybackType(), CompressedInfo, CallbacksNum, Keys, &CompressedAnimSet);

	// 古いアニメーションセット(圧縮されない)を削除する
	Animation->AnimController->UnregisterAnimationSet(AnimSetTemp);

	// 新しいアニメーションセット(圧縮された)を追加する
	Animation->AnimController->RegisterAnimationSet(CompressedAnimSet);

	// 終了処理
FunctionExit:

	SAFE_RELEASE(CompressedInfo);
	SAFE_RELEASE(AnimSetTemp);
	SAFE_RELEASE(CompressedAnimSet);
	SAFE_FREE(Keys);


	if (Message[0] == NULL)
	{
		return S_OK;
	}
	else
	{
		MessageBox(0, Message, "Error", 0);
		return E_FAIL;
	}
}


//=============================================================================
// アニメ再生の中断イベント
//=============================================================================
HRESULT AnimCallBackHandler::HandleCallback(UINT Track, LPVOID pCallbackData)
{
	static float PreActionSpeed = 0.0f;
	int KeyType = (int)pCallbackData;


	//eventの設置
	MOVEP *MoveP = GetMovePAdr();
	switch (KeyType)
	{
	case MovePAttackStart:
		MoveP->IsAttack = true;
		MoveP->AttackMove = true;
		break;
	case MovePAttackEnd:
		//安全のため全部を閉める
		MoveP->IsAttack = false;
		MoveP->IsAttack1 = false;
		MoveP->IsAttack2 = false;
		MoveP->AttackMove = false;
		MoveP->AttackMove1 = false;
		MoveP->AttackMove2 = false;
		AnimPointer->MotionEnd = true;
		break;
	case MovePJumpFireStart:
		break;
	case MovePJumpFireEnd:
		MoveP->IsJumpEnd = true;
		break;
	case MovePDeath:
		MoveP->IsDeath = true;
		break;
	case MovePIsDamage:
		MoveP->IsNoDefendDamage = true;
		break;
	case MovePIsDamageEnd:
		MoveP->IsNoDefendDamage = false;
		break;
	case MotionEnd:
		AnimPointer->MotionEnd = true;
		break;
	case ResetTime:
		AnimPointer->AnimController->ResetTime();
		AnimPointer->AnimController->SetTrackPosition(0, 0.0f);
		AnimPointer->MotionEnd = true;
		break;
	default:
		break;
	}

	return D3D_OK;
}

//=============================================================================
// プレイヤーのポインタを獲得する
//=============================================================================
ACTIONEVENTSETING *GetAESPAdr(void)
{
	return &ActionEventSeting;
}
