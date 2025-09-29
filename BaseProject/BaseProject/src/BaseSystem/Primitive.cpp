#include "Primitive.h"
#include <glut.h>
#include "CMatrix.h"
#include "Maths.h"

// ������`��i3D�j
void Primitive::DrawLine(const CVector& s, const CVector& e,
	const CColor& color, float lineWidth, EBlend blend)
{
	// ���݂̍s���ޔ����Ă���
	glPushMatrix();

	// �u�����h������L����
	Blend::EnableBlend(blend);
	// ���C�g�I�t
	glDisable(GL_LIGHTING);

	// DIFFUSE�ԐF�ݒ�
	float c[] = { color.R(), color.G(), color.B(), color.A() };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, c);
	glColor4fv(c);

	glLineWidth(lineWidth);
	// ������`��
	glBegin(GL_LINES);
	glVertex3f(s.X(), s.Y(), s.Z());
	glVertex3f(e.X(), e.Y(), e.Z());
	glEnd();
	glLineWidth(1.0f);

	// ���C�g�I��
	glEnable(GL_LIGHTING);
	// �u�����h�����𖳌���
	Blend::DisableBlend();

	// �`��O�̍s��ɖ߂�
	glPopMatrix();
}

// ������`��i2D�j
void Primitive::DrawLine2D(const CVector2& s, const CVector2& e,
	const CColor& color, float lineWidth, EBlend blend)
{
}

// ����`��
void Primitive::DrawSphere(const CMatrix& m, float rad,
	const CColor& color, EBlend blend)
{
	// ���݂̍s���ޔ����Ă���
	glPushMatrix();

	CMatrix sm;
	sm.Scale(rad, rad, rad);
	// �`��s��𔽉f
	glMultMatrixf((sm * m).M());

	// �u�����h������L����
	Blend::EnableBlend(blend);
	// ���C�g�I�t
	glDisable(GL_LIGHTING);

	// DIFFUSE�ԐF�ݒ�
	float col[] = { color.R(), color.G(), color.B(), color.A() };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, col);
	glColor4fv(col);

	const int cut = 32;
	CVector* vertex = new CVector[cut * cut * 4];
	float* s = new float[cut + 1];
	float* c = new float[cut + 1];
	for (int i = 0; i <= cut; ++i)
	{
		s[i] = sinf((float)M_PI * 2 * i / cut);
		c[i] = cosf((float)M_PI * 2 * i / cut);
	}

	int idx = 0;
	for (int i = 0; i < cut; ++i)
	{
		const float& is1 = s[i];
		const float& is2 = s[i + 1];
		const float& ic1 = c[i];
		const float& ic2 = c[i + 1];
		for (int j = 0; j < cut; ++j)
		{
			const float& js1 = s[j];
			const float& js2 = s[j + 1];
			const float& jc1 = c[j];
			const float& jc2 = c[j + 1];

			vertex[idx] = CVector(js1 * is1, ic1, jc1 * is1);
			vertex[idx + 1] = CVector(js2 * is1, ic1, jc2 * is1);
			vertex[idx + 2] = CVector(js2 * is2, ic2, jc2 * is2);
			vertex[idx + 3] = CVector(js1 * is2, ic2, jc1 * is2);
			idx += 4;
		}
	}

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, vertex);
	glDrawArrays(GL_QUADS, 0, cut * cut * 4);
	glDisableClientState(GL_VERTEX_ARRAY);

	delete[] vertex;
	delete[] s;
	delete[] c;

	// ���C�g�I��
	glEnable(GL_LIGHTING);
	// �u�����h�����𖳌���
	Blend::DisableBlend();
	// �A���t�@�u�����h����
	glDisable(GL_BLEND);

	// �`��O�̍s��ɖ߂�
	glPopMatrix();
}

// ���C���[�t���[���̋���`��
void Primitive::DrawWireSphere(const CMatrix& m, float rad,
	const CColor& color, EBlend blend)
{
	glPolygonMode(GL_FRONT, GL_LINE);
	DrawSphere(m, rad, color, blend);
	glPolygonMode(GL_FRONT, GL_FILL);
}

