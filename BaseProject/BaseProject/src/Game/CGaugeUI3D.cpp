#include "CGaugeUI3D.h"
#include "CCamera.h"
#include <glut.h>
#include "CImage3D.h"
#include "Maths.h"

#define WORLD_UNIT_PER_PIXEL 32.0f

// �R���X�g���N�^
CGaugeUI3D::CGaugeUI3D(CObjectBase* owner)
	: CObjectBase(ETag::eUI, ETaskPriority::eUI3D, 0, ETaskPauseType::eGame)
	, mpOwner(owner)
	, mpGaugeImg(nullptr)
	, mpWhiteImg(nullptr)
	, mMaxPoint(100)
	, mCurrPoint(mMaxPoint)
	, mPercent(1.0f)
{
	// �Q�[�W�̃C���[�W��ǂݍ���
	mpGaugeImg = new CImage3D
	(
		"UI\\gauge.png",
		ETag::eUI,
		ETaskPriority::eUI3D, 0,
		ETaskPauseType::eGame,
		false, false
	);
	mpGaugeImg->SetWorldUnitPerPixel(WORLD_UNIT_PER_PIXEL);
	mpGaugeImg->SetDepthMask(true);
	mGaugeSize = mpGaugeImg->GetSize();

	// ���C���[�W��ǂݍ���
	mpWhiteImg = new CImage3D
	(
		"UI\\white.png",
		ETag::eUI,
		ETaskPriority::eUI3D, 0,
		ETaskPauseType::eGame,
		false, false
	);
	mpWhiteImg->SetWorldUnitPerPixel(WORLD_UNIT_PER_PIXEL);
}

// �f�X�g���N�^
CGaugeUI3D::~CGaugeUI3D()
{
	// �ǂݍ��񂾃C���[�W���폜
	SAFE_DELETE(mpGaugeImg);
	SAFE_DELETE(mpWhiteImg);

	// ������ɍ폜���ꂽ���Ƃ�`����
	if (mpOwner != nullptr)
	{
		mpOwner->DeleteObject(this);
	}
}

// �������ݒ�
void CGaugeUI3D::SetOwner(CObjectBase* owner)
{
	mpOwner = owner;
}

// �|�C���g�c�ʂ𔽉f
void CGaugeUI3D::ApplyPoint()
{
	// �ő�l���s���l�łȂ����
	if (mMaxPoint > 0)
	{
		// ���ݒl���ő�l�̉��p�[�Z���g�����߂�
		mPercent = Math::Clamp01((float)mCurrPoint / mMaxPoint);
	}
	// �s���l�Ȃ�΁A100%�Œ�
	else
	{
		mPercent = 1.0f;
	}
}

// �ő�l��ݒ�
void CGaugeUI3D::SetMaxPoint(int point)
{
	mMaxPoint = point;
	ApplyPoint();
}

// ���ݒl��ݒ�
void CGaugeUI3D::SetCurrPoint(int point)
{
	mCurrPoint = point;
	ApplyPoint();
}

// �|�C���g�c�ʂ̊�����ݒ�i0.0�`1.0�j
void CGaugeUI3D::SetPercent(float per)
{
	mPercent = Math::Clamp01(per);
}

// �X�V
void CGaugeUI3D::Update()
{
	mpGaugeImg->Update();
	mpWhiteImg->Update();
}

// �`��
void CGaugeUI3D::Render()
{
	// �s��̕ۑ�
	glPushMatrix();

	// ��ɃJ�����̕�������������
	CCamera* cam = CCamera::CurrentCamera();
	CMatrix m = cam->GetViewMatrix().Inverse();
	m.Position(CVector::zero);
	glMultMatrixf((m * Matrix()).M());

	// �Q�[�W�w�i��`��
	mpWhiteImg->SetOffsetPos(CVector2::zero);
	mpWhiteImg->SetSize(mGaugeSize);
	mpWhiteImg->SetColor(CColor::black);
	mpWhiteImg->Render();

	// �o�[�̃T�C�Y�A���W�A�F��
	// �|�C���g�c�ʂ̊����ɍ��킹�Ē������āA�o�[��`��
	{
		// �o�[�̃T�C�Y�𒲐�
		CVector2 barSize = mGaugeSize;
		barSize.X(barSize.X() * mPercent);
		mpWhiteImg->SetSize(barSize);
		// �o�[�̍��W�𒲐�
		CVector2 barPos = mGaugeSize - barSize;
		mpWhiteImg->SetOffsetPos(-barPos);
		// �o�[�̐F��ݒ�
		CColor barColor = CColor::green;
		if (mPercent <= 0.2f) barColor = CColor::red;
		else if (mPercent <= 0.5f)barColor = CColor::yellow;
		mpWhiteImg->SetColor(barColor);
		// �o�[��`��
		mpWhiteImg->Render();
	}

	// �Q�[�W�{�̂�`��
	mpGaugeImg->Render();

	// �s���߂�
	glPopMatrix();
}
