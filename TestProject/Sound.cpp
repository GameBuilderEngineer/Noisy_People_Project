//===================================================================================================================================
//【MemoryViewer.cpp】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/09/20
// [更新日]2019/09/20
//===================================================================================================================================

//===================================================================================================================================
//【インクルード】
//===================================================================================================================================
#include "Sound.h"
#include "ImguiManager.h"

//===================================================================================================================================
//【コンストラクタ】
//===================================================================================================================================
SOUND::SOUND()
{
	//COMライブラリの初期化
	if (FAILED(CoInitializeEx(NULL, COINIT_MULTITHREADED)))
	{
		return;
	}

	//XAudio2オブジェクトの作成
	if (FAILED(XAudio2Create(&xAudio2)))
	{
		return ;
	}

	//マスターボイスの作成
	if (FAILED(xAudio2->CreateMasteringVoice(&masteringVoice)))
	{
		return;
	}

	//デバックツール
	XAUDIO2_DEBUG_CONFIGURATION debugConfig{ NULL };
	debugConfig.LogThreadID = true;
	debugConfig.TraceMask = XAUDIO2_LOG_WARNINGS;
	debugConfig.TraceMask = debugConfig.TraceMask | XAUDIO2_LOG_FUNC_CALLS | XAUDIO2_LOG_DETAIL;
	debugConfig.BreakMask = XAUDIO2_LOG_ERRORS;
	xAudio2->SetDebugConfiguration(&debugConfig);

	//3Dサウンドの初期化
	DWORD channelMask;
	masteringVoice->GetChannelMask(&channelMask);
	if (FAILED(X3DAudioInitialize(channelMask, X3DAUDIO_SPEED_OF_SOUND, x3DAudio)))
	{
		return;
	}
	memset(&emitter, 0, sizeof(X3DAUDIO_EMITTER));
	memset(&listener, 0, sizeof(X3DAUDIO_LISTENER));

	emitter.ChannelCount = INPUT_CHANNEL;					//チャンネル数
	emitter.CurveDistanceScaler = 1.0f;						//ボリュームのレベル距離カーブ
	emitter.pChannelAzimuths = emitterAzimuths;

	emitter.OrientFront = X3DAUDIO_VECTOR(0.0f,0.0f,1.0f);	//前方向の向き
	emitter.OrientTop = X3DAUDIO_VECTOR(0.0f,1.0f,0.0f);	//上方向の向き
	emitter.Position = X3DAUDIO_VECTOR(0.0f,457.0f,0.0f);	//音の発生源(位置)
	emitter.Velocity = X3DAUDIO_VECTOR(0.0f, 0.0f, 0.0f);	//ワールド単位/秒のベロシティ

	emitter.InnerRadius = 2.0f;
	emitter.InnerRadiusAngle = X3DAUDIO_PI / 4.0f;;

	emitter.pCone = &emitterCone;
	emitter.pCone->InnerAngle = 0.0f;
	emitter.pCone->OuterAngle = 0.0f;
	emitter.pCone->InnerVolume = 0.0f;
	emitter.pCone->OuterVolume = 1.0f;
	emitter.pCone->InnerLPF = 0.0f;
	emitter.pCone->OuterLPF = 1.0f;
	emitter.pCone->InnerReverb = 0.0f;
	emitter.pCone->OuterReverb = 1.0f;

	const X3DAUDIO_DISTANCE_CURVE_POINT Emitter_LFE_CurvePoints[3] = { 0.0f, 1.0f, 0.25f, 0.0f, 1.0f, 0.0f };
	const X3DAUDIO_DISTANCE_CURVE       Emitter_LFE_Curve = { (X3DAUDIO_DISTANCE_CURVE_POINT*)&Emitter_LFE_CurvePoints[0], 3 };
	const X3DAUDIO_DISTANCE_CURVE_POINT Emitter_Reverb_CurvePoints[3] = { 0.0f, 0.5f, 0.75f, 1.0f, 1.0f, 0.0f };
	const X3DAUDIO_DISTANCE_CURVE       Emitter_Reverb_Curve = { (X3DAUDIO_DISTANCE_CURVE_POINT*)&Emitter_Reverb_CurvePoints[0], 3 };

	emitter.pVolumeCurve = (X3DAUDIO_DISTANCE_CURVE*)&X3DAudioDefault_LinearCurve;
	emitter.pLFECurve = (X3DAUDIO_DISTANCE_CURVE*)&Emitter_LFE_Curve;
	emitter.pLPFDirectCurve = nullptr;
	emitter.pLPFReverbCurve = nullptr;
	emitter.pReverbCurve = (X3DAUDIO_DISTANCE_CURVE*)&Emitter_Reverb_Curve;
	emitter.CurveDistanceScaler = 14.0f;
	emitter.DopplerScaler = 1.0f;

	// アウトプットボイスの作成
	masteringVoice->GetVoiceDetails(&details);

	xAudio2->CreateSubmixVoice(
		&submixVoice,				// 作成するボイス(BGM)
		INPUT_CHANNEL,				// チャンネル数(IN)
		details.InputSampleRate,	// 全体のサンプリング周波数
		XAUDIO2_VOICE_USEFILTER);	// 機能(フィルタ)

	// 送信リスト
	XAUDIO2_SEND_DESCRIPTOR sendDescriptors[2] = {
		XAUDIO2_SEND_USEFILTER,masteringVoice,
		XAUDIO2_SEND_USEFILTER,submixVoice };
	const XAUDIO2_VOICE_SENDS sendlist = { 2, sendDescriptors };
	sourceVoice = setSourceVoice(sendlist);

	//再生
	sourceVoice->Start();
}

