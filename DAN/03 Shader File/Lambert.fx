//===================================================================================================================================
//【Lambert.fx】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/08/07
// [更新日]2019/09/03
//===================================================================================================================================
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//グローバル
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
float4x4 matrixProjection;		//プロジェクション（遠近射影）行列
float4x4 matrixView;			//ビュー行列
float4x4 matrixWorld;			//ワールド行列
float4x4 matrixWVP;				//ワールド行列
float4 lightDirection;			//平行光源の方向ベクトル
float4 ambient = 0.0f;			//環境光
float farZ;						//遠近射影行列の最遠近距離

sampler textureSmapler : register(s0);	//テクスチャサンプラー

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//バーテックス・シェーダー
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct VS_OUTPUT
{
	float4 position		: POSITION;
	float4 color		: COLOR0;
	float2 uv			: TEXCOORD0;
	float4 positionWVP	: TEXCOORD1;
};
VS_OUTPUT VS(
	float4	position	: POSITION,
	float4	normal		: NORMAL,
	float2	uv			: TEXCOORD0)
{
	VS_OUTPUT Out;
	
	Out.position = mul(position, matrixWVP);
	Out.uv = uv;

	float3 L = -lightDirection.xyz;
	float3 N = normalize(normal.xyz);
	Out.color = max(ambient, dot(N, L));

	Out.positionWVP = Out.position;

	return Out;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//ピクセル・シェーダー
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct PS_OUTPUT
{
	float4 color1 : COLOR0;
	float4 color2 : COLOR1;
};
PS_OUTPUT PS(VS_OUTPUT In)
{
	PS_OUTPUT Out;
	
	//色情報を格納する
	Out.color1 = tex2D(textureSmapler, In.uv);
	Out.color1.rgb = Out.color1.rgb * In.color.rgb;

	//Z値情報を格納する
	Out.color2 = In.positionWVP.z / farZ;

	return Out;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// テクニック
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
technique main
{
	pass P0
	{
		VertexShader = compile vs_2_0 VS();
		PixelShader = compile ps_2_0 PS();
	}
}