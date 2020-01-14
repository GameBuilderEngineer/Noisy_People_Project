///////////////////////////////////////////////////////////////////////////////////
//グローバル
///////////////////////////////////////////////////////////////////////////////////
//float4x4 mWorld: World;
//float4x4 mWorldViewProj : WorldViewProjection;	
float3 lightPosition = {100.0f, 100.0f, -100.0f};	//ライト位置
//float4 vLightDir;
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

//単位行列
#define IDENTITY_MATRIX float4x4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1)

//逆行列関数
float4x4 inverse(float4x4 m) {
	float n11 = m[0][0], n12 = m[1][0], n13 = m[2][0], n14 = m[3][0];
	float n21 = m[0][1], n22 = m[1][1], n23 = m[2][1], n24 = m[3][1];
	float n31 = m[0][2], n32 = m[1][2], n33 = m[2][2], n34 = m[3][2];
	float n41 = m[0][3], n42 = m[1][3], n43 = m[2][3], n44 = m[3][3];

	float t11 = n23 * n34 * n42 - n24 * n33 * n42 + n24 * n32 * n43 - n22 * n34 * n43 - n23 * n32 * n44 + n22 * n33 * n44;
	float t12 = n14 * n33 * n42 - n13 * n34 * n42 - n14 * n32 * n43 + n12 * n34 * n43 + n13 * n32 * n44 - n12 * n33 * n44;
	float t13 = n13 * n24 * n42 - n14 * n23 * n42 + n14 * n22 * n43 - n12 * n24 * n43 - n13 * n22 * n44 + n12 * n23 * n44;
	float t14 = n14 * n23 * n32 - n13 * n24 * n32 - n14 * n22 * n33 + n12 * n24 * n33 + n13 * n22 * n34 - n12 * n23 * n34;

	float det = n11 * t11 + n21 * t12 + n31 * t13 + n41 * t14;
	float idet = 1.0f / det;

	float4x4 ret;

	ret[0][0] = t11 * idet;
	ret[0][1] = (n24 * n33 * n41 - n23 * n34 * n41 - n24 * n31 * n43 + n21 * n34 * n43 + n23 * n31 * n44 - n21 * n33 * n44) * idet;
	ret[0][2] = (n22 * n34 * n41 - n24 * n32 * n41 + n24 * n31 * n42 - n21 * n34 * n42 - n22 * n31 * n44 + n21 * n32 * n44) * idet;
	ret[0][3] = (n23 * n32 * n41 - n22 * n33 * n41 - n23 * n31 * n42 + n21 * n33 * n42 + n22 * n31 * n43 - n21 * n32 * n43) * idet;

	ret[1][0] = t12 * idet;
	ret[1][1] = (n13 * n34 * n41 - n14 * n33 * n41 + n14 * n31 * n43 - n11 * n34 * n43 - n13 * n31 * n44 + n11 * n33 * n44) * idet;
	ret[1][2] = (n14 * n32 * n41 - n12 * n34 * n41 - n14 * n31 * n42 + n11 * n34 * n42 + n12 * n31 * n44 - n11 * n32 * n44) * idet;
	ret[1][3] = (n12 * n33 * n41 - n13 * n32 * n41 + n13 * n31 * n42 - n11 * n33 * n42 - n12 * n31 * n43 + n11 * n32 * n43) * idet;

	ret[2][0] = t13 * idet;
	ret[2][1] = (n14 * n23 * n41 - n13 * n24 * n41 - n14 * n21 * n43 + n11 * n24 * n43 + n13 * n21 * n44 - n11 * n23 * n44) * idet;
	ret[2][2] = (n12 * n24 * n41 - n14 * n22 * n41 + n14 * n21 * n42 - n11 * n24 * n42 - n12 * n21 * n44 + n11 * n22 * n44) * idet;
	ret[2][3] = (n13 * n22 * n41 - n12 * n23 * n41 - n13 * n21 * n42 + n11 * n23 * n42 + n12 * n21 * n43 - n11 * n22 * n43) * idet;

	ret[3][0] = t14 * idet;
	ret[3][1] = (n13 * n24 * n31 - n14 * n23 * n31 + n14 * n21 * n33 - n11 * n24 * n33 - n13 * n21 * n34 + n11 * n23 * n34) * idet;
	ret[3][2] = (n14 * n22 * n31 - n12 * n24 * n31 - n14 * n21 * n32 + n11 * n24 * n32 + n12 * n21 * n34 - n11 * n22 * n34) * idet;
	ret[3][3] = (n12 * n23 * n31 - n13 * n22 * n31 + n13 * n21 * n32 - n11 * n23 * n32 - n12 * n21 * n33 + n11 * n22 * n33) * idet;

	return ret;
}

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
	//float4 diffuse			: COLOR0;
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

	//逆ワールド行列
	float4x4 inverseWorld = inverse(worldMatrix);

	//UV座標
	Out.uv = localUV;

	//アンビエントカラー
	//float ambient = lightDirection.w;

	//ランバート演算カラー
	//float4 lambert = max(ambient, saturate(dot(normal, lightDirection)));
	//Out.diffuse = lambert * diffuse;
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
	float3 L = -mul( lightDirection, inverseWorld).xyz;//ライトの向きを反転
	//ライトの方向を接空間へ投影
	Out.lightDirection.x = dot(L,T);
	Out.lightDirection.y = dot(L,B);
	Out.lightDirection.z = dot(L,N);

	//バンプマップテクスチャUV座標
	Out.bumpUV	=	localUV;	
	Out.bumpUV	+=	waveMove;		//波の移動

	//ワールド行列（転置/逆）
	float4x4 WIT;	// world inverse transpose
	WIT = inverseWorld;
	WIT = transpose(WIT);

	//接空間法線
	float3 tangentNormal = normalize(normal);
	Out.tangentNormal = mul(tangentNormal, WIT).xyz;//ワールド空間へ補正する

	//頂点からライトまでのベクトル
	float4 f4Position		= float4(position.xyz,1);
    float3 worldPosition	= mul(f4Position, worldMatrix).xyz;//ワールド空間上の頂点位置
    Out.lightVector			= normalize(lightPosition - worldPosition);
	
	//頂点ワールド位置からビュー行列位置へのベクトル（直感的には頂点位置からカメラ位置へのベクトル）
	//※サンプルに書いてあったので今はそのまま書いたが、toEyeと同じ気がする。
	float4x4 viewInverse = inverse(matrixView);//ビュー行列（逆）
	Out.tangentView = normalize(viewInverse[3].xyz - worldPosition);
	
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
	float4 intensity =	dot(light,normal)							//ディフューズ項
						+ pow(dot(reflectVector,sightVector),12);	// スペキュラー項(照明モデルは　フォン phong)
	color = float4(0.2,0.2,0.7,1);							//青
	intensity += color;

	return intensity; 

	//float4 texel = tex2D(textureSampler, In.uv);
	//float4 finalColor = texel * In.diffuse;
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