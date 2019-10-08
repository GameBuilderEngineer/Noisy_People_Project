//===================================================================================================================================
//【InstancingBillboard.h】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/09/27
// [更新日]2019/10/08
//===================================================================================================================================
#pragma once

//===================================================================================================================================
//【インクルード】
//===================================================================================================================================
#include "Base.h"
#include "Object.h"
#include "LinkedList.h"

//===================================================================================================================================
//【インスタンシングビルボード頂点構造体】
//===================================================================================================================================
namespace InstancingBillboardNS {
	//===================================================================================================================================
	//【インスタンシングビルボード頂点構造体】
	//===================================================================================================================================
	struct Vertex
	{
		D3DXVECTOR2 coord;
		D3DXVECTOR2 uv;
	};

	//===================================================================================================================================
	//【インスタンスクラス】
	//===================================================================================================================================
	class Instance
	{
	public:
		D3DXVECTOR3		position;	//ポジション
		D3DXVECTOR3		rotation;	//回転
		D3DXVECTOR3		scale;		//サイズ
		D3DXVECTOR3		speed;		//サイズ
		D3DXVECTOR2		uv;			//UV
		D3DXCOLOR			color;		//カラー
		float existenceTimer;				//生存時間

		Instance() {
			ZeroMemory(this, sizeof(Instance));
			existenceTimer = 1.0f;
		}
		void update(float frameTime)
		{
			existenceTimer -= frameTime;
			if (existenceTimer <= 0)return;
			position += speed*frameTime;
			if (position.x > 500)position.x = -500;
			else if(position.x < -500)position.x = 500;
			if (position.y > 500)position.y = -500;
			else if(position.y < -500)position.y = 500;
			if (position.z > 500)position.z = -500;
			else if(position.z < -500)position.z = 500;
		}

	};
	//===================================================================================================================================
	//【インスタンスリストクラス】
	//===================================================================================================================================
	class InstanceList:public LinkedList<InstancingBillboardNS::Instance>
	{
	};
}


//===================================================================================================================================
//【インスタンシングビルボードクラス】
//===================================================================================================================================
class InstancingBillboard : public Base
{
protected:
	//Data
	LPDIRECT3DDEVICE9							device;					//DirectX9デバイス
	IDirect3DVertexDeclaration9*				declation;				//頂点宣言
	LPD3DXEFFECT									effect;					//シェーダーエフェクト
	bool													onRender;				//描画フラグ
	bool													didGenerate;			//生成フラグ
	bool													didDelete;				//削除フラグ

	//セットストリーム用バッファ
	LPDIRECT3DVERTEXBUFFER9				vertexBuffer;			//頂点バッファ
	LPDIRECT3DINDEXBUFFER9				indexBuffer;			//インデックスバッファ
	LPDIRECT3DVERTEXBUFFER9				positionBuffer;		//位置バッファ
	LPDIRECT3DVERTEXBUFFER9				colorBuffer;			//カラーバッファ
	LPDIRECT3DVERTEXBUFFER9				uvBuffer;				//UVバッファ

	//copy用配列
	D3DXVECTOR3*									position;				//位置配列
	D3DXCOLOR*									color;					//カラー配列
	D3DXVECTOR2*									uv;						//UV配列

	//共通テクスチャ
	LPDIRECT3DTEXTURE9						texture;					//テクスチャポインタ
	float													textureHight;			//テクスチャの高さ
	float													textureWidth;			//テクスチャの幅

	//インスタンスリスト
	InstancingBillboardNS::InstanceList*	instanceList;			//インスタンスリスト

public:
	//Method
	InstancingBillboard();
	~InstancingBillboard();

	//基本関数
	virtual HRESULT initialize(LPD3DXEFFECT effect,LPDIRECT3DTEXTURE9 texture);							//初期化
	void update(float frameTime);																										//更新
	virtual void render(D3DXMATRIX view, D3DXMATRIX projection, D3DXVECTOR3 cameraPositon);	//描画

	//描画をオフにする
	void offRender();

	//リスト操作
	void generateInstance(InstancingBillboardNS::Instance newInstance);		//描画するインスタンスを生成
	void deleteInstance(int instanceNo);														//描画するインスタンスを削除
	void updateAccessList();																			//ランダムアクセス用配列の更新：インスタンスの生成または削除を行った時に更新を行う。

	//各バッファ値更新
	void updatePosition();																				//位置バッファを更新する
	void updateUV();																					//UVバッファを更新する
	void updateColor();																				//カラーバッファを更新する

	//バッファのリソース更新
	void updateBuffer();
	//copy用配列のメモリ更新
	void updateArray();

	//getter
	int getInstanceNum();																			//インスタンスの数を取得する
	InstancingBillboardNS::InstanceList getList();											//リストを取得する

};
