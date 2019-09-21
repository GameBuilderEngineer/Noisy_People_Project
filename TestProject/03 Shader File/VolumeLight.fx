//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//グローバル
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
float4x4	matrixWVP;		//ワールド×ビュー×遠近射影
float4		color;			//ボリュームライトの色を指定する
float		farZ;			//遠近射影行列の最遠近距離

sampler textureSampler0 :register(s0);	//PASS0:ボリュームライトのテクスチャ PASS1:シーンのカラー情報テクスチャ
sampler textureSampler1 :register(s1);	//バックバッファのＺ値
sampler textureSampler2 :register(s2);	//ボリュームライトの手前面のZ値
sampler textureSampler3 :register(s3);	//ボリュームライトの奥面のZ値

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//ボリュームライトのZ値を取得する
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct VSVolume_OUTPUT
{
	float4 position		:POSITION;
	float2 uv			:TEXCOORD0;
	float4 positionWVP	:TEXCOORD1;
};

VSVolume_OUTPUT VSVolume(
	float4 position		: POSITION,
	float2 uv			: TEXCOORD0)
{
	VS_OUTPUT Out;

	Out.position = mul(position, matrixWVP);
	Out.uv = uv;
	Out.positionWVP = Out.position;

	return Out;
}

float4 PSVolume(VSVolume_OUTPUT In) : COLOR0
{
	float4 Out;

	//Z値をセット
	Out.r = In.positionWVP.z / farZ;

	//ボリュームライトに張り付けたテクスチャから色情報を取得する
	Out.g = tex2D(textureSampler0, In.uv);

	Out.b = 0.0f;
	Out.a = 0.0f;

	return Out;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//ボリュームライトの厚み情報を取得し、シーンのカラー情報と加算合成する
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct VSFinal_OUTPUT
{
	float4 position	: POSITION;
	float2 uv		:TEXCOORD0;
};

VSFinal_OUTPUT VSFinal(
	float4 position		: POSITION,
	float2 uv : TEXCOORD0)
{
	VSFinal_OUTPUT Out;
	Out.position = position;
	Out.uv = uv;

	return Out;
}

//ボリュームライトの明るさを調整するための関数
float Adjustment(float parameter)
{
	return parameter * 120.0f;
}

float4 PSFinal(VSFinal_OUTPUT In):COLOR
{
	float4 col;

	//バックバッファのZ値情報
	float z = tex2D(textureSampler1, In.uv).r;

	//ボリュームライトの手前面のZ値を取得
	float2 ColCCW = tex2D(textureSmapler2, In.uv).rg;

	//ボリュームライトの奥面のZ値を取得
	float2 ColCW = tex2D(textureSampler3, In.uv).rg;

	float C;

	//ボリュームライトが当たらない部分 または シーンのZ値 <= ボリュームライトの手前面のZ値のとき(ボリュームライトを完全に遮断するとき)
	if (ColCW.r == 0.0f || z <= ColCCW.r)
	{
		col = tex2D(texuterSampler0, In.uv);
	}

	//シーンのZ値 <= ボリュームライトの奥面のZ値のとき
	//ボリュームライトの中にオブジェクトが存在するとき
	else if (z <= ColCw.r && z >= ColCCW.r)
	{
		C = (z - ColCCW.r);

		//ボリュームライトの明るさを調節し、さらに手前面と奥面でそれぞれテクスチャから取得した色情報の平均値を積算する
		C = Adjustment(C)*(ColCCW.g + ColCW.g)*0.5f;
		col = tex2D(textureSampler0, In.uv) + color * C;
	}

	else
	{
		C = (ColCW.r - ColCCW.r);
		C = Adjustment(C) * (ColCCW.g + ColCW.g) * 0.5f;
		Col = tex2D(textureSampler0, In.uv) + color * C;
	}

	return col;
}

technique main
{
	//ボリュームライトのZ値を取得
	pass P0
	{
		VertexShader = compile vs_2_0 VSVolume();
		PixelShader = compile vs_2_0 PSVolume();
	}

	//ボリュームライトの厚みを計算し、シーンの色情報と加算合成する
	pass P1
	{
		VertexShader = compile vs_2_0 VSFinal();
		PixelShader = compile vs_2_0 PSFinal();
	}
}