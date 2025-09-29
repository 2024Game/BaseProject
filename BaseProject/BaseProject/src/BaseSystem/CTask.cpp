#include "CTask.h"
#include "CTaskManager.h"
#include "CSceneManager.h"

//�R���X�g���N�^
CTask::CTask(ETaskPriority prio, int sortOrder,
	ETaskPauseType pause, bool dontDelete, bool addTaskList)
	: mPriority(prio)
	, mSortOrder(sortOrder)
	, mIsSort(false)
	, mIsKill(false)
	, mIsEnable(true)
	, mIsShow(true)
	, mPauseType(pause)
	, mAddTaskList(addTaskList)
{
	//�V�[���J�ڂŔj�����Ȃ��^�X�N���ǂ�����
	//��������V�[���̃^�C�v��؂�ւ���
	mSceneType = dontDelete
		? EScene::eNone
		: CSceneManager::Instance()->GetCurrentScene();

	// �����o�^���I���Ȃ��
	if (mAddTaskList)
	{
		//�^�X�N�𐶐���������
		//�^�X�N�}�l�[�W���[�̃^�X�N���X�g�ɒǉ�
		CTaskManager::Instance()->Add(this);
	}
}

//�f�X�g���N�^
CTask::~CTask()
{
	if (mAddTaskList)
	{
		//�^�X�N�}�l�[�W���[�̃^�X�N���X�g�����菜��
		CTaskManager::Instance()->Remove(this);
	}
}

//�X�V
void CTask::Update()
{
}

//��X�V
void CTask::LateUpdate()
{
}

//�`��
void CTask::Render()
{
}

//�D��x��ݒ�
void CTask::SetPriority(ETaskPriority prio)
{
	//���ɐݒ肳��Ă���D��x�ł���Ώ������Ȃ�
	if (mPriority == prio) return;

	//�D��x��ύX���āA���ёւ��t���O�𗧂Ă�
	mPriority = prio;
	mIsSort = true;
}

//�D��x���擾
ETaskPriority CTask::GetPriority() const
{
	return mPriority;
}

//�D��x���̏��Ԃ�ݒ�
void CTask::SetSortOrder(int sortOder)
{
	//���ɐݒ肳��Ă��鏇�Ԃł���Ώ������Ȃ�
	if (mSortOrder == sortOder) return;

	//���Ԃ�ύX���A���ёւ��t���O�𗧂Ă�
	mSortOrder = sortOder;
	mIsSort = true;
}

//�D��x���̏��Ԃ��擾
int CTask::GetSortOrder() const
{
	return mSortOrder;
}

//�|�[�Y�̎�ނ�ݒ�
void CTask::SetPauseType(ETaskPauseType type)
{
	mPauseType = type;
}

//�|�[�Y�̎�ނ��擾
ETaskPauseType CTask::GetPauseType() const
{
	return mPauseType;
}

//��������V�[����ݒ�
void CTask::SetSceneType(EScene scene)
{
	mSceneType = scene;
}

//��������V�[�����擾
EScene CTask::GetSceneType() const
{
	return mSceneType;
}

//�^�X�N���폜
void CTask::Kill()
{
	mIsKill = true;
}

//�폜�t���O�擾
bool CTask::IsKill() const
{
	return mIsKill;
}

//�L���t���O��ݒ�
void CTask::SetEnable(bool enable)
{
	mIsEnable = enable;
}

//�L���t���O���擾
bool CTask::IsEnable() const
{
	return mIsEnable;
}

//�\���t���O��ݒ�
void CTask::SetShow(bool show)
{
	mIsShow = show;
}

//�\���t���O���擾
bool CTask::IsShow() const
{
	// �\���t���O��true���A
	// �폜�t���O�������Ă��Ȃ��ꍇ�́A�\������
	return mIsShow && !mIsKill;
}
