#include "CCharaBase.h"

// �R���X�g���N�^
CCharaBase::CCharaBase(ETag tag, ETaskPriority prio, int sortOrder, ETaskPauseType pause)
	: CObjectBase(tag, prio, sortOrder, pause)
	, mMaxHp(10)
	, mHp(mMaxHp)
{
}

// �f�X�g���N�^
CCharaBase::~CCharaBase()
{
}

// �ő�HP���擾
int CCharaBase::GetMaxHp() const
{
	return mMaxHp;
}

// ����HP���擾
int CCharaBase::GetHp() const
{
	return mHp;
}

// �_���[�W���󂯂�
void CCharaBase::TakeDamage(int damage, CObjectBase* causer)
{
	// ���Ɏ��S���Ă�����A�_���[�W���󂯂Ȃ�
	if (IsDeath()) return;

	// �󂯂��_���[�W������HP�ȏ�Ȃ�
	if (damage >= mHp)
	{
		// HP��0�ɂ��āA���S
		mHp = 0;
		Death();
	}
	// ����HP�̕��������ꍇ�́A�_���[�W�����炷
	else
	{
		mHp -= damage;
	}
}

// ���S
void CCharaBase::Death()
{
}

// ����ł��邩�ǂ���
bool CCharaBase::IsDeath() const
{
	// ����HP��0�Ȃ�΁A���S
	return mHp <= 0;
}
