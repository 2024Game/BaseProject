#pragma once
#include "CObjectBase.h"
#include "CColor.h"

// �R���C�_�[�\���N���X�̃x�[�X�N���X
class CColBase : public CObjectBase
{
public:
	// �R���X�g���N�^
	CColBase(const CColor& defaultColor);
	// �f�X�g���N�^
	virtual ~CColBase();

	/// <summary>
	/// �I����Ԃ�ݒ�
	/// </summary>
	/// <param name="isSelect">true�Ȃ�I��</param>
	void SetSelect(bool isSelect);

	/// <summary>
	/// �Փˏ���
	/// </summary>
	/// <param name="self">�Փ˂������g�̃R���C�_�[</param>
	/// <param name="other">�Փ˂�������̃R���C�_�[</param>
	/// <param name="hit">�Փ˂������̏��</param>
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

	// �X�V
	void Update() override;

protected:
	/// <summary>
	/// ���݂̐F���擾
	/// </summary>
	/// <returns></returns>
	CColor GetColor() const;

	bool mIsSelect;			// �I������Ă��邩�ǂ���

private:
	CColor mDefaultColor;	// �f�t�H���g�F
	bool mIsCollision;		// �����ƏՓ˂��Ă��邩�ǂ���
};
