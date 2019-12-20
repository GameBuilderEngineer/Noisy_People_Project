//===================================================================================================================================
//【InstancingBillboard.fx】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/09/23
// [更新日]2019/10/17
//===================================================================================================================================
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//グローバル
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
float4x4 matrixProjection;
float4x4 matrixView;
texture planeTexture;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//定義
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
sampler textureSampler = sampler_state
{
	texture = <planeTexture>;
	MipFilter = NONE;
	MinFilter = POINT;
	MagFilter = LINEAR;
	AddressU = BORDER;
	AddressV = BORDER;
	BorderColor = 0x00000000;
};

struct VS_OUT
{
	float4 position : POSITION;
	float2 uv : TEXCOORD0;
	float4 color : COLOR0;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//バーテックス・シェーダー
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//ビルボード
VS_OUT vsMain(
	float2 position				: POSITION, 
	float2 localUV				: TEXCOORD0,
	float3 worldPosition		: TEXCOORD1,
	float2 uv					: TEXCOORD2,
	float3 rotation				: TEXCOORD3,
	float2 scale				: TEXCOORD4,
	float4 color				: COLOR0)
{
	VS_OUT Out;

	Out.position = float4(
		position.x,
		position.y,
		0.0f,
		1.0f);

	float cosX = cos(rotation.x);
	float sinX = sin(rotation.x);
	float cosY = cos(rotation.y);
	float sinY = sin(rotation.y);
	float cosZ = cos(rotation.z);
	float sinZ = sin(rotation.z);

	//ワールド行列
	float4x4 matrixWorld = float4x4(
		1.0f,0.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		0.0f,0.0f,0.0f,1.0f);
	//スケール行列
	float4x4 scaleMatrix = float4x4(
		scale.x,0.0f,0.0f,0.0f,
		0.0f, scale.y,0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		0.0f,0.0f,0.0f,1.0f);
	//位置行列
	float4x4 positionMatrix = float4x4(
		1.0f,0.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		worldPosition.x, worldPosition.y, worldPosition.z,1.0f);
	//X軸回転行列
	float4x4 xRotationMatrix = float4x4(
		1.0f,0.0f,0.0f,0.0f,
		0.0f,cosX,-sinX,0.0f,
		0.0f,sinX,cosX,0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
	//Y軸回転行列
	float4x4 yRotationMatrix = float4x4(
		cosY,0.0f,sinY,0.0f,
		0.0f, 1.0f,0.0f,0.0f,
		-sinY,0.0f, cosY,0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
	//Z軸回転行列
	float4x4 zRotationMatrix = float4x4(
		cosZ,-sinZ,0.0f,0.0f,
		sinZ,cosZ,0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		0.0f,0.0f,0.0f,1.0f);
	//ビルボード処理
	float4x4 cancelRotation = matrixView;
	cancelRotation._41_42_43 = 0.0f;
	cancelRotation = transpose(cancelRotation);


	//行列の演算
	matrixWorld = mul(matrixWorld,scaleMatrix);				//*スケール
	matrixWorld = mul(matrixWorld,xRotationMatrix);			//*x軸回転
	matrixWorld = mul(matrixWorld,yRotationMatrix);			//*y軸回転
	matrixWorld = mul(matrixWorld,zRotationMatrix);			//*z軸回転
	matrixWorld = mul(matrixWorld,cancelRotation);			//*ビルボード
	matrixWorld = mul(matrixWorld,positionMatrix);			//*移動

	//ビュー行列
	matrixWorld = mul(matrixWorld,matrixView);
	//プロジェクション行列
	matrixWorld = mul(matrixWorld,matrixProjection);

	//頂点へ演算
	Out.position = mul(Out.position, matrixWorld);

	Out.uv = localUV+uv;

	Out.color = color;

	return Out;
}

//Y軸ビルボード
VS_OUT vsMainY(
	float2 position				: POSITION, 
	float2 localUV				: TEXCOORD0,
	float3 worldPosition		: TEXCOORD1,
	float2 uv					: TEXCOORD2,
	float3 rotation				: TEXCOORD3,
	float2 scale				: TEXCOORD4,
	float4 color				: COLOR0)
{
	VS_OUT Out;

	Out.position = float4(
		position.x,
		position.y,
		0.0f,
		1.0f);

	float cosX = cos(rotation.x);
	float sinX = sin(rotation.x);
	float cosY = cos(rotation.y);
	float sinY = sin(rotation.y);
	float cosZ = cos(rotation.z);
	float sinZ = sin(rotation.z);

	//ワールド行列
	float4x4 matrixWorld = float4x4(
		1.0f,0.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		0.0f,0.0f,0.0f,1.0f);
	//スケール行列
	float4x4 scaleMatrix = float4x4(
		scale.x,0.0f,0.0f,0.0f,
		0.0f, scale.y,0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		0.0f,0.0f,0.0f,1.0f);
	//位置行列
	float4x4 positionMatrix = float4x4(
		1.0f,0.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		worldPosition.x, worldPosition.y, worldPosition.z,1.0f);
	//X軸回転行列
	float4x4 xRotationMatrix = float4x4(
		1.0f,0.0f,0.0f,0.0f,
		0.0f,cosX,-sinX,0.0f,
		0.0f,sinX,cosX,0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
	//Y軸回転行列
	float4x4 yRotationMatrix = float4x4(
		cosY,0.0f,sinY,0.0f,
		0.0f, 1.0f,0.0f,0.0f,
		-sinY,0.0f, cosY,0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
	//Z軸回転行列
	float4x4 zRotationMatrix = float4x4(
		cosZ,-sinZ,0.0f,0.0f,
		sinZ,cosZ,0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		0.0f,0.0f,0.0f,1.0f);
	//ビルボード処理（Ｙ軸）
	float4x4 cancelRotation = matrixView;
	cancelRotation._41_42_43 = 0.0f;
	cancelRotation._12_22_32 = float3(0.0f,1.0f,0.0f);//Y軸回転はｷｬﾝｾﾙしない
	cancelRotation = transpose(cancelRotation);

	//行列の演算
	matrixWorld = mul(matrixWorld,scaleMatrix);				//*スケール
	matrixWorld = mul(matrixWorld,xRotationMatrix);			//*x軸回転
	matrixWorld = mul(matrixWorld,yRotationMatrix);			//*y軸回転
	matrixWorld = mul(matrixWorld,zRotationMatrix);			//*z軸回転
	matrixWorld = mul(matrixWorld ,cancelRotation);			//*ビルボード
	matrixWorld = mul(matrixWorld,positionMatrix);			//*移動

	//ビュー行列
	matrixWorld = mul(matrixWorld,matrixView);
	//プロジェクション行列
	matrixWorld = mul(matrixWorld,matrixProjection);

	//頂点へ演算
	Out.position = mul(Out.position, matrixWorld);

	Out.uv = localUV+uv;

	Out.color = color;

	return Out;
}

//ビルボードOFF
VS_OUT vsMainOFF(
	float2 position				: POSITION, 
	float2 localUV				: TEXCOORD0,
	float3 worldPosition		: TEXCOORD1,
	float2 uv					: TEXCOORD2,
	float3 rotation				: TEXCOORD3,
	float2 scale				: TEXCOORD4,
	float4 color				: COLOR0)
{
	VS_OUT Out;

	Out.position = float4(
		position.x,
		position.y,
		0.0f,
		1.0f);

	float cosX = cos(rotation.x);
	float sinX = sin(rotation.x);
	float cosY = cos(rotation.y);
	float sinY = sin(rotation.y);
	float cosZ = cos(rotation.z);
	float sinZ = sin(rotation.z);

	//ワールド行列
	float4x4 matrixWorld = float4x4(
		1.0f,0.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		0.0f,0.0f,0.0f,1.0f);
	//スケール行列
	float4x4 scaleMatrix = float4x4(
		scale.x,0.0f,0.0f,0.0f,
		0.0f, scale.y,0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		0.0f,0.0f,0.0f,1.0f);
	//位置行列
	float4x4 positionMatrix = float4x4(
		1.0f,0.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		worldPosition.x, worldPosition.y, worldPosition.z,1.0f);
	//X軸回転行列
	float4x4 xRotationMatrix = float4x4(
		1.0f,0.0f,0.0f,0.0f,
		0.0f,cosX,-sinX,0.0f,
		0.0f,sinX,cosX,0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
	//Y軸回転行列
	float4x4 yRotationMatrix = float4x4(
		cosY,0.0f,sinY,0.0f,
		0.0f, 1.0f,0.0f,0.0f,
		-sinY,0.0f, cosY,0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
	//Z軸回転行列
	float4x4 zRotationMatrix = float4x4(
		cosZ,-sinZ,0.0f,0.0f,
		sinZ,cosZ,0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		0.0f,0.0f,0.0f,1.0f);

	//行列の演算
	matrixWorld = mul(matrixWorld,scaleMatrix);				//*スケール
	matrixWorld = mul(matrixWorld,xRotationMatrix);			//*x軸回転
	matrixWorld = mul(matrixWorld,yRotationMatrix);			//*y軸回転
	matrixWorld = mul(matrixWorld,zRotationMatrix);			//*z軸回転
	matrixWorld = mul(matrixWorld,positionMatrix);			//*移動

	//ビュー行列
	matrixWorld = mul(matrixWorld,matrixView);
	//プロジェクション行列
	matrixWorld = mul(matrixWorld,matrixProjection);

	//頂点へ演算
	Out.position = mul(Out.position, matrixWorld);

	Out.uv = localUV+uv;

	Out.color = color;

	return Out;
}

//固定サイズビルボード
VS_OUT vsMainFixedSize(
	float2 position				: POSITION, 
	float2 localUV				: TEXCOORD0,
	float3 worldPosition		: TEXCOORD1,
	float2 uv					: TEXCOORD2,
	float3 rotation				: TEXCOORD3,
	float2 scale				: TEXCOORD4,
	float4 color				: COLOR0)
{
	VS_OUT Out;

	Out.position = float4(
		position.x,
		position.y,
		0.0f,
		1.0f);

	float cosX = cos(rotation.x);
	float sinX = sin(rotation.x);
	float cosY = cos(rotation.y);
	float sinY = sin(rotation.y);
	float cosZ = cos(rotation.z);
	float sinZ = sin(rotation.z);

	//ワールド行列
	float4x4 matrixWorld = float4x4(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
	//スケール行列
	float4x4 scaleMatrix = float4x4(
		scale.x, 0.0f, 0.0f, 0.0f,
		0.0f, scale.y, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);

	//位置行列
	//float4x4 positionMatrix = float4x4(
	//	1.0f, 0.0f, 0.0f, 0.0f,
	//	0.0f, 1.0f, 0.0f, 0.0f,
	//	0.0f, 0.0f, 1.0f, 0.0f,
	//	worldPosition.x, worldPosition.y, worldPosition.z, 1.0f);
	//X軸回転行列
	float4x4 xRotationMatrix = float4x4(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, cosX, -sinX, 0.0f,
		0.0f, sinX, cosX, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
	//Y軸回転行列
	float4x4 yRotationMatrix = float4x4(
		cosY, 0.0f, sinY, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		-sinY, 0.0f, cosY, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
	//Z軸回転行列
	float4x4 zRotationMatrix = float4x4(
		cosZ, -sinZ, 0.0f, 0.0f,
		sinZ, cosZ, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
	//ビルボード処理
	//float4x4 cancelRotation = matrixView;
	//cancelRotation._41_42_43 = 0.0f;
	//cancelRotation = transpose(cancelRotation);


	//行列の演算
	matrixWorld = mul(matrixWorld, scaleMatrix);				//*スケール
	//matrixWorld = mul(matrixWorld, xRotationMatrix);			//*x軸回転
	//matrixWorld = mul(matrixWorld, yRotationMatrix);			//*y軸回転
	//matrixWorld = mul(matrixWorld, zRotationMatrix);			//*z軸回転
	//matrixWorld = mul(matrixWorld, cancelRotation);				//*ビルボード
	//matrixWorld = mul(matrixWorld, positionMatrix);				//*移動

	//ビュー行列
	//matrixWorld = mul(matrixWorld, matrixView);
	//プロジェクション行列
	//matrixWorld = mul(matrixWorld, matrixProjection);
	//正射影行列
	//matrixWorld = mul(matrixWorld, matrixOrtho);

	//頂点へ演算
	Out.position = mul(Out.position, matrixWorld);

	//ビュー×プロジェクション変換
	float4 orthoPosition = float4(worldPosition, 1.0f);
	orthoPosition = mul(orthoPosition, matrixView);
	orthoPosition = mul(orthoPosition, matrixProjection);
	orthoPosition = orthoPosition / orthoPosition.w;
	Out.position.xy = Out.position.xy + orthoPosition.xy;

	Out.uv = localUV + uv;

	Out.color = color;

	return Out;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//ピクセル・シェーダー
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
float4 psMain(VS_OUT In) : COLOR0
{
	return tex2D(textureSampler, In.uv)*In.color;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// テクニック
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
technique mainTechnique {


	//通常ビルボード
	pass p0 {
		//ステート設定
		Zenable					= TRUE;				//Zバッファ
		ZWriteEnable			= TRUE;				//Zバッファへの書き込み
		CullMode				= CCW;				//背面をカリング
		VertexShader = compile vs_2_0 vsMain();
		PixelShader = compile ps_2_0 psMain();
	}

	//通常ビルボード（透過）
	pass p1 {

		//ステート設定
		Zenable					= TRUE;			//Zバッファ
		ZWriteEnable			= FALSE;		//Zバッファへの書き込み
		CullMode				= CCW;			//背面をカリング

		VertexShader = compile vs_2_0 vsMain();
		PixelShader = compile ps_2_0 psMain();
	}

	//通常ビルボード（最前面）
	pass p2 {

		//ステート設定
		Zenable					= FALSE;		//Zバッファ
		ZWriteEnable			= FALSE;		//Zバッファへの書き込み
		CullMode				= CCW;			//背面をカリング

		VertexShader = compile vs_2_0 vsMain();
		PixelShader = compile ps_2_0 psMain();
	}

	//Y軸ビルボード
	pass p3 {
		//ステート設定
		Zenable					= TRUE;				//Zバッファ
		ZWriteEnable			= TRUE;				//Zバッファへの書き込み
		CullMode				= CCW;				//背面をカリング
		VertexShader = compile vs_2_0 vsMainY();
		PixelShader = compile ps_2_0 psMain();
	}


	//Y軸ビルボード（透過）
	pass p4 {

		//ステート設定
		Zenable					= TRUE;			//Zバッファ
		ZWriteEnable			= FALSE;		//Zバッファへの書き込み
		CullMode				= CCW;			//背面をカリング

		VertexShader = compile vs_2_0 vsMainY();
		PixelShader = compile ps_2_0 psMain();
	}

	//Y軸ビルボード（最前面）
	pass p5 {

		//ステート設定
		Zenable					= FALSE;		//Zバッファ
		ZWriteEnable			= FALSE;		//Zバッファへの書き込み
		CullMode				= CCW;			//背面をカリング

		VertexShader = compile vs_2_0 vsMainY();
		PixelShader = compile ps_2_0 psMain();
	}

	//ビルボードOFF
	pass p6 {
		//ステート設定
		Zenable = TRUE;				//Zバッファ
		ZWriteEnable = TRUE;				//Zバッファへの書き込み
		CullMode = CCW;				//背面をカリング
		VertexShader = compile vs_2_0 vsMainOFF();
		PixelShader = compile ps_2_0 psMain();
	}


	//ビルボードOFF（透過）
	pass p7 {

		//ステート設定
		Zenable = TRUE;			//Zバッファ
		ZWriteEnable = FALSE;		//Zバッファへの書き込み
		CullMode = CCW;			//背面をカリング

		VertexShader = compile vs_2_0 vsMainOFF();
		PixelShader = compile ps_2_0 psMain();
	}

	//ビルボードOFF（最前面）
	pass p8 {

		//ステート設定
		Zenable = FALSE;		//Zバッファ
		ZWriteEnable = FALSE;		//Zバッファへの書き込み
		CullMode = CCW;			//背面をカリング

		VertexShader = compile vs_2_0 vsMainOFF();
		PixelShader = compile ps_2_0 psMain();
	}

	//固定サイズ
	pass p9 {
		//ステート設定
		Zenable = TRUE;				//Zバッファ
		ZWriteEnable = TRUE;				//Zバッファへの書き込み
		CullMode = CCW;				//背面をカリング
		MultiSampleAntialias = TRUE;		//アンチエイリアシングを有効
		VertexShader = compile vs_2_0 vsMainFixedSize();
		PixelShader = compile ps_2_0 psMain();
	}


	//固定サイズ（透過）
	pass p10 {

		//ステート設定
		Zenable = TRUE;			//Zバッファ
		ZWriteEnable = FALSE;		//Zバッファへの書き込み
		CullMode = CCW;			//背面をカリング
		MultiSampleAntialias = TRUE;		//アンチエイリアシングを有効

		VertexShader = compile vs_2_0 vsMainFixedSize();
		PixelShader = compile ps_2_0 psMain();
	}

	//固定サイズ（最前面）
	pass p11 {

		//ステート設定
		Zenable = FALSE;		//Zバッファ
		ZWriteEnable = FALSE;		//Zバッファへの書き込み
		CullMode = CCW;			//背面をカリング
		MultiSampleAntialias = TRUE;		//アンチエイリアシングを有効

		VertexShader = compile vs_2_0 vsMainFixedSize();
		PixelShader = compile ps_2_0 psMain();
	}

}
