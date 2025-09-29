#include "CColliderCapsule.h"
#include <glut.h>
#include "Maths.h"
#include "CColor.h"
#include "Primitive.h"

// �R���X�g���N�^
CColliderCapsule::CColliderCapsule(CObjectBase* owner, ELayer layer,
	const CVector& v0, const CVector& v1, float radius,
	bool isKinematic, float weight)
	: CCollider(owner, layer, EColliderType::eCapsule, isKinematic, weight)
	, mRadius(radius)
{
	// �J�v�Z�����\����������̒��_��ݒ�
	mV[0] = v0;
	mV[1] = v1;
}

// �J�v�Z���R���C�_�[�̐ݒ�
void CColliderCapsule::Set(CObjectBase* owner, ELayer layer,
	const CVector& v0, const CVector& v1, float radius)
{
	CCollider::Set(owner, layer);

	// �J�v�Z�����\����������̒��_��ݒ�
	mV[0] = v0;
	mV[1] = v1;

	// ���a��ݒ�
	mRadius = radius;
}

// �J�v�Z�����\����������̎��_�ƏI�_���擾
void CColliderCapsule::Get(CVector* v0, CVector* v1) const
{
	*v0 = mWV[0];
	*v1 = mWV[1];
}

// �J�v�Z���̔��a���擾
float CColliderCapsule::Radius() const
{
	return mRadius;
}

#if _DEBUG
// �R���C�_�[�`��
void CColliderCapsule::Render()
{
	// DIFFUSE�ԐF�ݒ�
	CColor col = CColor::red;
	if (!IsEnable() ||
		(Owner() != nullptr && !Owner()->IsEnableCol()))
	{
		col = CColor::gray;
	}

	CMatrix m = Matrix();
	CVector s = mV[0] * m;
	CVector e = mV[1] * m;
	Primitive::DrawWireCapsule
	(
		s, e, mRadius, col
	);
}
#endif

// �R���C�_�[�̏����X�V
void CColliderCapsule::UpdateCol(bool isInit)
{
	// �O��̍X�V�Ɠ����s��ł���΁A�������Ȃ�
	CMatrix m = Matrix();
	if (!isInit && m == mLastMtx) return;
	mLastMtx = m;

	// ���_�̃��[���h���W���Z�o
	mWV[0] = mV[0] * m;
	mWV[1] = mV[1] * m;

	// �o�E���f�B���O�{�b�N�X���X�V
	mBounds = CBounds::GetCapsuleBounds(mWV[0], mWV[1], mRadius);
}