//===================================================================================================================================
//【デストラクタ】
//===================================================================================================================================
SOUND::~SOUND()
{
	//音の停止
	sourceVoice->Stop();
	
	//ソースボイス
	if (sourceVoice != NULL)
	{
		sourceVoice->DestroyVoice();
		sourceVoice = NULL;
	}

	//サブミックス
	if (submixVoice != NULL)
	{
		submixVoice->DestroyVoice();
		submixVoice = NULL;
	}


	//バッファの解放
	free(wavFile.data.waveDataMain);

	//COMライブラリの終了処理
	CoUninitialize();

	//マスターボイス
	if (masteringVoice != NULL)
	{
		masteringVoice->DestroyVoice();
		masteringVoice = NULL;
	}

	//インターフェース
	if (xAudio2 != NULL)
	{
		xAudio2->Release();
		xAudio2 = NULL;
	}
}

//===================================================================================================================================
//【3Dサウンドの更新処理】
//===================================================================================================================================
void SOUND::updateSound(D3DXVECTOR3 pos, D3DXVECTOR3 front)
{
	const X3DAUDIO_CONE Listener_DirectionalCone = { X3DAUDIO_PI*5.0f / 6.0f, X3DAUDIO_PI*11.0f / 6.0f, 1.0f, 0.75f, 0.0f, 0.25f, 0.708f, 1.0f };

	listener.OrientFront = (X3DAUDIO_VECTOR)front;			//リスナーの向き
	listener.OrientTop = X3DAUDIO_VECTOR(0.0f, 1.0f, 0.0f);//上方向の向き
	listener.Position = (X3DAUDIO_VECTOR)pos;				//リスナーの位置
	listener.Velocity = X3DAUDIO_VECTOR(0.0f, 0.0f, 0.0f);	//ワールド単位/秒のベロシティ
	listener.pCone = (X3DAUDIO_CONE*)&Listener_DirectionalCone;

	X3DAUDIO_DSP_SETTINGS DSPSettings = { 0 };
	FLOAT32 matrix[OUTPUT_CHANNEL];
	memset(matrix, 0, sizeof(matrix));
	DSPSettings.SrcChannelCount = INPUT_CHANNEL;
	DSPSettings.DstChannelCount = details.InputChannels;
	DSPSettings.pMatrixCoefficients = matrix;

	X3DAudioCalculate(x3DAudio, &listener, &emitter, X3DAUDIO_CALCULATE_MATRIX | X3DAUDIO_CALCULATE_DOPPLER
		| X3DAUDIO_CALCULATE_LPF_DIRECT,&DSPSettings);

	sourceVoice->SetFrequencyRatio(DSPSettings.DopplerFactor);

	sourceVoice->SetOutputMatrix(masteringVoice, INPUT_CHANNEL, details.InputChannels,DSPSettings.pMatrixCoefficients);
	sourceVoice->SetOutputMatrix(submixVoice, INPUT_CHANNEL, 1, &DSPSettings.ReverbLevel);

	XAUDIO2_FILTER_PARAMETERS FilterParametersDirect = { LowPassFilter, 2.0f * sinf(X3DAUDIO_PI / 6.0f * DSPSettings.LPFDirectCoefficient), 1.0f };
	sourceVoice->SetOutputFilterParameters(masteringVoice, &FilterParametersDirect);
	XAUDIO2_FILTER_PARAMETERS FilterParametersReverb = { LowPassFilter, 2.0f * sinf(X3DAUDIO_PI / 6.0f * DSPSettings.LPFReverbCoefficient), 1.0f };
	sourceVoice->SetOutputFilterParameters(submixVoice, &FilterParametersReverb);

	updateSoundIMGUI();
}

