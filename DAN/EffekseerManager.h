//===================================================================================================================================
//【EffekseerManager.h】
// [作成者]HAL東京GP12A332 11 菅野 樹
// [作成日]2019/10/18
// [更新日]2019/10/21
//===================================================================================================================================
#pragma once

//===================================================================================================================================
//備考：このクラスをプロジェクトへ使用する際に、プロジェクトの設定を行う必要があります。
//	①プロジェクト->プロパティ->C/C++->コード生成->ランタイムライブラリを以下の通りに設定する
//		Debug時		：マルチスレッドデバッグ(/MTd)
//		Release時	：マルチスレッド(/MT)
//		※ランタイムライブラリの設定をプロジェクト上、上記にできない場合は、Effekseerのランタイムライブラリの
//		　リソースを公式サイトやGitHub上からダウンロードし、自身のプロジェクトに合致するように、新たなライブラリを
//		　ランタイムライブラリを作成する必要がある。基本的には、EffekseerのHelp参照。
//	②プロジェクト->プロパティ->C/C++->全般->追加のインクルードディレクトリ
//		$(ProjectDir)Effekseer\Compiled\includeを入力	
//	③プロジェクト->プロパティ->リンカー->全般->追加のライブラリディレクトリ
//		$(ProjectDir)Effekseer\Compiled\lib	
//===================================================================================================================================

//===================================================================================================================================
//【インクルード】
//===================================================================================================================================
#include "Base.h"
#include <stdio.h>
#include <windows.h>
#include <string>
#include <d3d9.h>
#if(XADUIO2_STATE)
#include <XAudio2.h>
#endif
#include <Effekseer.h>
#include <EffekseerRendererDX9.h>
#include <EffekseerSoundXAudio2.h>
#include "LinkedList.h"

//===================================================================================================================================
//【ライブラリの読み込み】
//===================================================================================================================================
#pragma comment(lib, "d3d9.lib" )
#if(XADUIO2_STATE)
#pragma comment(lib, "xaudio2.lib" )
#endif
#if _DEBUG
#pragma comment(lib, "VS2017/Debug/Effekseer.lib" )
#pragma comment(lib, "VS2017/Debug/EffekseerRendererDX9.lib" )
#if(XADUIO2_STATE)
#pragma comment(lib, "VS2017/Debug/EffekseerSoundXAudio2.lib" )
#endif
#else
#pragma comment(lib, "VS2017/Release/Effekseer.lib" )
#pragma comment(lib, "VS2017/Release/EffekseerRendererDX9.lib" )
#if(_MSC_VER >= GAME_MSC_VER)
#pragma comment(lib, "VS2017/Release/EffekseerSoundXAudio2.lib" )
#endif
#endif

//===================================================================================================================================
//【名前空間】
//===================================================================================================================================
namespace effekseerNS
{
	enum EFFEKSEER_NUMBER
	{
		BLOW,
		MUZZLE,
		DIGIT_TREE,
		DIGIT_TREE_SELECT,
		DIGIT_TREE_RAID,
		DAC,
		GREENING,
		DIGIT_MODE,
		DROP_ITEM,
		MAX_EFFEKSEER,
	};

	class Instance
	{
	public:
		int	effectNo;
		::Effekseer::Handle		handle;
		Node<Instance*>*		nodePointer;
		D3DXVECTOR3 position;
		D3DXVECTOR3 speed;
		D3DXVECTOR3 rotation;
		D3DXVECTOR3 deltaRadian;
		D3DXVECTOR3 scale;
		D3DXVECTOR3 deltaScale;

		Instance(int effectNo = BLOW)
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
		void setShown(bool flag);
	};

	//===================================================================================================================================
	//【外部参照】
	//===================================================================================================================================
	effekseerNS::Instance* play(Instance*);
	void stop();
	void stop(::Effekseer::Handle handle);
	void stop(::effekseerNS::Instance* instance);
	void setProjectionMatrix(float fov, float windowWidth, float windowHeight, float zn, float zf);
	void setCameraMatrix(D3DXVECTOR3 position, D3DXVECTOR3 eye, D3DXVECTOR3 up);
	void render();
	void pause(bool flag);
	void pause(::Effekseer::Handle handle, bool flag);
}

//===================================================================================================================================
//【エフェクシアーマネージャー】
//===================================================================================================================================
class EffekseerManager
{
public:

	::Effekseer::Manager*				manager;
	::EffekseerRendererDX9::Renderer*	renderer;
#if(XADUIO2_STATE)
	::EffekseerSound::Sound*			sound;
	IXAudio2*							xa2;
	IXAudio2MasteringVoice*				xa2Master;
#endif

	//カメラ情報
	::Effekseer::Vector3D				position;
	::Effekseer::Vector3D				eye;
	::Effekseer::Vector3D				up;

	//ファイル名
	const wchar_t*						fileName[effekseerNS::MAX_EFFEKSEER];

	//エフェクト
	::Effekseer::Effect*				effect[effekseerNS::MAX_EFFEKSEER];

	//ハンドルリスト
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
	effekseerNS::Instance* play(effekseerNS::Instance* instance);
	void stop();
	void stop(::Effekseer::Handle handle);
	void stop(::effekseerNS::Instance* instance);
	void pause(bool flag);
	void pause(::Effekseer::Handle handle, bool flag);

};

EffekseerManager* getEffekseerManager();
