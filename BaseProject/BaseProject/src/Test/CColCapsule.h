#pragma once
#include "CColBase.h"
#include "CColliderCapsule.h"

// �J�v�Z���R���C�_�[�\���p�̃e�X�g�N���X
class CColCapsule : public CColBase
{
public:
	// �R���X�g���N�^
	CColCapsule(const CVector& start, const CVector& end, float radius,
		const CColor& defaultColor, bool isKinematic = false, float weight = 1.0f);
	// �f�X�g���N�^
	~CColCapsule();

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
	// �J�v�Z���R���C�_�[
	CColliderCapsule* mpCollider;
	CVector mStartPos;	// �J�v�Z�����\����������̎n�_
	CVector mEndPos;	// �J�v�Z�����\����������̏I�_
	float mRadius;		// �J�v�Z���̔��a
};
