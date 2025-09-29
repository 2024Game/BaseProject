#pragma once
#include "CObjectBase.h"

/// <summary>
/// �L�����N�^�[�̃x�[�X�N���X
/// </summary>
class CCharaBase : public CObjectBase
{
public:
	// �R���X�g���N�^
	CCharaBase(ETag tag, ETaskPriority prio, int sortOrder = 0,
		ETaskPauseType pause = ETaskPauseType::eGame);
	// �f�X�g���N�^
	virtual ~CCharaBase();

	// �ő�HP���擾
	int GetMaxHp() const;
	// ����HP���擾
	int GetHp() const;

	// �_���[�W���󂯂�
	virtual void TakeDamage(int damage, CObjectBase* causer);

	// ���S
	virtual void Death();
	// ����ł��邩�ǂ���
	bool IsDeath() const;

protected:
	int mMaxHp;	// �ő�HP
	int mHp;	// ����HP
};
