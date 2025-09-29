#pragma once
#include "CSceneBase.h"
class CText;

// �N�����̃��j���[�V�[��
class CBootMenuScene : public CSceneBase
{
public:
	// �R���X�g���N�^
	CBootMenuScene();
	// �f�X�g���N�^
	~CBootMenuScene();

	// �V�[���ǂݍ���
	void Load() override;
	// �X�V
	void Update() override;

private:
	// ���̍��ڂ�I��
	void SelectNextMenu();
	// �O�̍��ڂ�I��
	void SelectPrevMenu();

	CText* mpText;	// ������`��p

	// ���j���[���
	struct MenuInfo
	{
		std::string name;	// �\����
		EScene scene;		// �J�ڐ�̃V�[��
	};
	// ���j���[���̃��X�g
	std::vector<MenuInfo> mMenuInfoList;

	int mSelectIndex;	// �I�𒆂̃C���f�b�N�X�l
};