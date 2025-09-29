#include "System.h"
#include "CColor.h"
#include "CVector.h"
#include <glut.h>

// �Q�[���̏I���t���O
bool gIsExitGame = false;
// ���[�V�����u���[���L�����ǂ���
bool gIsEnableMotionBlur = false;
// ���[�V�����u���[�̕���
CVector gMotionBlurDir = CVector(0.0f, 0.0f, 1.0f);
// ���[�V�����u���[�̕�
float gMotionBlurWidth = 1.0f;
// ���[�V�����u���[�̔�����
int gMotionBlurCount = 2;

// �Q�[�����I��
void System::ExitGame()
{
	gIsExitGame = true;
}

// �Q�[�����I�����邩�ǂ�����Ԃ�
bool System::IsExitGame()
{
	return gIsExitGame;
}

// ��ʂ̃N���A�J���[��ݒ�irgba�j
void System::SetClearColor(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
}

// ��ʂ̃N���A�J���[��ݒ�iCColor�j
void System::SetClearColor(const CColor& color)
{
	SetClearColor(color.R(), color.G(), color.B(), color.A());
}

// ���[�V�����u���[�̗L�������ݒ�
void System::SetEnableMotionBlur(bool enable)
{
	gIsEnableMotionBlur = enable;
}

// ���[�V�����u���[���L�����ǂ���
bool System::IsEnableMotionBlur()
{
	return gIsEnableMotionBlur;
}

// ���[�V�����u���[�̃p�����[�^�ݒ�
void System::SetMotionBlur(const CVector& dir, float width, int count)
{
	gMotionBlurDir = dir;
	gMotionBlurWidth = width;
	gMotionBlurCount = count;
}

// ���[�V�����u���[�̃p�����[�^���擾
void System::GetMotionBlurParam(CVector* outDir, float* outWidth, int* outCount)
{
	*outDir = gMotionBlurDir;
	*outWidth = gMotionBlurWidth;
	*outCount = gMotionBlurCount;
}
