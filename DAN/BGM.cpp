//===================================================================================================================================
//【BGM.cpp】
// [作成者]HAL東京GP12A332 16 蔡 友剛
// [作成日]2019/10/14
// [更新日]2019/10/14
//===================================================================================================================================
#include "BGM.h"
#include "Sound.h"
#include "ImguiManager.h"
#include "AbstractScene.h"

//===================================================================================================================================
//【グローバル変数】
//===================================================================================================================================
const char * const BGMManager::splashBGMPathList[] = { "BGM_Splash.wav" };
const char * const BGMManager::titleBGMPathList[] = { "BGM_Title.wav" };
const char * const BGMManager::gameBGMPathList[] = { "BGM_Game.wav" };
int BGMManager::BGMScene = SceneList::SPLASH;
float BGMManager::startTime = 0;
float BGMManager::currentTime = 0;

//===================================================================================================================================
//【コンストラクタ】
//===================================================================================================================================
BGMManager::BGMManager()
{

}

//===================================================================================================================================
//【デストラクタ】
//===================================================================================================================================
BGMManager::~BGMManager()
{
	SAFE_DELETE_ARRAY(BGMBufferList);
}

//===================================================================================================================================
//【再生速度の設定】
//===================================================================================================================================
void	 BGMManager::SetSpeed(float speed)
{
	//ターゲット情報
	SOUND_PARAMETERS targetSoundParameters;
	targetSoundParameters.playParameters.endpointVoiceId = ENDPOINT_VOICE_LIST::ENDPOINT_BGM;
	targetSoundParameters.playParameters.soundId = GAME_BGM_LIST::GAME_BGM_01;
	targetSoundParameters.playParameters.loop = true;

	if (BGMScene == SceneList::GAME)
	{
		for (int i = 0; i < soundParametersList->nodeNum - 1; i++)
		{
			SOUND_PARAMETERS *tmpSoundParameters = soundParametersList->getValue(i);
			if ((tmpSoundParameters->playParameters.endpointVoiceId == targetSoundParameters.playParameters.endpointVoiceId) &&
				(tmpSoundParameters->playParameters.soundId == targetSoundParameters.playParameters.soundId) &&
				(tmpSoundParameters->isSpeed))
			{
				//バッファ
				LIST_BUFFER *tmpBuffer = GetBuffer(targetSoundParameters.playParameters.endpointVoiceId,
					targetSoundParameters.playParameters.soundId,
					targetSoundParameters.playParameters.loop);

				//停止処理
				tmpSoundParameters->SourceVoice->Stop(XAUDIO2_PLAY_TAILS);
				XAUDIO2_VOICE_STATE state = { 0 };
				tmpSoundParameters->SourceVoice->Discontinuity();
				tmpSoundParameters->SourceVoice->FlushSourceBuffers();
				tmpSoundParameters->SourceVoice->GetState(&state);

				//キューに何でもないければ
				if (state.BuffersQueued == 0)
				{
					//再生速度
					tmpSoundParameters->SourceVoice->SetSourceSampleRate(
						tmpBuffer->wavFile.fmt.fmtSampleRate * tmpSoundParameters->playParameters.speed);

					//停止位置
					tmpSoundParameters->stopPoint = state.SamplesPlayed;
					while (tmpSoundParameters->stopPoint >= (int)(tmpBuffer->wavFile.data.waveSize/(int)sizeof(long)))
					{
						tmpSoundParameters->stopPoint -= (int)(tmpBuffer->wavFile.data.waveSize / (int)sizeof(long));
					}

					//再生位置
					tmpBuffer->buffer.PlayBegin = tmpSoundParameters->stopPoint;

					//バッファの提出
					tmpSoundParameters->SourceVoice->SubmitSourceBuffer(&tmpBuffer->buffer);
					
					//再生処理
					tmpSoundParameters->SourceVoice->Start();

					//再生速度
					tmpSoundParameters->isSpeed = false;
				}
			}			
		}
	}
}

//===================================================================================================================================
//【再生速度の設定(On)】
//===================================================================================================================================
void BGMManager::SetSpeedOn(void)
{

}

//===================================================================================================================================
//【ImGUIへの出力】
//===================================================================================================================================
void BGMManager::outputBGMGUI(void)
{
#ifdef _DEBUG
	if (!ImGui::CollapsingHeader("BGMInformation"))
	{
		ImGui::Text("Buffer List:%d", BGMBufferMax);

		ImGui::Text("Playing List:");
		for (int i = 0; i < soundParametersList->nodeNum - 1; i++)
		{
			XAUDIO2_VOICE_STATE state = { 0 };
			SOUND_PARAMETERS *tmpSoundParameters = soundParametersList->getValue(i);
			{
				if (tmpSoundParameters->isPlaying)		//再生している
				{
					switch (BGMScene)
					{
					case SceneList::SPLASH:
						tmpSoundParameters->SourceVoice->GetState(&state);
						ImGui::Text("%s", splashBGMPathList[tmpSoundParameters->playParameters.soundId]);
						ImGui::Text("%d", ((int)state.SamplesPlayed / 44100));
						break;
					case SceneList::TITLE:
						tmpSoundParameters->SourceVoice->GetState(&state);
						ImGui::Text("%s", titleBGMPathList[tmpSoundParameters->playParameters.soundId]);
						ImGui::Text("%d", ((int)state.SamplesPlayed / 44100));
						break;
					case SceneList::TUTORIAL:

						break;
					case SceneList::CREDIT:

						break;
					case SceneList::GAME:
						tmpSoundParameters->SourceVoice->GetState(&state);
						ImGui::Text("%s", gameBGMPathList[tmpSoundParameters->playParameters.soundId]);
						ImGui::Text("%d", ((int)state.SamplesPlayed/44100));

						ImGui::Checkbox("Speed Up卍", &tmpSoundParameters->isSpeed);
						break;
					case SceneList::RESULT:

						break;
					case SceneList::NONE_SCENE:

						break;
					default:
						break;
					}

				}
			}
		}
	}
#endif
}

