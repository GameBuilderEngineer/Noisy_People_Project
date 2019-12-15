#include "SaiDelayReverb.h"

//=============================================================================
// APOクラスの登録
//=============================================================================
XAPO_REGISTRATION_PROPERTIES SAI_DELAY_REVERB_APO::_regProps = {
	__uuidof(SAI_DELAY_REVERB_APO),
	L"SAI_DELAY_REVERB_APO",
	L"Copyright (C)2019 CHOI YAU KONG",
	1,
	0,
	XAPOBASE_DEFAULT_FLAG,
	1, 1, 1, 1 };

//=============================================================================
// クラスの初期化
//=============================================================================
SAI_DELAY_REVERB_APO::SAI_DELAY_REVERB_APO() : CXAPOParametersBase(&_regProps, (BYTE *)apoParameter, sizeof(SAI_APO_DELAY_REVERB), false)
{
	// パラメータの初期化
	for (int i = 0; i < 3; i++)
	{
		apoParameter[i].wetVol = DELAY_REVERB_WET_VOLUME;
		apoParameter[i].dryVol = DELAY_REVERB_DRY_VOLUME;
		apoParameter[i].delayTime = DELAY_REVERB_TIME_DEFAULT;
		apoParameter[i].reverbVol = DELAY_REVERB_REVERB_VOLUME;
	}
}

//=============================================================================
// クラスの終了処理
//=============================================================================
SAI_DELAY_REVERB_APO::~SAI_DELAY_REVERB_APO()
{
	// メモリ解放
	SAFE_DELETE(backupBuf);
}

//=============================================================================
// LockForProcessのオーバーライド
//=============================================================================
HRESULT SAI_DELAY_REVERB_APO::LockForProcess
(UINT32 inputLockedParameterCount,
	const XAPO_LOCKFORPROCESS_BUFFER_PARAMETERS * pInputLockedParameters,
	UINT32 outputLockedParameterCount,
	const XAPO_LOCKFORPROCESS_BUFFER_PARAMETERS * pOutputLockedParameters)
{
	const HRESULT hr = CXAPOParametersBase::LockForProcess(
		inputLockedParameterCount,
		pInputLockedParameters,
		outputLockedParameterCount,
		pOutputLockedParameters);

	if (SUCCEEDED(hr))
	{
		memcpy(&inFormat, pInputLockedParameters[0].pFormat, sizeof(inFormat));
		memcpy(&outFormat, pOutputLockedParameters[0].pFormat, sizeof(outFormat));

		// 遅延が発生できるサンプリング数(2秒の遅延)
		delaySample = inFormat.nSamplesPerSec * inFormat.nChannels * 2;

		// メモリ確保(2秒のサンプリング)
		backupBuf = new float[delaySample];

		// 初期化
		memset(backupBuf, 0, sizeof(float) * delaySample);

		// 読み込み位置の初期化
		readPos = 0;

		// 書き出し位置の初期化
		writePos = 0;
	}

	return hr;
}

//=============================================================================
// OnSetParametersのオーバーライド
//=============================================================================
void SAI_DELAY_REVERB_APO::OnSetParameters
(const void* pParameters, UINT32 ParameterByteSize)
{
	SAI_APO_DELAY_REVERB *tmpParameters = ((SAI_APO_DELAY_REVERB *)pParameters);

	XAPOASSERT(sizeof(SAI_APO_DELAY_REVERB) > 0);
	XAPOASSERT(pParameters != NULL);
	XAPOASSERT(ParameterByteSize == sizeof(SAI_APO_DELAY_REVERB));
	XAPOASSERT(tmpParameters->dryVol >= 0);
	XAPOASSERT(tmpParameters->wetVol >= 0);
	XAPOASSERT(tmpParameters->reverbVol >= 0);
	XAPOASSERT(tmpParameters->delayTime >= 0);
	XAPOASSERT(tmpParameters->dryVol <= 1.0f);
	XAPOASSERT(tmpParameters->wetVol <= 1.0f);
	XAPOASSERT(tmpParameters->reverbVol <= 0.5f);
	XAPOASSERT(tmpParameters->delayTime <= 2.0f);

	// 初期化
	memset(backupBuf, 0, sizeof(float) * delaySample);

	// 読み込み位置の初期化
	readPos = 0;

	// 書き出し位置の初期化
	writePos = 0;
}

//=============================================================================
// Processのオーバーライド
//=============================================================================
void SAI_DELAY_REVERB_APO::Process
(UINT32 inputProcessParameterCount,
	const XAPO_PROCESS_BUFFER_PARAMETERS * pInputProcessParameters,
	UINT32 outputProcessParameterCount,
	XAPO_PROCESS_BUFFER_PARAMETERS * pOutputProcessParameters,
	BOOL isEnabled)
{
	if (isEnabled)
	{
		if (pInputProcessParameters->BufferFlags != XAPO_BUFFER_SILENT)
		{
			// 仮のパラメータ構造体 = 使用するパラメータのポインたー
			SAI_APO_DELAY_REVERB *tmpParmeter = (SAI_APO_DELAY_REVERB *)BeginProcess();

			int tmpDelaySample = (int)(delaySample * (tmpParmeter->delayTime / 2.0f));

			// 入出力のバッファ
			float *inputBuf = (float *)pInputProcessParameters->pBuffer;
			float *outputBuf = (float *)pOutputProcessParameters->pBuffer;

			for (int i = 0; i < ((int)pInputProcessParameters->ValidFrameCount * inFormat.nChannels); i++)
			{
				// バッファの保存
				backupBuf[writePos] = inputBuf[i] + backupBuf[readPos] * tmpParmeter->reverbVol;
				writePos++;
				if (writePos >= tmpDelaySample)	// オーバー防止
				{
					// 0番目に戻る
					writePos = 0;
				}

				// 結果
				readPos = (int)(writePos - tmpDelaySample);
				if (readPos < 0)	// オーバー防止
				{
					// 0番目に戻る
					readPos += tmpDelaySample;
				}

				// 結果
				outputBuf[i] = (backupBuf[readPos] * tmpParmeter->wetVol) + (inputBuf[i] * tmpParmeter->dryVol);
			}

			// エンドプロセス
			EndProcess();
		}
	}
}