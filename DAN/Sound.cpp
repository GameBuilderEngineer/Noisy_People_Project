//===================================================================================================================================
//【Sound.cpp】
// [作成者]HAL東京GP12A332 16 蔡 友剛
// [作成日]2019/09/26
// [更新日]2019/10/04
//===================================================================================================================================
#include "Sound.h"
#include "ImguiManager.h"

//===================================================================================================================================
//【グローバル変数】
//===================================================================================================================================
IXAudio2 *SoundInterface::XAudio2Interface = nullptr;
XAUDIO2_VOICE_SENDS SoundInterface::SendList[ENDPOINT_VOICE_LIST::ENDPOINT_MAX] = { NULL };
SEManager *SoundInterface::SE = new SEManager();
BGMManager *SoundInterface::BGM = new BGMManager();

//===================================================================================================================================
//【コンストラクタ】
//・インタフェースの準備
//・マスターボイスの作成
//・エンドポイントボイスの作成
//・エンドポイントへの送信リストの作成
//===================================================================================================================================
SoundInterface::SoundInterface()
{
	//COMライブラリの初期化
	if (FAILED(CoInitializeEx(NULL, COINIT_MULTITHREADED)))
	{
		return;
	}

	//XAudio2オブジェクトの作成
	if (FAILED(XAudio2Create(&XAudio2Interface)))
	{
		return;
	}

	//マスターボイスの作成
	if (FAILED(XAudio2Interface->CreateMasteringVoice(&MasteringVoice)))
	{
		return;
	}

	//デバックツール
	XAUDIO2_DEBUG_CONFIGURATION debugConfig{ NULL };
	debugConfig.LogThreadID = true;
	debugConfig.TraceMask = XAUDIO2_LOG_WARNINGS;
	debugConfig.TraceMask = debugConfig.TraceMask | XAUDIO2_LOG_FUNC_CALLS | XAUDIO2_LOG_DETAIL;
	debugConfig.BreakMask = XAUDIO2_LOG_ERRORS;
	XAudio2Interface->SetDebugConfiguration(&debugConfig);

	//エンドポイントボイスの作成(BGM)
	if (FAILED(XAudio2Interface->CreateSubmixVoice(
		&EndpointVoice[ENDPOINT_VOICE_LIST::ENDPOINT_BGM],	//サブミックスボイス
		ENDPOINT_INPUT_CHANNEL,								//チャンネル数(入力)
		ENDPOINT_SAMPLE_RATE,								//サンプリングレート(入力)
		XAUDIO2_VOICE_USEFILTER,								//フィルター機能
		NULL,												//意味ないもの
		NULL)))												//送信リスト(NULL:Mastering Voiceへの単一の出力となる)
	{														//エフェクトチェーン
		return;
	}
	//エンドポイントボイスの作成(SE)
	if (FAILED(XAudio2Interface->CreateSubmixVoice(
		&EndpointVoice[ENDPOINT_VOICE_LIST::ENDPOINT_SE],	//サブミックスボイス
		ENDPOINT_INPUT_CHANNEL,								//チャンネル数(入力)
		ENDPOINT_SAMPLE_RATE,								//サンプリングレート(入力)
		XAUDIO2_VOICE_USEFILTER,								//フィルター機能
		NULL,												//意味ないもの
		NULL,												//送信リスト(NULL:Mastering Voiceへの単一の出力となる)
		NULL)))												//エフェクトチェーン
	{
		return;
	}

	//エンドポイントボイスへの送信リストの作成
	SendDescriptor[ENDPOINT_VOICE_LIST::ENDPOINT_BGM] = { XAUDIO2_SEND_USEFILTER,EndpointVoice[ENDPOINT_VOICE_LIST::ENDPOINT_BGM] };
	SendList[ENDPOINT_VOICE_LIST::ENDPOINT_BGM] = { 1,&SendDescriptor[ENDPOINT_VOICE_LIST::ENDPOINT_BGM] };
	SendDescriptor[ENDPOINT_VOICE_LIST::ENDPOINT_SE] = { XAUDIO2_SEND_USEFILTER,EndpointVoice[ENDPOINT_VOICE_LIST::ENDPOINT_SE] };
	SendList[ENDPOINT_VOICE_LIST::ENDPOINT_SE] = { 1,&SendDescriptor[ENDPOINT_VOICE_LIST::ENDPOINT_SE] };
}

//===================================================================================================================================
//【デストラクタ】
//===================================================================================================================================
SoundInterface::~SoundInterface()
{
	//SE
	SAFE_DELETE(SE);
	
	//BGM
	SAFE_DELETE(BGM);

	//エンドポイントボイス
	SAFE_DESTROY_VOICE(EndpointVoice[ENDPOINT_VOICE_LIST::ENDPOINT_BGM])
	SAFE_DESTROY_VOICE(EndpointVoice[ENDPOINT_VOICE_LIST::ENDPOINT_SE])

	//マスターボイス
	SAFE_DESTROY_VOICE(MasteringVoice);

	//インターフェース
	SAFE_RELEASE(XAudio2Interface);

	//COMライブラリ
	CoUninitialize();
}

//===================================================================================================================================
//【送信リストを取得する】
//引数:エンドポイントボイスのID
//===================================================================================================================================
XAUDIO2_VOICE_SENDS SoundInterface::GetSendList(int endpointVoiceId)
{
	return SendList[endpointVoiceId];
}

//===================================================================================================================================
//【XAudio2のインタフェースを取得する】
//===================================================================================================================================
IXAudio2 *SoundInterface::GetXAudio2Interface(void)
{
	return XAudio2Interface;
}

//===================================================================================================================================
//【シーンの更新】
//===================================================================================================================================
void SoundInterface::SwitchAudioBuffer(int scene)
{
	//シーンの更新
	SEManager::SwitchAudioBuffer(scene);	
	BGMManager::SwitchAudioBuffer(scene);
}

//===================================================================================================================================
//【更新処理】
//===================================================================================================================================
void SoundInterface::UpdateSound(void)
{
	//SEの更新処理
	SE->updateSound();
	//BGMの更新処理
	BGM->updateSound();

	//ImGUI
#ifdef _DEBUG
	outputSoundGUI();
#endif
}

//===================================================================================================================================
//【ImGUIへの出力】
//===================================================================================================================================
void SoundInterface::outputSoundGUI(void)
{
#ifdef _DEBUG
	ImGui::Begin("SoundInformation");
	
	//SE
	SE->outputSEGUI();
	BGM->outputBGMGUI();

	ImGui::End();
#endif
}

//===================================================================================================================================
//【再生】
//===================================================================================================================================
void SoundInterface::playSound(int endpointVoiceId, int soundId, bool loop)
{
	if (endpointVoiceId == ENDPOINT_VOICE_LIST::ENDPOINT_BGM)
	{
		BGM->playSound(endpointVoiceId,soundId, loop);
	}
	else if (endpointVoiceId == ENDPOINT_VOICE_LIST::ENDPOINT_SE)
	{
		//SE
		SE->playSound(endpointVoiceId,soundId, loop);
	}
}

//===================================================================================================================================
//【停止】
//===================================================================================================================================
void SoundInterface::stopSound(int endpointVoiceId, int soundId, bool loop)
{
	if (endpointVoiceId == ENDPOINT_VOICE_LIST::ENDPOINT_BGM)
	{
		BGM->stopSound(endpointVoiceId, soundId, loop);
	}
	else if (endpointVoiceId == ENDPOINT_VOICE_LIST::ENDPOINT_SE)
	{
		//SE
		SE->stopSound(endpointVoiceId, soundId, loop);
	}
}