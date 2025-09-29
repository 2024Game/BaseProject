#include "CCactus.h"
#include "CColliderCapsule.h"
#include "CColliderSphere.h"
#include "Maths.h"
#include "CCactusNeedle.h"

#define ANIM_PATH "Character\\Enemy\\Cactus\\Anim\\"
#define BODY_HEIGHT 13.0f
#define BODY_RADIUS 5.0f
#define GAUGE_OFFSET_Y 15.0f
#define DEATH_WAIT_TIME 3.0f
#define ATTACK_RANGE 15.0f
#define ATTACK_START_FRAME 20.0f
#define ATTACK_END_FRAME 30.0f
#define ATTACK_COL_RADIUS 6.0f
#define ATTACK_COL_POS CVector(0.0f, 2.5f, 5.0f)
#define CHAISE_SPEED 20.0f
#define LOOKAT_SPEED 90.0f
#define BATTLE_IDLE_TIME_MIN 2.0f
#define BATTLE_IDLE_TIME_MAX 5.0f

#define ATTACK2_DIST 50.0f			// �j�U�����s������
#define ATTACK2_PROB 70				// �j�U�����s���m���i�p�[�Z���g�j
#define ATTACK2_START_FRAME 30.0f	// �j���ˊJ�n�t���[��
#define ATTACK2_END_FRAME 42.0f		// �j���ˏI���t���[��
// �j���ˈʒu�̃I�t�Z�b�g���W
#define ATTACK2_NEEDLE_OFFSET_POS CVector(0.0f, 5.0f, 10.0f)
#define ATTACK2_NEEDLE_SPEED 100.0f	// �j�̑��x
#define ATTACK2_NEEDLE_DIST 200.0f	// �j���ړ��ł��鋗��
#define ATTACK2_NEEDLE_SHOT_COUNT 3	// �j�𔭎˂����
#define ATTACK2_NEEDLE_SHOT_DIR_COUNT 3	// �j�𔭎˂�������̐�
#define ATTACK2_NEEDLE_SHOT_ANGLE 15.0f	// �j�𔭎˂���p�x

// �G�̃A�j���[�V�����f�[�^�̃e�[�u��
const std::vector<CEnemy::AnimData> ANIM_DATA =
{
	{ "",						true,	0.0f,	1.0f	},	// T�|�[�Y
	{ ANIM_PATH"idle.x",		true,	40.0f,	1.0f	},	// �ҋ@
	{ ANIM_PATH"idle_battle.x",	true,	36.0f,	1.0f	},	// �퓬���̑ҋ@
	{ ANIM_PATH"walk.x",		true,	62.0f,	1.0f	},	// ���s
	{ ANIM_PATH"attack.x",		false,	42.0f,	1.0f	},	// �p���`�U��
	{ ANIM_PATH"attack2.x",		false,	52.0f,	1.0f	},	// �j�U��
	{ ANIM_PATH"hit.x",			false,	46.0f,	1.0f	},	// ������
	{ ANIM_PATH"death.x",		false,	46.0f,	1.0f	},	// ���S
};

