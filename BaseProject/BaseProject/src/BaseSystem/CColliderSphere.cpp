#include "CColliderSphere.h"
#include <glut.h>
#include "CColor.h"

// �R���X�g���N�^
CColliderSphere::CColliderSphere(CObjectBase* owner, ELayer layer, float radius,
	bool isKinematic, float weight)
	: CCollider(owner, layer, EColliderType::eSphere, isKinematic, weight)
	, mRadius(radius)
{
}

// ���R���C�_�[�̐ݒ�
void CColliderSphere::Set(CObjectBase* owner, ELayer layer, float radius)
{
	CCollider::Set(owner, layer);

	// ���a��ݒ�
	mRadius = radius;
}

// ���̍��W�Ɣ��a���擾
void CColliderSphere::Get(CVector* pos, float* rad) const
{
	*pos = mWPos;
	*rad = mWRadius;
}

#if _DEBUG
void CColliderSphere::Render()
{
	// ���݂̍s���ޔ����Ă���
	glPushMatrix();

	// ���g�̍s���K�p
	CMatrix m;
	m.Translate(Position());
	glMultMatrixf((m * Matrix()).M());
	// �A���t�@�u�����h��L���ɂ���
	glEnable(GL_BLEND);
	// �u�����h���@���w��
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// ���C�g�I�t
	glDisable(GL_LIGHTING);

	// DIFFUSE�ԐF�ݒ�
	CColor col = CColor::red;
	if (!IsEnable() ||
		(Owner() != nullptr && !Owner()->IsEnableCol()))
	{
		col = CColor::gray;
	}
	col.A(0.2f);
	float* c = (float*)&col;
	glMaterialfv(GL_FRONT, GL_DIFFUSE, c);
	glColor4fv(c);

	// ����`��
	glutWireSphere(mRadius, 32, 32);

	// ���C�g�I��
	glEnable(GL_LIGHTING);
	// �A���t�@�u�����h����
	glDisable(GL_ALPHA);

	// �`��O�̍s��ɖ߂�
	glPopMatrix();
}
#endif

// �R���C�_�[�̏����X�V
void CColliderSphere::UpdateCol(bool isInit)
{
	// �O��̍X�V�Ɠ����s��ł���΁A�������Ȃ�
	CMatrix m = Matrix();
	if (!isInit && m == mLastMtx) return;
	mLastMtx = m;

	// �s��𔽉f�������S�ʒu�Ɣ��a���v�Z
	mWPos = Position() * m;
	mWRadius = mRadius * m.VectorX().Length();

	// �o�E���f�B���O�{�b�N�X���X�V
	mBounds = CBounds::GetSphereBounds(mWPos, mWRadius);
}
