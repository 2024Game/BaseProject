#include "CColBase.h"
#include "CDebugInput.h"
#include "CCamera.h"

// �R���X�g���N�^
CColBase::CColBase(const CColor& defaultColor)
	: CObjectBase(ETag::eNone, ETaskPriority::eDefault)
	, mDefaultColor(defaultColor)
	, mIsCollision(false)
	, mIsSelect(false)
{
}

// �f�X�g���N�^
CColBase::~CColBase()
{
}

// �I����Ԃ�ݒ�
void CColBase::SetSelect(bool isSelect)
{
	mIsSelect = isSelect;
}

// �Փˏ���
void CColBase::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	mIsCollision = true;
}

// �X�V
void CColBase::Update()
{
#if _DEBUG
	mIsCollision = false;

	// �I����ԂłȂ���΁A�ȍ~�������Ȃ�
	if (!mIsSelect) return;

	if (CDebugInput::Key(VK_CONTROL))
	{
		// ���̓x�N�g�����擾
		CVector input;
		if (CDebugInput::Key(VK_SHIFT))
		{
			if (CDebugInput::Key(VK_LEFT))		input.Z(1.0f);
			else if (CDebugInput::Key(VK_RIGHT))input.Z(-1.0f);
		}
		else
		{
			if (CDebugInput::Key(VK_UP))		input.X(1.0f);
			else if (CDebugInput::Key(VK_DOWN))	input.X(-1.0f);
			if (CDebugInput::Key(VK_LEFT))		input.Y(-1.0f);
			else if (CDebugInput::Key(VK_RIGHT))input.Y(1.0f);
		}

		// ���̓x�N�g���̒����œ��͂���Ă��邩����
		if (input.LengthSqr() > 0.0f)
		{
			static const float rotateSpeed = 5.0f;
			Rotate(input * rotateSpeed);
		}
	}
	else
	{
		// ���̓x�N�g�����擾
		CVector input;
		if (CDebugInput::Key(VK_SHIFT))
		{
			if (CDebugInput::Key(VK_UP))		input.Y(1.0f);
			else if (CDebugInput::Key(VK_DOWN))
			{
				input.Y(-1.0f);
			}
		}
		else
		{
			if (CDebugInput::Key(VK_UP))		input.Z(-1.0f);
			else if (CDebugInput::Key(VK_DOWN))	input.Z(1.0f);
			if (CDebugInput::Key(VK_LEFT))		input.X(-1.0f);
			else if (CDebugInput::Key(VK_RIGHT))input.X(1.0f);
		}

		// ���̓x�N�g���̒����œ��͂���Ă��邩����
		if (input.LengthSqr() > 0.0f)
		{
			CVector move = input;
			if (input.Y() == 0.0f)
			{
				// �J�����̌����ɍ��킹���ړ��x�N�g���ɕϊ�
				CQuaternion camRot = CCamera::CurrentCamera()->Rotation();
				move = camRot * input;
				move.Y(0.0f);
			}
			move.Normalize();

			// �R���C�_�[���ړ�
			static const float moveSpeed = 0.25f;
			Position(Position() + move * moveSpeed);
		}
	}
#endif
}

// ���݂̐F���擾
CColor CColBase::GetColor() const
{
	CColor color = mDefaultColor;
	if (mIsSelect)
	{
		if (mIsCollision) color = CColor(1.0f, 0.0f, 1.0f, 1.0f);
		else color = CColor(1.0f, 0.75f, 0.0f, 1.0f);
	}
	else
	{
		if (mIsCollision) color = CColor::red;
	}
	return color;
}
