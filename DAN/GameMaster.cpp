//===================================================================================================================================
//yGameMaster.cppz
// [ì¬Ò]HAL“Œ‹GP12A332 11 ›–ì ÷
// [ì¬“ú]2019/09/20
// [XV“ú]2019/10/25
//===================================================================================================================================
#include "GameMaster.h"
#include "ImguiManager.h"
#include "Input.h"

//===================================================================================================================================
//y–¼‘O‹óŠÔz
//===================================================================================================================================
namespace gameMasterNS
{
	Input* input = NULL;
}

//===================================================================================================================================
//yusingéŒ¾z
//===================================================================================================================================
using namespace gameMasterNS;

//===================================================================================================================================
//yƒRƒ“ƒXƒgƒ‰ƒNƒ^z
//===================================================================================================================================
GameMaster::GameMaster()
{
	gameTimer			= 0.0f;								//ƒQ[ƒ€ŠÔ
	countDownTimer		= 0.0f;								//ƒJƒEƒ“ƒgƒ_ƒEƒ“ŠÔ
	for (int i = 0; i < gameMasterNS::PLAYER_NUM; i++)
	{
		ZeroMemory(&playerInformation[i], sizeof(PlayerTable));
	}
	treeNum				= 0;
	//conversionOrder		= NULL;
	input				= getInput();
	gameTimerStop		= false;
#ifdef _DEBUG
	showGUI = true;
#endif // _DEBUG
}

//===================================================================================================================================
//yƒfƒXƒgƒ‰ƒNƒ^z
//===================================================================================================================================
GameMaster::~GameMaster()
{
	//SAFE_DELETE_ARRAY(conversionOrder);
}

//===================================================================================================================================
//y‰Šú‰»z
//===================================================================================================================================
void GameMaster::initialize()
{

}

//===================================================================================================================================
//yXVz
//===================================================================================================================================
void GameMaster::update(float frameTime)
{
	//ƒQ[ƒ€’†‚È‚ç‚ÎAƒ|[ƒYØ‚è‘Ö‚¦ó•t
	if (gameTimer > 0)
	{
		//ƒXƒ^[ƒgƒ{ƒ^ƒ“/‚oƒL[‚ğ“ü—Í‚³‚ê‚½ê‡ƒ|[ƒY‚É‚·‚éB
		if (input->wasKeyPressed('P') || 
			input->getController()[0]->wasButton(virtualControllerNS::SPECIAL_MAIN)||
			input->getController()[1]->wasButton(virtualControllerNS::SPECIAL_MAIN))
		{
			pause = !pause;
		}
#ifdef _DEBUG
		//ƒ^ƒbƒ`ƒpƒbƒh(SUB)ƒ{ƒ^ƒ“/QƒL[‚ğ“ü—Í‚³‚ê‚½ê‡ƒQ[ƒ€ƒ^ƒCƒ}[‚ğ’â~B
		if (input->wasKeyPressed('Q') ||
			input->getController()[0]->wasButton(virtualControllerNS::SPECIAL_SUB) ||
			input->getController()[1]->wasButton(virtualControllerNS::SPECIAL_SUB))
		{
			gameTimerStop = !gameTimerStop;
		}

#endif // _DEBUG


	}
}

//===================================================================================================================================
//yƒ|[ƒYˆ—z
//===================================================================================================================================
bool GameMaster::paused()
{
	return pause;
}

//===================================================================================================================================
//yƒQ[ƒ€ŠJnˆ—z
//===================================================================================================================================
void GameMaster::startGame()
{
	gameTimer		= GAME_TIME;
	gameTimerStop	= false;
	pause = false;
	progress = 0x00000000;
	discardConversionOrder();
}

//===================================================================================================================================
//yƒQ[ƒ€ŠÔ‚ÌXVz
//===================================================================================================================================
void GameMaster::updateGameTime(float frameTime)
{
	if (gameTimerStop)return;

	gameTimer -= frameTime;
}

