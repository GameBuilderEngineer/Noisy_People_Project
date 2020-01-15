///////////////////////////////////////////////////////////////////////////////////
//グローバル
///////////////////////////////////////////////////////////////////////////////////
//float4x4 mWorld: World;
//float4x4 mWorldViewProj : WorldViewProjection;	
float4x4 WIT;	// ワールド(world)逆(inverse)転置(transpose)
float3 lightPosition = {100.0f, 100.0f, -100.0f};	//ライト位置
float4x4 inverseView;//逆ビュー行列
//float4 lightDirection;
//float4 vColor;
float3 eyePosition;		//頂点からカメラへのベクトル
float2 waveMove;		//波の移動位置(UV)
float height;			//波の高さ
//texture DecaleTex;
texture normalMap;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
float4x4	matrixProjection;
float4x4	matrixView;
texture		textureDecal;
float4		diffuse;
float		alphaValue;
float4		lightDirection = float4(-1.0f, 1.0f, -1.0f, 0.2f);

sampler textureSampler = sampler_state
{
    Texture = <textureDecal>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = NONE;

    AddressU = Clamp;
    AddressV = Clamp;
};
sampler normalSampler = sampler_state
{
    Texture = <normalMap>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = NONE;

    AddressU = Wrap;
    AddressV = Wrap;
};

///////////////////////////////////////////////////////////////////////////////////
//型定義
///////////////////////////////////////////////////////////////////////////////////
struct VS_OUT
{
    float4 position			: POSITION;
	float2 uv				: TEXCOORD0;
	float4 diffuse			: COLOR0;
	float4 color			: COLOR1;
    //float4 Color: COLOR0;    
    //float2 Tex: TEXCOORD0;
    float3 lightDirection	: TEXCOORD1;//ライトの方向（バーテックスシェーダーで反転して保存）
    float3 toEye			: TEXCOORD2;//頂点からカメラへのベクトル（接空間投影）
    float2 bumpUV			: TEXCOORD3;//バンプテクスチャUV
    float3 lightVector		: TEXCOORD4;//頂点からライトまでのベクトル
    float3 tangentNormal	: TEXCOORD5;//接空間法線
    float3 tangentView		: TEXCOORD6;//接空間逆ビュー
};

///////////////////////////////////////////////////////////////////////////////////
//バーテックス・シェーダー
///////////////////////////////////////////////////////////////////////////////////
VS_OUT VS(
	float4 position		: POSITION0,
	float2 localUV		: TEXCOORD0,
	float3 normal		: NORMAL0,
	float3 tangent		: TANGENT0,
	float4 matrix1		: TEXCOORD1,
	float4 matrix2		: TEXCOORD2,
	float4 matrix3		: TEXCOORD3,
	float4 matrix4		: TEXCOORD4
)
{
	VS_OUT Out=(VS_OUT)0;

	//頂点を保存
	Out.position = position;

	//ワールド行列を用意する。
	float4x4 worldMatrix = float4x4(
		matrix1,
		matrix2,
		matrix3,
		matrix4);

	//*ビュー行列
	worldMatrix = mul(worldMatrix, matrixView);
	//*プロジェクション行列
	worldMatrix = mul(worldMatrix, matrixProjection);
	//*ワールド行列
	Out.position = mul(Out.position, worldMatrix);

	//UV座標
	Out.uv = localUV;

	//アンビエントカラー
	float ambient = lightDirection.w;

	//ランバート演算カラー
	float4 lambert = max(ambient, saturate(dot(normal, lightDirection)));
	Out.diffuse = lambert * diffuse;
	//Out.color = color;
	//Out.Color = vColor;

	//Out.Tex = Texcoord;

	normal.y += height;			//波の高さを設定

	//接空間の作成
	float3 N = normal;			//法線
	float3 T = tangent;			//接線
	float3 B = cross(N,T);		//従法線

	//頂点からカメラへのベクトル
	float3 toEye = eyePosition - position.xyz;	
	//カメラへのベクトルを接空間へ投影する
	Out.toEye.x = dot(toEye,T);
	Out.toEye.y = dot(toEye,B);
	Out.toEye.z = dot(toEye,N);

	//ライトの向きを保存
	float3 L = -lightDirection.xyz;//ライトの向きを反転
	//ライトの方向を接空間へ投影
	Out.lightDirection.x = dot(L,T);
	Out.lightDirection.y = dot(L,B);
	Out.lightDirection.z = dot(L,N);

	//バンプマップテクスチャUV座標
	Out.bumpUV	=	localUV;	
	Out.bumpUV	+=	waveMove;		//波の移動

	//接空間法線
	float3 tangentNormal = normalize(normal);
	Out.tangentNormal = mul(tangentNormal, WIT).xyz;//ワールド空間へ補正する

	//頂点からライトまでのベクトル
	float4 f4Position		= float4(position.xyz,1);
    float3 worldPosition	= mul(f4Position, worldMatrix).xyz;//ワールド空間上の頂点位置
    Out.lightVector			= normalize(lightPosition - worldPosition);
	
	//頂点ワールド位置からビュー行列位置へのベクトル（直感的には頂点位置からカメラ位置へのベクトル）
	Out.tangentView = normalize(inverseView[3].xyz - worldPosition);
	
	return Out;
}
///////////////////////////////////////////////////////////////////////////////////
//ピクセル・シェーダー
///////////////////////////////////////////////////////////////////////////////////
float4 PS(VS_OUT In) : COLOR
{   
	float4 color			= (float4)0;
	float3 light			= (float3)0;
	float3 reflectVector	= (float3)0;
	float3 normal			= (float3)0;
	float3 sightVector		= (float3)0;

	light = In.lightDirection.xyz;

	normal			= tex2D( normalSampler, In.bumpUV).xyz;	//バンプテクスチャの読み取り
	normal.y		= (1-normal.y)*height;					//波の高さの設定
	normal			= normal*2-1;							//

	//反射ベクトルを算出する
	reflectVector	= reflect(-normalize(In.toEye), normal);//reflect関数:反射ベクトルvを返す → i:入射ベクトル,n:サーフェス法線 reflect(i,n){ v = i - 2*dot(i,n)*n }
	
	//視線ベクトル
	sightVector = In.toEye - eyePosition;

	//色の演算
	//反射強度
	float4 intensity = dot(light, normal);							//ディフューズ項
						+ pow(dot(reflectVector,sightVector),12);	// スペキュラー項(照明モデルは　フォン phong)
	color = float4(0.2,0.2,0.7,1);							//青
	intensity += color;

	return intensity; 

	float4 texel = tex2D(normalSampler, In.uv);
	float4 finalColor = texel;// *In.diffuse;
	//return finalColor;

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