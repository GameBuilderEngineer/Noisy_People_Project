#pragma once

#include "Base.h"
#include <xapobase.h>
#pragma comment(lib,"xapobase.lib")

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define DELAY_REVERB_WET_VOLUME		(0.0f)
#define DELAY_REVERB_DRY_VOLUME		(0.0f)
#define DELAY_REVERB_TIME_DEFAULT		(0.0f)
#define DELAY_REVERB_REVERB_VOLUME		(0.0f)

//*****************************************************************************
// 構造体宣言
//*****************************************************************************
typedef struct	// SAI式I 遅延処理(only for 2ch)
{
	float			wetVol;			// ウェットの音量
	float			dryVol;			// ドライの音量
	float			delayTime;		// 遅延の秒数
	float			reverbVol;		// 残響の音量
}SAI_APO_DELAY_REVERB;

//*****************************************************************************
// SAI_DELAY_REVERB_APO (only for 2ch)
//*****************************************************************************
class __declspec(uuid("{80D4BED4-7605-4E4C-B29C-5479C375C599}"))SAI_DELAY_REVERB_APO : public CXAPOParametersBase
{
public:
	SAI_DELAY_REVERB_APO();	// 初期化用
	~SAI_DELAY_REVERB_APO();					// 終了用

	// LockForProcessのオーバーライド
	STDMETHOD(LockForProcess)
		(UINT32 inputLockedParameterCount,
			const XAPO_LOCKFORPROCESS_BUFFER_PARAMETERS * pInputLockedParameters,
			UINT32 outputLockedParameterCount,
			const XAPO_LOCKFORPROCESS_BUFFER_PARAMETERS * pOutputLockedParameters)
		override;

	// Processのオーバーライド
	STDMETHOD_(void, Process)
		(UINT32 inputProcessParameterCount,
			const XAPO_PROCESS_BUFFER_PARAMETERS * pInputProcessParameters,
			UINT32 outputProcessParameterCount,
			XAPO_PROCESS_BUFFER_PARAMETERS * pOutputProcessParameters,
			BOOL isEnabled)
		override;

	// OnSetParametersのオーバーライト
	virtual void OnSetParameters
	(const void* pParameters, UINT32 ParameterByteSize);

private:
	static XAPO_REGISTRATION_PROPERTIES _regProps;

	// フォーマット
	WAVEFORMATEX inFormat;
	WAVEFORMATEX outFormat;

	// エコー用
	float	*backupBuf;
	int		delaySample;
	int		readPos;
	int		writePos;
	bool	play;

	// パラメータ
	SAI_APO_DELAY_REVERB apoParameter[3];
};