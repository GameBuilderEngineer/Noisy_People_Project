//===================================================================================================================================
//【SE.cpp】
// [作成者]HAL東京GP12A332 16 蔡 友剛
// [作成日]2019/10/04
// [更新日]2019/10/04
//===================================================================================================================================
#include "SE.h"
#include "Sound.h"
#include "ImguiManager.h"
#include "AbstractScene.h"

//===================================================================================================================================
//【グローバル変数】
//===================================================================================================================================
const char * const SEManager::splashSEPathList[] = { "SE_Attack.wav"};
const char * const SEManager::titleSEPathList[] = { "SE_Game_Start.wav","SE_Revival_Point.wav" };
const char * const SEManager::gameSEPathList[] = { "SE_Game_Start.wav","SE_Revival_Point.wav" };
int SEManager::SEScene = SceneList::SPLASH;

//===================================================================================================================================
//【デストラクタ】
//===================================================================================================================================
SEManager::~SEManager()
{
	SAFE_DELETE_ARRAY(SEBufferList);
}

//===================================================================================================================================
//【ImGUIへの出力】
//===================================================================================================================================
void SEManager::outputSEGUI(void)
{
#ifdef _DEBUG
	if (!ImGui::CollapsingHeader("SEInformation"))
	{
		//使用中のバッファ数
		ImGui::Text("Number of buffers:%d", SEBufferMax);
		
		//使用中のボイス数
		ImGui::Text("Number of voice:%d", soundParametersList->nodeNum - 1);

		for (int i = 0; i < soundParametersList->nodeNum - 1; i++)
		{
			//サウンドのパラメーター
			SOUND_PARAMETERS *tmpSoundParameters = soundParametersList->getValue(i);

			//バッファ
			LIST_BUFFER *tmpBuffer = GetBuffer(
				tmpSoundParameters->playParameters.endpointVoiceId,
				tmpSoundParameters->playParameters.soundId,
				tmpSoundParameters->playParameters.loop);

			//再生位置&情報取得
			XAUDIO2_VOICE_STATE state = { 0 };
			tmpSoundParameters->SourceVoice->GetState(&state);
			long playPoint = (long)state.SamplesPlayed;
			while (playPoint >= (int)(tmpBuffer->wavFile.data.waveSize / sizeof(short) / tmpBuffer->wavFile.fmt.fmtChannel))
			{
				playPoint -= (int)(tmpBuffer->wavFile.data.waveSize / sizeof(short) / tmpBuffer->wavFile.fmt.fmtChannel);
			}

			if (tmpSoundParameters->isPlaying)		//再生している
			{
				switch (SEScene)
				{
				case SceneList::SPLASH:
					ImGui::Text("%s", splashSEPathList[tmpSoundParameters->playParameters.soundId]);
					break;
				case SceneList::TITLE:
					ImGui::Text("%s", titleSEPathList[tmpSoundParameters->playParameters.soundId]);
					break;
				case SceneList::TUTORIAL:

					break;
				case SceneList::CREDIT:

					break;
				case SceneList::GAME:
					ImGui::Text("%s", gameSEPathList[tmpSoundParameters->playParameters.soundId]);
					break;
				case SceneList::RESULT:

					break;
				case SceneList::NONE_SCENE:

					break;
				default:
					break;
				}

				//波形の描画
				int saveDataMax = 11024;	//取得するデータ数
				int dataMax = (saveDataMax / tmpBuffer->wavFile.fmt.fmtChannel) + 2;	 //セーブしたいデータの数/チャンネル数 + 2
				float *fData = new (float[dataMax]);
				memset(fData, 0, sizeof(float)*dataMax);
				fData[0] = (float)SHRT_MIN;
				fData[dataMax - 1] = (float)SHRT_MAX;
				int wtPos = dataMax - 2;
				for (int j = saveDataMax - 1; j > 0; j -= tmpBuffer->wavFile.fmt.fmtChannel)
				{
					for (int k = 0; k < tmpBuffer->wavFile.fmt.fmtChannel; k++)
					{
						if (playPoint - j < 0)
						{
							fData[wtPos] = 0.0f;
							continue;
						}

						fData[wtPos] += ((float)tmpBuffer->wavFile.data.waveData[playPoint - j - k] / (float)tmpBuffer->wavFile.fmt.fmtChannel);
					}
					wtPos--;
				}
				ImVec2 plotextent(ImGui::GetContentRegionAvailWidth(), 100);
				ImGui::PlotLines("", fData, dataMax, 0, "Sound wave", FLT_MAX, FLT_MAX, plotextent);

				SAFE_DELETE_ARRAY(fData);

				//再生位置
				ImGui::ProgressBar(playPoint / (float)(tmpBuffer->wavFile.data.waveSize / sizeof(short) / tmpBuffer->wavFile.fmt.fmtChannel));
			}
		}
	}
#endif
}