//===================================================================================================================================
//【ステージ遷移に合わせて必要なサウンドバッファを用意する】
//===================================================================================================================================
void	 BGMManager::SwitchAudioBuffer(int scene)
{
	//サウンドディレクトリに設定する
	setSoundDirectory(ENDPOINT_VOICE_LIST::ENDPOINT_BGM);

	SAFE_DELETE_ARRAY(BGMBufferList);

	//シーンの更新
	BGMScene = scene;

	switch (BGMScene)
	{
	case SceneList::SPLASH:
		BGMManager::BGMBufferList = new LIST_BUFFER[SPLASH_BGM_LIST::SPLASH_BGM_MAX];
		for (int i = 0; i < SPLASH_BGM_LIST::SPLASH_BGM_MAX; i++)
		{
			memset(&BGMManager::BGMBufferList[i].buffer, 0, sizeof(XAUDIO2_BUFFER));

			FILE *fp = nullptr;
			fp = fopen(BGMManager::splashBGMPathList[i], "rb");
			BGMManager::BGMBufferList[i].wavFile = LoadWavChunk(fp);
			BGMManager::BGMBufferList[i].soundId = i;
			BGMManager::BGMBufferList[i].buffer.pAudioData = (BYTE*)BGMManager::BGMBufferList[i].wavFile.data.waveData;
			BGMManager::BGMBufferList[i].buffer.AudioBytes = BGMManager::BGMBufferList[i].wavFile.data.waveSize;
			BGMManager::BGMBufferList[i].buffer.Flags = XAUDIO2_END_OF_STREAM;

			fclose(fp);
		}
		BGMBufferMax = SPLASH_BGM_LIST::SPLASH_BGM_MAX;
		break;
	case SceneList::TITLE:
		BGMManager::BGMBufferList = new LIST_BUFFER[TITLE_BGM_LIST::TITLE_BGM_MAX];
		for (int i = 0; i < TITLE_BGM_LIST::TITLE_BGM_MAX; i++)
		{
			memset(&BGMManager::BGMBufferList[i].buffer, 0, sizeof(XAUDIO2_BUFFER));

			FILE *fp = nullptr;
			fp = fopen(BGMManager::titleBGMPathList[i], "rb");
			BGMManager::BGMBufferList[i].wavFile = LoadWavChunk(fp);
			BGMManager::BGMBufferList[i].soundId = i;
			BGMManager::BGMBufferList[i].buffer.pAudioData = (BYTE*)BGMManager::BGMBufferList[i].wavFile.data.waveData;
			BGMManager::BGMBufferList[i].buffer.AudioBytes = BGMManager::BGMBufferList[i].wavFile.data.waveSize;
			BGMManager::BGMBufferList[i].buffer.Flags = XAUDIO2_END_OF_STREAM;

			fclose(fp);
		}
		BGMBufferMax = TITLE_BGM_LIST::TITLE_BGM_MAX;
		break;
	case SceneList::TUTORIAL:
		break;
	case SceneList::CREDIT:
		break;
	case SceneList::GAME:
		BGMManager::BGMBufferList = new LIST_BUFFER[GAME_BGM_LIST::GAME_BGM_MAX];
		for (int i = 0; i < GAME_BGM_LIST::GAME_BGM_MAX; i++)
		{
			memset(&BGMManager::BGMBufferList[i].buffer, 0, sizeof(XAUDIO2_BUFFER));

			FILE *fp = nullptr;
			fp = fopen(BGMManager::gameBGMPathList[i], "rb");
			BGMManager::BGMBufferList[i].wavFile = LoadWavChunk(fp);
			BGMManager::BGMBufferList[i].soundId = i;
			BGMManager::BGMBufferList[i].buffer.pAudioData = (BYTE*)BGMManager::BGMBufferList[i].wavFile.data.waveData;
			BGMManager::BGMBufferList[i].buffer.AudioBytes = BGMManager::BGMBufferList[i].wavFile.data.waveSize;
			BGMManager::BGMBufferList[i].buffer.Flags = XAUDIO2_END_OF_STREAM;

			fclose(fp);
		}
		BGMBufferMax = GAME_BGM_LIST::GAME_BGM_MAX;
		break;
	case SceneList::RESULT:
		break;
	case SceneList::NONE_SCENE:
		break;
	default:
		break;
	}
}