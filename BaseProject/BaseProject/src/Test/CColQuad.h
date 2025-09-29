#pragma once
#include "CColBase.h"
#include "CColliderTriangle.h"

// �|���S���\���p�̃e�X�g�N���X
class CColQuad : public CColBase
{
public:
	// �R���X�g���N�^
	CColQuad(const CVector2& size, const CColor& defaultColor,
		bool isKinematic = false, float weight = 1.0f);
	// �f�X�g���N�^
	~CColQuad();

	// �`��
	void Render() override;

private:
	// �O�p�`�R���C�_�[0
	CColliderTriangle* mpCollider0;
	// �O�p�`�R���C�_�[1
	CColliderTriangle* mpCollider1;
	// �|���S���̃T�C�Y
	CVector2 mSize;
	// �|���S����4���_
	CVector mVertices[4];
};
