#include "CWeapon.h"

// �R���X�g���N�^
CWeapon::CWeapon(ETag tag)
	: CObjectBase(tag, ETaskPriority::eWeapon, 0, ETaskPauseType::eGame)
	, mpOwner(nullptr)
	, mpAttachMtx(nullptr)
	, mpCollider(nullptr)
{
	// �ŏ��̓R���C�_�[�I�t
	SetEnableCol(false);
}

// �f�X�g���N�^
CWeapon::~CWeapon()
{
	if (mpOwner != nullptr)
	{
		mpOwner->DeleteObject(this);
		mpOwner = nullptr;
	}

	// �R���C�_�[���폜
	SAFE_DELETE(mpCollider);
}

// ����̎������ݒ�
void CWeapon::SetOwner(CObjectBase* owner)
{
	mpOwner = owner;
}

// ����̎�������擾
CObjectBase* CWeapon::GetOwner() const
{
	return mpOwner;
}

// ������A�^�b�`����s���ݒ�
void CWeapon::SetAttachMtx(const CMatrix* mtx)
{
	mpAttachMtx = mtx;
}

// �s����擾
CMatrix CWeapon::Matrix() const
{
	CMatrix m = CTransform::Matrix();
	// ��������s�񂪐ݒ肳��Ă����
	if (mpAttachMtx != nullptr)
	{
		// ���̍s��ɂ�������
		m = m * mAttachMtx;
	}
	// �����傪�ݒ肳��Ă����
	else if (mpOwner != nullptr)
	{
		// ������̍s��ɕ���
		m = m * mpOwner->Matrix();
	}
	return m;
}

// �Փˏ���
void CWeapon::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	if (mpOwner != nullptr)
	{
		mpOwner->Collision(self, other, hit);
	}
}

// �R���C�_�[���擾
CCollider* CWeapon::Collider() const
{
	return mpCollider;
}

// �s����X�V
void CWeapon::UpdateMtx()
{
	// ��������s�񂪐ݒ肳��Ă���΁A
	if (mpAttachMtx != nullptr)
	{
		// ��������s��������o�ϐ��ɃR�s�[
		mAttachMtx = *mpAttachMtx;
	}
}
