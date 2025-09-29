#include "CSword.h"
#include "CColliderCapsule.h"

#define SWORD_LENGTH 100.0f
#define SWORD_RADIUS 1.0f

// �R���X�g���N�^
CSword::CSword(CObjectBase* owner, ETag tag,
	Tags hitTags, Layers hitLayers)
	: CWeapon(tag)
	, mpModel(nullptr)
{
	mpOwner = owner;

	// ���f���f�[�^�擾
	mpModel = CResourceManager::Get<CModel>("Sword");

	// ���̃R���C�_�[���쐬
	mpCollider = new CColliderCapsule
	(
		this, ELayer::eAttackCol,
		CVector(0.0f, 0.0f, 0.0f),
		CVector(SWORD_LENGTH - SWORD_RADIUS, 0.0f, 0.0f),
		SWORD_RADIUS
	);
	// �^�O�ƃ��C���[�̏Փːݒ�
	mpCollider->SetCollisionTags(hitTags);
	mpCollider->SetCollisionLayers(hitLayers);
}

// �f�X�g���N�^
CSword::~CSword()
{
}

// �`��
void CSword::Render()
{
	mpModel->Render(Matrix());
}
