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
#if(_MSC_VER >= GAME_MSC_VER)
IXAudio2 *SoundInterface::XAudio2Interface = nullptr;
SEManager *SoundInterface::SE = nullptr;
BGMManager *SoundInterface::BGM = nullptr;
#endif

//===================================================================================================================================
//【コンストラクタ】
//・インタフェースの準備
//・マスターボイスの作成
//・エンドポイントボイスの作成
//・エンドポイントへの送信リストの作成
//===================================================================================================================================
SoundInterface::SoundInterface()
{
#if(_MSC_VER >= GAME_MSC_VER)
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

	//マネージャー
	SE = new SEManager();
	BGM = new BGMManager();

#endif
}

//===================================================================================================================================
//【デストラクタ】
//===================================================================================================================================
SoundInterface::~SoundInterface()
{
#if(_MSC_VER >= GAME_MSC_VER)
	//SE
	SAFE_DELETE(SE);
	
	//BGM
	SAFE_DELETE(BGM);

	//マスターボイス
	SAFE_DESTROY_VOICE(MasteringVoice);

	//インターフェース
	SAFE_RELEASE(XAudio2Interface);

	//COMライブラリ
	CoUninitialize();
#endif
}

//===================================================================================================================================
//【XAudio2のインタフェースを取得する】
//===================================================================================================================================
#if(_MSC_VER >= GAME_MSC_VER)
IXAudio2 *SoundInterface::GetXAudio2Interface(void)
{
	return XAudio2Interface;
}
#endif

//===================================================================================================================================
//【シーンの更新】
//===================================================================================================================================
void SoundInterface::SwitchAudioBuffer(int scene)
{
#if(_MSC_VER >= GAME_MSC_VER)
	//シーンの更新
	BGM->SwitchAudioBuffer(scene);
	SE->SwitchAudioBuffer(scene);
#endif
}

//===================================================================================================================================
//【更新処理】
//===================================================================================================================================
void SoundInterface::UpdateSound(void)
{
#if(_MSC_VER >= GAME_MSC_VER)
	//SEの更新処理
	SE->updateSound();

	//BGMの更新処理
	BGM->updateSound();
	BGM->SetSpeed();

	//ImGUI
#ifdef _DEBUG
	outputSoundGUI();
#endif
#endif
}

//===================================================================================================================================
//【ImGUIへの出力】
//===================================================================================================================================
void SoundInterface::outputSoundGUI(void)
{
#ifdef _DEBUG
#if(_MSC_VER >= GAME_MSC_VER)
	ImGui::Begin("SoundInformation");
	
	//SE
	SE->outputGUI();
	BGM->outputGUI();

	ImGui::End();
#endif
#endif
}
