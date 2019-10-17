//===================================================================================================================================
//【InstancingStaticMesh.fx】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/09/23
// [更新日]2019/10/17
//===================================================================================================================================
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//グローバル
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
float4x4		matrixProjection;
float4x4		matrixView;
texture		textureDecal;
float4		diffuse;
float4		ambient				= float4(0.1f, 0.1f, 0.1f, 0.1f);
float3		lightDirection		= float3(1.0f, 1.0f, 1.0f);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//定義
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
sampler textureSampler = sampler_state
{
	texture				= <textureDecal>;
	MinFilter			= ANISOTROPIC;
	MagFilter			= POINT;
	MipFilter			= POINT;
	MaxAnisotropy	= 4;
	AddressU			= Wrap;
	AddressV			= Wrap;
};

struct VS_OUT
{
	float4 position				: POSITION;
	float2 uv						: TEXCOORD0;
	float3 normal					: NORMAL;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//バーテックス・シェーダー
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
VS_OUT VS(
	float4 position	: POSITION,
	float2 localUV		: TEXCOORD0,
	float3 normal		: NORMAL,
	float3 pos			: TEXCOORD1)
{
	VS_OUT Out;

	//頂点を保存
	Out.position = float4(
		position.x,
		position.y,
		position.z,
		1.0f);

	//ワールド行列を用意する。（現在は移動情報のみ）
	float4x4 worldMatrix = float4x4(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		pos.x, pos.y, pos.z, 1.0f);

	//あとで実装予定
	//計算を全てGPU側で行った方が高速？？
	//移動行列の適用
	//回転行列の適用
	//スケール行列の適用

	//ビュー行列
	worldMatrix = mul(worldMatrix, matrixView);
	//プロジェクション行列
	worldMatrix = mul(worldMatrix, matrixProjection);

	Out.position = mul(Out.position, worldMatrix);
	
	Out.uv = localUV;

	Out.normal = normal;

	return Out;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//ピクセル・シェーダー
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
float4 PS(VS_OUT In) : COLOR0
{
	float4 lambert		= saturate(dot(In.normal, lightDirection));
	float4 texel			= tex2D(textureSampler, In.uv);
	float4 finalColor		= texel*lambert*0.5f + ambient*texel + diffuse*lambert + diffuse*ambient;

	return texel;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// テクニック
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
technique mainTechnique {
	pass p0 {
		//ステート設定
		Zenable							= TRUE;			//Zバッファ有効
		ZWriteEnable					= TRUE;			//Zバッファへの書き込み有効
		//ShadeMode					= GOURAUD;		//グーロー・シェーディング
		CullMode						= CCW;				//背面をカリング
		//MultiSampleAntialias		= TRUE;			//アンチエイリアシングを有効

		//シェーダ設定
		VertexShader		= compile vs_3_0 VS();
		PixelShader		= compile ps_3_0 PS();
	}
}
