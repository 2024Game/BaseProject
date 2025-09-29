#pragma once
#include "CCollider.h"

/// <summary>
/// �J�v�Z���R���C�_�[
/// </summary>
class CColliderCapsule : public CCollider
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="owner">�R���C�_�[�̎�����</param>
	/// <param name="layer">�Փ˔���p���C���[</param>
	/// <param name="v0">�J�v�Z�����\����������̎n�_</param>
	/// <param name="v1">�J�v�Z�����\����������̏I�_</param>
	/// <param name="radius">�J�v�Z���̔��a</param>
	/// <param name="isKinematic">true�Ȃ�΁A�Փˎ��ɉ����߂��̉e�����󂯂Ȃ�</param>
	/// <param name="weight">�R���C�_�[�̏d��</param>
	CColliderCapsule(CObjectBase* owner, ELayer layer,
		const CVector& v0, const CVector& v1, float radius,
		bool isKinematic = false, float weight = 1.0f);

	/// <summary>
	/// �J�v�Z���R���C�_�[�̐ݒ�
	/// </summary>
	/// <param name="owner">�R���C�_�[�̎�����</param>
	/// <param name="layer">�Փ˔���p���C���[</param>
	/// <param name="v0">�J�v�Z�����\����������̎n�_</param>
	/// <param name="v1">�J�v�Z�����\����������̏I�_</param>
	/// <param name="radius">�J�v�Z���̔��a</param>
	void Set(CObjectBase* owner, ELayer layer,
		const CVector& v0, const CVector& v1, float radius);

	/// <summary>
	/// �J�v�Z�����\����������̎��_�ƏI�_���擾
	/// </summary>
	/// <param name="v0">�n�_�i�[�p</param>
	/// <param name="v1">�I�_�i�[�p</param>
	void Get(CVector* v0, CVector* v1) const;

	// �J�v�Z���̔��a���擾
	float Radius() const;

#if _DEBUG
	// �R���C�_�[�`��
	void Render() override;
#endif

protected:
	// �R���C�_�[�̏����X�V
	void UpdateCol(bool isInit = false) override;

private:
	CVector mV[2];	// �J�v�Z�����\����������̒��_�z��
	CVector mWV[2];	// �J�v�Z�����\����������̒��_�̃��[���h���W
	float mRadius;	// �J�v�Z���̔��a
};