#include "CCactusNeedle.h"
#include "CColliderSphere.h"
#include "CCharaBase.h"

// �R���X�g���N�^
CCactusNeedle::CCactusNeedle(float speed, float dist)
	: CObjectBase(ETag::eEnemy, ETaskPriority::eWeapon, 0, ETaskPauseType::eGame)
	, mpModel(nullptr)
	, mpCollider(nullptr)
	, mMoveSpeed(speed)
	, mMoveDist(dist)
	, mCurrDist(0.0f)
{
	// �j�����F�ɂ���
	mColor = CColor::yellow;

	// ���f���f�[�^�擾
	mpModel = CResourceManager::Get<CModel>("CactusNeedle");

	// �R���C�_�[���쐬
	mpCollider = new CColliderSphere
	(
		this, ELayer::eAttackCol,
		3.0f
	);
	// �v���C���[�ƃt�B�[���h�ƏՓ˂���悤�ɐݒ�
	mpCollider->SetCollisionTags({ ETag::ePlayer/*, ETag::eField*/ });
	mpCollider->SetCollisionLayers({ ELayer::ePlayer, ELayer::eField });
}

// �f�X�g���N�^
CCactusNeedle::~CCactusNeedle()
{
	// �R���C�_�[�폜
	SAFE_DELETE(mpCollider);
}

// �Փˏ���
void CCactusNeedle::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// �v���C���[�ɏՓ˂���
	if (other->Layer() == ELayer::ePlayer)
	{
		// �v���C���[�Ƀ_���[�W��^����
		CCharaBase* chara = dynamic_cast<CCharaBase*>(other->Owner());
		if (chara != nullptr)
		{
			chara->TakeDamage(1, this);
		}
	}

	// �����ɂԂ�������A���g���폜
	Kill();
}

// �X�V
void CCactusNeedle::Update()
{
	// ���̃t���[���ňړ����������߂�
	float moveDist = mMoveSpeed * Times::DeltaTime();

	// �ړ��������A�ړ�������
	CVector pos = Position();
	pos += VectorZ() * moveDist;
	Position(pos);

	// �ړ��������������Z
	mCurrDist += moveDist;
	// �ړ��o���鋗���𒴂�����A���g���폜
	if (mCurrDist >= mMoveDist)
	{
		Kill();
	}
}

// �`��
void CCactusNeedle::Render()
{
	mpModel->SetColor(mColor);
	mpModel->Render(Matrix());
}