//===================================================================================================================================
//yc‚èŠÔ1•ª‚ÌXVz
//===================================================================================================================================
bool GameMaster::playActionRamaining1Min()
{
	if (gameTimer > 60)return false;
	if (whetherAchieved(PASSING_REMAINING_ONE_MINUTE))return false;

	setProgress(PASSING_REMAINING_ONE_MINUTE);

	return true;
}


#pragma region conversionOrderFunction
//===================================================================================================================================
//y—Î‰»‚µ‚½–Ø‚Ì–{”‚ğ‹L˜^z
//===================================================================================================================================
void GameMaster::recordTreeTable(TreeTable treeTable)
{
	TreeTable data = treeTable;
	data.eventTime = GAME_TIME - gameTimer;
	treeTableList.insertFront(data);
	treeTableList.listUpdate();
}

//===================================================================================================================================
//y•ÏŠ·‡”Ô•Ï”‚ğ€”õ‚·‚éz
//===================================================================================================================================
void GameMaster::readyTreeTable() 
{

}

//===================================================================================================================================
//y•ÏŠ·‡”Ô•Ï”‚ğ”jŠü‚·‚éz
//===================================================================================================================================
void GameMaster::discardConversionOrder() {
	this->treeNum = 0;
	treeTableList.allClear();
}
#pragma endregion

//===================================================================================================================================
//ysetterz
//===================================================================================================================================
void GameMaster::setConversionOrder(int* newValue) {} // conversionOrder = newValue; };
void GameMaster::setProgress(int achievement) { progress |= achievement; }

//===================================================================================================================================
//ygetterz
//===================================================================================================================================
PlayerTable* GameMaster::getPlayerInfomation(){	return playerInformation;}	//ƒvƒŒƒCƒ„[î•ñ‚Ìæ“¾
float GameMaster::getGameTime() {return gameTimer;}							//ƒQ[ƒ€§ŒÀŠÔ‚Ìæ“¾
int GameMaster::getProgress() { return progress; }
bool GameMaster::whetherAchieved(int ahievement) { return progress & ahievement; }

//===================================================================================================================================
//yGUIz
//===================================================================================================================================
#ifdef _DEBUG
void GameMaster::createGUI()
{
	ImGui::Text("gameTime = %f",		gameTimer);
	ImGui::Text("countDownTimer = %f",	countDownTimer);
	ImGui::Text("TreeNum = %d",	treeNum);
	if (ImGui::CollapsingHeader("TreeTableList"))
	{
		ImGui::Text("listNum: %d ", treeTableList.nodeNum);
		
		for (int i = 0; i < treeTableList.nodeNum; i++)
		{
			if (ImGui::CollapsingHeader("TreeTable"))
			{
				TreeTable table = *treeTableList.getValue(i);
				ImGui::Text("No:[%d]",i);				
				ImGui::Text("TreeId = %d",table.id);
				ImGui::Text("PlayerNo:%d",table.player+1);
				ImGui::Text("EventTime = %.02f",table.eventTime);
				ImGui::Text("model = %d",table.modelType);
				ImGui::Text("position(%.02f,%.02f,%.02f)",table.position.x,table.position.y,table.position.z);
				ImGui::Text("rotation(%.02f,%.02f,%.02f,%.02f)",table.rotation.x,table.rotation.y,table.rotation.z,table.rotation.w);
				ImGui::Text("scale(%.02f,%.02f,%.02f)",table.scale.x,table.scale.y,table.scale.z);
				switch (table.eventType)
				{
				case EVENT_TYPE::TO_DEAD:				ImGui::Text("EVENT:TO_DEAD");					break;
				case EVENT_TYPE::TO_GREEN_WITH_ANALOG:	ImGui::Text("EVENT:TO_GREEN_WITH_ANALOG");		break;
				case EVENT_TYPE::TO_GREEN_WITH_DIGITAL:	ImGui::Text("EVENT:TO_GREEN_WITH_DIGITAL");		break;
				}
			}
		}
	}
}
#endif