// �R���X�g���N�^
CCactus::CCactus()
	: mIsBattle(false)
	, mBattleIdletime(0.0f)
	, mpBattleTarget(nullptr)
	, mpAttack1Col(nullptr)
	, mShotNeedleCount(0)
{
	// �Q�[�W�̃I�t�Z�b�g�ʒu��ݒ�
	mGaugeOffsetPos = CVector(0.0f, GAUGE_OFFSET_Y, 0.0f);

	// �G��������
	InitEnemy("Cactus", &ANIM_DATA);

	// �ŏ��͑ҋ@�A�j���[�V�������Đ�
	ChangeAnimation((int)EAnimType::eIdle);

	// �{�̂̃R���C�_�[���쐬
	mpBodyCol = new CColliderCapsule
	(
		this, ELayer::eEnemy,
		CVector(0.0f, BODY_RADIUS, 0.0f),
		CVector(0.0f, BODY_HEIGHT - BODY_RADIUS, 0.0f),
		BODY_RADIUS
	);
	// �t�B�[���h�ƁA�v���C���[�̍U���R���C�_�[�ƃq�b�g����悤�ɐݒ�
	mpBodyCol->SetCollisionTags({ ETag::eField, ETag::ePlayer });
	mpBodyCol->SetCollisionLayers({ ELayer::eField, ELayer::ePlayer, ELayer::eAttackCol });

	// �U���R���C�_�[���쐬
	mpAttack1Col = new CColliderSphere
	(
		this, ELayer::eAttackCol,
		ATTACK_COL_RADIUS
	);
	// �v���C���[�̖{�̃R���C�_�[�Ƃ̂݃q�b�g����悤�ɐݒ�
	mpAttack1Col->SetCollisionTags({ ETag::ePlayer });
	mpAttack1Col->SetCollisionLayers({ ELayer::ePlayer });
	// �U���R���C�_�[�̍��W��ݒ�
	mpAttack1Col->Position(ATTACK_COL_POS);
	// �U���R���C�_�[���ŏ��̓I�t�ɂ��Ă���
	mpAttack1Col->SetEnable(false);
}

// �f�X�g���N�^
CCactus::~CCactus()
{
	// �U���R���C�_�[���폜
	SAFE_DELETE(mpAttack1Col);
}

// �U������
bool CCactus::IsAttacking() const
{
	// �p���`�U����
	if (mState == (int)EState::eAttack1) return true;
	// �j�U����
	if (mState == (int)EState::eAttack2) return true;

	// �U�����łȂ�
	return false;
}

// �U���J�n
void CCactus::AttackStart()
{
	// �x�[�X�N���X�̍U���J�n�������Ăяo��
	CEnemy::AttackStart();

	// �p���`�U�����ł���΁A�p���`�U���̃R���C�_�[���I���ɂ���
	if (mState == (int)EState::eAttack1)
	{
		mpAttack1Col->SetEnable(true);
	}
}

// �U���I��
void CCactus::AttackEnd()
{
	// �x�[�X�N���X�̍U���I���������Ăяo��
	CEnemy::AttackEnd();

	// �U���R���C�_�[���I�t
	mpAttack1Col->SetEnable(false);
}

// �_���[�W���󂯂�
void CCactus::TakeDamage(int damage, CObjectBase* causer)
{
	// �x�[�X�N���X�̃_���[�W�������Ăяo��
	CEnemy::TakeDamage(damage, causer);

	// ���S���Ă��Ȃ���΁A
	if (!IsDeath())
	{
		// �������Ԃֈڍs
		ChangeState((int)EState::eHit);

		// �U���������������퓬����ɐݒ�
		mpBattleTarget = causer;

		// �U��������������̕����֌���
		LookAtBattleTarget(true);

		// �퓬��Ԃ֐؂�ւ�
		mIsBattle = true;

		// �ړ����~
		mMoveSpeed = CVector::zero;
	}
}

// ���S
void CCactus::Death()
{
	// ���S��Ԃɐ؂�ւ�
	ChangeState((int)EState::eDeath);
}

// �Փˏ���
void CCactus::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// �x�[�X�̏Փˏ������Ăяo��
	CEnemy::Collision(self, other, hit);

	// �U���R���C�_�[���q�b�g����
	if (self == mpAttack1Col)
	{
		// �q�b�g�����̂��L�����N�^�[���A
		// �܂��U�����q�b�g���Ă��Ȃ��L�����N�^�[�ł����
		CCharaBase* chara = dynamic_cast<CCharaBase*>(other->Owner());
		if (chara != nullptr && !IsAttackHitObj(chara))
		{
			// �_���[�W��^����
			chara->TakeDamage(1, this);
			// �U���q�b�g�ς݃��X�g�ɓo�^
			AddAttackHitObj(chara);
		}
	}
}

