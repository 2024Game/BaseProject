//�v���C���[�N���X�̃C���N���[�h
#include "CPlayer.h"
#include "CInput.h"
#include "CCamera.h"
#include "CBullet.h"
#include "CFlamethrower.h"
#include "CSlash.h"
#include "Maths.h"
#include "CSword.h"
#include "CColliderCapsule.h"
#include "CColliderSphere.h"

// �A�j���[�V�����̃p�X
#define ANIM_PATH "Character\\Player\\anim\\"
#define BODY_HEIGHT 16.0f	// �{�̂̃R���C�_�[�̍���
#define BODY_RADIUS 3.0f	// �{�̂̃R���C�_�[�̕�
#define MOVE_SPEED 0.75f	// �ړ����x
#define JUMP_SPEED 1.5f		// �W�����v���x
#define GRAVITY 0.0625f		// �d�͉����x

#define MOTION_BLUR_TIME 3.0f	// ���[�V�����u���[���|���鎞��
#define MOTION_BLUR_WIDTH 1.0f	// ���[�V�����u���[�̕�
#define MOTION_BLUR_COUNT 5		// ���[�V�����u���[�̔�����

#define ATTACK_START_FRAME 26.0f	// �a��U���̊J�n�t���[��
#define ATTACK_END_FRAME 50.0f		// �a��U���̏I���t���[��
// �a��U���̌��̃I�t�Z�b�g���W
#define ATTACK_SWORD_OFFSET_POS CVector(0.0f, 7.2f, 3.5f)
// �a��U���̌��̃I�t�Z�b�g����
#define ATTACK_SWORD_OFFSET_ROT CVector(-20.0f, 0.0f, -7.0f)

#define ATTACK2_START_FRAME 26.0f	// �R��U���̊J�n�t���[��
#define ATTACK2_END_FRAME 40.0f		// �R��U���̏I���t���[��
#define ATTACK2_COL_RADIUS 7.5f		// �R��U���̃R���C�_�[�̔��a
// �R��U���̃R���C�_�[�̃I�t�Z�b�g���W
#define ATTACK2_COL_OFFSET_POS CVector(0.0f, 4.0f, 5.0f)

// �v���C���[�̃C���X�^���X
CPlayer* CPlayer::spInstance = nullptr;

// �v���C���[�̃A�j���[�V�����f�[�^�̃e�[�u��
const CPlayer::AnimData CPlayer::ANIM_DATA[] =
{
	{ "",						true,	0.0f,	1.0f	},	// T�|�[�Y
	{ ANIM_PATH"idle.x",		true,	153.0f,	1.0f	},	// �ҋ@
	{ ANIM_PATH"walk.x",		true,	66.0f,	1.0f	},	// ���s
	{ ANIM_PATH"attack.x",		false,	92.0f,	1.0f	},	// �a��U��
	{ ANIM_PATH"kick.x",		false,	74.0f,	1.0f	},	// �R��U��
	{ ANIM_PATH"jump_start.x",	false,	25.0f,	1.0f	},	// �W�����v�J�n
	{ ANIM_PATH"jump.x",		true,	1.0f,	1.0f	},	// �W�����v��
	{ ANIM_PATH"jump_end.x",	false,	26.0f,	1.0f	},	// �W�����v�I��
	{ ANIM_PATH"hit.x",			false,	44.0f,	1.0f	},	// ������
};

