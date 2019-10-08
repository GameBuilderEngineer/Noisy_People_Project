//===================================================================================================================================
//�yTextureLoader.cpp�z
// [�쐬��]HAL����GP12A332 11 ���� ��
// [�쐬��]2019/09/22
// [�X�V��]2019/10/05
//===================================================================================================================================

//===================================================================================================================================
//�y�C���N���[�h�z
//===================================================================================================================================
#include "TextureLoader.h"

//===================================================================================================================================
//�y���O��ԁz
//===================================================================================================================================
using namespace textureNS;

//===================================================================================================================================
//�y�O���[�o���ϐ��z
//===================================================================================================================================
TextureLoader* pointerTextureLoader = NULL;	//�O���Q�Ɨp

//===================================================================================================================================
//�y�R���X�g���N�^�z
//===================================================================================================================================
TextureLoader::TextureLoader()
{
	//�O���[�o���|�C���^�֎��g�̃|�C���^����
	if(pointerTextureLoader==NULL)pointerTextureLoader = this;

	// Splash
	textureName[SPLASH] = { "Splash.png" };

	// Tutorial
	textureName[TUTORIAL] = { "tutorial1.png" };

	// Title
	textureName[TITLE_LOGO] = { "Title_Logo.png" };

	// Operation
	textureName[OPERATION_GAMEPAD] = { "Operation_GamePad.png" };
	textureName[OPERATION_KEYBOARD] = { "Operation_KeyBoard.png" };

	// Credit
	textureName[CREDIT] = { "Credit.png" };

	// Game
	textureName[UI_COUNT_DOWN_ONE] = { "Game_CountDown_One.png" };

	//effect
	textureName[EFFECT_DAMAGE] = { "Game_Damage_Effect.png" };

	// Result
	textureName[RESULT_WINDOW] = { "Result_Window.png" };

	// etc
	textureName[TOON_OUT_LINE] = { "Outline.bmp" };
	textureName[TOON_SHADE] = { "Shade.bmp" };
	textureName[LIGHT_001] = { "Light_001.png" };
	textureName[TARGET] = { "stencil.png" };
	textureName[SAMPLE_TREE] = { "tree.png" };
	textureName[SAMPLE_GRASS] = { "middleGrass.png" };

}

//===================================================================================================================================
//�y�f�X�g���N�^�z
//===================================================================================================================================
TextureLoader::~TextureLoader()
{
	pointerTextureLoader = NULL;
	release();
}

//===================================================================================================================================
//�y���[�h�z
//===================================================================================================================================
void TextureLoader::load(LPDIRECT3DDEVICE9 device)
{
	setVisualDirectory();
	for (int i = 0; i < MAX_TEXTURE; i++)
	{
		//�e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(device,textureName[i],&texture[i]);
	}
}

//===================================================================================================================================
//�y����z
//===================================================================================================================================
void TextureLoader::release()
{
	for (int i = 0; i < MAX_TEXTURE; i++)
	{
		if (texture[i] != NULL)
		{//�e�N�X�`���̉��
			texture[i]->Release();
			texture[i] = NULL;
		}
	}
}

//===================================================================================================================================
//�y�e�N�X�`���O���Q�Ɗ֐��z
//===================================================================================================================================
LPDIRECT3DTEXTURE9* textureNS::reference(int textureNo)
{
	return pointerTextureLoader->getTexture(textureNo);
}