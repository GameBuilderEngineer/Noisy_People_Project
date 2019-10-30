//===================================================================================================================================
//【3DSound.cpp】
// [作成者]HAL東京GP12A332 16 蔡 友剛
// [作成日]2019/10/29
// [更新日]2019/10/29
//===================================================================================================================================
#include "3DSound.h"
#include "Sound.h"
#include "ImguiManager.h"
#include "AbstractScene.h"
#include "Player.h"

//===================================================================================================================================
//【コンストラクタ】
//===================================================================================================================================
X3DSoundManager::X3DSoundManager()
{
#if(_MSC_VER >= GAME_MSC_VER)
	//ミッドポイントボイスの作成(P1)
	if (FAILED(SoundInterface::GetXAudio2Interface()->CreateSubmixVoice(
		&MidpointVoice[playerNS::PLAYER1],									//サブミックスボイス
		ENDPOINT_INPUT_CHANNEL,												//チャンネル数(入力)
		ENDPOINT_SAMPLE_RATE,												//サンプリングレート(入力)
		XAUDIO2_VOICE_USEFILTER,												//フィルター機能
		NULL,																//意味ないもの
		SendList,															//送信リスト(S3D)
		NULL)))																//エフェクトチェーン
	{
		return;
	}
	//ミッドポイントボイスの作成(P2)
	if (FAILED(SoundInterface::GetXAudio2Interface()->CreateSubmixVoice(
		&MidpointVoice[playerNS::PLAYER2],									//サブミックスボイス
		ENDPOINT_INPUT_CHANNEL,												//チャンネル数(入力)
		ENDPOINT_SAMPLE_RATE,												//サンプリングレート(入力)
		XAUDIO2_VOICE_USEFILTER,												//フィルター機能
		NULL,																//意味ないもの
		SendList,															//送信リスト(S3D)
		NULL)))																//エフェクトチェーン
	{
		return;
	}

	//ミッドポイントボイスへの送信リストの作成
	SendDescriptor[playerNS::PLAYER1] = { XAUDIO2_SEND_USEFILTER,MidpointVoice[playerNS::PLAYER1] };
	SendList[playerNS::PLAYER1] = { 1,&SendDescriptor[playerNS::PLAYER1] };
	SendDescriptor[playerNS::PLAYER2] = { XAUDIO2_SEND_USEFILTER,MidpointVoice[playerNS::PLAYER2] };
	SendList[playerNS::PLAYER2] = { 1,&SendDescriptor[playerNS::PLAYER2] };

	////ミッドポイントボイスの初期化
	//MidpointVoice[playerNS::PLAYER2]->SetOutputMatrix()

#endif
}

//===================================================================================================================================
//【デストラクタ】
//===================================================================================================================================
X3DSoundManager::~X3DSoundManager()
{
#if(_MSC_VER >= GAME_MSC_VER)
	//ミッドポイントボイス
	SAFE_DESTROY_VOICE(MidpointVoice[playerNS::PLAYER1])
	SAFE_DESTROY_VOICE(MidpointVoice[playerNS::PLAYER2])
#endif
}