// �R���X�g���N�^
CPlayer::CPlayer()
	: CXCharacter(ETag::ePlayer, ETaskPriority::ePlayer)
	, mState(EState::eIdle)
	, mStateStep(0)
	, mElapsedTime(0.0f)
	, mMoveSpeedY(0.0f)
	, mIsGrounded(false)
	, mpRideObject(nullptr)
	, mIsPlayedSlashSE(false)
	, mIsSpawnedSlashEffect(false)
	, mMotionBlurRemainTime(0.0f)
	, mpSword(nullptr)
{
	mMaxHp = 100000;
	mHp = mMaxHp;

	//�C���X�^���X�̐ݒ�
	spInstance = this;

	// ���f���f�[�^�擾
	CModelX* model = CResourceManager::Get<CModelX>("Player");

	// �e�[�u�����̃A�j���[�V�����f�[�^��ǂݍ���
	int size = ARRAY_SIZE(ANIM_DATA);
	for (int i = 0; i < size; i++)
	{
		const AnimData& data = ANIM_DATA[i];
		if (data.path.empty()) continue;
		model->AddAnimationSet(data.path.c_str());
	}
	// CXCharacter�̏�����
	Init(model);

	// �ŏ��͑ҋ@�A�j���[�V�������Đ�
	ChangeAnimation(EAnimType::eIdle);

	// �{�̂̃R���C�_�[���쐬
	mpBodyCol = new CColliderCapsule
	(
		this, ELayer::ePlayer,
		CVector(0.0f, BODY_RADIUS, 0.0f),
		CVector(0.0f, BODY_HEIGHT - BODY_RADIUS, 0.0f),
		BODY_RADIUS
	);
	mpBodyCol->SetCollisionTags({ ETag::eField, ETag::eRideableObject, ETag::eEnemy });
	mpBodyCol->SetCollisionLayers({ ELayer::eField, ELayer::eEnemy, ELayer::eAttackCol });

	mpSlashSE = CResourceManager::Get<CSound>("SlashSound");

	mpFlamethrower = new CFlamethrower
	(
		this, nullptr,
		CVector(0.0f, 14.0f, -1.0f),
		CQuaternion(0.0f, 90.0f, 0.0f).Matrix()
	);

	// �v���C���[�̌����쐬
	mpSword = new CSword
	(
		this,
		ETag::ePlayer,
		{ ETag::eEnemy },	// �G�̃^�O���ݒ肳�ꂽ�R���C�_�[�ƏՓ�
		{ ELayer::eEnemy }	// �G�̃��C���[���ݒ肳�ꂽ�R���C�_�[�ƏՓ�
	);

	// �E��̃t���[�����擾���A
	// ���Ƀv���C���[�̉E��̍s����A�^�b�`
	CModelXFrame* frame = mpModel->FinedFrame("Armature_mixamorig_RightHand");
	mpSword->SetAttachMtx(&frame->CombinedMatrix());
	mpSword->Position(ATTACK_SWORD_OFFSET_POS);
	mpSword->Rotation(ATTACK_SWORD_OFFSET_ROT);

	// �R��U���p�̃R���C�_�[���쐬
	mpAttack2Col = new CColliderSphere
	(
		this, ELayer::eAttackCol,
		ATTACK2_COL_RADIUS
	);
	// �G�̖{�̂̃R���C�_�[�Ƃ̂݃q�b�g����悤�ɐݒ�
	mpAttack2Col->SetCollisionTags({ ETag::eEnemy });
	mpAttack2Col->SetCollisionLayers({ ELayer::eEnemy });
	// �v���C���[�̐��ʂɃY����
	mpAttack2Col->Position(ATTACK2_COL_OFFSET_POS);
	// �U���R���C�_�[�͍ŏ��̓I�t�ɂ��Ă���
	mpAttack2Col->SetEnable(false);
}

CPlayer::~CPlayer()
{
	// �R���C�_�[���폜
	SAFE_DELETE(mpBodyCol);
	SAFE_DELETE(mpAttack2Col);

	// �������݂�����A
	if (mpSword != nullptr)
	{
		// ��������������Ă���A�폜
		mpSword->SetOwner(nullptr);
		mpSword->Kill();
	}
}

CPlayer* CPlayer::Instance()
{
	return spInstance;
}

// �A�j���[�V�����؂�ւ�
void CPlayer::ChangeAnimation(EAnimType type, bool restart)
{
	if (!(EAnimType::None < type && type < EAnimType::Num)) return;
	AnimData data = ANIM_DATA[(int)type];
	CXCharacter::ChangeAnimation((int)type, data.loop, data.frameLength, restart);
}

// ��Ԃ�؂�ւ�
void CPlayer::ChangeState(EState state)
{
	if (mState == state) return;

	// �U�����ɑ��ɏ�Ԃɕς�鎞�́A
	// �U���I���������Ăяo���Ă���
	if (IsAttacking())
	{
		AttackEnd();
	}

	mState = state;
	mStateStep = 0;
	mElapsedTime = 0.0f;
}