//===================================================================================================================================
//【IMGuiへの送信】
//===================================================================================================================================
void SOUND::updateSoundIMGUI(void)
{
#ifdef _DEBUG
	ImGui::Begin("PlayerInformation(Sound)");

	if (ImGui::CollapsingHeader("soundInformation"))
	{
		float limitTop = 1000;
		float limitBottom = -1000;

		//サウンド状態
		if (ImGui::CollapsingHeader("soundState"))
		{
			float outputMatrix[OUTPUT_CHANNEL];
			memset(outputMatrix, 0, sizeof(outputMatrix));
			sourceVoice->GetOutputMatrix(masteringVoice, INPUT_CHANNEL, details.InputChannels, outputMatrix);
			for (int i = 0; i < OUTPUT_CHANNEL; i++)
			{
				ImGui::Text("source->master outputMatrix[%d]:%f", i, outputMatrix[i]);
			}

			ImGui::Text("--------------------------------------------------------");

			ImGui::Text("1.:LowPassFilter           2.:BandPassFilter");
			ImGui::Text("3.:HighPassFilter          4.:NotchFilter");
			ImGui::Text("5.:LowPassOnePoleFilter    6.:HighPassOnePoleFilter");

			ImGui::Text("--------------------------------------------------------");

			XAUDIO2_FILTER_PARAMETERS FilterParametersDirect;
			sourceVoice->GetOutputFilterParameters(masteringVoice, &FilterParametersDirect);
			XAUDIO2_FILTER_PARAMETERS FilterParametersReverb;
			sourceVoice->GetOutputFilterParameters(submixVoice, &FilterParametersReverb);

			ImGui::Text("filter parameters(Direct)    filter parameters(Reverb)");
			ImGui::Text("type:%d                       type:%d", FilterParametersDirect.Type, FilterParametersReverb.Type);
			ImGui::Text("oneOverQ:%.3f               oneOverQ:%.3f", FilterParametersDirect.OneOverQ, FilterParametersReverb.OneOverQ);
			ImGui::Text("frequency:%.3f              frequency:%.3f", FilterParametersDirect.Frequency, FilterParametersReverb.Frequency);
		}

		//エミッター
		if (ImGui::CollapsingHeader("emitter"))
		{
			if (ImGui::CollapsingHeader("soundCone"))
			{
				ImGui::SliderFloat3("position", (float *)&emitter.Position, limitBottom, limitTop);							//位置

				if (ImGui::CollapsingHeader("soundCone"))
				{
					ImGui::SliderFloat("innerAngle", &emitter.pCone->InnerAngle, 0, X3DAUDIO_2PI);							//innerAngle
					ImGui::SliderFloat("outerAngle", &emitter.pCone->OuterAngle, emitter.pCone->InnerAngle, X3DAUDIO_2PI);	//outerAngle

					ImGui::SliderFloat("innerVolume", &emitter.pCone->InnerVolume, 0, 2.0f);								//innerVolume
					ImGui::SliderFloat("outerVolume", &emitter.pCone->OuterVolume, 0, 2.0f);								//outerVolume

					ImGui::SliderFloat("innerLPF", &emitter.pCone->InnerLPF, 0, 1.0f);										//innerLPF
					ImGui::SliderFloat("outerLPF", &emitter.pCone->OuterLPF, 0, 1.0f);										//outerLPF

					ImGui::SliderFloat("innerReverb", &emitter.pCone->InnerReverb, 0, 1.0f);								//innerReverb
					ImGui::SliderFloat("outerReverb", &emitter.pCone->OuterReverb, 0, 1.0f);								//outerReverb
				}
			}
		}

		//リスナー
		if (ImGui::CollapsingHeader("listener"))
		{
			ImGui::SliderFloat3("position", (float *)&listener.Position, limitBottom, limitTop);							//位置

			if (ImGui::CollapsingHeader("soundCone"))
			{
				ImGui::SliderFloat("innerAngle", &listener.pCone->InnerAngle, 0, X3DAUDIO_2PI);								//innerAngle
				ImGui::SliderFloat("outerAngle", &listener.pCone->OuterAngle, listener.pCone->InnerAngle, X3DAUDIO_2PI);	//outerAngle

				ImGui::SliderFloat("innerVolume", &listener.pCone->InnerVolume, 0, 2.0f);									//innerVolume
				ImGui::SliderFloat("outerVolume", &listener.pCone->OuterVolume, 0, 2.0f);									//outerVolume

				ImGui::SliderFloat("innerLPF", &listener.pCone->InnerLPF, 0, 1.0f);											//innerLPF
				ImGui::SliderFloat("outerLPF", &listener.pCone->OuterLPF, 0, 1.0f);											//outerLPF

				ImGui::SliderFloat("innerReverb", &listener.pCone->InnerReverb, 0, 1.0f);									//innerReverb
				ImGui::SliderFloat("outerReverb", &listener.pCone->OuterReverb, 0, 1.0f);									//outerReverb
			}
		}


		//ImGui::SliderFloat3("position", position, limitBottom, limitTop);				//位置
		//ImGui::SliderFloat4("quaternion", quaternion, limitBottom, limitTop);			//回転
		//ImGui::SliderFloat3("scale", scale, limitBottom, limitTop);					//スケール
		//ImGui::SliderFloat("radius", &radius, 0, limitTop);							//半径
		//ImGui::SliderFloat("alpha", &alpha, 0, 255);									//透過値
		//ImGui::SliderFloat3("speed", speed, limitBottom, limitTop);					//速度
		//ImGui::SliderFloat3("acceleration", acceleration, limitBottom, limitTop);		//加速度
		//ImGui::SliderFloat3("gravity", gravity, limitBottom, limitTop);				//重力

		//ImGui::Checkbox("onGravity", &onGravity);										//重力有効化フラグ
		//ImGui::Checkbox("onActive", &onActive);										//アクティブ化フラグ
		//ImGui::Checkbox("onRender", &onRender);										//描画有効化フラグ
		//ImGui::Checkbox("onLighting", &onLighting);									//光源処理フラグ
		//ImGui::Checkbox("onTransparent", &onTransparent);								//透過フラグ
		//ImGui::Checkbox("operationAlpha", &operationAlpha);							//透過値の操作有効フラグ
		//ImGui::Checkbox("sound", &onSound);											//サウンド

		//ImGui::SliderInt("renderNum", &renderNum, 1, (int)limitTop);					//透過値の操作有効フラグ
	}

	ImGui::End();

#endif
}

