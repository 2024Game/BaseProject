#pragma once
#include "SceneType.h"
class CTask;

//�V�[���̃x�[�X�N���X
class CSceneBase
{
public:
	//�R���X�g���N�^
	CSceneBase(EScene scene);
	//�f�X�g���N�^
	virtual ~CSceneBase();

	//�V�[���ǂݍ��ݏ���(�p����Ŏ���)
	virtual void Load() = 0;
	//�V�[���X�V����(�p����Ŏ���)
	virtual void Update() = 0;
	//�V�[���̎�ނ��擾
	EScene GetSceneType() const;
	//�V�[���ɏ�������^�X�N��ǉ�
	void AddTask(CTask* task);

private:
	EScene mSceneType;	//�V�[���̎��
};
