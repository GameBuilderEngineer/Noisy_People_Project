///////////////////////////////////////////////////////////////////////////////////
//グローバル
///////////////////////////////////////////////////////////////////////////////////
float4x4	matrixProjection;
float4x4	matrixView;
float4		lightDirection;
float3		eyePosition;		//頂点からカメラへのベクトル
float2		waveMove;			//波の移動位置(UV)
float		height;				//波の高さ
texture		textureDecal;
texture		normalMap;
//float4		diffuse;
//float		alphaValue;
//float4		lightDirection = float4(-1.0f, 1.0f, -1.0f, 0.2f);

sampler textureSampler = sampler_state
{
    Texture = <textureDecal>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = NONE;

	AddressU = Wrap;
	AddressV = Wrap;
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
	//float4 diffuse		: COLOR0;
	//float4 color			: COLOR1;
    //float3 lightDirection	: TEXCOORD1;//ライトの方向（バーテックスシェーダーで反転して保存）
    float3 eye				: TEXCOORD2;//頂点からカメラへのベクトル（接空間投影）
    float2 bumpUV			: TEXCOORD3;//バンプテクスチャUV
};

///////////////////////////////////////////////////////////////////////////////////
//バーテックス・シェーダー
///////////////////////////////////////////////////////////////////////////////////
VS_OUT VS(
	float4 position		: POSITION0,
	float3 normal		: NORMAL,
	float3 tangent		: TANGENT0,
	float2 localUV		: TEXCOORD0,
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
	//float ambient = lightDirection.w;

	//ランバート演算カラー
	//float4 lambert = max(ambient, saturate(dot(normal, lightDirection)));
	//Out.diffuse = lambert * diffuse;
	//Out.color = color;
	//Out.Color = vColor;

	//Out.Tex = Texcoord;

	//①視線ベクトルを計算
	//float3 eye = normalize(eyePos.xyz - pos.xyz);
	//②視線ベクトルを頂点座標系に変換する
	//Out.eye.x = dot(eye, tangent);
	//Out.eye.y = dot(eye, binormal);
	//Out.eye.z = dot(eye, normal);
	//Out.eye = normalize(Out.eye);
	//③頂点座標->ライトの位置ベクトル
	//float3 light = -lightDirection.xyz;
	//④ライトベクトルを頂点座標系に変換する
	//Out.light.x = dot(light, tangent);
	//Out.light.y = dot(light, binormal);
	//Out.light.z = dot(light, normal);
	//Out.light = normalize(Out.light);


	//接空間の作成
	float3 N = normal;			//法線
	N.y += height;				//波の高さを追加
	float3 T = tangent;			//接線
	float3 B = cross(N,T);		//従法線
	//float3 B = binormal;		//従法線

	//頂点からカメラへのベクトル
	float3 eye = eyePosition - position.xyz;	
	//カメラへのベクトルを接空間へ投影する
	Out.eye.x = dot(eye,T);
	Out.eye.y = dot(eye,B);
	Out.eye.z = dot(eye,N);
	//Out.eye = normalize(Out.eye);

	//ライトの向きを保存
	//float3 L = -lightDirection.xyz;//ライトの向きを反転
	//ライトの方向を接空間へ投影
	//Out.lightDirection.x = dot(L,T);
	//Out.lightDirection.y = dot(L,B);
	//Out.lightDirection.z = dot(L,N);
	//Out.lightDirection = normalize(Out.lightDirection);

	//バンプマップテクスチャUV座標
	Out.bumpUV	=	localUV;
	Out.bumpUV	+=	waveMove;		//波の移動

	return Out;
}
///////////////////////////////////////////////////////////////////////////////////
//ピクセル・シェーダー
///////////////////////////////////////////////////////////////////////////////////
float4 PS(VS_OUT In) : COLOR
{   
	//①法線マップを参照し、法線を取得する
	//法線マップは0.0f～1.0fの範囲に保存してあるので-1.0f～1.0fの範囲に変換する
	//float3 normal = 2.0f * tex2D(normalSampler,In.bumpUV).xyz -1.0f;
	//②フォンシェーディングによるスペキュラーの色を計算する
	//ハーフベクトルの計算
	//float3 H = normalize(In.light + In.eye);
	//③スペキュラーカラーを計算する
	//float3 S = pow(max(0.0f,dot(normal,H)),specular)* specularPower
	//④合成する
	//return tex2D(textureSampler, In.localUV)*max(ambient,dot(normal,In.light))+S;

	//float4 colorValue		= (float4)0;
	//float3 light			= (float3)0;
	//float3 reflectVector	= (float3)0;
	//float3 normalVector		= (float3)0;
	//float3 sightVector		= (float3)0;

	//colorValue = float4(0.2, 0.2, 0.7, 1);							//青

	//light = lightDirection.xyz;

	//normalVector = tex2D(normalSampler, In.bumpUV).xyz;
	//normalVector.y = (1 - normalVector.y) * height;
	//normalVector = normalVector * 2 - 1;
	//normal = normalVector;
	//normal			= tex2D( normalSampler, In.bumpUV).xyz;	//バンプテクスチャの読み取り
	//normal.y		= (1-normal.y)*height;					//波の高さの設定
	//normal			= normal*2-1;							//-1.0f～1.0fの範囲に変換

	//明るさ(ディフューズ項)
	//float bright = saturate(dot(light, normal));

	//スペキュラの計算
	//float3 H = normalize(In.eye + light);		//ハーフベクトル
	//float specular = max(0, dot(H, normal));	//反射光
	//specular = pow(specular, 12);



	//反射ベクトルを算出する
	//reflect関数:反射ベクトルvを返す 
	//→ i:入射ベクトル,n:サーフェス法線 reflect(i,n){ v = i - 2*dot(i,n)*n }
	//reflectVector	= reflect(-normalize(In.eye), normalVector);
	
	//視線ベクトル
	//sightVector = In.eye - eyePosition;

	//色の演算
	//反射強度
	//float4 intensity = dot(light, normal);							//ディフューズ項
	//float4 intensity;// = bright;							//ディフューズ項
	//					+ pow(dot(reflectVector,sightVector),12);	// スペキュラー項(照明モデルは　フォン phong)
	//float4 intensity = dot(light, normalVector);
	//	+ pow(dot(reflectVector, sightVector), 12);
	//intensity += colorValue;
	//return intensity;
	//intensity = color * bright;
	//intensity.xyz += specular;

	//return intensity; 

	//float4 texel = tex2D(normalSampler, In.bumpUV);
	float4 texel = tex2D(textureSampler, In.bumpUV);
	float4 finalColor = texel;// *In.diffuse;
	return finalColor;

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