//===================================================================================================================================
//【ステージ遷移に合わせて必要なサウンドバッファを用意する】
//===================================================================================================================================
void	 SEManager::SwitchAudioBuffer(int scene)
{
	//サウンドディレクトリに設定する
	setSoundDirectory(ENDPOINT_VOICE_LIST::ENDPOINT_SE);

	//シーンの更新
	SEManager::SEScene = scene;

	//解放処理
	for (int i = 0; i < SEManager::SEBufferMax; i++)
	{
		SAFE_DELETE_ARRAY(SEManager::SEBufferList[i].wavFile.data.waveData);
	}
	SAFE_DELETE_ARRAY(SEManager::SEBufferList);

	switch (SEManager::SEScene)
	{
	case SceneList::SPLASH:
		SEManager::SEBufferList = new LIST_BUFFER[SPLASH_SE_LIST::SPLASH_SE_MAX];
		for (int i = 0; i < SPLASH_SE_LIST::SPLASH_SE_MAX; i++)
		{
			SEManager::SEBufferList[i].buffer = { 0 };

			FILE *fp = nullptr;
			fp = fopen(SEManager::splashSEPathList[i], "rb");
			SEManager::SEBufferList[i].wavFile = LoadWavChunk(fp);

			SEManager::SEBufferList[i].buffer.pAudioData = (BYTE*)SEManager::SEBufferList[i].wavFile.data.waveData;
			SEManager::SEBufferList[i].buffer.AudioBytes = SEManager::SEBufferList[i].wavFile.data.waveSize;
			SEManager::SEBufferList[i].buffer.Flags = XAUDIO2_END_OF_STREAM;

			fclose(fp);
		}
		SEBufferMax = SPLASH_SE_LIST::SPLASH_SE_MAX;
		break;
	case SceneList::TITLE:
		SEManager::SEBufferList = new LIST_BUFFER[TITLE_SE_LIST::TITLE_SE_MAX];
		for (int i = 0; i < TITLE_SE_LIST::TITLE_SE_MAX; i++)
		{
			SEManager::SEBufferList[i].buffer = { 0 };

			FILE *fp = nullptr;
			fp = fopen(SEManager::titleSEPathList[i], "rb");
			SEManager::SEBufferList[i].wavFile = LoadWavChunk(fp);

			SEManager::SEBufferList[i].buffer.pAudioData = (BYTE*)SEManager::SEBufferList[i].wavFile.data.waveData;
			SEManager::SEBufferList[i].buffer.AudioBytes = SEManager::SEBufferList[i].wavFile.data.waveSize;
			SEManager::SEBufferList[i].buffer.Flags = XAUDIO2_END_OF_STREAM;

			fclose(fp);
		}
		SEBufferMax = TITLE_SE_LIST::TITLE_SE_MAX;
		break;
	case SceneList::TUTORIAL:
		break;
	case SceneList::CREDIT:
		break;
	case SceneList::GAME:
		SEManager::SEBufferList = new LIST_BUFFER[GAME_SE_LIST::GAME_SE_MAX];
		for (int i = 0; i < GAME_SE_LIST::GAME_SE_MAX; i++)
		{
			SEManager::SEBufferList[i].buffer = { 0 };

			FILE *fp = nullptr;
			fp = fopen(SEManager::gameSEPathList[i], "rb");
			SEManager::SEBufferList[i].wavFile = LoadWavChunk(fp);

			SEManager::SEBufferList[i].buffer.pAudioData = (BYTE*)SEManager::SEBufferList[i].wavFile.data.waveData;
			SEManager::SEBufferList[i].buffer.AudioBytes = SEManager::SEBufferList[i].wavFile.data.waveSize;
			SEManager::SEBufferList[i].buffer.Flags = XAUDIO2_END_OF_STREAM;

			fclose(fp);
		}
		SEBufferMax = GAME_SE_LIST::GAME_SE_MAX;
		break;
	case SceneList::RESULT:	
		break;
	case SceneList::NONE_SCENE:
		break;
	case SceneList::CREATE:
		SEBufferMax = CREATE_SE_LIST::CREATE_SE_MAX;
		break;
	default:
		break;
	}
}