#pragma once
#include "TaskPriority.h"
#include "TaskPauseType.h"
#include "SceneType.h"
class CTaskManager;

//�^�X�N�N���X
class CTask
{
	friend CTaskManager;
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="prio">�^�X�N�D��x</param>
	/// <param name="sortOrder">�^�X�N�D��x���̏���</param>
	/// <param name="pause">�|�[�Y���</param>
	/// <param name="dontDelete">true�ɂ���ƃV�[���J�ڂō폜����Ȃ�</param>
	/// <param name="addTaskList">false�ɂ���ƃ^�X�N���X�g�Ɏ����o�^����Ȃ�</param>
	CTask(ETaskPriority prio = ETaskPriority::eDefault, int sortOrder = 0,
		ETaskPauseType pause = ETaskPauseType::eDefault,
		bool dontDelete = false, bool addTaskList = true);
	//�f�X�g���N�^
	virtual ~CTask();
	//�X�V
	virtual void Update();
	//��X�V
	virtual void LateUpdate();
	//�`��
	virtual void Render();

	//�D��x��ݒ�
	void SetPriority(ETaskPriority prio);
	//�D��x���擾
	ETaskPriority GetPriority() const;
	//�D��x���̏��Ԃ�ݒ�
	void SetSortOrder(int sortOder);
	//�D��x���̏��Ԃ��擾
	int GetSortOrder() const;

	//�|�[�Y�̎�ނ�ݒ�
	void SetPauseType(ETaskPauseType type);
	//�|�[�Y�̎�ނ��擾
	ETaskPauseType GetPauseType() const;

	//��������V�[����ݒ�
	void SetSceneType(EScene scene);
	//��������V�[�����擾
	EScene GetSceneType() const;

	//�^�X�N���폜
	virtual void Kill();
	//�폜�t���O�擾
	bool IsKill() const;

	//�L���t���O��ݒ�
	//�i�I�t�ɂ���ƁA�X�V�A�`��A�Փ˔�����~�j
	void SetEnable(bool enable);
	//�L���t���O�擾
	bool IsEnable() const;

	//�\���t���O��ݒ�
	virtual void SetShow(bool show);
	//�\���t���O���擾
	bool IsShow() const;

private:
	ETaskPriority mPriority;	//�D��x
	int mSortOrder;		//�D��x���ł̏���
	bool mIsSort;		//�\�[�g�̕K�v�����邩
	bool mIsKill;		//�폜�t���O
	bool mIsEnable;		//�L���t���O
	bool mIsShow;		//�\���t���O
	ETaskPauseType mPauseType;//�|�[�Y�̎��
	bool mAddTaskList;	//�^�X�N���X�g�ɒǉ����Ă��邩�ǂ���
	EScene mSceneType;	//��������V�[�����
};