// �퓬����̕��֌���
void CCactus::LookAtBattleTarget(bool immediate)
{
	// �퓬���肪���Ȃ���΁A�������Ȃ�
	if (mpBattleTarget == nullptr) return;

	// �퓬����܂ł̕����x�N�g�������߂�
	CVector targetPos = mpBattleTarget->Position();
	CVector vec = targetPos - Position();
	vec.Y(0.0f);
	vec.Normalize();
	// �����ɐ퓬����̕����֌���
	if (immediate)
	{
		Rotation(CQuaternion::LookRotation(vec));
	}
	// ���X�ɐ퓬����̕����֌���
	else
	{
		CVector forward = CVector::Slerp
		(
			VectorZ(), vec,
			LOOKAT_SPEED * Times::DeltaTime()
		);
		Rotation(CQuaternion::LookRotation(forward));
	}
}

// �j�𔭎�
void CCactus::ShotNeedle()
{
	CVector pos = Position() + Rotation() * ATTACK2_NEEDLE_OFFSET_POS;
	CVector forward = VectorZ();
	float startAngle = -ATTACK2_NEEDLE_SHOT_ANGLE;
	float endAngle = ATTACK2_NEEDLE_SHOT_ANGLE;
	for (int i = 0; i < ATTACK2_NEEDLE_SHOT_DIR_COUNT; i++)
	{
		float alpha = (float)i / (ATTACK2_NEEDLE_SHOT_DIR_COUNT - 1);
		float angle = Math::Lerp(startAngle, endAngle, alpha);
		CVector dir = CQuaternion(0.0f, angle, 0.0f) * forward;

		CCactusNeedle* needle = new CCactusNeedle(ATTACK2_NEEDLE_SPEED, ATTACK2_NEEDLE_DIST);
		needle->Position(pos);
		needle->Rotation(CQuaternion::LookRotation(dir));
	}
}

// ��Ԑ؂�ւ�
void CCactus::ChangeState(int state)
{
	// �U�����ɑ��̏�Ԃֈڍs����ꍇ��
	// �U���I���������Ăяo��
	if (mState != state && IsAttacking())
	{
		AttackEnd();
	}

	// ��Ԑ؂�ւ�
	CEnemy::ChangeState(state);
}

// �ҋ@��Ԃ̍X�V����
void CCactus::UpdateIdle()
{
	// �ʏ펞�̑ҋ@
	if (!mIsBattle)
	{
		ChangeAnimation((int)EAnimType::eIdle);
	}
	// �퓬���̑ҋ@
	else
	{
		ChangeAnimation((int)EAnimType::eIdleBattle);
		// ���X�ɐ퓬����̕����֌���
		LookAtBattleTarget();

		// �X�e�b�v���Ƃɏ�����؂�ւ�
		switch (mStateStep)
		{
			// �X�e�b�v0�F�ҋ@���Ԃ������_���Ō���
			case 0:
				// �ҋ@���Ԃ����܂��ĂȂ����
				if (mBattleIdletime <= 0.0f)
				{
					// �����_���őҋ@���Ԃ�����
					mBattleIdletime = Math::Rand
					(
						BATTLE_IDLE_TIME_MIN,
						BATTLE_IDLE_TIME_MAX
					);
				}
				mStateStep++;
				break;
			// �X�e�b�v1�F�ҋ@���Ԃ̌o�ߑ҂�
			case 1:
				// �퓬���̑ҋ@���ԑ҂�
				if (mElapsedTime < mBattleIdletime)
				{
					mElapsedTime += Times::DeltaTime();
				}
				// �ҋ@���Ԃ��o�߂���
				else
				{
					// ���̏�ԁi�f�t�H���g�͒ǐՏ�ԁj
					EState nextState = EState::eChase;

					// �퓬����܂ł̋��������߂�
					CVector targetPos = mpBattleTarget->Position();
					CVector vec = targetPos - Position();
					vec.Y(0.0f);
					float dist = vec.Length();
					// �퓬����܂ł̋���������Ă�����A
					if (dist >= ATTACK2_DIST)
					{
						// ���m���ŁA�j�U���ɕύX
						int rand = Math::Rand(0, 99);
						if (rand < ATTACK2_PROB) nextState = EState::eAttack2;
					}

					// ���̏�Ԃֈڍs
					ChangeState((int)nextState);

					// �퓬�ҋ@���Ԃ�������
					mBattleIdletime = 0.0f;
				}
				break;
		}
	}
}

