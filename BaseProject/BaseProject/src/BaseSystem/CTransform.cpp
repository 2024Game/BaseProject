#include "CTransform.h"

// �R���X�g���N�^
CTransform::CTransform(const CVector& pos, const CQuaternion& rot, const CVector& scale)
	: mpParent(nullptr)
{
	Position(pos);
	Rotation(rot);
	Scale(scale);
}

// �R���X�g���N�^
CTransform::CTransform(const CVector& pos, const CVector& eulerAngles, const CVector& scale)
	: CTransform(pos, CQuaternion(eulerAngles), scale)
{
}

// �f�X�g���N�^
CTransform::~CTransform()
{
	// �e�����݂���΁A�e�Ƃ̌q���������
	if (mpParent != nullptr)
	{
		mpParent->RemoveChild(this);
		mpParent = nullptr;
	}

	// �q�����݂���΁A�q�Ƃ̌q���������
	auto itr = mChildren.begin();
	auto end = mChildren.end();
	while (itr != end)
	{
		CTransform* child = *itr;
		itr++;
		child->SetParent(nullptr);
	}
	mChildren.clear();
}

// ���[���h�ʒu���擾
const CVector& CTransform::Position() const
{
	return mPosition;
}

// ���[���h�ʒu��ݒ�
void CTransform::Position(const CVector& pos)
{
	// �e�����݂���ꍇ
	if (mpParent != nullptr)
	{
		// ���[���h�ʒu�͂��̂܂ܐݒ�
		mPosition = pos;
		// ���g�̃��[���h�ʒu - �e�̃��[���h�ʒu ��
		// ���g�̃��[�J���ʒu�����߂�
		mLocalPosition = mpParent->Rotation().Inverse() * (mPosition - mpParent->Position());
	}
	// �e�����݂��Ȃ��ꍇ
	else
	{
		// ���[���h�ʒu�A���[�J���ʒu�ɂ��̂܂ܐݒ�
		mPosition = pos;
		mLocalPosition = pos;
	}

	// �ύX���q�ɔ��f
	ApplyChildren();
}

// ���[���h�ʒu��ݒ�
void CTransform::Position(float x, float y, float z)
{
	Position(CVector(x, y, z));
}

// ���[�J���ʒu���擾
const CVector& CTransform::LocalPosition() const
{
	return mLocalPosition;
}

// ���[�J���ʒu��ݒ�
void CTransform::LocalPosition(const CVector& pos)
{
	// �e�����݂���ꍇ
	if (mpParent != nullptr)
	{
		// ���[�J���ʒu�͂��̂܂ܐݒ�
		mLocalPosition = pos;
		// �e�̃��[���h�ʒu + ���g�̃��[�J���ʒu ��
		// ���g�̃��[���h�ʒu�����߂�
		mPosition = mpParent->Position() + mpParent->Rotation() * mLocalPosition;
	}
	// �e�����݂��Ȃ��ꍇ
	else
	{
		// ���[�J���ʒu�A���[���h�ʒu�ɂ��̂܂ܐݒ�
		mLocalPosition = pos;
		mPosition = pos;
	}

	// �ύX���q�ɔ��f
	ApplyChildren();
}

// ���[�J���ʒu��ݒ�
void CTransform::LocalPosition(float x, float y, float z)
{
	LocalPosition(CVector(x, y, z));
}

// ���[���h��]�l�i�N�H�[�^�j�I���j���擾
const CQuaternion& CTransform::Rotation() const
{
	return mRotation;
}

// ���[�J����]�l�i�N�H�[�^�j�I���j���擾
const CQuaternion& CTransform::LocalRotation() const
{
	return mLocalRotation;
}

// ���[���h��]�p�x���擾�i�f�B�O���[�j
CVector CTransform::EulerAngles() const
{
	return mRotation.Euler();
}

// ���[�J����]�p�x���擾�i�f�B�O���[�j
CVector CTransform::LocalEulerAngles() const
{
	return mLocalRotation.Euler();
}

// ���[���h��]�l��ݒ�
void CTransform::Rotation(const CQuaternion& rot)
{
	// �e�����݂���ꍇ
	if (mpParent != nullptr)
	{
		// ���[���h��]�l�͂��̂܂ܐݒ�
		mRotation = rot;
		// �e�̃��[���h��]�l�̋t�N�H�[�^�j�I�� * ���g�̃��[���h��]�l ��
		// ���g�̃��[�J����]�l�����߂�
		mLocalRotation = mpParent->Rotation().Inverse() * mRotation;
	}
	// �e�����݂��Ȃ��ꍇ
	else
	{
		// ���[���h��]�l�A���[�J����]�l�ɂ��̂܂ܐݒ�
		mRotation = rot;
		mLocalRotation = rot;
	}

	// �ύX���q�ɔ��f
	ApplyChildren();
}

