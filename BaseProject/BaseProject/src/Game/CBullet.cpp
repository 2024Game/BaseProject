#include "CBullet.h"
#include "CTrailEffect.h"
#include "Primitive.h"

// �R���X�g���N�^
CBullet::CBullet(const CVector& pos, const CVector& dir,
	float speed, float distance)
	: CObjectBase(ETag::eBullet, ETaskPriority::eDefault, 0, ETaskPauseType::eGame)
	, mMoveSpeed(speed)
	, mFlyingDistance(distance)
	, mCurrentFlyingDistance(0.0f)
{
	Position(pos);
	Rotation(CQuaternion::LookRotation(dir, CVector::up));

	// �O�Ղ̃G�t�F�N�g���쐬
	mpTrailEffect = new CTrailEffect
	(
		ETag::eBullet,	// �I�u�W�F�N�g�^�O
		this,
		nullptr,
		CVector(0.0f, 0.0f, 0.0f),
		0.01f,			// �X�V�Ԋu�i���ԁj
		30.0f,			// �X�V�Ԋu�i�����j
		2.0f,			// �J�n���̋O�Ղ̕�
		0.0f,			// �I�����̋O�Ղ̕�
		0.0625f			// �\������
	);
	mpTrailEffect->SetTexture("Laser");
	mpTrailEffect->SetColor(CColor(1.0f, 0.75f, 0.25f, 1.0f));
}

// �f�X�g���N�^
CBullet::~CBullet()
{
	mpTrailEffect->SetOwner(nullptr);
}

// �X�V
void CBullet::Update()
{
	if (IsKill()) return;

	// �c��򋗗���0�Ȃ�΁A�e�ۍ폜
	float remain = mFlyingDistance - mCurrentFlyingDistance;
	if (remain <= 0.0f)
	{
		Kill();
		return;
	}

	// �ړ����x���v�Z
	// �ړ����x���c��̔򋗗����傫���ꍇ�́A
	// �c��̔򋗗����ړ����x�Ƃ���
	float moveSpeed = mMoveSpeed * Times::DeltaTime();
	if (abs(moveSpeed) > remain)
	{
		moveSpeed = remain * (moveSpeed < 0.0f ? -1.0f : 1.0f);
	}

	// �e�ۂ𐳖ʕ����Ɉړ�
	Position(Position() + VectorZ() * moveSpeed);
	// ���݂̔򋗗����X�V
	mCurrentFlyingDistance += abs(moveSpeed);
}

// �`��
void CBullet::Render()
{
	Primitive::DrawSphere(Matrix(), 0.1f, mColor);
}
