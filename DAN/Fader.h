//===================================================================================================================================
//【Fader.h】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/09/20
// [更新日]2019/11/01
//===================================================================================================================================
#pragma once
//===================================================================================================================================
//【フェード処理概要】
//①フェードアウト
//②フェードアニメーションの再生
//③フェードイン
//④実行命令待機
//Start(④)-命令→①→②→③→④-命令→①・・・以降ループ
//===================================================================================================================================

//===================================================================================================================================
//【インクルード】
//===================================================================================================================================
#include "Base.h"
#include "Sprite.h"
#include "ShaderLoader.h"

//===================================================================================================================================
//【フェード頂点構造体】
//===================================================================================================================================
struct FADE_VERTEX
{
	float x, y, z;
	float u, v;
};

//===================================================================================================================================
//【フェーダー名前空間】
//===================================================================================================================================
namespace faderNS
{
	enum SHADER_TYPE
	{
		NORMAL,
		BLUR,
		MAX_NUM
	};

	enum STATE
	{
		FADE_IN,
		FADE_OUT,
		FADE_WAIT,
		FADE_PLAY,
		STATE_NUM
	};

	//抽象シェーダーインターフェース
	class ShaderState
	{
	public:
		LPD3DXEFFECT	effect;		//シェーダーエフェクト
		float			inTime;		//フェードイン時間
		float			outTime;	//フェードアウト時間
		float			waitTime;	//待機時間
		float			playTime;	//再生時間
	public:
		//コンストラクタ
		ShaderState()
		{
			effect = NULL;
		}
		//シェーダー開始
		void begin(UINT i){
			effect->Begin(NULL, 0);
			effect->BeginPass(i);
		}
		//シェーダー終了
		void end() {
			effect->EndPass();
			effect->End();
		};
		//シェーダーへ値をセット
		virtual void setValue(D3DSURFACE_DESC desc) = 0;
		//シェーダーの値の更新
		virtual void updateValue(float rate) = 0;
	};

	//ノーマルシェーダー
	class NormalShader:public ShaderState
	{
	public:
		LPDIRECT3DTEXTURE9	targetTexture;		//ターゲットテクスチャ
		float				alpha;				//α値
		//コンストラクタ
		NormalShader(LPDIRECT3DTEXTURE9 _targetTexture)
			:targetTexture(_targetTexture),ShaderState()
		{
			effect		= *shaderNS::reference(shaderNS::NORMAL_FADE);
			inTime		= 1.0f;
			outTime		= 1.0f;
			waitTime	= 1.0f;
			playTime	= 1.0f;
		}
		//値の設定
		virtual void setValue(D3DSURFACE_DESC desc) override
		{
			effect->SetTechnique("main");
			effect->SetFloat("TexWidth",	(float)desc.Width);
			effect->SetFloat("TexHeight",	(float)desc.Height);
		}
		//値の更新
		virtual void updateValue(float rate) override
		{
			effect->SetTexture("Tex", targetTexture);
			effect->SetFloat("rate", rate);
			effect->CommitChanges();
		}
	};
	//ブラーシェーダー
	class BlurShader:public ShaderState
	{
	public:
		float				level;				//ブラーレベル
		LPDIRECT3DTEXTURE9	targetTexture;		//ターゲットテクスチャ
		//コンストラクタ
		BlurShader(float _level, LPDIRECT3DTEXTURE9 _targetTexture)
			:level(_level),targetTexture(_targetTexture),ShaderState()
		{
			effect		= *shaderNS::reference(shaderNS::BLUR);
			inTime		= 3.0f;
			outTime		= 1.0f;
			waitTime	= 3.0f;
			playTime	= 3.0f;
		}
		//値の設定
		virtual void setValue(D3DSURFACE_DESC desc) override
		{
			effect->SetTechnique("tecBlur");
			effect->SetFloat("TexWidth", (float)desc.Width);
			effect->SetFloat("TexHeight", (float)desc.Height);
			effect->SetFloat("Level", level);
		}
		//値の更新
		virtual void updateValue(float rate) override
		{
			effect->SetTexture("BlurTex", targetTexture);
			effect->CommitChanges();
		}
	};
}

//===================================================================================================================================
//【フェーダークラス】
//===================================================================================================================================
class Fader :public Base
{
private:
	LPDIRECT3DDEVICE9		device;			//デバイスポインタ保存
	LPDIRECT3DTEXTURE9		targetTexture;	//描画対象テクスチャ
	LPDIRECT3DSURFACE9		surface;		//描画テクスチャサーフェース
	LPDIRECT3DSURFACE9		textureZBuffer;	//ｚバッファ描画テクスチャ
	FADE_VERTEX				point[4 + 1];	//テクスチャ貼り付けポリゴン
	faderNS::ShaderState*	shaderState;	//シェーダー切り替クラス

	int						state;			//フェーダーの状態
	float					frameTime;		//フレーム時間の保存変数

	//タイマー
	float					shaderRate;		//シェーダーレート
	float					inTimer;		//フェードインタイマー
	float					outTimer;		//フェードアウトタイマー
	float					waitTimer;		//待機タイマー
	float					playTimer;		//再生タイマー

	//遷移時間の最大値・・・都度設定する。
	float					limitInTime;	//フェードイン時間
	float					limitOutTime;	//フェードアウト時間
	float					limitWaitTime;	//待機時間
	float					limitPlayTime;	//再生時間

	bool					processing;		//フェード処理中

public:
	Fader();								//コンストラクタ
	~Fader();								//デストラクタ
	void update(float frameTime);			//更新処理
	void render();							//描画処理
	void start();							//フェード開始命令

	//操作メソッド
	void setShader(int shaderType);			//シェーダーの設定
	void setRenderTexture();				//レンダー対象をフェード用テクスチャへ切り替える
	bool nowPlaying();						//再生中かどうか
	bool nowProcessing();					//処理中かどうか

#ifdef _DEBUG
	bool showGUI;							//GUIの可視フラグ
	void outputGUI();						//GUI出力
	bool* getShowGUI(){ return &showGUI; };	//GUIの可視フラグgetter
#endif // _DEBUG


private:
	//内部処理メソッド
	void fadeIn();							//フェードイン処理
	void play();							//再生処理
	void fadeOut();							//フェードアウト処理
	void wait();							//待機処理
};

//===================================================================================================================================
//【外部参照】
//===================================================================================================================================
Fader* getFader();