//===================================================================================================================================
//【ソースボイスの作成】
//===================================================================================================================================
IXAudio2SourceVoice *SOUND::setSourceVoice(const XAUDIO2_VOICE_SENDS sendList)
{
	IXAudio2SourceVoice *tmpSourceVoice = NULL;
	
	WAVEFORMATEX		pcm;	// WAVEFORMATEX構造体
	memset(&pcm, 0, sizeof(WAVEFORMATEX));

	XAUDIO2_BUFFER		buffer;	// バッファ
	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));

	//ファイルオープン
	setSoundDirectory();
	fp = fopen(SOUND_PATH_TEST, "rb");
	wavFile = LoadWavChunk(fp);

	pcm.cbSize = 0;
	pcm.nChannels = wavFile.fmt.fmtChannel;
	pcm.wBitsPerSample = wavFile.fmt.fmtBitPerSample;
	pcm.nSamplesPerSec = wavFile.fmt.fmtSampleRate;
	pcm.wFormatTag = WAVE_FORMAT_PCM;
	pcm.nBlockAlign = (wavFile.fmt.fmtChannel*wavFile.fmt.fmtBitPerSample) / 8;
	pcm.nAvgBytesPerSec = wavFile.fmt.fmtSampleRate*pcm.nBlockAlign;

	// バッファの設定
	buffer.pAudioData = (BYTE*)wavFile.data.waveDataMain;
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.AudioBytes = wavFile.data.waveSize;
	buffer.LoopCount = XAUDIO2_LOOP_INFINITE;

	// ソースボイスの作成
	if (FAILED(xAudio2->CreateSourceVoice(&tmpSourceVoice, &pcm, XAUDIO2_VOICE_USEFILTER, XAUDIO2_MAX_FREQ_RATIO,nullptr, &sendList)))
	{
		return NULL;
	}

	// バッファを提出
	if (FAILED(tmpSourceVoice->SubmitSourceBuffer(&buffer)))
	{
		return NULL;
	}

	return tmpSourceVoice;
}