// �ҋ@
void CPlayer::UpdateIdle()
{
	// �ڒn���Ă���΁A
	if (mIsGrounded)
	{
		// ���N���b�N�Ŏa���U���ֈڍs
		if (CInput::PushKey(VK_LBUTTON))
		{
			mMoveSpeed = CVector::zero;
			ChangeState(EState::eAttack1);
		}
		// �E�N���b�N�ŃL�b�N�U���ֈȍ~
		else if (CInput::PushKey(VK_RBUTTON))
		{
			mMoveSpeed = CVector::zero;
			ChangeState(EState::eAttack2);
		}
		// SPACE�L�[�ŃW�����v�J�n�ֈڍs
		else if (CInput::PushKey(VK_SPACE))
		{
			ChangeState(EState::eJumpStart);
		}
	}
}

// �a��U��
void CPlayer::UpdateAttack1()
{
	switch (mStateStep)
	{
		case 0:
			// �U���A�j���[�V�������J�n
			ChangeAnimation(EAnimType::eAttack, true);
			// �a��SE�̍Đ��ς݃t���O��������
			mIsPlayedSlashSE = false;
			// �a���G�t�F�N�g�̐����ς݃t���O��������
			mIsSpawnedSlashEffect = false;

			mStateStep++;
			break;
		case 1:
			if (GetAnimationFrame() >= ATTACK_START_FRAME)
			{
				// �a��SE���Đ�
				mpSlashSE->Play();
				// �U���J�n
				AttackStart();

				mStateStep++;
			}
			break;
		case 2:
			if (GetAnimationFrame() >= ATTACK_END_FRAME)
			{
				// �U���I��
				AttackEnd();

				mStateStep++;
			}
			break;
		case 3:
			// �U���A�j���[�V�������I��������A
			if (IsAnimationFinished())
			{
				// �ҋ@��Ԃֈڍs
				ChangeState(EState::eIdle);
				ChangeAnimation(EAnimType::eIdle);
			}
			break;
	}
}

// �R��U��
void CPlayer::UpdateAttack2()
{
	switch (mStateStep)
	{
		case 0:
			// �U���A�j���[�V�������J�n
			ChangeAnimation(EAnimType::eKick, true);
			mStateStep++;
			break;
		case 1:
			if (GetAnimationFrame() >= ATTACK2_START_FRAME)
			{
				AttackStart();
				mStateStep++;
			}
			break;
		case 2:
			if (GetAnimationFrame() >= ATTACK2_END_FRAME)
			{
				AttackEnd();
				mStateStep++;
			}
			break;
		case 3:
			// �U���A�j���[�V�������I��������A
			if (IsAnimationFinished())
			{
				// �ҋ@��Ԃֈڍs
				ChangeState(EState::eIdle);
				ChangeAnimation(EAnimType::eIdle);
			}
			break;
	}
}

// �W�����v�J�n
void CPlayer::UpdateJumpStart()
{
	ChangeAnimation(EAnimType::eJumpStart);
	ChangeState(EState::eJump);

	mMoveSpeedY += JUMP_SPEED;
	mIsGrounded = false;
}

// �W�����v��
void CPlayer::UpdateJump()
{
	if (mMoveSpeedY <= 0.0f)
	{
		ChangeAnimation(EAnimType::eJumpEnd);
		ChangeState(EState::eJumpEnd);
	}
}

// �W�����v�I��
void CPlayer::UpdateJumpEnd()
{
	// �W�����v�A�j���[�V�������I�����A
	// �n�ʂɐڒn������A�ҋ@��Ԃ֖߂�
	if (IsAnimationFinished() && mIsGrounded)
	{
		ChangeState(EState::eIdle);
	}
}

// ������
void CPlayer::UpdateHit()
{
	switch (mStateStep)
	{
		case 0:
			// ������A�j���[�V�������J�n
			ChangeAnimation(EAnimType::eHit, true);
			mStateStep++;
			break;
		case 1:
			// ������A�j���[�V�������I��������
			if (IsAnimationFinished())
			{
				// �ҋ@��Ԃֈڍs
				ChangeState(EState::eIdle);
				ChangeAnimation(EAnimType::eIdle);
			}
			break;
	}
}

// �I�u�W�F�N�g�폜��`����
void CPlayer::DeleteObject(CObjectBase* obj)
{
	// ������ɍ폜���ꂽ��A���̃|�C���^��������
	if (mpSword == obj)
	{
		mpSword = nullptr;
	}
}