// ���[���h��]�l��ݒ�
void CTransform::Rotation(float x, float y, float z)
{
	Rotation(CQuaternion(x, y, z));
}

// ���[���h��]�l��ݒ�
void CTransform::Rotation(const CVector& eulerAngles)
{
	Rotation(CQuaternion(eulerAngles));
}

// ���[�J����]�l��ݒ�
void CTransform::LocalRotation(const CQuaternion& rot)
{
	// �e�����݂���ꍇ
	if (mpParent != nullptr)
	{
		// ���[�J����]�l�͂��̂܂ܐݒ�
		mLocalRotation = rot;
		// �e�̃��[���h��]�l * ���g�̃��[�J����]�l ��
		// ���g�̃��[���h��]�l�����߂�
		mRotation = mpParent->Rotation() * mLocalRotation;
	}
	// �e�����݂��Ȃ��ꍇ
	else
	{
		// ���[�J����]�l�A���[���h��]�l�ɂ��̂܂ܐݒ�
		mLocalRotation = rot;
		mRotation = rot;
	}

	// �ύX���q�ɔ��f
	ApplyChildren();
}

// ���[�J����]�l��ݒ�
void CTransform::LocalRotation(float x, float y, float z)
{
	LocalRotation(CQuaternion(x, y, z));
}

// ���[�J����]�l��ݒ�
void CTransform::LocalRotation(const CVector& eulerAngles)
{
	LocalRotation(CQuaternion(eulerAngles));
}

// ���[���h�X�P�[���l���擾
const CVector& CTransform::Scale() const
{
	return mScale;
}

// ���[�J���X�P�[���l���擾
const CVector& CTransform::LocalScale() const
{
	return mLocalScale;
}

// ���[���h�X�P�[���l��ݒ�
void CTransform::Scale(const CVector& scale)
{
	// �e�����݂���ꍇ
	if (mpParent != nullptr)
	{
		// ���[���h�X�P�[���l�͂��̂܂ܐݒ�
		mScale = scale;
		// ���g�̃��[���h�X�P�[���l / �e�̃��[���h�X�P�[���l ��
		// ���g�̃��[�J���X�P�[���l�����߂�
		CVector parentScale = mpParent->Scale();
		mLocalScale.X(mScale.X() / parentScale.X());
		mLocalScale.Y(mScale.Y() / parentScale.Y());
		mLocalScale.Z(mScale.Z() / parentScale.Z());
	}
	// �e�����݂��Ȃ��ꍇ
	else
	{
		// ���[�J���X�P�[���l�A���[���h�X�P�[���l�ɂ��̂܂ܐݒ�
		mScale = scale;
		mLocalScale = scale;
	}

	// �ύX���q�ɔ��f
	ApplyChildren();
}

// ���[���h�X�P�[���l��ݒ�
void CTransform::Scale(float x, float y, float z)
{
	Scale(CVector(x, y, z));
}

// ���[�J���X�P�[���l��ݒ�
void CTransform::LocalScale(const CVector& scale)
{
	// �e�����݂���ꍇ
	if (mpParent != nullptr)
	{
		// ���[�J���X�P�[���l�͂��̂܂ܐݒ�
		mLocalScale = scale;
		// �e�̃��[���h�X�P�[���l * ���g�̃��[�J���X�P�[���l ��
		// ���g�̃��[���h�X�P�[���l�����߂�
		CVector parentScale = mpParent->Scale();
		mScale.X(parentScale.X() * mLocalScale.X());
		mScale.Y(parentScale.Y() * mLocalScale.Y());
		mScale.Z(parentScale.Z() * mLocalScale.Z());
	}
	// �e�����݂��Ȃ��ꍇ
	else
	{
		// ���[�J���X�P�[���l�A���[���h�X�P�[���l�ɂ��̂܂ܐݒ�
		mLocalScale = scale;
		mScale = scale;
	}

	// �ύX���q�ɔ��f
	ApplyChildren();
}

// ���[�J���X�P�[���l��ݒ�
void CTransform::LocalScale(float x, float y, float z)
{
	LocalScale(CVector(x, y, z));
}

// �ʒu�A��]�l�A�X�P�[���l�����킹���s����擾
CMatrix CTransform::Matrix() const
{
	CMatrix s, r, t;
	s.Scale(mScale);
	r = mRotation.Matrix();
	t.Translate(mPosition);
	return s * r * t;
}

// ���ʕ����̃x�N�g�����擾
CVector CTransform::VectorZ() const
{
	return mRotation * CVector::forward;
}