//=============================================================================
// WAVファイルのチャンク処理
// 引数:FILE構造体(fopen済み) 戻り値:WAVファイルの構造体(WAVの中身)
// 使用方法:
// パスはいらない、fopen終わったらここに入る
//=============================================================================
WAV_FILE SOUND::LoadWavChunk(FILE *inFp)
{
	WAV_FILE tmpWavFile;	// WAVファイル構造体
	memset(&tmpWavFile, 0, sizeof(tmpWavFile));

	// riffの読み込み
	fread(&tmpWavFile.riff, sizeof(RIFF_CHUNK), 1, inFp);

	int	chunkFlag = NULL;	// チャンクを探すループのフラグ

	// fmt_とdataの読み込み
	while (chunkFlag != FLAG_CHUNK_END)
	{
		char	chunk[CHUNK_ID_SIZE];
		long	size = 0;

		// チャンクとサイズの読み込み
		fread(&chunk, sizeof(chunk), 1, inFp);
		fread(&size, sizeof(size), 1, inFp);

		// fmt_チャンク
		if (memcmp(chunk, CHUNK_FMT, CHUNK_ID_SIZE) == 0)
		{
			// チャンクとサイズを設定
			memcpy(tmpWavFile.fmt.fmtChunk, chunk, CHUNK_ID_SIZE);
			tmpWavFile.fmt.fmtSize = size;

			// フォーマットIDから読み込み
			fread(&tmpWavFile.fmt.fmtFormatTag,
				sizeof(FMT_CHUNK) - (sizeof(tmpWavFile.fmt.fmtChunk) + sizeof(tmpWavFile.fmt.fmtSize)), 1, inFp);

			// フラグ処理
			chunkFlag += FLAG_CHUNK_FMT;

			continue;
		}

		// dataチャンク
		if (memcmp(chunk, CHUNK_DATA, CHUNK_ID_SIZE) == 0)
		{
			// チャンクとサイズを設定
			memcpy(tmpWavFile.data.dataChunk, CHUNK_DATA, CHUNK_ID_SIZE);
			tmpWavFile.data.waveSize = size;

			// データの読み込み
			tmpWavFile.data.waveDataMain = (short *)malloc(tmpWavFile.data.waveSize);
			fread(tmpWavFile.data.waveDataMain, tmpWavFile.data.waveSize, 1, inFp);

			// フラグ処理
			chunkFlag += FLAG_CHUNK_DATA;

			continue;
		}

		// 他のチャンク
		else
		{
			// いらない分をスキップ!
			fseek(inFp, size, SEEK_CUR);
		}
	}

	fclose(inFp);

	return tmpWavFile;
}
