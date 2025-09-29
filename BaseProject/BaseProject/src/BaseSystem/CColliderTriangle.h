#pragma once
#include "CCollider.h"

/// <summary>
/// �O�p�`�R���C�_�[
/// </summary>
class CColliderTriangle : public CCollider
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="owner">�R���C�_�[�̎�����</param>
	/// <param name="layer">�Փ˔���p���C���[</param>
	/// <param name="v0">�O�p�`�̒��_ 0</param>
	/// <param name="v1">�O�p�`�̒��_ 1</param>
	/// <param name="v2">�O�p�`�̒��_ 2</param>
	/// <param name="isKinematic">true�Ȃ�΁A�Փˎ��ɉ����߂��̉e�����󂯂Ȃ�</param>
	/// <param name="weight">�R���C�_�[�̏d��</param>
	CColliderTriangle(CObjectBase* owner, ELayer layer,
		const CVector& v0, const CVector& v1, const CVector& v2,
		bool isKinematic = false, float weight = 1.0f);

	/// <summary>
	/// �O�p�`�R���C�_�[�̐ݒ�
	/// </summary>
	/// <param name="owner">�R���C�_�[�̎�����</param>
	/// <param name="layer">�Փ˔���p���C���[</param>
	/// <param name="v0">�O�p�`�̒��_ 0</param>
	/// <param name="v1">�O�p�`�̒��_ 1</param>
	/// <param name="v2">�O�p�`�̒��_ 2</param>
	void Set(CObjectBase* owner, ELayer layer,
		const CVector& v0, const CVector& v1, const CVector& v2);

	/// <summary>
	/// �O�p�`�̒��_���擾
	/// </summary>
	/// <param name="v0">���_1�i�[�p</param>
	/// <param name="v1">���_2�i�[�p</param>
	/// <param name="v2">���_3�i�[�p</param>
	void Get(CVector* v0, CVector* v1, CVector* v2) const;

#if _DEBUG
	// �R���C�_�[�`��
	void Render() override;
#endif

protected:
	// �R���C�_�[�̏����X�V
	void UpdateCol(bool isInit = false) override;

private:
	CVector mV[3];	// �O�p�`�̒��_�̔z��
	CVector mWV[3];	// �O�p�`�̒��_�̃��[���h���W
};
