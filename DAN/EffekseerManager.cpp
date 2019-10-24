//===================================================================================================================================
//�yEffekseerManager.cpp�z
// [�쐬��]HAL����GP12A332 11 ���� ��
// [�쐬��]2019/10/18
// [�X�V��]2019/10/21
//===================================================================================================================================

//===================================================================================================================================
//�y�C���N���[�h�z
//===================================================================================================================================
#include "EffekseerManager.h"
#include "Direct3D9.h"

//===================================================================================================================================
//�yusing�錾�z
//===================================================================================================================================
using namespace effekseerNS;

//===================================================================================================================================
//�y�O���[�o���ϐ��z
//===================================================================================================================================
static EffekseerManager* pointerEffekseerManager;

//===================================================================================================================================
//�y�R���X�g���N�^�z
//===================================================================================================================================
EffekseerManager::EffekseerManager()
{
	pointerEffekseerManager = this;
	manager		= NULL;
	renderer	= NULL;
	sound		= NULL;	
	xa2			= NULL;
	xa2Master	= NULL;

	fileName[TEST0] = { L"test.efk" };
	fileName[TEST1] = { L"r_square.efk" };

	instanceList = new LinkedList<::effekseerNS::Instance*>;
}

//===================================================================================================================================
//�y�f�X�g���N�^�z
//===================================================================================================================================
EffekseerManager::~EffekseerManager()
{
	uninitialize();
}

//===================================================================================================================================
//�y�������z
//===================================================================================================================================
void EffekseerManager::initialize()
{
	// XAudio2�̏��������s��
	XAudio2Create(&xa2);
	xa2->CreateMasteringVoice(&xa2Master);

	// �`��p�C���X�^���X�̐���
	renderer = ::EffekseerRendererDX9::Renderer::Create(getDevice(), 2000);

	// �G�t�F�N�g�Ǘ��p�C���X�^���X�̐���
	manager = ::Effekseer::Manager::Create(2000);

	// �`��p�C���X�^���X����`��@�\��ݒ�
	manager->SetSpriteRenderer(renderer->CreateSpriteRenderer());
	manager->SetRibbonRenderer(renderer->CreateRibbonRenderer());
	manager->SetRingRenderer(renderer->CreateRingRenderer());
	manager->SetTrackRenderer(renderer->CreateTrackRenderer());
	manager->SetModelRenderer(renderer->CreateModelRenderer());

	// �`��p�C���X�^���X����e�N�X�`���̓Ǎ��@�\��ݒ�
	// �Ǝ��g���\�A���݂̓t�@�C������ǂݍ���ł���B
	manager->SetTextureLoader(renderer->CreateTextureLoader());
	manager->SetModelLoader(renderer->CreateModelLoader());

	// ���Đ��p�C���X�^���X�̐���
	sound = ::EffekseerSound::Sound::Create(xa2, 16, 16);

	// ���Đ��p�C���X�^���X����Đ��@�\���w��
	manager->SetSoundPlayer(sound->CreateSoundPlayer());

	// ���Đ��p�C���X�^���X����T�E���h�f�[�^�̓Ǎ��@�\��ݒ�
	// �Ǝ��g���\�A���݂̓t�@�C������ǂݍ���ł���B
	manager->SetSoundLoader(sound->CreateSoundLoader());

	//���W�n�̎w��(LH�F����n�j
	manager->SetCoordinateSystem(Effekseer::CoordinateSystem::LH);

	// ���_�ʒu���m��
	position = ::Effekseer::Vector3D(10.0f, 5.0f, 20.0f);

	// �J�����O���s���͈͂�ݒ�
	//manager->CreateCullingWorld(10000.0f, 10000.0f, 10000.0f, 6);

	//�G�t�F�N�V�A�[�f�B���N�g���փJ�����g
	setEffekseerDirectory();

	// �G�t�F�N�g�̓Ǎ�
	for (int i = 0; i < MAX_EFFEKSEER; i++)
	{
		effect[i] = Effekseer::Effect::Create(manager, (const EFK_CHAR*)fileName[i]);
	}

}

