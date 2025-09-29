#pragma once
#include "CObjectBase.h"

// ����֘A�̕\���N���X
class CMeasure : public CObjectBase
{
public:
	// �R���X�g���N�^
	CMeasure(int gridCount);
	// �f�X�g���N�^
	~CMeasure();

	// �\������O���b�h����ݒ�
	void SetGridCount(int count);

	// �X�V
	void Update() override;
	// �`��
	void Render() override;

private:
	int mGridCount;	// �\������O���b�h��
};
