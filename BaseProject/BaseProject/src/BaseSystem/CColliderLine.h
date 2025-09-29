#pragma once
#include "CCollider.h"

/// <summary>
/// �����R���C�_�[
/// </summary>
class CColliderLine : public CCollider
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="owner">�R���C�_�[�̎�����</param>
	/// <param name="layer">�Փ˔���p���C���[</param>
	/// <param name="v0">�����̎n�_</param>
	/// <param name="v1">�����̏I�_</param>
	/// <param name="isKinematic">true�Ȃ�΁A�Փˎ��ɉ����߂��̉e�����󂯂Ȃ�</param>
	/// <param name="weight">�R���C�_�[�̏d��</param>
	CColliderLine(CObjectBase* owner, ELayer layer,
		const CVector& v0, const CVector& v1,
		bool isKinematic = false, float weight = 1.0f);

	/// <summary>
	/// �����R���C�_�[�̐ݒ�
	/// </summary>
	/// <param name="owner">�R���C�_�[�̎�����</param>
	/// <param name="layer">�Փ˔���p���C���[</param>
	/// <param name="v0">�����̎n�_</param>
	/// <param name="v1">�����̏I�_</param>
	void Set(CObjectBase* owner, ELayer layer,
		const CVector& v0, const CVector& v1);

	/// <summary>
	/// �����̎n�_�ƏI�_���擾
	/// </summary>
	/// <param name="v0">�n�_�i�[�p</param>
	/// <param name="v1">�I�_�i�[�p</param>
	void Get(CVector* v0, CVector* v1) const;

	float Radius() const;

#if _DEBUG
	// �R���C�_�[�`��
	void Render() override;
#endif

protected:
	// �R���C�_�[�̏����X�V
	void UpdateCol(bool isInit = false) override;

private:
	CVector mV[2];	// �����̒��_�̔z��
	CVector mWV[2];	// �����̒��_�̃��[���h���W
	float mRadius;
};
