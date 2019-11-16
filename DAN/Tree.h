//-----------------------------------------------------------------------------
// ツリー基底クラス [Tree.h]
// Author : HAL東京昼間部 2年制ゲーム学科 GP12A332 32 中込和輝
// 作成開始日 : 2019/10/13
//-----------------------------------------------------------------------------
// 更新日 : 2019/11/12 【菅野 樹】
//-----------------------------------------------------------------------------
#pragma once
#include "Object.h"
#include "BoundingSphere.h"
#include "GreeningArea.h"

//=============================================================================
// 名前空間
//=============================================================================
namespace treeNS
{
	// ツリー種別
	enum TREE_TYPE
	{
		ANALOG_TREE,
		DIGITAL_TREE,
		TREE_TYPE_MAX
	};

	// 緑化状態
	enum GREEN_STATE
	{
		GREEN,
		DEAD,
		GREEN_STATE_MAX
	};

	// サイズ
	enum TREE_SIZE
	{
		STANDARD,
		LARGE,
		VERY_LARGE,
		TREE_SIZE_MAX
	};

	// モデル
	enum TREE_MODEL
	{
		A_MODEL,
		B_MODEL,
		C_MODEL,
		TREE_MAX
	};

	//ツリーデータ
	typedef struct TreeData
	{
		int treeID;						// 識別番号
		int model	;					// モデルID
		TREE_TYPE type;					// ツリータイプ
		GREEN_STATE greenState;			// 緑化状態
		TREE_SIZE size;					// 木のサイズ
		int hp;							// HP
		bool isAttaked;					// エネミーが攻撃中
		int digitalHp;					// A/D変換後エネミーが削るHP
		D3DXVECTOR3 initialPosition;	// 初期座標
		D3DXVECTOR3 initialDirection;	// 初期方角
		void zeroClear() { ZeroMemory(this, sizeof(TreeData)); }
	} TREESET;

	//定数
	const int	MAX_HP					= 100;		//最大HP
	const float	AROUND_GREEN_TIME		= 5.0f;		//周辺への緑化時間
	const float	AROUND_GREEN_RANGE_S	= 50.0f;	//周辺への緑化範囲
	const float	AROUND_GREEN_RANGE_L	= 100.0f;	//周辺への緑化範囲
	const float	AROUND_GREEN_RANGE_V	= 200.0f;	//周辺への緑化範囲


	//=============================================================================
	//【抽象状態クラス】
	//=============================================================================
	class AbstractState
	{
	public:
		float	frameTime;
		int		type;
	public:
		virtual void start() = 0;
		virtual void update(float frameTime) = 0;
		virtual void end() = 0;
		virtual AbstractState* transition() = 0;
	};
}


//=============================================================================
//【ツリークラス】
//=============================================================================
class Tree:public Object
{
private:
	treeNS::TreeData		treeData;			// ツリーデータ
	Object					leaf;				// 葉オブジェクト
	GreeningArea			greeningArea;		// 緑化範囲
	LPD3DXMESH				attractorMesh;		// 重力（引力）発生メッシュ
	D3DXMATRIX*				attractorMatrix;	// 重力（引力）発生オブジェクトマトリックス
	static int				numOfTree;			// ツリーオブジェクトの総数
	treeNS::AbstractState*	state;				//状態クラス
	bool					onTransState;		//状態遷移開始フラグ
	bool					nowAroundGreening;	//現在周囲を緑化中
public:
	Tree(treeNS::TreeData _treeData);
	~Tree();
	//基本処理
	void update(float frameTime);
	void render();

	void setAttractor(LPD3DXMESH _attractorMesh, D3DXMATRIX* _attractorMatrix);
	void grounding();					// 接地処理
	void greeningAround();				//周辺の緑化(デジタル化時)
	void transState();					//状態遷移

	// Getter
	Object* getLeaf();
	Object* getTrunk();
	Object* getGreeningArea();
	static int getNumOfTree();			// ツリーの数を取得
	treeNS::TreeData* getTreeData();	// ツリーデータを取得
	LPD3DXMESH getMesh();				// 衝突用メッシュへの参照
	bool isAroundGreening();			//周囲へ緑化中か

	// Setter
	void setDataToTree(treeNS::TreeData _treeData);	
	void addHp(int value);							//デジタル化するHPの増加
	void setGreeningArea(float value);				//緑化エリアのスケールを設定
	void disableAroundGreening();					//周囲への緑化を終了

};

//=============================================================================
//【名前空間】
//=============================================================================
namespace treeNS
{
	//=============================================================================
	//【デジタル状態クラス】
	//=============================================================================
	class DigitalState :public AbstractState
	{
	private:
		Tree* tree;
		float aroundGreenTimer;//周囲への緑化タイマー
		float aroundGreenRange;
	public:
		DigitalState(Tree* target);
		~DigitalState();

		virtual void start();
		virtual void update(float frameTime);
		virtual void end();
		virtual AbstractState* transition();
	};

	//=============================================================================
	//【アナログ状態クラス】
	//=============================================================================
	class AnalogState :public AbstractState
	{
	private:
		Tree* tree;
	public:
		AnalogState(Tree* target);
		~AnalogState();

		virtual void start();
		virtual void update(float frameTime);
		virtual void end();
		virtual AbstractState* transition();
	};

	//=============================================================================
	//【緑化状態クラス】
	//=============================================================================
	class GreenState :public AbstractState
	{
	private:
		Tree* tree;
	public:
		GreenState(Tree* target);
		~GreenState();

		virtual void start();
		virtual void update(float frameTime);
		virtual void end();
		virtual AbstractState* transition();
	};

}