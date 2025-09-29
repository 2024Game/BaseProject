#pragma once
#include "CBillBoardImage.h"
#include "CColliderSphere.h"

// ���̃X�P�[���l�̍ő�l
#define FLAME_SCALE 10.0f
// ���̃X�P�[���l���ő�l�ɂȂ�܂ł̎���
#define FLAME_SCALE_ANIM_TIME 3.0f

// ���̃G�t�F�N�g
class CFlame : public CBillBoardImage
{
public:
	// �R���X�g���N�^
	CFlame(ETag tag);
	// �f�X�g���N�^
	~CFlame();

	/// <summary>
	/// �e�p�����[�^��ݒ�
	/// </summary>
	/// <param name="pos">�����ʒu</param>
	/// <param name="dir">�ړ�����</param>
	/// <param name="speed">�ړ����x</param>
	void Setup(const CVector& pos, const CVector& dir, float speed);

	// �J���[��ݒ�
	void SetColor(const CColor& color) override;
	// �u�����h�^�C�v��ݒ�
	void SetBlendType(EBlend type);
	// ���̃X�P�[���̍ő�l��ݒ�
	void SetFlameScale(float flameScale);
	// ���̃X�P�[���l���ő�l�ɂȂ�܂ł̎��Ԃ�ݒ�
	void SetFlameScaleAnimTime(float flameScaleAnimTime);

	// �폜�t���O�������Ă��邩�ǂ���
	bool IsDeath() const;

	/// <summary>
	/// �Փˏ���
	/// </summary>
	/// <param name="self">�Փ˂������g�̃R���C�_�[</param>
	/// <param name="other">�Փ˂�������̃R���C�_�[</param>
	/// <param name="hit">�Փ˂������̏��</param>
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

	// �X�V
	void Update() override;

private:
	// �A�j���[�V�����f�[�^
	static TexAnimData msAnimData;
	CVector mMoveSpeed;	// �ړ����x
	float mElapsedTime;	// �o�ߎ���
	bool mIsDeath;		// �폜�t���O
	CColliderSphere* mpCollider;
	// ���̃X�P�[���̍ő�l
	float mFlameScale;
	// ���̃X�P�[���l���ő�l�ɂȂ�܂ł̎���
	float mFlameScaleAnimTime;
};
