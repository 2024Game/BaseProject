#include "CEffect.h"

// �R���X�g���N�^
CEffect::CEffect(ETag tag)
	: CObjectBase(tag, ETaskPriority::eEffect, 0, ETaskPauseType::eGame)
{
}

// �f�X�g���N�^
CEffect::~CEffect()
{
}

// �X�V
void CEffect::Update()
{
}

// �`��
void CEffect::Render()
{
}
