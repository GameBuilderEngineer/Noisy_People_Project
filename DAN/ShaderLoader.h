//===================================================================================================================================
//【ShaderLoader.h】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/09/23
// [更新日]2019/10/28
//===================================================================================================================================
#pragma once
#include "Base.h"

namespace shaderNS
{
	enum SHADER_NUMBER {
		TOON,
		INSTANCE_BILLBOARD,
		INSTANCE_STATIC_MESH,
		BLUR,
		MAX_SHADER,
	};

	//外部参照関数
	LPD3DXEFFECT* reference(int shaderNo);
}

class ShaderLoader :public Base
{
private:
	const char* shaderName[shaderNS::MAX_SHADER];
	LPD3DXEFFECT effect[shaderNS::MAX_SHADER];
public:
	ShaderLoader();
	~ShaderLoader();
	void load(LPDIRECT3DDEVICE9 device);
	void release();

	LPD3DXEFFECT* getEffect(int shaderNo) { return &effect[shaderNo]; }
};

