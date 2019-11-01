//===================================================================================================================================
//【Fader.h】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/09/20
// [更新日]2019/11/01
//===================================================================================================================================
#pragma once

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
		BLUR,
		MAX_NUM
	};

	//抽象シェーダーインターフェース
	class ShaderState
	{
	public:
		LPD3DXEFFECT effect;
		float outTime;
		float inTime;
		float stopTime;
	public:
		ShaderState()
		{
			effect = NULL;
		}


		void begin(UINT i){
			effect->Begin(NULL, 0);
			effect->BeginPass(i);
		}

		void end() {
			effect->EndPass();
			effect->End();
		};

		virtual void setValue(D3DSURFACE_DESC desc) = 0;
		virtual void updateValue() = 0;
	};

	//ブラーシェーダー
	class BlurShader:public ShaderState
	{
	public:
		float level;
		LPDIRECT3DTEXTURE9 targetTexture;
		BlurShader(float _level, LPDIRECT3DTEXTURE9 _targetTexture)
			:level(_level),targetTexture(_targetTexture),ShaderState()
		{
			effect = *shaderNS::reference(shaderNS::BLUR);
			inTime = 3.0f;
			outTime = 1.0f;
			stopTime = 3.0f;
		}

		virtual void setValue(D3DSURFACE_DESC desc) override
		{
			effect->SetTechnique("tecBlur");
			effect->SetFloat("TexWidth", desc.Width);
			effect->SetFloat("TexHeight", desc.Height);
			effect->SetFloat("Level", level);
		}

		virtual void updateValue() override
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
public:
	bool playing;
	LPDIRECT3DTEXTURE9 targetTexture;
	LPDIRECT3DSURFACE9 textureZBuffer;
	FADE_VERTEX point[4 + 1];
	LPDIRECT3DDEVICE9 device;

	float inTimer;
	float outTimer;
	float stopTimer;

	faderNS::ShaderState* shaderState;

public:
	Fader();
	~Fader();

	void setShader(int shaderType);
	void render();
	void play();
	void stop();
	void setRenderTexture();
	
};

//===================================================================================================================================
//【外部参照】
//===================================================================================================================================
Fader* getFader();