// �������̃x�N�g�����擾
CVector CTransform::VectorX() const
{
	return mRotation * CVector::right;
}

// ������̃x�N�g�����擾
CVector CTransform::VectorY() const
{
	return mRotation * CVector::up;
}

// ���s�ړ��i���݂̌����ɑ΂��Ă̈ړ��j
void CTransform::Translate(const CVector& translation)
{
	Position(mPosition + mRotation * translation);
}

// ���s�ړ��i���݂̌����ɑ΂��Ă̈ړ��j
void CTransform::Translate(float x, float y, float z)
{
	Translate(CVector(x, y, z));
}

// ��]
void CTransform::Rotate(const CVector& eulerAngles)
{
	Rotation(mRotation * CQuaternion(eulerAngles));
}

// ��]
void CTransform::Rotate(float x, float y, float z)
{
	Rotate(CVector(x, y, z));
}

// �C�ӎ���]
void CTransform::RotateAxis(const CVector& axis, float angle)
{
	CQuaternion q;
	q.RotationAxis(axis, angle);
	Rotation(q * mRotation);
}

// �^�[�Q�b�g�ʒu�֌�����
void CTransform::LookAt(const CVector& target)
{
	CVector forward = (target - mPosition).Normalized();
	Rotation(CQuaternion::LookRotation(forward));
}

// �e��ݒ�
void CTransform::SetParent(CTransform* parent)
{
	// ���ɐݒ�ς݂̐e�Ȃ�A�������Ȃ�
	if (mpParent == parent) return;

	// ���ɑ��̐e���ݒ肳��Ă���΁A
	if (mpParent != nullptr)
	{
		// ���̐e�Ɛe�q�֌W������
		mpParent->RemoveChild(this);
		mpParent = nullptr;
		// �e�q�֌W�����������̂ŁA
		// ���g�̃��[�J����Ԃł̈ʒu�A��]�l�A�X�P�[���l��
		// ���[���h��Ԃł̊e�l��ݒ肷��
		LocalPosition(mPosition);
		LocalRotation(mRotation);
		LocalScale(mScale);
	}

	// �V�����e��ݒ�
	mpParent = parent;

	// �V�����e�� nullptr �łȂ���΁A
	if (mpParent != nullptr)
	{
		// �V�����e�̎q�Ɏ��g��ݒ�
		mpParent->AddChild(this);
		// �V�����e��ݒ肵���̂ŁA
		// ���g�̃��[�J����Ԃł̈ʒu�A��]�l�A�X�P�[���l���Čv�Z����
		// �i���[���h��Ԃł̊e�l��ݒ肵�������ƂŁA���[�J����Ԃ̒l���Čv�Z�����j
		Position(mPosition);
		Rotation(mRotation);
		Scale(mScale);
	}
}

// �e���擾
CTransform* CTransform::GetParent() const
{
	return mpParent;
}

// �q�̃��X�g�ɒǉ�
void CTransform::AddChild(CTransform* child)
{
	auto find = std::find(mChildren.begin(), mChildren.end(), child);
	if (find != mChildren.end()) return;
	mChildren.push_back(child);
}

// �q�̃��X�g�����菜��
void CTransform::RemoveChild(CTransform* child)
{
	mChildren.remove(child);
}

// �q�Ɏ��g�̕ύX�𔽉f
void CTransform::ApplyChildren()
{
	for (CTransform* child : mChildren)
	{
		child->Update();
	}
}

// �X�V
void CTransform::Update()
{
	// �e�����݂���ꍇ
	if (mpParent != nullptr)
	{
		// ���[���h�ʒu�́A�u�e�̈ʒu + �e�̉�]�l * ���[�J���ʒu�v�ŋ��߂�
		mPosition = mpParent->Position() + mpParent->Rotation() * mLocalPosition;
		// ���[���h��]�l�́A�u�e�̉�]�l * ���[�J����]�l�v�ŋ��߂�
		mRotation = mpParent->Rotation() * mLocalRotation;
		// ���[���h�X�P�[���l�́A�u�e�̃X�P�[���l * ���[�J����]�l�v�ŋ��߂�
		CVector parentScale = mpParent->Scale();
		mScale.X(parentScale.X() * mLocalScale.X());
		mScale.Y(parentScale.Y() * mLocalScale.Y());
		mScale.Z(parentScale.Z() * mLocalScale.Z());
	}
	// �e�����݂��Ȃ��ꍇ
	else
	{
		// ���[���h��Ԃł̊e�l�ƃ��[�J����Ԃł̊e�l�Ɠ���
		mPosition = mLocalPosition;
		mRotation = mLocalRotation;
		mScale = mLocalScale;
	}

	// �ύX���q�ɔ��f
	ApplyChildren();
}