// �O�p�`��`��
void Primitive::DrawTriangle(const CVector& v0, const CVector& v1, const CVector& v2,
	const CColor& color, EBlend blend)
{
	// ���݂̍s���ޔ����Ă���
	glPushMatrix();

	// �u�����h������L����
	Blend::EnableBlend(blend);
	// ���C�g�I�t
	glDisable(GL_LIGHTING);

	// DIFFUSE�ԐF�ݒ�
	float c[] = { color.R(), color.G(), color.B(), color.A() };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, c);
	glColor4fv(c);

	// �O�p�`��`��
	glBegin(GL_TRIANGLES);
	glVertex3f(v0.X(), v0.Y(), v0.Z());
	glVertex3f(v1.X(), v1.Y(), v1.Z());
	glVertex3f(v2.X(), v2.Y(), v2.Z());
	glEnd();

	// ���C�g�I��
	glEnable(GL_LIGHTING);
	// �u�����h�����𖳌���
	Blend::DisableBlend();

	// �`��O�̍s��ɖ߂�
	glPopMatrix();
}

// �|���S����`��
void Primitive::DrawQuad(const CMatrix& m, const CVector2& size,
	const CColor& color, EBlend blend)
{
	// �|���S����4���_
	static const CVector v[4] =
	{
		{ -0.5f, 0.0f, -0.5f },
		{ -0.5f, 0.0f,  0.5f },
		{  0.5f, 0.0f,  0.5f },
		{  0.5f, 0.0f, -0.5f },
	};

	// ���݂̍s���ޔ����Ă���
	glPushMatrix();

	// �`��s�񔽉f
	glMultMatrixf(m.M());

	// �u�����h������L����
	Blend::EnableBlend(blend);
	// ���C�g�I�t
	glDisable(GL_LIGHTING);

	// DIFFUSE�ԐF�ݒ�
	float c[] = { color.R(), color.G(), color.B(), color.A() };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, c);
	glColor4fv(c);

	// �|���S����`��
	glBegin(GL_QUADS);
	glVertex3f(v[0].X() * size.X(), v[0].Y(), v[0].Z() * size.Y());
	glVertex3f(v[1].X() * size.X(), v[1].Y(), v[1].Z() * size.Y());
	glVertex3f(v[2].X() * size.X(), v[2].Y(), v[2].Z() * size.Y());
	glVertex3f(v[3].X() * size.X(), v[3].Y(), v[3].Z() * size.Y());
	glEnd();

	// ���C�g�I��
	glEnable(GL_LIGHTING);
	// �A���t�@�u�����h����
	glDisable(GL_BLEND);
	// �u�����h�����𖳌���
	Blend::DisableBlend();

	// �`��O�̍s��ɖ߂�
	glPopMatrix();
}