// �L�[�̓��͏�񂩂�ړ��x�N�g�������߂�
CVector CPlayer::CalcMoveVec() const
{
	CVector move = CVector::zero;

	// �L�[�̓��̓x�N�g�����擾
	CVector input = CVector::zero;
	if (CInput::Key('W'))		input.Y(-1.0f);
	else if (CInput::Key('S'))	input.Y(1.0f);
	if (CInput::Key('A'))		input.X(-1.0f);
	else if (CInput::Key('D'))	input.X(1.0f);

	// ���̓x�N�g���̒����œ��͂���Ă��邩����
	if (input.LengthSqr() > 0.0f)
	{
		// ������x�N�g��(�ݒu���Ă���ꍇ�́A�n�ʂ̖@��)
		CVector up = mIsGrounded ? mGroundNormal : CVector::up;
		// �J�����̌����ɍ��킹���ړ��x�N�g���ɕϊ�
		CCamera* mainCamera = CCamera::MainCamera();
		CVector camForward = mainCamera->VectorZ();
		camForward.Y(0.0f);
		camForward.Normalize();
		// �J�����̐��ʕ����x�N�g���Ə�����x�N�g���̊O�ς���
		// �������̈ړ��x�N�g�������߂�
		CVector moveSide = CVector::Cross(up, camForward);
		// �������̈ړ��x�N�g���Ə�����x�N�g���̊O�ς���
		// ���ʕ����̈ړ��x�N�g�������߂�
		CVector moveForward = CVector::Cross(moveSide, up);

		// ���߂��e�����̈ړ��x�N�g������A
		// �ŏI�I�ȃv���C���[�̈ړ��x�N�g�������߂�
		move = moveForward * input.Y() + moveSide * input.X();
		move.Normalize();
	}

	return move;
}

// �ړ��̍X�V����
void CPlayer::UpdateMove()
{
	mMoveSpeed = CVector::zero;

	// �v���C���[�̈ړ��x�N�g�������߂�
	CVector move = CalcMoveVec();
	// ���߂��ړ��x�N�g���̒����œ��͂���Ă��邩����
	if (move.LengthSqr() > 0.0f)
	{
		mMoveSpeed += move * MOVE_SPEED;

		// �ҋ@��Ԃł���΁A���s�A�j���[�V�����ɐ؂�ւ�
		if (mState == EState::eIdle)
		{
			ChangeAnimation(EAnimType::eWalk);
		}
	}
	// �ړ��L�[����͂��Ă��Ȃ�
	else
	{
		// �ҋ@��Ԃł���΁A�ҋ@�A�j���[�V�����ɐ؂�ւ�
		if (mState == EState::eIdle)
		{
			ChangeAnimation(EAnimType::eIdle);
		}
	}
}

// ���[�V�����u���[�̍X�V����
void CPlayer::UpdateMotionBlur()
{
	// ���[�V�����u���[�̎c�莞�Ԃ��c���Ă��Ȃ���΁A�������Ȃ�
	if (mMotionBlurRemainTime <= 0.0f) return;
	// ���݂̃J�������擾���A���݂��Ȃ���Ώ������Ȃ�
	CCamera* currentCamera = CCamera::CurrentCamera();
	if (currentCamera == nullptr) return;

	// �J�����̌����Ɣ��Ε����փu���[���|���邽�߁A
	// ���]�����J�����̌������擾
	CVector camDir = -currentCamera->VectorZ();

	// �c�莞�Ԃ���o�ߎ��Ԃ̊������擾�i�o�ߎ��Ԃ̊��� = 1 - �c�莞�Ԃ̊����j
	float percent = 1.0f - mMotionBlurRemainTime / MOTION_BLUR_TIME;
	// �u���[�̕����T�C���J�[�u�Ōo�ߎ��Ԃɍ��킹�đ���������
	float ratio = sinf(M_PI * percent);
	float width = MOTION_BLUR_WIDTH * ratio;

	// ���[�V�����u���[�̃p�����[�^��ݒ�
	System::SetMotionBlur(camDir, width, MOTION_BLUR_COUNT);

	// �c�莞�Ԃ��o�ߎ��ԕ�����������
	mMotionBlurRemainTime -= Times::DeltaTime();
	// �c�莞�Ԃ��Ȃ��Ȃ�΁A
	if (mMotionBlurRemainTime <= 0.0f)
	{
		// ���[�V�����u���[���I�t�ɂ���
		System::SetEnableMotionBlur(false);
		mMotionBlurRemainTime = 0.0f;
	}
}

