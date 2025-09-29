#include "Blend.h"
#include "glut.h"

// �w�肵���u�����h������L����
void Blend::EnableBlend(EBlend blend)
{
	//�u�����h������L��
	glEnable(GL_BLEND);

	//�u�����h�^�C�v�ɂ���ď�����؂�ւ�
	switch (blend)
	{
		// �A���t�@�u�����h
		case EBlend::eAlpha:
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			break;
		// ���Z�u�����h
		case EBlend::eAdd:
			glBlendFunc(GL_ONE, GL_ONE);
			break;
		// ��Z�u�����h
		case EBlend::eMultiply:
			glBlendFunc(GL_ZERO, GL_SRC_COLOR);
			break;
		// �F���]
		case EBlend::eInvert:
			glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ZERO);
			break;
	}
}

// �u�����h�����𖳌���
void Blend::DisableBlend()
{
	glDisable(GL_BLEND);
}
