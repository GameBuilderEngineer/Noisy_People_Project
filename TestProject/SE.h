//===================================================================================================================================
//�ySE.h�z
// [�쐬��]HAL����GP12A332 16 �� �F��
// [�쐬��]2019/10/04
// [�X�V��]2019/10/04
//===================================================================================================================================
#pragma once

//===================================================================================================================================
//�y�C���N���[�h�z
//===================================================================================================================================
#include "Base.h"
#include "LinkedList.h"
#include "SoundBase.h"

//===================================================================================================================================
//�y�񋓌^�萔�z
//===================================================================================================================================
enum TITLE_SE_LIST
{
	SE_01,
	SE_02,
	SE_MAX
};

//===================================================================================================================================
//�y�T�E���h(XAudio2)�z
//�T�E���h��SE�N���X
//===================================================================================================================================
class SEManager : public SoundBase
{
public:
	SEManager() {};
	~SEManager();

	//�ϐ�
	static const char * const titleSEPathList[];
	
	//�֐�
	static void		SwitchAudioBuffer(int scene);		//�X�e�[�W�J�ڂɍ��킹�ĕK�v�ȃT�E���h�o�b�t�@��p�ӂ���
};