// �ǂ������鎞�̍X�V����
void CCactus::UpdateChase()
{
	mMoveSpeed = CVector::zero;

	// ���ݒn�ƖړI�n���擾
	CVector pos = Position();
	CVector targetPos = mpBattleTarget->Position();
	targetPos.Y(pos.Y());
	// ���ݒn����ړI�n�܂ł̃x�N�g�������߂�
	CVector vec = targetPos - pos;
	// �U���͈͓��ł����
	float dist = vec.Length();
	if (dist <= ATTACK_RANGE)
	{
		// �U����Ԃֈڍs
		ChangeState((int)EState::eAttack1);
	}
	// �U���͈͊O
	else
	{
		// ���s�A�j���[�V�������Đ�
		ChangeAnimation((int)EAnimType::eWalk);

		// �c�苗�����ړ��������傫���ꍇ�́A�ړ��������ړ�
		CVector dir = vec.Normalized();
		float moveDist = CHAISE_SPEED * Times::DeltaTime();
		if (dist > moveDist)
		{
			mMoveSpeed = dir * moveDist;
		}
		// �c�苗���̕����������ꍇ�́A
		// �c�苗�����ړ����āA�ҋ@��Ԃֈڍs
		else
		{
			mMoveSpeed = dir * dist;
			ChangeState((int)EState::eIdle);
		}
	}

	// ���X�ɐ퓬����̕����֌���
	LookAtBattleTarget();
}

// �p���`�U�����̍X�V����
void CCactus::UpdateAttack1()
{
	// �X�e�b�v���Ƃɏ����𕪂���
	switch (mStateStep)
	{
		// �X�e�b�v0�F�U���A�j���[�V�����Đ�
		case 0:
			ChangeAnimation((int)EAnimType::eAttack1, true);
			mStateStep++;
			break;
		// �X�e�b�v1�F�U���J�n
		case 1:
			// �U�����J�n����܂ŁA���X�ɐ퓬����̕����֌���
			LookAtBattleTarget();

			// �U���J�n�t���[���܂Ōo�߂�����
			if (GetAnimationFrame() >= ATTACK_START_FRAME)
			{
				// �U���J�n�������Ăяo��
				AttackStart();
				mStateStep++;
			}
			break;
		// �X�e�b�v2�F�U���I��
		case 2:
			// �U���I���t���[���܂Ōo�߂�����
			if (GetAnimationFrame() >= ATTACK_END_FRAME)
			{
				// �U���I���������Ăяo��
				AttackEnd();
				mStateStep++;
			}
			break;
		// �X�e�b�v3�F�U���A�j���[�V�����I���҂�
		case 3:
			// �A�j���[�V�����I��������A�ҋ@��Ԃ֖߂�
			if (IsAnimationFinished())
			{
				ChangeState((int)EState::eIdle);
			}
			break;
	}
}

