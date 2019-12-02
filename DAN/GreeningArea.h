//===================================================================================================================================
//yGreeningArea.hz
// [์ฌา]HALGP12A332 11 ์ ๗
// [์ฌ๚]2019/11/15
// [XV๚]2019/11/15
//===================================================================================================================================
#pragma once

//===================================================================================================================================
//yCN[hz
//===================================================================================================================================
#include "Object.h"
#include "EffekseerManager.h"

//===================================================================================================================================
//yผO๓ิz
//===================================================================================================================================
namespace GreeningAreaNS
{
	//ฮปGtFNg
	class GreeningEffect :public effekseerNS::Instance
	{
	public:
		D3DXVECTOR3* syncPosition;
		D3DXVECTOR3* syncScale;
		GreeningEffect(D3DXVECTOR3* syncPosition,D3DXVECTOR3* syncScale) {
			this->syncPosition = syncPosition;
			this->syncScale = syncScale;
			effectNo = effekseerNS::GREENING;
		}
		virtual void update() {
			position = *syncPosition;
			scale = *syncScale;
			Instance::update();
		};
	};
}



//===================================================================================================================================
//yฮปออNXFีหป่๐sคฝ฿ษ์ฌz
//===================================================================================================================================
class GreeningArea :public Object
{
private:

public:
	GreeningArea();
	~GreeningArea();
	void update(float frameTime);
	void render();	
};