// �X�V
void CPlayer::Update()
{
	SetParent(mpRideObject);
	mpRideObject = nullptr;

	// ��Ԃɍ��킹�āA�X�V������؂�ւ���
	switch (mState)
	{
		// �ҋ@���
		case EState::eIdle:			UpdateIdle();		break;
		// �a��U��
		case EState::eAttack1:		UpdateAttack1();	break;
		// �R��U��
		case EState::eAttack2:		UpdateAttack2();	break;
		// �W�����v�J�n
		case EState::eJumpStart:	UpdateJumpStart();	break;
		// �W�����v��
		case EState::eJump:			UpdateJump();		break;
		// �W�����v�I��
		case EState::eJumpEnd:		UpdateJumpEnd();	break;
		// ������
		case EState::eHit:			UpdateHit();		break;
	}

	// �ҋ@���ƃW�����v���́A�ړ��������s��
	if (mState == EState::eIdle
		|| mState == EState::eJumpStart
		|| mState == EState::eJump
		|| mState == EState::eJumpEnd)
	{
		UpdateMove();
	}

	mMoveSpeedY -= GRAVITY;
	CVector moveSpeed = mMoveSpeed + CVector(0.0f, mMoveSpeedY, 0.0f);

	// �ړ�
	Position(Position() + moveSpeed);

	// �v���C���[���ړ������֌�����
	CVector current = VectorZ();
	CVector target = moveSpeed;
	target.Y(0.0f);
	target.Normalize();
	CVector forward = CVector::Slerp(current, target, 0.125f);
	Rotation(CQuaternion::LookRotation(forward));

	// �z�C�[���N���b�N�Œe�۔���
	if (CInput::PushKey(VK_MBUTTON))
	{
		// �e�ۂ𐶐�
		new CBullet
		(
			// ���ˈʒu
			Position() + CVector(0.0f, 10.0f, 0.0f) + VectorZ() * 20.0f,
			VectorZ(),	// ���˕���
			1000.0f,	// �ړ�����
			1000.0f		// �򋗗�
		);
	}

	// �uE�v�L�[�ŉ��̔��˂��I���I�t����
	if (CInput::PushKey('E'))
	{
		if (!mpFlamethrower->IsThrowing())
		{
			mpFlamethrower->Start();
		}
		else
		{
			mpFlamethrower->Stop();
		}
	}

	// �uP�v�L�[����������A�Q�[�����I��
	if (CInput::PushKey('P'))
	{
		System::ExitGame();
	}

	// �uB�v�L�[����������A���[�V�����u���[�J�n
	if (CInput::PushKey('B'))
	{
		// ���[�V�����u���[���|���Ă���Œ��ł���΁A
		// �V�������[�V�����u���[���|�������Ȃ�
		if (mMotionBlurRemainTime <= 0.0f)
		{
			System::SetEnableMotionBlur(true);
			mMotionBlurRemainTime = MOTION_BLUR_TIME;
		}
	}

	// ���[�V�����u���[�̍X�V����
	UpdateMotionBlur();

	// �L�����N�^�[�̍X�V
	CXCharacter::Update();

	// ����̍s����X�V
	mpSword->UpdateMtx();

	CVector pos = Position();
	CDebugPrint::Print("PlayerHP:%d / %d\n", mHp, mMaxHp);
	CDebugPrint::Print("PlayerPos:%.2f, %.2f, %.2f\n", pos.X(), pos.Y(), pos.Z());
	CDebugPrint::Print("PlayerGrounded:%s\n", mIsGrounded ? "true" : "false");
	CDebugPrint::Print("PlayerState:%d\n", mState);

	mIsGrounded = false;

	CDebugPrint::Print("FPS:%f\n", Times::FPS());
}

// �U������
bool CPlayer::IsAttacking() const
{
	// �a��U����
	if (mState == EState::eAttack1) return true;
	// �R��U����
	if (mState == EState::eAttack2) return true;

	// �U�����łȂ�
	return false;
}

// �U���J�n
void CPlayer::AttackStart()
{
	// �x�[�X�N���X�̍U���J�n�������Ăяo��
	CXCharacter::AttackStart();

	// �a��U�����ł���΁A���̃R���C�_�[���I���ɂ���
	if (mState == EState::eAttack1)
	{
		mpSword->SetEnableCol(true);
	}
	// �R��U�����ł���΁A�R��U���p�̃R���C�_�[���I���ɂ���
	else if (mState == EState::eAttack2)
	{
		mpAttack2Col->SetEnable(true);
	}
}

