//===================================================================================================================================
//【InstancingStaticMesh.fx】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/09/23
// [更新日]2019/10/23
//===================================================================================================================================
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//グローバル
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
float4x4	matrixProjection;
float4x4	matrixView;
texture		textureDecal;
float4		diffuse;
float		alphaValue;
float4		lightDirection		= float4(-1.0f, 1.0f, -1.0f, 0.2f);


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//定義
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
sampler textureSampler = sampler_state
{
	texture			= <textureDecal>;
	//MinFilter		= ANISOTROPIC;
	//MagFilter		= POINT;
	//MipFilter		= LINEAR;
	//MaxAnisotropy	= 4;
	//AddressU		= Wrap;
	//AddressV		= Wrap;
};

struct VS_OUT
{
	float4 position	: POSITION;
	float2 uv		: TEXCOORD0;
	float4 diffuse	: COLOR0;
	float4 color	: COLOR1;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//バーテックス・シェーダー
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
VS_OUT VS(
	float4 position		: POSITION,
	float2 localUV		: TEXCOORD0,
	float3 normal		: NORMAL,
	float4 matrix1		: TEXCOORD1,
	float4 matrix2		: TEXCOORD2,
	float4 matrix3		: TEXCOORD3,
	float4 matrix4		: TEXCOORD4,
	float4 color		: TEXCOORD5)
{
	VS_OUT Out;
	
	//頂点を保存
	Out.position = float4(
		position.x,
		position.y,
		position.z,
		1.0f);


	//ワールド行列を用意する。
	float4x4 worldMatrix = float4x4(
		matrix1,
		matrix2,
		matrix3,
		matrix4);

	//あとで実装予定
	//現在はCPU側で演算
	//計算を全てGPU側で行った方が高速？？
	//移動行列の適用
	//回転行列の適用
	//スケール行列の適用

	//ビュー行列
	worldMatrix = mul(worldMatrix, matrixView);
	//プロジェクション行列
	worldMatrix = mul(worldMatrix, matrixProjection);
	//ワールド行列
	Out.position = mul(Out.position, worldMatrix);
	
	//UV座標
	Out.uv = localUV;

	//アンビエントカラー
	float ambient = lightDirection.w;

	//ランバート演算カラー
	float4 lambert = max(ambient,saturate(dot(normal, lightDirection)));
	Out.diffuse = lambert*diffuse;
	Out.color = color;

	return Out;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//ピクセル・シェーダー
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
float4 PS(VS_OUT In) : COLOR0
{
	float4 texel		= tex2D(textureSampler, In.uv);
	float4 finalColor	= texel*In.diffuse;
	return finalColor;
}
//ライトを考慮しない
float4 PS1(VS_OUT In) : COLOR0
{
	float4 texel		= tex2D(textureSampler, In.uv);
	float4 finalColor	= texel * In.color;
	return finalColor;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// テクニック
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
technique mainTechnique {
	//ライトをON ランバート拡散反射により出力
	pass p0 {
		//ステート設定
		Zenable					= TRUE;			//Zバッファ有効
		ZWriteEnable			= TRUE;			//Zバッファへの書き込み有効
		ShadeMode				= GOURAUD;		//グーロー・シェーディング
		CullMode				= CCW;			//背面をカリング
		MultiSampleAntialias	= TRUE;			//アンチエイリアシングを有効
		AlphaTestEnable			= TRUE;			//αテストの有効
		AlphaBlendEnable		= FALSE;		//αブレンドの無効

		//シェーダ設定
		VertexShader			= compile vs_3_0 VS();
		PixelShader				= compile ps_3_0 PS();
	}

	//ライトをOFF ランバート拡散反射を考慮せずテクセルカラーのみで出力
	pass p1 {
		//ステート設定
		Zenable					= TRUE;			//Zバッファ有効
		ZWriteEnable			= TRUE;			//Zバッファへの書き込み有効
		ShadeMode				= GOURAUD;		//グーロー・シェーディング
		CullMode				= CCW;			//背面をカリング
		MultiSampleAntialias	= FALSE;		//アンチエイリアシングを有効
		AlphaTestEnable			= FALSE;		//αテストの有効
		AlphaBlendEnable		= FALSE;		//αブレンドの無効
		//AlphaFunc				= GREATEREQUAL;	//アルファ比較関数(>=)
		//AlphaRef				= 0x00000001;	//α比較後、黒以上の場合描画

		//シェーダ設定
		VertexShader			= compile vs_3_0 VS();
		PixelShader				= compile ps_3_0 PS1();
	}

	//透過有効描画
	pass p2 {
		//ステート設定
		Zenable					= TRUE;			//Zバッファ有効
		ZWriteEnable			= TRUE;			//Zバッファへの書き込み有効
		ShadeMode				= GOURAUD;		//グーロー・シェーディング
		CullMode				= NONE;			//背面をカリング
		MultiSampleAntialias	= FALSE;		//アンチエイリアシングを有効
		AlphaTestEnable			= TRUE;			//αテストの有効
		AlphaBlendEnable		= TRUE;			//αブレンドの無効
		AlphaFunc				= GREATEREQUAL;	//アルファ比較関数(>=)
		AlphaRef				= 0x00000001;	//α比較後、黒以上の場合描画

		//シェーダ設定
		VertexShader			= compile vs_3_0 VS();
		PixelShader				= compile ps_3_0 PS1();
	}

	//透過有効描画&最前面描画
	pass p3 {
		//ステート設定
		Zenable					= FALSE;		//Zバッファ有効
		ZWriteEnable			= TRUE;			//Zバッファへの書き込み有効
		ShadeMode				= GOURAUD;		//グーロー・シェーディング
		CullMode				= NONE;			//背面をカリング
		MultiSampleAntialias	= FALSE;		//アンチエイリアシングを有効
		AlphaTestEnable			= TRUE;			//αテストの有効
		AlphaBlendEnable		= FALSE;		//αブレンドの無効
		AlphaFunc				= GREATEREQUAL;	//アルファ比較関数(>=)
		AlphaRef				= 0x00000001;	//α比較後、黒以上の場合描画

		//シェーダ設定
		VertexShader			= compile vs_3_0 VS();
		PixelShader				= compile ps_3_0 PS1();
	}
}
