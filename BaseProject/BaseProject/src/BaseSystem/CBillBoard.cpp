#include "CBillBoard.h"
//#include "CApplication.h"
#include "CCamera.h"

CBillBoard::CBillBoard(ETag tag, ETaskPauseType pause)
	: CObjectBase(tag, ETaskPriority::eBillboard, 0, pause)
{
}

void CBillBoard::SetSize(const CVector2& size)
{
	//�O�p�`�̒��_���W�ݒ�
	mT[0].Vertex
	(
		CVector(size.X(), size.Y(), 0.0f),
		CVector(-size.X(), -size.Y(), 0.0f),
		CVector(size.X(), -size.Y(), 0.0f)
	);
	mT[1].Vertex
	(
		CVector(-size.X(), size.Y(), 0.0f),
		CVector(-size.X(), -size.Y(), 0.0f),
		CVector(size.X(), size.Y(), 0.0f)
	);
	//�@����Z������
	mT[0].Normal(CVector(0.0f, 0.0f, 1.0f));
	mT[1].Normal(CVector(0.0f, 0.0f, 1.0f));
}

void CBillBoard::Update()
{
}

void CBillBoard::Render()
{
	Render(&mMaterial);
}

//Render(�}�e���A���̃|�C���^)
void CBillBoard::Render(CMaterial* mpMaterial)
{
	//�s��̕ۑ�
	glPushMatrix();

	//��ɃJ�����̕�������������
	CCamera* cam = CCamera::CurrentCamera();
	CMatrix m = cam->GetViewMatrix().Inverse();
	m.Position(CVector::zero);
	glMultMatrixf((m * Matrix()).M());

	//�f�v�X�l�̏������݂��I�t
	glDepthMask(false);
	//���C�g�I�t
	glDisable(GL_LIGHTING);
	//�}�e���A���K�p
	mpMaterial->Enabled(mColor);
	//�O�p�`�̕`��
	mT[0].Render();
	mT[1].Render();
	//�}�e���A������
	mpMaterial->Disabled();
	//���C�g�I��
	glEnable(GL_LIGHTING);
	//�f�v�X�l�̏������݂��I���ɖ߂�
	glDepthMask(true);

	//�s���߂�
	glPopMatrix();
}
