#pragma once
#include "CObjectBase.h"

// �G�t�F�N�g�̃x�[�X�N���X
class CEffect : public CObjectBase
{
public:
	// �R���X�g���N�^
	CEffect(ETag tag);
	// �f�X�g���N�^
	virtual ~CEffect();

	// �X�V
	void Update() override;
	// �`��
	void Render() override;
private:
};
