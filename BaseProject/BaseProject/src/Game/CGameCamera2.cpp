#include "CGameCamera2.h"
#include "CInput.h"
#include "Maths.h"

// �J�����̉�]���x
#define ROTATE_SPEED 0.1f
// �J�����̏㉺��]�͈̔�
#define ROTATE_RANGE_X 45.0f

// �R���X�g���N�^
CGameCamera2::CGameCamera2(const CVector& eye, const CVector& center, bool isMainCamera)
	: CCamera(eye, center, isMainCamera)
	, mFollowDefaultEyeVec(CVector::forward)
	, mRotateAngle(CVector::zero)
{
}

// �f�X�g���N�^
CGameCamera2::~CGameCamera2()
{
}

void CGameCamera2::SetFollowTargetTf(CTransform* target)
{
	mFollowTargetTf = target;
	if (mFollowTargetTf != nullptr)
	{
		mFollowDefaultEyeVec = mTargetEye - mAt;
		mFollowOffsetPos = mAt - mFollowTargetTf->Position();
	}
}

void CGameCamera2::LookAt(const CVector& eye, const CVector& at, const CVector& up, bool updateTargetEye)
{
	CCamera::LookAt(eye, at, up, updateTargetEye);
	if (mFollowTargetTf != nullptr)
	{
		if (updateTargetEye)
		{
			mFollowDefaultEyeVec = mTargetEye - mAt;
		}
		mFollowOffsetPos = mAt - mFollowTargetTf->Position();
	}
}

// ��X�V
void CGameCamera2::LateUpdate()
{
	// �Ǐ]����^�[�Q�b�g���ݒ肳��Ă���΁A
	if (mFollowTargetTf != nullptr)
	{
		// �}�E�X�̈ړ��ʂɍ��킹�āA�J�����̉�]�p�x��ύX
		CVector2 delta = CInput::GetDeltaMousePos();
		float x = Math::Clamp(mRotateAngle.X() + delta.Y() * ROTATE_SPEED, -ROTATE_RANGE_X, ROTATE_RANGE_X);
		float y = Math::Repeat(mRotateAngle.Y() + delta.X() * ROTATE_SPEED, 360.0f);
		mRotateAngle.X(x);
		mRotateAngle.Y(y);

		// ��]�l�����߂āA�����_���王�_�܂ł̃x�N�g������]�����邱�ƂŁA
		// ���_�̈ʒu���X�V����
		CQuaternion rot = CQuaternion(mRotateAngle);
		mAt = mFollowTargetTf->Position() + mFollowOffsetPos;
		mTargetEye = mAt + rot * mFollowDefaultEyeVec;
		mEye = mTargetEye;
	}

	CDebugPrint::Print("Angle:%f, %f, %f", mRotateAngle.X(), mRotateAngle.Y(), mRotateAngle.Z());

	// �ݒ肳��Ă���R���C�_�[�ƏՓ˂���ꍇ�́A
	// �J�����̈ʒu�������o��
	ApplyCollision();

	// ���_�A�����_�A��x�N�g������e�s����X�V
	LookAt(mEye, mAt, mUp, false);

	// �J�����̃x�[�X�̍X�V����
	//CCamera::Update();
}
