//===================================================================================================================================
//�yEffekseerManager.h�z
// [�쐬��]HAL����GP12A332 11 ���� ��
// [�쐬��]2019/10/18
// [�X�V��]2019/10/21
//===================================================================================================================================
#pragma once

//===================================================================================================================================
//���l�F���̃N���X���v���W�F�N�g�֎g�p����ۂɁA�v���W�F�N�g�̐ݒ���s���K�v������܂��B
//	�@�v���W�F�N�g->�v���p�e�B->C/C++->�R�[�h����->�����^�C�����C�u�������ȉ��̒ʂ�ɐݒ肷��
//		Debug��		�F�}���`�X���b�h�f�o�b�O(/MTd)
//		Release��	�F�}���`�X���b�h(/MT)
//		�������^�C�����C�u�����̐ݒ���v���W�F�N�g��A��L�ɂł��Ȃ��ꍇ�́AEffekseer�̃����^�C�����C�u������
//		�@���\�[�X�������T�C�g��GitHub�ォ��_�E�����[�h���A���g�̃v���W�F�N�g�ɍ��v����悤�ɁA�V���ȃ��C�u������
//		�@�����^�C�����C�u�������쐬����K�v������B��{�I�ɂ́AEffekseer��Help�Q�ƁB
//	�A�v���W�F�N�g->�v���p�e�B->C/C++->�S��->�ǉ��̃C���N���[�h�f�B���N�g��
//		$(ProjectDir)Effekseer\Compiled\include�����	
//	�B�v���W�F�N�g->�v���p�e�B->�����J�[->�S��->�ǉ��̃��C�u�����f�B���N�g��
//		$(ProjectDir)Effekseer\Compiled\lib	
//===================================================================================================================================

//===================================================================================================================================
//�y�C���N���[�h�z
//===================================================================================================================================
#include "Base.h"
#include <stdio.h>
#include <windows.h>
#include <string>
#include <d3d9.h>
#include <XAudio2.h>
#include <Effekseer.h>
#include <EffekseerRendererDX9.h>
#include <EffekseerSoundXAudio2.h>
#include "LinkedList.h"

//===================================================================================================================================
//�y���C�u�����̓ǂݍ��݁z
//===================================================================================================================================
#pragma comment(lib, "d3d9.lib" )
#pragma comment(lib, "xaudio2.lib" )
#if _DEBUG
#pragma comment(lib, "VS2017/Debug/Effekseer.lib" )
#pragma comment(lib, "VS2017/Debug/EffekseerRendererDX9.lib" )
#pragma comment(lib, "VS2017/Debug/EffekseerSoundXAudio2.lib" )
#else
#pragma comment(lib, "VS2017/Release/Effekseer.lib" )
#pragma comment(lib, "VS2017/Release/EffekseerRendererDX9.lib" )
#pragma comment(lib, "VS2017/Release/EffekseerSoundXAudio2.lib" )
#endif

//===================================================================================================================================
//�y���O��ԁz
//===================================================================================================================================
namespace effekseerNS
{
	enum EFFEKSEER_NUMBER
	{
		TEST0,
		TEST1,
		MAX_EFFEKSEER,
	};

	class Instance
	{
	public:
		int	effectNo;
		::Effekseer::Handle		handle;
		D3DXVECTOR3 position;
		D3DXVECTOR3 speed;
		D3DXVECTOR3 rotation;
		D3DXVECTOR3 deltaRadian;
		D3DXVECTOR3 scale;
		D3DXVECTOR3 deltaScale;


		Instance(int effectNo = TEST0)
		{
			this->effectNo	= effectNo;
			handle		= -1;
			position	= D3DXVECTOR3(0,0,0);
			speed		= D3DXVECTOR3(0,0,0);
			rotation	= D3DXVECTOR3(0,0,0);
			deltaRadian	= D3DXVECTOR3(0,0,0);
			scale		= D3DXVECTOR3(1,1,1);
			deltaScale	= D3DXVECTOR3(0,0,0);
		}

		virtual void update();
	};

	//===================================================================================================================================
	//�y�O���Q�Ɓz
	//===================================================================================================================================
	void play(Instance*);
	void stop();
	void stop(::Effekseer::Handle handle);
	void setProjectionMatrix(float fov, float windowWidth, float windowHeight, float zn, float zf);
	void setCameraMatrix(D3DXVECTOR3 position, D3DXVECTOR3 eye, D3DXVECTOR3 up);
	void render();
	void pause(bool flag);
	void pause(::Effekseer::Handle handle, bool flag);
}

//===================================================================================================================================
//�y�G�t�F�N�V�A�[�}�l�[�W���[�z
//===================================================================================================================================
class EffekseerManager
{
public:

	::Effekseer::Manager*				manager;
	::EffekseerRendererDX9::Renderer*	renderer;
	::EffekseerSound::Sound*			sound;

	//�J�������
	::Effekseer::Vector3D				position;
	::Effekseer::Vector3D				eye;
	::Effekseer::Vector3D				up;

	IXAudio2*							xa2;
	IXAudio2MasteringVoice*				xa2Master;

	//�t�@�C����
	const wchar_t*						fileName[effekseerNS::MAX_EFFEKSEER];

	//�G�t�F�N�g
	::Effekseer::Effect*				effect[effekseerNS::MAX_EFFEKSEER];

	//�n���h�����X�g
	LinkedList<effekseerNS::Instance*>*	instanceList;

public:
	EffekseerManager();
	~EffekseerManager();
	void initialize();
	void uninitialize();
	void update();
	void begin();
	void end();
	void render();

	void setProjectionMatrix(float fov, float windowWidth, float windowHeight, float zn, float zf);
	void setCameraMatrix(D3DXVECTOR3 position, D3DXVECTOR3 eye, D3DXVECTOR3 up);
	void play(effekseerNS::Instance* instance);
	void stop();
	void stop(::Effekseer::Handle handle);
	void pause(bool flag);
	void pause(::Effekseer::Handle handle, bool flag);

};

EffekseerManager* getEffekseerManager();