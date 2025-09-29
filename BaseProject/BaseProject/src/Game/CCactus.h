#pragma once
#include "CEnemy.h"

// �T�{�e���̓G�N���X
class CCactus : public CEnemy
{
public:
	// �R���X�g���N�^
	CCactus();
	// �f�X�g���N�^
	~CCactus();

	// �U������
	bool IsAttacking() const override;
	// �U���J�n
	void AttackStart() override;
	// �U���I��
	void AttackEnd() override;

	// �_���[�W���󂯂�
	void TakeDamage(int damage, CObjectBase* causer) override;
	// ���S
	void Death() override;
	// �Փˏ���
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

	// �X�V
	void Update() override;

private:
	// �A�j���[�V�����̎��
	enum class EAnimType
	{
		None = -1,

		eTPose,		// T�|�[�Y
		eIdle,		// �ҋ@
		eIdleBattle,// �퓬���̑ҋ@
		eWalk,		// ���s
		eAttack1,	// �p���`�U��
		eAttack2,	// �j�U��
		eHit,		// ������
		eDeath,		// ���S

		Num
	};

	// �v���C���[�̏��
	enum class EState
	{
		eIdle,		// �ҋ@���
		eChase,		// �ǂ�������
		eAttack1,	// �p���`�U��
		eAttack2,	// �j�U��
		eHit,		// ������
		eDeath,		// ���S
	};

	// �퓬����̕��֌���
	void LookAtBattleTarget(bool immediate = false);

	// �j�𔭎�
	void ShotNeedle();

	// ��Ԑ؂�ւ�
	void ChangeState(int state) override;

	// �ҋ@��Ԃ̍X�V����
	void UpdateIdle();
	// �ǂ������鎞�̍X�V����
	void UpdateChase();
	// �p���`�U�����̍X�V����
	void UpdateAttack1();
	// �j�U�����̍X�V����
	void UpdateAttack2();
	// �������Ԃ̍X�V����
	void UpdateHit();
	// ���S��Ԃ̍X�V����
	void UpdateDeath();

	bool mIsBattle;					// �퓬��Ԃ�
	float mBattleIdletime;			// �퓬���̑ҋ@����
	CObjectBase* mpBattleTarget;	// �퓬����
	CCollider* mpAttack1Col;		// �p���`�U���p�̃R���C�_�[
	int mShotNeedleCount;			// �j�𔭎˂�����
};
