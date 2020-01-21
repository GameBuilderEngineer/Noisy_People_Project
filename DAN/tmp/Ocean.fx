///////////////////////////////////////////////////////////////////////////////////
//グローバル
///////////////////////////////////////////////////////////////////////////////////
float4x4 mWorld: World;
float4x4 mWorldViewProj : WorldViewProjection;	
float4x4 mWorldInverseTranspose : WorldInverseTranspose;
float4x4 mViewInverse: ViewInverse;
float3 LightPos = {100.0f, 100.0f, -100.0f};
float4 vLightDir;
float4 vColor;
float3 vEyePos;
float2 f2Wave;
float g_fHeight;
texture DecaleTex;
texture NormalMap;

sampler DecaleSamp = sampler_state
{
    Texture = <DecaleTex>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = NONE;

    AddressU = Clamp;
    AddressV = Clamp;
};
sampler NormalSamp = sampler_state
{
    Texture = <NormalMap>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = NONE;

    AddressU = Wrap;
    AddressV = Wrap;
};

///////////////////////////////////////////////////////////////////////////////////
//型定義
///////////////////////////////////////////////////////////////////////////////////
struct VS_OUTPUT
{
    float4 Pos: POSITION;
    float4 Color: COLOR0;    
    float2 Tex: TEXCOORD0;
    float3 L: TEXCOORD1;
    float3 E: TEXCOORD2;
    float2 TexCoord: TEXCOORD3;
    float3 LightVec: TEXCOORD4;
    float3 WorldNormal: TEXCOORD5;
    float3 WorldView: TEXCOORD6;
};

///////////////////////////////////////////////////////////////////////////////////
//バーテックス・シェーダー
///////////////////////////////////////////////////////////////////////////////////
VS_OUTPUT VS(float4 Pos: POSITION0,float3 Normal: NORMAL,float3 Tangent  : TANGENT0,float2 Texcoord : TEXCOORD0)
{
	VS_OUTPUT Out=(VS_OUTPUT)0;
	Out.Pos = mul(Pos, mWorldViewProj);
	Out.Color = vColor;
	Out.Tex = Texcoord;
	Normal.y+=g_fHeight;
	float3 N = Normal;
	float3 T = Tangent;
	float3 B = cross(N,T);
	float3 E = vEyePos - Pos.xyz;	
	Out.E.x = dot(E,T);
	Out.E.y = dot(E,B);
	Out.E.z = dot(E,N);
	float3 L = -vLightDir.xyz;		
	Out.L.x = dot(L,T);
	Out.L.y = dot(L,B);
	Out.L.z = dot(L,N);
	
	Out.Tex+=f2Wave;

	float3 normal = normalize(Normal);
    Out.WorldNormal = mul(normal, mWorldInverseTranspose).xyz;
    float4 Po = float4(Pos.xyz,1);
    float3 Pw = mul(Po, mWorld).xyz;
    Out.LightVec = normalize(LightPos - Pw);
    Out.TexCoord = Texcoord;
    Out.WorldView = normalize(mViewInverse[3].xyz - Pw);
	
	return Out;
}
///////////////////////////////////////////////////////////////////////////////////
//ピクセル・シェーダー
///////////////////////////////////////////////////////////////////////////////////
float4 PS(VS_OUTPUT In) : COLOR
{   
	float4 f4Color=(float4)0;
	float3 f3Light=(float3)0;
	float3 f3Reflect=(float3)0;
	float3 f3Normal=(float3)0;
	float3 f3Sight=(float3)0;
	float fK=0;

	f4Color=In.Color;
	f3Light=vLightDir.xyz;
	f3Normal=tex2D( NormalSamp, In.Tex ).xyz;
	f3Normal.y=(1-f3Normal.y)*g_fHeight;
	f3Normal=f3Normal*2-1;
	f3Reflect=reflect(-normalize(In.E), f3Normal);
	
	f3Sight=In.E- vEyePos;
	fK=0.1;	
	
	float4 f4Intensity=dot(f3Light,f3Normal) //ディフューズ項
			+ pow(dot(f3Reflect,f3Sight),12);// スペキュラー項(照明モデルは　フォン phong)
			
			f4Color=float4(0.2,0.2,0.7,1);//青
			f4Intensity+=f4Color;
			
	return f4Intensity; 

}
///////////////////////////////////////////////////////////////////////////////////
// テクニック
///////////////////////////////////////////////////////////////////////////////////
technique tecOcean
{
    pass P0
    {
        VertexShader = compile vs_2_0 VS();
        PixelShader  = compile ps_2_0 PS();   
    }
}