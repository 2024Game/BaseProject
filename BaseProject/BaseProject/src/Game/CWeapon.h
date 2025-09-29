#pragma once
#include "CObjectBase.h"

// ����̃x�[�X�N���X
class CWeapon : public CObjectBase
{
public:
	// �R���X�g���N�^
	CWeapon(ETag tag);
	// �f�X�g���N�^
	virtual ~CWeapon();

	// ����̎������ݒ�
	void SetOwner(CObjectBase* owner);
	// ����̎�������擾
	CObjectBase* GetOwner() const;

	// ������A�^�b�`����s���ݒ�
	void SetAttachMtx(const CMatrix* mtx);

	// �s����擾
	CMatrix Matrix() const override;

	/// <summary>
	/// �Փˏ���
	/// </summary>
	/// <param name="self">�Փ˂������g�̃R���C�_�[</param>
	/// <param name="other">�Փ˂�������̃R���C�_�[</param>
	/// <param name="hit">�Փ˂������̏��</param>
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

	// �R���C�_�[���擾
	CCollider* Collider() const;

	// �s����X�V
	void UpdateMtx();

protected:
	CObjectBase* mpOwner;		// ����̎�����
	CCollider* mpCollider;		// ����̃R���C�_�[
	const CMatrix* mpAttachMtx;	// ��������s��̃|�C���^�[
	CMatrix mAttachMtx;			// ��������s��̖{��
};
