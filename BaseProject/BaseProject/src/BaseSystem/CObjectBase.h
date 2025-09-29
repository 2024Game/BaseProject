#pragma once
#include "CTask.h"
#include "CTransform.h"
#include "ObjectTag.h"
#include "CCollider.h"
#include "CColor.h"

/// <summary>
/// 3D��Ԃɔz�u����I�u�W�F�N�g�̃x�[�X�N���X
/// </summary>
class CObjectBase : public CTask, public CTransform
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="tag">�I�u�W�F�N�g�̃^�O</param>
	/// <param name="prio">�^�X�N�̗D��x</param>
	CObjectBase(ETag tag = ETag::eNone,
		ETaskPriority prio = ETaskPriority::eDefault,
		int sortOrder = 0,
		ETaskPauseType pause = ETaskPauseType::eDefault,
		bool dontDelete = false,
		bool addTaskList = true);

	// �f�X�g���N�^
	virtual ~CObjectBase();

	// �I�u�W�F�N�g�폜��`����֐�
	virtual void DeleteObject(CObjectBase* obj);

	// �I�u�W�F�N�g�^�O���擾
	ETag Tag() const;

	// �Փ˔�����s�����ݒ�
	void SetEnableCol(bool isEnable);
	// �Փ˔�����s�����ǂ���
	bool IsEnableCol() const;

	// �J���[��ݒ�
	virtual void SetColor(const CColor& color);
	// �J���[���擾
	const CColor& GetColor() const;
	// �A���t�@�l�ݒ�
	void SetAlpha(float alpha);
	// �A���t�@�l�擾
	float GetAlpha() const;

	// �J�����܂ł̋������v�Z
	virtual void CalcDepth();
	// �J��������̋������擾
	float GetDepth() const;

	/// <summary>
	/// �Փˏ���
	/// </summary>
	/// <param name="self">�Փ˂������g�̃R���C�_�[</param>
	/// <param name="other">�Փ˂�������̃R���C�_�[</param>
	/// <param name="hit">�Փ˂������̏��</param>
	virtual void Collision(CCollider* self, CCollider* other, const CHitInfo& hit);
	/// <summary>
	/// ���C�ƃI�u�W�F�N�g�̏Փ˔���
	/// </summary>
	/// <param name="start">���C�̊J�n�ʒu</param>
	/// <param name="end">���C�̏I���ʒu</param>
	/// <param name="hit">�Փˈʒu�ԋp�p</param>
	/// <returns></returns>
	virtual bool CollisionRay(const CVector& start, const CVector& end, CHitInfo* hit);

	// �U������
	virtual bool IsAttacking() const;
	/// <summary>
	/// �U���J�n
	/// </summary>
	virtual void AttackStart();
	/// <summary>
	/// �U���I��
	/// </summary>
	virtual void AttackEnd();

private:
	ETag mTag;			// �I�u�W�F�N�g���ʗp�̃^�O
	bool mIsEnableCol;	// �Փ˔�����s�����ǂ���

protected:
	/// <summary>
	/// �U�����q�b�g�����I�u�W�F�N�g��ǉ�
	/// </summary>
	/// <param name="obj"></param>
	void AddAttackHitObj(CObjectBase* obj);
	/// <summary>
	/// ���ɍU�����q�b�g���Ă���I�u�W�F�N�g���ǂ���
	/// </summary>
	/// <param name="obj"></param>
	/// <returns></returns>
	bool IsAttackHitObj(CObjectBase* obj) const;

	float mDepth;		// �J��������̋���
	CColor mColor;		// �I�u�W�F�N�g�̐F

	// �U�����q�b�g�����I�u�W�F�N�g�̃��X�g
	std::list<CObjectBase*> mAttackHitObjects;
};