// �J�v�Z����`��
void Primitive::DrawCapsule(const CVector& sp, const CVector& ep, float rad,
	const CColor& color, EBlend blend)
{
	// ���݂̍s���ޔ����Ă���
	glPushMatrix();

	float height = CVector::Distance(sp, ep);

	// �u�����h������L����
	Blend::EnableBlend(blend);
	// ���C�g�I�t
	glDisable(GL_LIGHTING);

	// DIFFUSE�ԐF�ݒ�
	float col[] = { color.R(), color.G(), color.B(), color.A() };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, col);
	glColor4fv(col);

	const int cut = 32;
	int vtxCnt = cut * cut * 4 + cut * 8;
	CVector* vertex = new CVector[vtxCnt];
	float* s = new float[cut + 1];
	float* c = new float[cut + 1];
	for (int i = 0; i <= cut; ++i)
	{
		s[i] = sinf((float)M_PI * 2 * i / cut);
		c[i] = cosf((float)M_PI * 2 * i / cut);
	}

	CVector up = (ep - sp).Normalized();
	CVector sd = CVector::Cross(up, CVector::up).Normalized();
	if (sd.LengthSqr() == 0.0f)
	{
		sd = CVector::Cross(up, CVector::forward).Normalized();
	}
	CVector fw = CVector::Cross(sd, up).Normalized();

	int idx = 0;
	// �n�_�ƏI�_
	for (int i = 0; i < 2; ++i)
	{
		CVector offset = i == 0 ? sp : ep;
		for (int j = 0; j < cut / 2; ++j)
		{
			float sj0 = s[j + 0] * (i == 0 ? -1.0f : 1.0f);
			float sj1 = s[j + 1] * (i == 0 ? -1.0f : 1.0f);
			float cj0 = c[j + 0];
			float cj1 = c[j + 1];
			for (int k = 0; k < cut; ++k)
			{
				float sk0 = s[k + 0];
				float sk1 = s[k + 1];
				float ck0 = c[k + 0];
				float ck1 = c[k + 1];
				vertex[idx + 0] = offset + (sd * sk0 * cj0 + up * sj0 + fw * ck0 * cj0) * rad;
				vertex[idx + 1] = offset + (sd * sk1 * cj0 + up * sj0 + fw * ck1 * cj0) * rad;
				vertex[idx + 2] = offset + (sd * sk1 * cj1 + up * sj1 + fw * ck1 * cj1) * rad;
				vertex[idx + 3] = offset + (sd * sk0 * cj1 + up * sj1 + fw * ck0 * cj1) * rad;
				idx += 4;
			}
		}
	}

	// �~��
	for (int i = 0; i < cut; ++i)
	{
		float si0 = s[i + 0];
		float si1 = s[i + 1];
		float ci0 = c[i + 0];
		float ci1 = c[i + 1];
		vertex[idx + 0] = sp + (sd * si0 + fw * ci0) * rad;
		vertex[idx + 1] = sp + (sd * si1 + fw * ci1) * rad;
		vertex[idx + 2] = ep + (sd * si1 + fw * ci1) * rad;
		vertex[idx + 3] = ep + (sd * si0 + fw * ci0) * rad;

		vertex[idx + 4] = ep + (sd * si0 + fw * ci0) * rad;
		vertex[idx + 5] = ep + (sd * si1 + fw * ci1) * rad;
		vertex[idx + 6] = sp + (sd * si1 + fw * ci1) * rad;
		vertex[idx + 7] = sp + (sd * si0 + fw * ci0) * rad;

		idx += 8;
	}

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, vertex);
	glDrawArrays(GL_QUADS, 0, vtxCnt);
	glDisableClientState(GL_VERTEX_ARRAY);

	delete[] vertex;
	delete[] s;
	delete[] c;

	// ���C�g�I��
	glEnable(GL_LIGHTING);
	// �u�����h�����𖳌���
	Blend::DisableBlend();

	// �`��O�̍s��ɖ߂�
	glPopMatrix();
}

// ���C���[�t���[���̃J�v�Z����`��
void Primitive::DrawWireCapsule(const CVector& sp, const CVector& ep, float rad,
	const CColor& color, EBlend blend)
{
	glPolygonMode(GL_FRONT, GL_LINE);
	DrawCapsule(sp, ep, rad, color, blend);
	glPolygonMode(GL_FRONT, GL_FILL);
}

// ��`��`��
void Primitive::DrawSector(const CMatrix& m, float startAngle, float endAngle,
	float rad, const CColor& color, EBlend blend)
{
	// ���݂̍s���ޔ����Ă���
	glPushMatrix();

	CMatrix sm;
	sm.Scale(rad, rad, rad);
	// �`��s��𔽉f
	glMultMatrixf((sm * m).M());

	// �u�����h�����L����
	Blend::EnableBlend(blend);
	// ���C�g�I�t
	glDisable(GL_LIGHTING);

	// DIFFUSE�F�ݒ�
	float col[] = { color.R(), color.G(), color.B(), color.A() };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, col);
	glColor4fv(col);

	float start = Math::DegreeToRadian(startAngle);
	float end = Math::DegreeToRadian(endAngle);

	const int cut = 45;
	CVector* vertex = new CVector[cut * 3];
	float* s = new float[cut + 1];
	float* c = new float[cut + 1];
	for (int i = 0; i <= cut; ++i)
	{
		s[i] = sinf(Math::Lerp(start, end, (float)i / cut));
		c[i] = cosf(Math::Lerp(start, end, (float)i / cut));
	}

	int idx = 0;
	for (int i = 0; i < cut; ++i)
	{
		const float& is1 = s[i];
		const float& is2 = s[i + 1];
		const float& ic1 = c[i];
		const float& ic2 = c[i + 1];
		vertex[idx] = CVector::zero;
		vertex[idx + 1] = CVector(is1, 0.0f, ic1);
		vertex[idx + 2] = CVector(is2, 0.0f, ic2);
		idx += 3;
	}

	// ��`��`��
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, vertex);
	glDrawArrays(GL_TRIANGLES, 0, cut * 3);
	glDisableClientState(GL_VERTEX_ARRAY);

	// ���C�g�I��
	glEnable(GL_LIGHTING);
	// �u�����h����������
	Blend::DisableBlend();

	// �`��O�̍s��ɖ߂�
	glPopMatrix();

	delete[] vertex;
	delete[] s;
	delete[] c;
}

