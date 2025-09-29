#pragma once
#include "CCollider.h"

/// <summary>
/// ���R���C�_�[
/// </summary>
class CColliderSphere : public CCollider
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="owner">�R���C�_�[�̎�����</param>
	/// <param name="layer">�Փ˔���p���C���[</param>
	/// <param name="radius">���̔��a</param>
	/// <param name="isKinematic">true�Ȃ�΁A�Փˎ��ɉ����߂��̉e�����󂯂Ȃ�</param>
	/// <param name="weight">�R���C�_�[�̏d��</param>
	CColliderSphere(CObjectBase* owner, ELayer layer, float radius,
		bool isKinematic = false, float weight = 1.0f);

	/// <summary>
	/// ���R���C�_�[�̐ݒ�
	/// </summary>
	/// <param name="owner">�R���C�_�[�̎�����</param>
	/// <param name="layer">�Փ˔���p���C���[</param>
	/// <param name="radius">���̔��a</param>
	void Set(CObjectBase* owner, ELayer layer, float radius);

	/// <summary>
	/// ���̍��W�Ɣ��a���擾
	/// </summary>
	/// <param name="pos">���̍��W</param>
	/// <param name="rad">���̔��a</param>
	void Get(CVector* pos, float* rad) const;

#if _DEBUG
	// �R���C�_�[�`��
	void Render() override;
#endif

protected:
	// �R���C�_�[�̏����X�V
	void UpdateCol(bool isInit = false) override;

private:
	float mRadius;	// ���̔��a
	CVector mWPos;	// ���[���h���W
	float mWRadius;	// ���[���h���a
};
