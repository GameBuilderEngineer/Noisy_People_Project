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
#if(XADUIO2_STATE)
IXAudio2 *SoundInterface::XAudio2Interface = nullptr;
#endif
SEManager *SoundInterface::SE = nullptr;
BGMManager *SoundInterface::BGM = nullptr;
S3DManager *SoundInterface::S3D = nullptr;

//===================================================================================================================================
//【コンストラクタ】
//・インタフェースの準備
//・マスターボイスの作成
//・エンドポイントボイスの作成
//・エンドポイントへの送信リストの作成
//===================================================================================================================================
SoundInterface::SoundInterface()
{
#if(XADUIO2_STATE)
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
	S3D = new S3DManager();
#endif
}

//===================================================================================================================================
//【デストラクタ】
//===================================================================================================================================
SoundInterface::~SoundInterface()
{
#if(XADUIO2_STATE)
	//SE
	SAFE_DELETE(SE);
	
	//BGM
	SAFE_DELETE(BGM);
	
	//S3D
	SAFE_DELETE(S3D);

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
#if(XADUIO2_STATE)
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
#if(XADUIO2_STATE)
	//シーンの更新
	BGM->SwitchAudioBuffer(scene);
	SE->SwitchAudioBuffer(scene);
	S3D->SwitchAudioBuffer(scene);
#endif
}

//===================================================================================================================================
//【更新処理】
//===================================================================================================================================
void SoundInterface::UpdateSound(void)
{
#if(XADUIO2_STATE)
	//SEの更新処理
	SE->updateSound();

	//BGMの更新処理
	BGM->updateSound();
	BGM->SetSpeed();

	//S3Dの更新処理
	S3D->updateSound();

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
#if(XADUIO2_STATE)
	ImGui::Begin("SoundInformation");
	
	//SE
	SE->outputGUI();
	BGM->outputGUI();
	S3D->outputGUI();

	ImGui::End();
#endif
#endif
}