//===================================================================================================================================
//�y�����_���[�̓��e�s���ݒ�z
//===================================================================================================================================
void EffekseerManager::setProjectionMatrix(float fov, float windowWidth, float windowHeight, float zn, float zf)
{
	renderer->SetProjectionMatrix(
		::Effekseer::Matrix44().PerspectiveFovLH(fov, (float)windowWidth / (float)windowHeight, zn, zf));
}

//===================================================================================================================================
//�y�����_���[�̃J�����s���ݒ�z
//===================================================================================================================================
void EffekseerManager::setCameraMatrix(D3DXVECTOR3 position,D3DXVECTOR3 eye,D3DXVECTOR3 up)
{
	this->position	= ::Effekseer::Vector3D(position.x, position.y, position.z);
	this->eye		= ::Effekseer::Vector3D(eye.x, eye.y, eye.z);
	this->up		= ::Effekseer::Vector3D(up.x, up.y, up.z);

	renderer->SetCameraMatrix(
		::Effekseer::Matrix44().LookAtLH(this->position, this->eye, this->up));
}

//===================================================================================================================================
//�y�Đ��z
//===================================================================================================================================
void EffekseerManager::play(effekseerNS::Instance* instance)
{
	instance->handle = manager->Play(
		effect[instance->effectNo], 
		instance->position.x,
		instance->position.y,
		instance->position.z);
	instanceList->insertFront(instance);
}

//===================================================================================================================================
//�y�ꎞ��~/�ēx�Đ��F�S�āz
//===================================================================================================================================
void EffekseerManager::pause(bool flag)
{
	manager->SetPausedToAllEffects(flag);
}

//===================================================================================================================================
//�y�ꎞ��~/�ēx�Đ��F1�n���h���z
//===================================================================================================================================
void EffekseerManager::pause(::Effekseer::Handle handle,bool flag )
{
	manager->SetPaused(handle, flag);
}

//===================================================================================================================================
//�y�S��~�z
//===================================================================================================================================
void EffekseerManager::stop()
{
	manager->StopAllEffects();
}

//===================================================================================================================================
//�y��~�F1�n���h���z
//===================================================================================================================================
void EffekseerManager::stop(::Effekseer::Handle handle)
{
	manager->StopEffect(handle);
}

//===================================================================================================================================
//�y�I�������z
//===================================================================================================================================
void EffekseerManager::uninitialize()
{
	manager->StopAllEffects();
	for (int i = 0; i < MAX_EFFEKSEER;i++)
	{
		// �G�t�F�N�g�̔j��
		ES_SAFE_RELEASE(effect[i]);
		// �G�t�F�N�g�̒�~
	}

	// ��ɃG�t�F�N�g�Ǘ��p�C���X�^���X��j��
	manager->Destroy();

	// ���ɉ��Đ��p�C���X�^���X��j��
	sound->Destroy();

	// ���ɕ`��p�C���X�^���X��j��
	renderer->Destroy();

	// XAudio2�̉��
	if (xa2Master != NULL)
	{
		xa2Master->DestroyVoice();
		xa2Master = NULL;
	}
	ES_SAFE_RELEASE(xa2);

	for (int i = 0;i <instanceList->nodeNum; i++)
	{
		SAFE_DELETE(*instanceList->getValue(i));
	}
	instanceList->terminate();
	SAFE_DELETE(instanceList);
}

