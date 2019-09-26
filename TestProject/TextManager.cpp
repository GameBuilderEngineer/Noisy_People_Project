//============================================================================================================================================
// Document
//============================================================================================================================================
// TextManager.cpp
// HAL東京 GP-12A-332 09 亀岡竣介
// 2019/08/02
//============================================================================================================================================
#include "TextManager.h"
//============================================================================================================================================
// Using Declaration
// using宣言
//============================================================================================================================================
using namespace textManagerNS;
//============================================================================================================================================
// Constructor
// コンストラクタ
//============================================================================================================================================
TextManager::TextManager(void)
{
	// ディレクトリの設定
	setDataDirectory();

	// 初期化
	for (int i = 0; i < TYPE::TYPE_MAX; i++)
	{
		switch (i)
		{
		case NEW_RODIN_CHARACTER_SELECT_TIME:
		case NEW_RODIN_RESULT_CHINGIN_BACK:
		case NEW_RODIN_RESULT_CHINGIN:
		case NEW_RODIN_GAME_TIME:
			dataName[i] = { "FOT-NewRodinPro-UB.otf" };		//	フォントデータリソースの名前
			fontName[i] = { "FOT-ニューロダン Pro UB" };	//	フォントの名前
			break;
		case FUTURA:
			dataName[i] = { "unicode.futurabb.ttf" };	//	フォントデータリソースの名前
			fontName[i] = { "Futura-Black" };			//	フォントの名前
		default:
			break;
		}
	}

	for (int i = 0; i < TYPE::TYPE_MAX; i++)
	{
		// フォントデータの読み込み
		AddFontResourceEx(dataName[i], FR_PRIVATE, NULL);
		// インスタンス作成
		text[i] = new Text();
	}

	return;
}
//============================================================================================================================================
// Destructor
// デストラクタ
//============================================================================================================================================
TextManager::~TextManager(void)
{
	for (int i = 0; i < TYPE::TYPE_MAX; i++)
	{
		// フォントデータ削除
		RemoveFontResourceEx(dataName[i], FR_PRIVATE, NULL);
		// インスタンス削除
		SAFE_DELETE(text[i]);
	}

	return;
}
//============================================================================================================================================
// initialize
// 初期化
//============================================================================================================================================
void TextManager::initialize(LPDIRECT3DDEVICE9 _device)
{
	// 初期化
	for (int i = 0; i < TYPE::TYPE_MAX; i++)
	{
		switch (i)
		{
		case NEW_RODIN_CHARACTER_SELECT_TIME:
			text[i]->initialize(_device, 80, 45, D3DXCOLOR(0, 0, 0, 255), fontName[i]);
			break;
		case NEW_RODIN_RESULT_CHINGIN_BACK:
			text[i]->initialize(_device, 160, 90, D3DXCOLOR(0, 0, 0, 255), fontName[i]);
			break;
		case NEW_RODIN_RESULT_CHINGIN:
			text[i]->initialize(_device, 160, 90, D3DXCOLOR(255, 255, 255, 255), fontName[i]);
			break;
		case NEW_RODIN_GAME_TIME:
			text[i]->initialize(_device, 48, 27, D3DXCOLOR(0, 0, 0, 255), fontName[i]);
			break;
		case FUTURA:
			text[i]->initialize(_device, 48, 27, D3DXCOLOR(0, 0, 0, 255), fontName[i]);
		default:
			break;
		}
	}

	return;
}