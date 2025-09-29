#pragma once
#include "CObjectBase.h"
#include "CModel.h"

// �T�{�e���̐j�̃N���X
class CCactusNeedle : public CObjectBase
{
public:
	// �R���X�g���N�^
	CCactusNeedle(float speed, float dist);
	// �f�X�g���N�^
	~CCactusNeedle();

	// �Փˏ���
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

	// �X�V
	void Update() override;
	// �`��
	void Render() override;

private:
	CModel* mpModel;		// �j�̃��f���f�[�^
	CCollider* mpCollider;	// �j�̃R���C�_�[
	float mMoveSpeed;		// �ړ����x
	float mMoveDist;		// �ړ��ł��鋗��
	float mCurrDist;		// ���݂̈ړ�����
};
