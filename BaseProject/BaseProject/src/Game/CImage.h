#pragma once
#include "CVector.h"
#include "CTexture.h"
#include "CUIBase.h"

//2D�C���[�W�`��p�N���X
//�ǂݍ���CTexture��2D�\������
class CImage : public CUIBase
{
private:
	CTexture* mpTexture;	//�\������C���[�W�̃e�N�X�`��
	CVector4 mUV;			//�\������e�N�X�`����͈̔́iUV�j

public:
	//�R���X�g���N�^
	CImage(const char* path,
		ETaskPriority prio = ETaskPriority::eUI,
		int sortOrder = 0,
		ETaskPauseType pause = ETaskPauseType::eDefault,
		bool dontDelete = false,
		bool addTaskList = true);
	//�f�X�g���N�^
	~CImage();

	//�e�N�X�`���̓ǂݍ���
	void Load(const char* path, bool dontDelete = false);

	//�\������͈͂�ݒ�iCVector4�Łj
	void SetUV(const CVector4& uv);
	//�\������͈͂�ݒ�
	void SetUV(const float& left, const float& top, const float& right, const float& bottom);
	//�\������͈͂��擾
	const CVector4& GetUV() const;

	//�`��
	void Render();
};
