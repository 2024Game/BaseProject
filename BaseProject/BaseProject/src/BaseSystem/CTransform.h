#ifndef CTRANSFORM_H
#define CTRANSFORM_H
#include "CVector.h"
#include "CQuaternion.h"
#include <list>

/*
�g�����X�t�H�[���N���X
�ʒu�A��]�A�g�k�A�ϊ��s��̊�{�N���X
*/
class CTransform
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="pos">�ʒu</param>
	/// <param name="rot">��]�l�i�N�H�[�^�j�I���j</param>
	/// <param name="scale">�X�P�[���l</param>
	CTransform(const CVector& pos = CVector::zero,
		const CQuaternion& rot = CQuaternion::identity,
		const CVector& scale = CVector::one);
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="pos">�ʒu</param>
	/// <param name="eulerAngles">��]�p�x �I�C���[�p�i�f�B�O���[�j</param>
	/// <param name="scale">�X�P�[���l</param>
	CTransform(const CVector& pos, const CVector& eulerAngles, const CVector& scale);

	// �f�X�g���N�^
	~CTransform();

	// ���[���h�ʒu���擾
	const CVector& Position() const;
	/// <summary>
	/// ���[���h�ʒu��ݒ�
	/// </summary>
	/// <param name="pos">���[���h�ʒu</param>
	virtual void Position(const CVector& pos);
	/// <summary>
	/// ���[���h�ʒu��ݒ�
	/// </summary>
	/// <param name="x">���[���h�ʒu X���W</param>
	/// <param name="y">���[���h�ʒu Y���W</param>
	/// <param name="z">���[���h�ʒu Z���W</param>
	void Position(float x, float y, float z);

	// ���[�J���ʒu���擾
	const CVector& LocalPosition() const;
	/// <summary>
	/// ���[�J���ʒu��ݒ�
	/// </summary>
	/// <param name="pos">���[�J���ʒu</param>
	virtual void LocalPosition(const CVector& pos);
	/// <summary>
	/// ���[�J���ʒu��ݒ�
	/// </summary>
	/// <param name="x">���[�J���ʒu X���W</param>
	/// <param name="y">���[�J���ʒu Y���W</param>
	/// <param name="z">���[�J���ʒu Z���W</param>
	void LocalPosition(float x, float y, float z);

	// ���[���h��]�l�i�N�H�[�^�j�I���j���擾
	const CQuaternion& Rotation() const;
	// ���[�J����]�l�i�N�H�[�^�j�I���j���擾
	const CQuaternion& LocalRotation() const;

	// ���[���h��]�p�x���擾�i�f�B�O���[�j
	CVector EulerAngles() const;
	// ���[�J����]�p�x���擾�i�f�B�O���[�j
	CVector LocalEulerAngles() const;

	/// <summary>
	/// ���[���h��]�l��ݒ�
	/// </summary>
	/// <param name="rot">���[���h��]�l�i�N�H�[�^�j�I���j</param>
	virtual void Rotation(const CQuaternion& rot);
	/// <summary>
	/// ���[���h��]�l��ݒ�
	/// </summary>
	/// <param name="x">�I�C���[�p X���i�f�B�O���[�j</param>
	/// <param name="y">�I�C���[�p Y���i�f�B�O���[�j</param>
	/// <param name="z">�I�C���[�p Z���i�f�B�O���[�j</param>
	void Rotation(float x, float y, float z);
	/// <summary>
	/// ���[���h��]�l��ݒ�
	/// </summary>
	/// <param name="eulerAngles">�I�C���[�p�i�f�B�O���[�j</param>
	void Rotation(const CVector& eulerAngles);

	/// <summary>
	/// ���[�J����]�l��ݒ�
	/// </summary>
	/// <param name="rot">���[�J����]�l�i�N�H�[�^�j�I���j</param>
	virtual void LocalRotation(const CQuaternion& rot);
	/// <summary>
	/// ���[�J����]�l��ݒ�
	/// </summary>
	/// <param name="x">�I�C���[�p X���i�f�B�O���[�j</param>
	/// <param name="y">�I�C���[�p Y���i�f�B�O���[�j</param>
	/// <param name="z">�I�C���[�p Z���i�f�B�O���[�j</param>
	void LocalRotation(float x, float y, float z);
	/// <summary>
	/// ���[�J����]�l��ݒ�
	/// </summary>
	/// <param name="eulerAngles">�I�C���[�p�i�f�B�O���[�j</param>
	void LocalRotation(const CVector& eulerAngles);


	// ���[���h�X�P�[���l���擾
	const CVector& Scale() const;
	// ���[�J���X�P�[���l���擾
	const CVector& LocalScale() const;

	/// <summary>
	/// ���[���h�X�P�[���l��ݒ�
	/// </summary>
	/// <param name="scale">���[���h�X�P�[���l</param>
	virtual void Scale(const CVector& scale);
	/// <summary>
	/// ���[���h�X�P�[���l��ݒ�
	/// </summary>
	/// <param name="x">���[���h�X�P�[���l X��</param>
	/// <param name="y">���[���h�X�P�[���l Y��</param>
	/// <param name="z">���[���h�X�P�[���l Z��</param>
	void Scale(float x, float y, float z);
	/// <summary>
	/// ���[�J���X�P�[���l��ݒ�
	/// </summary>
	/// <param name="scale">���[�J���X�P�[���l</param>
	virtual void LocalScale(const CVector& scale);
	/// <summary>
	/// ���[�J���X�P�[���l��ݒ�
	/// </summary>
	/// <param name="x">���[�J���X�P�[���l X��</param>
	/// <param name="y">���[�J���X�P�[���l Y��</param>
	/// <param name="z">���[�J���X�P�[���l Z��</param>
	void LocalScale(float x, float y, float z);

	// �ʒu�A��]�l�A�X�P�[���l�����킹���s����擾
	virtual CMatrix Matrix() const;

	// ���ʕ����̃x�N�g�����擾
	CVector VectorZ() const;
	// �������̃x�N�g�����擾
	CVector VectorX() const;
	// ������̃x�N�g�����擾
	CVector VectorY() const;

	/// <summary>
	/// ���s�ړ��i���݂̌����ɑ΂��Ă̈ړ��j
	/// </summary>
	/// <param name="translation">�ړ���</param>
	void Translate(const CVector& translation);
	/// <summary>
	/// ���s�ړ��i���݂̌����ɑ΂��Ă̈ړ��j
	/// </summary>
	/// <param name="x">X���̈ړ���</param>
	/// <param name="y">Y���̈ړ���</param>
	/// <param name="z">Z���̈ړ���</param>
	void Translate(float x, float y, float z);

	/// <summary>
	/// ��]
	/// </summary>
	/// <param name="eulerAngles">��]�p�x�i�f�B�O���[�j</param>
	void Rotate(const CVector& eulerAngles);
	/// <summary>
	/// ��]
	/// </summary>
	/// <param name="x">X���̉�]�p�x�i�f�B�O���[�j</param>
	/// <param name="y">Y���̉�]�p�x�i�f�B�O���[�j</param>
	/// <param name="z">Z���̉�]�p�x�i�f�B�O���[�j</param>
	void Rotate(float x, float y, float z);
	/// <summary>
	/// �C�ӎ���]
	/// </summary>
	/// <param name="axis">��]��</param>
	/// <param name="angle">��]�p�x�i�f�B�O���[�j</param>
	void RotateAxis(const CVector& axis, float angle);

	/// <summary>
	/// �^�[�Q�b�g�ʒu�֌�����
	/// </summary>
	/// <param name="target">�^�[�Q�b�g�ʒu</param>
	void LookAt(const CVector& target);

	/// <summary>
	/// �e��ݒ�
	/// </summary>
	/// <param name="parent">�ݒ肷��e��CTransform</param>
	void SetParent(CTransform* parent);
	/// <summary>
	/// �e���擾
	/// </summary>
	/// <returns>�ݒ肳��Ă���e</returns>
	CTransform* GetParent() const;

protected:
	/// <summary>
	/// �q�̃��X�g�ɒǉ�
	/// </summary>
	/// <param name="child">���X�g�ɒǉ�����q��CTransform</param>
	void AddChild(CTransform* child);
	/// <summary>
	/// �q�̃��X�g�����菜��
	/// </summary>
	/// <param name="child">���X�g�����菜���q��CTransform</param>
	void RemoveChild(CTransform* child);

	// �q�Ɏ��g�̕ύX�𔽉f
	void ApplyChildren();

	// �X�V
	void Update();

	CVector mPosition;			// ���[���h��Ԃł̈ʒu
	CQuaternion mRotation;		// ���[���h��Ԃł̉�]�l
	CVector mScale;				// ���[���h��Ԃł̃X�P�[���l

	CVector mLocalPosition;		// ���[�J����Ԃł̈ʒu
	CQuaternion mLocalRotation;	// ���[�J����Ԃł̉�]�l
	CVector mLocalScale;		// ���[�J����Ԃł̃X�P�[���l

	CTransform* mpParent;		// �e��CTransform
	std::list<CTransform*> mChildren;	// �q��CTransform�̃��X�g
};

#endif
