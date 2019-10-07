//===================================================================================================================================
//【SE.cpp】
// [作成者]HAL東京GP12A332 16 蔡 友剛
// [作成日]2019/10/04
// [更新日]2019/10/04
//===================================================================================================================================
#include "SE.h"
#include "AbstractScene.h"

//===================================================================================================================================
//【グローバル変数】
//===================================================================================================================================
const char * const SEManager::titleSEPathList[] = { "BGM_Character_Select.wav","BGM_Credit.wav" };

//===================================================================================================================================
//【デストラップ】
//===================================================================================================================================
SEManager::~SEManager()
{
	SAFE_DELETE_ARRAY(bufferList)
}

//===================================================================================================================================
//【ステージ遷移に合わせて必要なサウンドバッファを用意する】
//===================================================================================================================================
void	 SEManager::SwitchAudioBuffer(int scene)
{
	//サウンドディレクトリに設定する
	setSoundDirectory();

	SAFE_DELETE_ARRAY(bufferList)

	switch (scene)
	{
	case SceneList::SPLASH:
		break;
	case SceneList::TITLE:
		SEManager::bufferList = new LIST_BUFFER[TITLE_SE_LIST::SE_MAX];
		for (int i = 0; i < TITLE_SE_LIST::SE_MAX; i++)
		{
			memset(&SEManager::bufferList[i].buffer, 0, sizeof(XAUDIO2_BUFFER));

			FILE *fp = nullptr;
			fp = fopen(SEManager::titleSEPathList[i], "rb");
			SEManager::bufferList[i].wavFile = LoadWavChunk(fp);

			SEManager::bufferList[i].buffer.pAudioData = (BYTE*)SEManager::bufferList[i].wavFile.data.waveData;
			SEManager::bufferList[i].buffer.AudioBytes = SEManager::bufferList[i].wavFile.data.waveSize;
			SEManager::bufferList[i].buffer.Flags = XAUDIO2_END_OF_STREAM;

			fclose(fp);
		}
		bufferMax = TITLE_SE_LIST::SE_MAX;
		break;
	case SceneList::TUTORIAL:
		break;
	case SceneList::OPERATION:
		break;
	case SceneList::CREDIT:
		break;
	case SceneList::GAME:
		break;
	case SceneList::RESULT:	
		break;
	case SceneList::NONE_SCENE:
		break;
	default:
		break;
	}
}