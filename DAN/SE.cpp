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
//【ImGUIへの出力】
//===================================================================================================================================
#ifdef _DEBUG
void SEManager::outputGUI(void)
{
#if(XADUIO2_STATE)
	if (!ImGui::CollapsingHeader("SEInformation"))
	{
		//使用中のバッファ数
		ImGui::Text("Number of buffers:%d", bufferMax);
		
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
				//ボイスID
				ImGui::Text("Voice ID:%d", tmpSoundParameters->playParameters.voiceID);

				//サウンド名
				switch (scene)
				{
				case SceneList::TITLE:
					ImGui::Text("%s", SE_PATH_LIST_TAIL(title,[tmpSoundParameters->playParameters.soundId]));
					break;
				case SceneList::TUTORIAL:
					ImGui::Text("%s", SE_PATH_LIST_TAIL(tutorial, [tmpSoundParameters->playParameters.soundId]));
					break;
				case SceneList::CREDIT:
					ImGui::Text("%s", SE_PATH_LIST_TAIL(credit, [tmpSoundParameters->playParameters.soundId]));
					break;
				case SceneList::GAME:
					ImGui::Text("%s", SE_PATH_LIST_TAIL(game,[tmpSoundParameters->playParameters.soundId]));
					break;
				case SceneList::RESULT:
					ImGui::Text("%s", SE_PATH_LIST_TAIL(result, [tmpSoundParameters->playParameters.soundId]));
					break;
				default:
					break;
				}

				//波形の描画
				int saveDataMax = 5512;	//取得するデータ数
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

						fData[wtPos] += ((float)tmpBuffer->wavFile.data.waveData[((playPoint - j) * tmpBuffer->wavFile.fmt.fmtChannel) - k] / (float)tmpBuffer->wavFile.fmt.fmtChannel);
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
#endif

//===================================================================================================================================
//【ステージ遷移に合わせて必要なサウンドバッファを用意する】
//===================================================================================================================================
void SEManager::SwitchAudioBuffer(int scene)
{
#if(XADUIO2_STATE)
	//サウンドディレクトリに設定する
	setSoundDirectory(ENDPOINT_VOICE_LIST::ENDPOINT_SE);

#if _DEBUG
	//シーンの更新
	SEManager::scene = scene;
#endif

	//解放処理
	for (int i = 0; i < SEManager::bufferMax; i++)
	{
		SAFE_DELETE_ARRAY(SEManager::bufferList[i].wavFile.data.waveData);
	}
	SAFE_DELETE_ARRAY(SEManager::bufferList);

	switch (scene)
	{
	case SceneList::SPLASH:
		bufferMax = SPLASH_SE_LIST::SPLASH_SE_MAX;
		break;

	case SceneList::TITLE:
		SEManager::bufferList = new LIST_BUFFER[TITLE_SE_LIST::TITLE_SE_MAX];
		for (int i = 0; i < TITLE_SE_LIST::TITLE_SE_MAX; i++)
		{
			SEManager::bufferList[i].buffer = { 0 };

			FILE *fp = nullptr;
			fp = fopen(SE_PATH_LIST_TAIL(title, [i]), "rb");
			SEManager::bufferList[i].wavFile = LoadWavChunk(fp);

			SEManager::bufferList[i].buffer.pAudioData = (BYTE*)SEManager::bufferList[i].wavFile.data.waveData;
			SEManager::bufferList[i].buffer.AudioBytes = SEManager::bufferList[i].wavFile.data.waveSize;
			SEManager::bufferList[i].buffer.Flags = XAUDIO2_END_OF_STREAM;

			fclose(fp);
		}
		bufferMax = TITLE_SE_LIST::TITLE_SE_MAX;
		break;

	case SceneList::TUTORIAL:
		SEManager::bufferList = new LIST_BUFFER[TUTORIAL_SE_LIST::TUTORIAL_SE_MAX];
		for (int i = 0; i < TUTORIAL_SE_LIST::TUTORIAL_SE_MAX; i++)
		{
			SEManager::bufferList[i].buffer = { 0 };

			FILE *fp = nullptr;
			fp = fopen(SE_PATH_LIST_TAIL(tutorial, [i]), "rb");
			SEManager::bufferList[i].wavFile = LoadWavChunk(fp);

			SEManager::bufferList[i].buffer.pAudioData = (BYTE*)SEManager::bufferList[i].wavFile.data.waveData;
			SEManager::bufferList[i].buffer.AudioBytes = SEManager::bufferList[i].wavFile.data.waveSize;
			SEManager::bufferList[i].buffer.Flags = XAUDIO2_END_OF_STREAM;

			fclose(fp);
		}
		bufferMax = TUTORIAL_SE_LIST::TUTORIAL_SE_MAX;
		break;

	case SceneList::CREDIT:
		SEManager::bufferList = new LIST_BUFFER[CREDIT_SE_LIST::CREDIT_SE_MAX];
		for (int i = 0; i < CREDIT_SE_LIST::CREDIT_SE_MAX; i++)
		{
			SEManager::bufferList[i].buffer = { 0 };

			FILE *fp = nullptr;
			fp = fopen(SE_PATH_LIST_TAIL(credit, [i]), "rb");
			SEManager::bufferList[i].wavFile = LoadWavChunk(fp);

			SEManager::bufferList[i].buffer.pAudioData = (BYTE*)SEManager::bufferList[i].wavFile.data.waveData;
			SEManager::bufferList[i].buffer.AudioBytes = SEManager::bufferList[i].wavFile.data.waveSize;
			SEManager::bufferList[i].buffer.Flags = XAUDIO2_END_OF_STREAM;

			fclose(fp);
		}
		bufferMax = CREDIT_SE_LIST::CREDIT_SE_MAX;
		break;

	case SceneList::GAME:
		SEManager::bufferList = new LIST_BUFFER[GAME_SE_LIST::GAME_SE_MAX];
		for (int i = 0; i < GAME_SE_LIST::GAME_SE_MAX; i++)
		{
			SEManager::bufferList[i].buffer = { 0 };

			FILE *fp = nullptr;
			fp = fopen(SE_PATH_LIST_TAIL(game, [i]), "rb");
			SEManager::bufferList[i].wavFile = LoadWavChunk(fp);

			SEManager::bufferList[i].buffer.pAudioData = (BYTE*)SEManager::bufferList[i].wavFile.data.waveData;
			SEManager::bufferList[i].buffer.AudioBytes = SEManager::bufferList[i].wavFile.data.waveSize;
			SEManager::bufferList[i].buffer.Flags = XAUDIO2_END_OF_STREAM;

			fclose(fp);
		}
		bufferMax = GAME_SE_LIST::GAME_SE_MAX;
		break;

	case SceneList::RESULT:	
		SEManager::bufferList = new LIST_BUFFER[RESULT_SE_LIST::RESULT_SE_MAX];
		for (int i = 0; i < RESULT_SE_LIST::RESULT_SE_MAX; i++)
		{
			SEManager::bufferList[i].buffer = { 0 };

			FILE *fp = nullptr;
			fp = fopen(SE_PATH_LIST_TAIL(result, [i]), "rb");
			SEManager::bufferList[i].wavFile = LoadWavChunk(fp);

			SEManager::bufferList[i].buffer.pAudioData = (BYTE*)SEManager::bufferList[i].wavFile.data.waveData;
			SEManager::bufferList[i].buffer.AudioBytes = SEManager::bufferList[i].wavFile.data.waveSize;
			SEManager::bufferList[i].buffer.Flags = XAUDIO2_END_OF_STREAM;

			fclose(fp);
		}
		bufferMax = RESULT_SE_LIST::RESULT_SE_MAX;
		break;

	case SceneList::CREATE:
		bufferMax = CREATE_SE_LIST::CREATE_SE_MAX;
		break;

	case SceneList::NONE_SCENE:
		bufferMax = 0;
		break;

	default:
		break;
	}
#endif
}