// �{�b�N�X�̒��_�f�[�^
CVector boxVtx[8] =
{
	CVector(0.5f, -0.5f, -0.5f),
	CVector(0.5f, -0.5f,  0.5f),
	CVector(-0.5f, -0.5f,  0.5f),
	CVector(-0.5f, -0.5f, -0.5f),

	CVector(-0.5f,  0.5f, -0.5f),
	CVector(-0.5f,  0.5f,  0.5f),
	CVector(0.5f,  0.5f,  0.5f),
	CVector(0.5f,  0.5f, -0.5f),
};

// �{�b�N�X�`��p�̃C���f�b�N�X�f�[�^
int boxIdx[24] =
{
	0, 1, 2, 3,
	4, 5, 6, 7,
	7, 6, 1, 0,
	5, 4, 3, 2,
	0, 3, 4, 7,
	6, 5, 2, 1,
};

// ���C���[�{�b�N�X�`��p�̃C���f�b�N�X�f�[�^
int boxWireIdx[24] =
{
	0, 1, 1, 2, 2, 3, 3, 0,
	4, 5, 5, 6, 6, 7, 7, 4,
	0, 7, 1, 6, 2, 5, 3, 4,
};


// �{�b�N�X��`��
void Primitive::DrawBox(const CVector& center, const CVector& size, const CColor& color, EBlend blend)
{
	// ���݂̍s���ޔ����Ă���
	glPushMatrix();

	CMatrix tm;
	tm.Translate(center);
	CMatrix sm;
	sm.Scale(size);
	// �`��s��𔽉f
	glMultMatrixf((sm * tm).M());

	// �u�����h�����L����
	Blend::EnableBlend(blend);
	// ���C�g�I�t
	glDisable(GL_LIGHTING);

	// DIFFUSE�F�ݒ�
	float col[] = { color.R(), color.G(), color.B(), color.A() };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, col);
	glColor4fv(col);

	// ��`��`��
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_INDEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, boxVtx);
	glDrawElements(GL_QUADS, 24, GL_UNSIGNED_INT, boxIdx);
	glDisableClientState(GL_INDEX_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	// ���C�g�I��
	glEnable(GL_LIGHTING);
	// �u�����h����������
	Blend::DisableBlend();

	// �`��O�̍s��ɖ߂�
	glPopMatrix();
}

// ���C���[�t���[���̃{�b�N�X��`��
void Primitive::DrawWireBox(const CVector& center, const CVector& size, const CColor& color, EBlend blend)
{
	// ���݂̍s���ޔ����Ă���
	glPushMatrix();

	CMatrix tm;
	tm.Translate(center);
	CMatrix sm;
	sm.Scale(size);
	// �`��s��𔽉f
	glMultMatrixf((sm * tm).M());

	// �u�����h�����L����
	Blend::EnableBlend(blend);
	// ���C�g�I�t
	glDisable(GL_LIGHTING);

	// DIFFUSE�F�ݒ�
	float col[] = { color.R(), color.G(), color.B(), color.A() };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, col);
	glColor4fv(col);

	// ��`��`��
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_INDEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, boxVtx);
	glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, boxWireIdx);
	glDisableClientState(GL_INDEX_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	// ���C�g�I��
	glEnable(GL_LIGHTING);
	// �u�����h����������
	Blend::DisableBlend();

	// �`��O�̍s��ɖ߂�
	glPopMatrix();
}
