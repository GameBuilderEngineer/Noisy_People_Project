#pragma once

#include "Base.h"
#include <xapobase.h>
#pragma comment(lib,"xapobase.lib")

//*****************************************************************************
// 構造体宣言
//*****************************************************************************
#define FADE_TIME	(0.0f)

//*****************************************************************************
// 構造体宣言
//*****************************************************************************
typedef struct	// SAI式I フェイドイン(only for 2ch)
{
	float			fadeTime;		// 遅延の秒数
}SAI_APO_FADE_IN;

//*****************************************************************************
// SAI_FADE_IN_APO (only for 2ch)
//*****************************************************************************
class __declspec(uuid("{80D4BED4-7605-4E4C-B29C-5479C375C599}"))SAI_FADE_IN_APO : public CXAPOParametersBase
{
public:
	SAI_FADE_IN_APO();	// 初期化用
	~SAI_FADE_IN_APO() {};					// 終了用

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

	// フェイド用
	bool	isFade;		// true:フェイド中 false:フェイド完了
	float	*backupBuf;
	int		fadeSample;
	int		fadeSampleIndex;

	IXAudio2SourceVoice *sourceVoice;

	// パラメータ
	SAI_APO_FADE_IN apoParameter[3];
};