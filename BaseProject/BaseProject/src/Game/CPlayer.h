#pragma once
//�L�����N�^�N���X�̃C���N���[�h
#include "CXCharacter.h"
#include "CRideableObject.h"
#include "CSound.h"

class CCollider;
class CFlamethrower;
class CSword;

/*
�v���C���[�N���X
�L�����N�^�N���X���p��
*/
class CPlayer : public CXCharacter
{
public:
	//�C���X�^���X�̃|�C���^�̎擾
	static CPlayer* Instance();

	// �R���X�g���N�^
	CPlayer();
	// �f�X�g���N�^
	~CPlayer();

	// �X�V
	void Update();

	// �U������
	bool IsAttacking() const override;
	// �U���J�n
	void AttackStart() override;
	// �U���I��
	void AttackEnd() override;

	// �_���[�W���󂯂�
	void TakeDamage(int damage, CObjectBase* causer) override;

	/// <summary>
	/// �Փˏ���
	/// </summary>
	/// <param name="self">�Փ˂������g�̃R���C�_�[</param>
	/// <param name="other">�Փ˂�������̃R���C�_�[</param>
	/// <param name="hit">�Փ˂������̏��</param>
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

	// �`��
	void Render();

private:
	// �I�u�W�F�N�g�폜��`����
	void DeleteObject(CObjectBase* obj) override;

	// �L�[�̓��͏�񂩂�ړ��x�N�g�������߂�
	CVector CalcMoveVec() const;

	// �ҋ@���
	void UpdateIdle();
	// �a��U��
	void UpdateAttack1();
	// �R��U��
	void UpdateAttack2();
	// �W�����v�J�n
	void UpdateJumpStart();
	// �W�����v��
	void UpdateJump();
	// �W�����v�I��
	void UpdateJumpEnd();
	// ������
	void UpdateHit();

	// �ړ��̍X�V����
	void UpdateMove();

	// ���[�V�����u���[�̍X�V����
	void UpdateMotionBlur();

	// �A�j���[�V�����̎��
	enum class EAnimType
	{
		None = -1,

		eTPose,		// T�|�[�Y
		eIdle,		// �ҋ@
		eWalk,		// ���s
		eAttack,	// �a��U��
		eKick,		// �R��U��
		eJumpStart,	// �W�����v�J�n
		eJump,		// �W�����v��
		eJumpEnd,	// �W�����v�I��
		eHit,		// ������

		Num
	};
	// �A�j���[�V�����؂�ւ�
	void ChangeAnimation(EAnimType type, bool restart = false);

	// �v���C���[�̃C���X�^���X
	static CPlayer* spInstance;

	// �A�j���[�V�����f�[�^
	struct AnimData
	{
		std::string path;	// �A�j���[�V�����f�[�^�̃p�X
		bool loop;			// ���[�v���邩�ǂ���
		float frameLength;	// �A�j���[�V�����̃t���[����
		float speed;		// �A�j���[�V�������x�i1.0�œ��{�j
	};
	// �A�j���[�V�����f�[�^�̃e�[�u��
	static const AnimData ANIM_DATA[];

	// �v���C���[�̏��
	enum class EState
	{
		eIdle,		// �ҋ@
		eAttack1,	// �a��U��
		eAttack2,	// �R��U��
		eJumpStart,	// �W�����v�J�n
		eJump,		// �W�����v��
		eJumpEnd,	// �W�����v�I��
		eHit,		// ������
	};
	// ��Ԃ�؂�ւ�
	void ChangeState(EState state);

	EState mState;				// �v���C���[�̏��
	int mStateStep;				// ��ԓ��̃X�e�b�v�Ǘ��p
	float mElapsedTime;			// �o�ߎ��Ԍv���p

	CVector mMoveSpeed;			// �O�㍶�E�̈ړ����x
	float mMoveSpeedY;			// �d�͂�W�����v�ɂ��㉺�̈ړ����x

	bool mIsGrounded;			// �ڒn���Ă��邩�ǂ���
	CVector mGroundNormal;		// �ڒn���Ă���n�ʂ̖@��

	CCollider* mpBodyCol;		// �{�̂̃R���C�_�[
	CCollider* mpAttack2Col;	// �R��U���p�R���C�_�[
	CTransform* mpRideObject;

	CSound* mpSlashSE;
	bool mIsPlayedSlashSE;
	bool mIsSpawnedSlashEffect;

	// �Ή����˃G�t�F�N�g
	CFlamethrower* mpFlamethrower;

	// ���[�V�����u���[���|����c�莞��
	float mMotionBlurRemainTime;

	// ���̃N���X
	CSword* mpSword;
};
