#pragma once
#include "CColBase.h"
#include "CColliderLine.h"

// �����R���C�_�[�\���p�̃e�X�g�N���X
class CColLine : public CColBase
{
public:
	// �R���X�g���N�^
	CColLine(const CVector& start, const CVector& end, const CColor& defaultColor,
		bool isKinematic = false, float weight = 1.0f);
	// �f�X�g���N�^
	~CColLine();

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
	// �����R���C�_�[
	CColliderLine* mpCollider;
	CVector mStartPos;	// �����̎n�_
	CVector mEndPos;	// �����̏I�_
};
