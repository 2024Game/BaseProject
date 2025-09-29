#pragma once
#include "CColBase.h"
#include "CColliderSphere.h"

// ���R���C�_�[�\���p�̃e�X�g�N���X
class CColSphere : public CColBase
{
public:
	// �R���X�g���N�^
	CColSphere(float rad, const CColor& defaultColor,
		bool isKinematic = false, float weight = 1.0f);
	// �f�X�g���N�^
	~CColSphere();

	/// <summary>
	/// �Փˏ���
	/// </summary>
	/// <param name="self">�Փ˂������g�̃R���C�_�[</param>
	/// <param name="other">�Փ˂�������̃R���C�_�[</param>
	/// <param name="hit">�Փ˂������̏��</param>
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

	// �`��
	void Render() override;

private:
	// ���R���C�_�[
	CColliderSphere* mpCollider;
	float mRadius;			// ���̔��a
};