//===================================================================================================================================
//�y�X�V�z
//===================================================================================================================================
void EffekseerManager::update()
{
	//���X�g�X�V
	instanceList->listUpdate();

	//�폜����
	int nodeNum = instanceList->nodeNum;
	for (int i = 0; i < nodeNum; i++)
	{
		::Effekseer::Handle handle = (*instanceList->getValue(i))->handle;
		if (!manager->Exists(handle))
		{
			SAFE_DELETE((*instanceList->getValue(i)));
			instanceList->remove(instanceList->getNode(i));
		}
	}

	//���X�g�X�V
	instanceList->listUpdate();

	// �G�t�F�N�g�̈ړ��������s��
	for (int i = 0; i < instanceList->nodeNum; i++)
	{
		(*instanceList->getValue(i))->update();
	}

	// �G�t�F�N�g�̍X�V�������s��
	manager->Update();
}

//===================================================================================================================================
//�y�`��J�n�z
//===================================================================================================================================
void EffekseerManager::begin()
{
	renderer->BeginRendering();
}

//===================================================================================================================================
//�y�`��J�n�z
//===================================================================================================================================
void EffekseerManager::end()
{
	renderer->EndRendering();
}

//===================================================================================================================================
//�y�`��z
//===================================================================================================================================
void EffekseerManager::render()
{
	//manager->CalcCulling(renderer->GetCameraProjectionMatrix(), false);//�J�����O�̌v�Z
	begin();
	manager->Draw();
	end();
}

//===================================================================================================================================
//�y�O���Q�ƁF�Đ��z
//===================================================================================================================================
void effekseerNS::play(effekseerNS::Instance* instance)
{
	pointerEffekseerManager->play(instance);
}

//===================================================================================================================================
//�y�O���Q�ƁF�S��~�z
//===================================================================================================================================
void effekseerNS::stop()
{
	pointerEffekseerManager->stop();
}

//===================================================================================================================================
//�y�O���Q�ƁF��~�z
//===================================================================================================================================
void effekseerNS::stop(::Effekseer::Handle handle)
{
	pointerEffekseerManager->stop(handle);
}

//===================================================================================================================================
//�y�O���Q�ƁF�v���W�F�N�V�����s��ݒ�z
//===================================================================================================================================
void effekseerNS::setProjectionMatrix(float fov, float windowWidth, float windowHeight, float zn, float zf)
{
	pointerEffekseerManager->setProjectionMatrix(fov, windowWidth, windowHeight, zn, zf);
}

//===================================================================================================================================
//�y�O���Q�ƁF�J����(�r���[)�s��ݒ�z
//===================================================================================================================================
void effekseerNS::setCameraMatrix(D3DXVECTOR3 position, D3DXVECTOR3 eye, D3DXVECTOR3 up)
{
	pointerEffekseerManager->setCameraMatrix(position, eye, up);
}

//===================================================================================================================================
//�y�O���Q�ƁF�`��z
//===================================================================================================================================
void effekseerNS::render()
{
	pointerEffekseerManager->render();
}

//===================================================================================================================================
//�y�O���Q�ƁF�ꎞ��~�z
//===================================================================================================================================
void effekseerNS::pause(bool flag)
{
	pointerEffekseerManager->pause(flag);
}

//===================================================================================================================================
//�y�O���Q�ƁF�ꎞ��~�z
//===================================================================================================================================
void effekseerNS::pause(::Effekseer::Handle handle, bool flag)
{
	pointerEffekseerManager->pause(handle, flag);
}

//===================================================================================================================================
//�y�O���Q�ƁF�}�l�[�W���[�擾�z
//===================================================================================================================================
EffekseerManager* getEffekseerManager()
{
	return pointerEffekseerManager;
}

//===================================================================================================================================
//�y�C���X�^���X�F�X�V�z
//===================================================================================================================================
void effekseerNS::Instance::update()
{
	::Effekseer::Manager*	manager = pointerEffekseerManager->manager;
	position	 += speed;
	rotation	 += deltaRadian;
	scale		 += deltaScale;
	manager->SetLocation(handle, position.x,position.y,position.z);
	manager->SetRotation(handle,rotation.x,rotation.y,rotation.z);
	manager->SetScale(handle,scale.x,scale.y,scale.z);
}