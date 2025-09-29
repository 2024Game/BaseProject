#include "CColliderTriangle.h"
#include "glut.h"
#include "Maths.h"
#include "CColor.h"

// �R���X�g���N�^
CColliderTriangle::CColliderTriangle(CObjectBase* owner, ELayer layer,
	const CVector& v0, const CVector& v1, const CVector& v2,
	bool isKinematic, float weight)
	: CCollider(owner, layer, EColliderType::eTriangle, isKinematic, weight)
{
	// �O�p�`�̒��_��ݒ�
	mV[0] = v0;
	mV[1] = v1;
	mV[2] = v2;
}

// �O�p�`�R���C�_�[�̐ݒ�
void CColliderTriangle::Set(CObjectBase* owner, ELayer layer,
	const CVector& v0, const CVector& v1, const CVector& v2)
{
	CCollider::Set(owner, layer);

	// �O�p�`�̒��_��ݒ�
	mV[0] = v0;
	mV[1] = v1;
	mV[2] = v2;
}

// �O�p�`�̒��_���擾
void CColliderTriangle::Get(CVector* v0, CVector* v1, CVector* v2) const
{
	*v0 = mWV[0];
	*v1 = mWV[1];
	*v2 = mWV[2];
}

#if _DEBUG
// �R���C�_�[�`��
void CColliderTriangle::Render()
{
	// ���݂̍s���ޔ����Ă���
	glPushMatrix();

	// ���g�̍s���K�p
	glMultMatrixf(Matrix().M());
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

	// �O�p�`��`��
	glBegin(GL_TRIANGLES);
	glVertex3f(mV[0].X(), mV[0].Y(), mV[0].Z());
	glVertex3f(mV[1].X(), mV[1].Y(), mV[1].Z());
	glVertex3f(mV[2].X(), mV[2].Y(), mV[2].Z());
	glEnd();

	// ���C�g�I��
	glEnable(GL_LIGHTING);
	// �A���t�@�u�����h����
	glDisable(GL_ALPHA);

	// �`��O�̍s��ɖ߂�
	glPopMatrix();
}
#endif

// �R���C�_�[�̏����X�V
void CColliderTriangle::UpdateCol(bool isInit)
{
	// �O��̍X�V�Ɠ����s��ł���΁A�������Ȃ�
	CMatrix m = Matrix();
	if (!isInit && m == mLastMtx) return;
	mLastMtx = m;

	// �s��𔽉f�����e���_�̍��W���v�Z
	mWV[0] = mV[0] * m;
	mWV[1] = mV[1] * m;
	mWV[2] = mV[2] * m;

	// �o�E���f�B���O�{�b�N�X���X�V
	mBounds = CBounds::GetTriangleBounds(mWV[0], mWV[1], mWV[2]);
}