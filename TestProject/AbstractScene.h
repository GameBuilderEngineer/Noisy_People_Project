//===================================================================================================================================
//【AbstractScene.h】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/09/19
// [更新日]2019/10/06
//===================================================================================================================================
#pragma once

//===================================================================================================================================
//【インクルード】
//===================================================================================================================================
#include <string>
#include "Base.h"
#include "Direct3D9.h"
#include "Input.h"
#include "ImguiManager.h"
#include "Camera.h"
#include "Light.h"
#include "Sound.h"
#include "TextureLoader.h"
#include "StaticMeshLoader.h"
//#include "ShaderLoader.h"
//#include "TextManager.h"
//#include "GameMaster.h"
//#include "AnimationLoader.h"

//===================================================================================================================================
//【名前空間】
//===================================================================================================================================
//シーンリスト
namespace SceneList
{
	enum {
		NONE_SCENE = -1,
		SPLASH,			//スプラッシュ
		TITLE,				//タイトル
		TUTORIAL,			//チュートリアル
		CREDIT,			//クレジット
		GAME,				//ゲーム
		RESULT,			//リザルト
	};
}

//===================================================================================================================================
//【基底シーンクラス：抽象インターフェース】
//各シーンの継承元クラス
//===================================================================================================================================
class AbstractScene :public Base
{
protected:
	//Data
	LPDIRECT3DDEVICE9 device;
	Direct3D9* direct3D9;
	Input* input;
	Camera* camera;
	Light* light;
	SOUND* sound;
	//TextureLoader* textureLoader;
	//StaticMeshLoader* staticMeshLoader;
	//ShaderLoader* shaderLoader;
	//AnimationLoader* animationLoader;
	//TextManager* textManager;
	//GameMaster* gameMaster;
	float sceneTimer;
	float frameTime = 0.0f;//フレームタイム保存変数
	float stopTimer = 0.0f;	//処理落ち時間
	bool onChange;
	int nextScene;	
	std::string sceneName;
#ifdef _DEBUG
	bool showGUI;
#endif // _DEBUG

public:
	//VirtualMethod
	AbstractScene();										//コンストラクタ
	virtual ~AbstractScene();							//デストラクタ
	virtual void initialize() = 0;							//初期化
	virtual void uninitialize() = 0;						//終了処理
	virtual void update(float frameTime) = 0;		//更新
	virtual void render() = 0;							//描画
	virtual void collisions() = 0;							//衝突処理
	virtual void AI() = 0;									//AI処理
#ifdef _DEBUG
	virtual void createGUI() = 0;						//GUI処理
	bool* getShowGUI() { return &showGUI; };
#endif // _DEBUG

	//Method
	void changeScene(int i) { onChange = true; nextScene = i; };		//シーン遷移処理関数
	bool checkChangeOrder() { return onChange; };							//シーン遷移要求関数
	int checkNextScene() { return nextScene; }									//次シーン参照関数
	std::string* getSceneName() { return &sceneName; }					//シーン名取得関数
	//void copyGameMaster(GameMaster* destination);						//
	//void setGameMaster(GameMaster* resorce);								//
	//void setAnimationLoader(AnimationLoader* animationLoader);	//
};