// �U���I��
void CPlayer::AttackEnd()
{
	// �x�[�X�N���X�̍U���I���������Ăяo��
	CXCharacter::AttackEnd();

	// �U���R���C�_�[���I�t
	mpSword->SetEnableCol(false);
	mpAttack2Col->SetEnable(false);
}

// �_���[�W���󂯂�
void CPlayer::TakeDamage(int damage, CObjectBase* causer)
{
	// �x�[�X�N���X�̃_���[�W�������Ăяo��
	CXCharacter::TakeDamage(damage, causer);

	// ���S���Ă��Ȃ���΁A
	if (!IsDeath())
	{
		// �������Ԃֈڍs
		ChangeState(EState::eHit);

		// �U��������������̕����֌���
		CVector targetPos = causer->Position();
		CVector vec = targetPos - Position();
		vec.Y(0.0f);
		Rotation(CQuaternion::LookRotation(vec.Normalized()));

		// �ړ����~
		mMoveSpeed = CVector::zero;
	}
}

// �Փˏ���
void CPlayer::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// �{�̂̃R���C�_�[�̏Փ˔���
	if (self == mpBodyCol)
	{
		// �t�B�[���h�Ƃ̏Փ�
		if (other->Layer() == ELayer::eField)
		{
			// �⓹�Ŋ���Ȃ��悤�ɁA�����߂��x�N�g����X��Z�̒l��0�ɂ���
			CVector adjust = hit.adjust;
			adjust.X(0.0f);
			adjust.Z(0.0f);

			Position(Position() + adjust * hit.weight);

			// �Փ˂����n�ʂ������V�䂩����ςŔ���
			CVector normal = hit.adjust.Normalized();
			float dot = CVector::Dot(normal, CVector::up);
			// ���ς̌��ʂ��v���X�ł���΁A���ƏՓ˂���
			if (dot >= 0.0f)
			{
				// �����Ȃǂŏ��ɏォ��Փ˂������i���ړ��j�̂�
				// �㉺�̈ړ����x��0�ɂ���
				if (mMoveSpeedY < 0.0f)
				{
					mMoveSpeedY = 0.0f;
				}

				// �ڒn����
				mIsGrounded = true;
				// �ڒn�����n�ʂ̖@�����L�����Ă���
				mGroundNormal = hit.adjust.Normalized();

				if (other->Tag() == ETag::eRideableObject)
				{
					mpRideObject = other->Owner();
				}
			}
			// ���ς̌��ʂ��}�C�i�X�ł���΁A�V��ƏՓ˂���
			else
			{
				// �W�����v�ȂǂœV��ɉ�����Փ˂������i��ړ��j�̂�
				// �㉺�̈ړ����x��0�ɂ���
				if (mMoveSpeedY > 0.0f)
				{
					mMoveSpeedY = 0.0f;
				}
			}
		}
		// �G�ƏՓ˂����ꍇ
		else if (other->Layer() == ELayer::eEnemy)
		{
			// �������ɂ̂݉����߂����߁A
			// �����߂��x�N�g����Y�̒l��0�ɂ���
			CVector adjust = hit.adjust;
			adjust.Y(0.0f);
			Position(Position() + adjust * hit.weight);
		}
	}
	// ���̃R���C�_�[���Փ˂���
	else if (self == mpSword->Collider())
	{
		CCharaBase* hitChara = dynamic_cast<CCharaBase*>(other->Owner());
		if (hitChara != nullptr && !IsAttackHitObj(hitChara))
		{
			AddAttackHitObj(hitChara);
			hitChara->TakeDamage(1, this);
		}
	}
	// �R��U���̃R���C�_�[���Փ˂���
	else if (self == mpAttack2Col)
	{
		CCharaBase* hitChara = dynamic_cast<CCharaBase*>(other->Owner());
		if (hitChara != nullptr && !IsAttackHitObj(hitChara))
		{
			AddAttackHitObj(hitChara);
			hitChara->TakeDamage(1, this);
		}
	}
}

// �`��
void CPlayer::Render()
{
	CXCharacter::Render();
}