// �j�U�����̍X�V����
void CCactus::UpdateAttack2()
{
	// �U���I���܂ŁA���X�ɐ퓬����̕����֌���
	if (mStateStep < 3)
	{
		LookAtBattleTarget();
	}

	// �X�e�b�v���Ƃɏ����𕪂���
	switch (mStateStep)
	{
		// �X�e�b�v0�F�U���A�j���[�V�����Đ�
		case 0:
			ChangeAnimation((int)EAnimType::eAttack2, true);
			mShotNeedleCount = 0;	// �j�𔭎˂����񐔂�������
			mStateStep++;
			break;
		// �X�e�b�v1�F�U���J�n
		case 1:
			// �U���J�n�t���[���܂Ōo�߂�����
			if (GetAnimationFrame() >= ATTACK2_START_FRAME)
			{
				// �U���J�n�������Ăяo��
				AttackStart();
				mStateStep++;
			}
			break;
		// �X�e�b�v2�F�j���ˁ��U���I���҂�
		case 2:
			// �j�𔭎˂����񐔂��A���˂���񐔂ɖ����Ȃ��ꍇ
			if (mShotNeedleCount < ATTACK2_NEEDLE_SHOT_COUNT)
			{
				// �U���J�n����U���I���܂ł�
				// �A�j���[�V�����t���[���̐i�s�x�̊����ɍ��킹��
				// ���ɐj�𔭎˂���t���[�������߂�
				float length = ATTACK2_END_FRAME - ATTACK2_START_FRAME;
				float alpha = (float)mShotNeedleCount / (ATTACK2_NEEDLE_SHOT_COUNT - 1);
				float nextFrame = ATTACK2_START_FRAME + length * alpha;
				// ���ɐj�𔭎˂���t���[���𒴂���
				if (GetAnimationFrame() >= nextFrame)
				{
					// �j�𔭎�
					ShotNeedle();
					mShotNeedleCount++;
				}
			}
			// �j��S�Ĕ��˂���
			else
			{
				// �U���I���t���[���܂Ōo�߂�����
				if (GetAnimationFrame() >= ATTACK2_END_FRAME)
				{
					// �U���I���������Ăяo��
					AttackEnd();
					mStateStep++;
				}
			}
			break;
		// �X�e�b�v3�F�U���A�j���[�V�����I���҂�
		case 3:
			// �A�j���[�V�����I��������A�ҋ@��Ԃ֖߂�
			if (IsAnimationFinished())
			{
				ChangeState((int)EState::eIdle);
			}
			break;
	}
}

// �������Ԃ̍X�V����
void CCactus::UpdateHit()
{
	// �X�e�b�v���Ƃɏ����𕪂���
	switch (mStateStep)
	{
		// �X�e�b�v0�F������A�j���[�V�����Đ�
		case 0:
			ChangeAnimation((int)EAnimType::eHit, true);
			mStateStep++;
			break;
		// �X�e�b�v1�F�A�j���[�V�����I���҂�
		case 1:
			// ������A�j���[�V�������I��������A
			// �ҋ@��Ԃ֖߂�
			if (IsAnimationFinished())
			{
				ChangeState((int)EState::eIdle);
			}
			break;
	}
}

// ���S��Ԃ̍X�V����
void CCactus::UpdateDeath()
{
	// �X�e�b�v���Ƃɏ����𕪂���
	switch (mStateStep)
	{
		// �X�e�b�v0�F���S�A�j���[�V�����Đ�
		case 0:
			ChangeAnimation((int)EAnimType::eDeath, true);
			mStateStep++;
			break;
		// �X�e�b�v1�F�A�j���[�V�����I���҂�
		case 1:
			// ���S�A�j���[�V�������I��������A�폜
			if (IsAnimationFinished())
			{
				mStateStep++;
			}
			break;
		// �X�e�b�v2�F���S��̑҂�
		case 2:
			if (mElapsedTime < DEATH_WAIT_TIME)
			{
				mElapsedTime += Times::DeltaTime();
			}
			// �҂����Ԃ��I��������A�폜
			else
			{
				Kill();
			}
			break;
	}
}

// �X�V
void CCactus::Update()
{
	// ��Ԃɍ��킹�āA�X�V������؂�ւ���
	switch ((EState)mState)
	{
	// �ҋ@���
	case EState::eIdle:		UpdateIdle();	break;
	// �ǂ�������
	case EState::eChase:	UpdateChase();	break;
	// �p���`�U��
	case EState::eAttack1:	UpdateAttack1();break;
	// �j�U��
	case EState::eAttack2:	UpdateAttack2();break;
	// ������
	case EState::eHit:		UpdateHit();	break;
	// ���S���
	case EState::eDeath:	UpdateDeath();	break;
	}

	// �G�̃x�[�X�N���X�̍X�V
	CEnemy::Update();

	// �퓬����܂ł̋������f�o�b�O�\��
	if (mpBattleTarget != nullptr)
	{
		CVector targetPos = mpBattleTarget->Position();
		CVector pos = Position();
		targetPos.Y(pos.Y());
		float dist = CVector::Distance(targetPos, pos);
		CDebugPrint::Print("Dist:%.2f\n", dist);
	}
}
