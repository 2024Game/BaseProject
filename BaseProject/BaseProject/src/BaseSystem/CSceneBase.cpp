#include "CSceneBase.h"
#include "CTaskManager.h"
#include "CResourceManager.h"

//�R���X�g���N�^
CSceneBase::CSceneBase(EScene scene)
	: mSceneType(scene)
{
}

//�f�X�g���N�^
CSceneBase::~CSceneBase()
{
	//�폜����V�[���ɏ�������^�X�N�ƃ��\�[�X�͑S�č폜
	CTaskManager::Instance()->DeleteInScene(mSceneType);
	CResourceManager::DeleteInScene(mSceneType);
}

//�V�[���̎�ނ��擾
EScene CSceneBase::GetSceneType() const
{
	return mSceneType;
}

//�V�[���ɏ�������^�X�N��ǉ�
void CSceneBase::AddTask(CTask* task)
{
	if (task == nullptr) return;
	task->SetSceneType(mSceneType);
}
