#pragma once
#include "CObjectBase.h"

// �J����
class CCamera : public CObjectBase
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="eye">�J�����̎��_</param>
	/// <param name="center">�J�����̒����_</param>
	/// <param name="isMainCamera">���C���J�������ǂ���</param>
	CCamera(const CVector& eye, const CVector& center, bool isMainCamera = true);
	// �f�X�g���N�^
	virtual ~CCamera();

	// ���C���J�������擾
	static CCamera* MainCamera();
	// ���݂̃J�������擾
	static CCamera* CurrentCamera();

	/// <summary>
	/// �J�����̏�Ԃ��R�s�[
	/// </summary>
	/// <param name="copy">�R�s�[���̃J����</param>
	void CopyCamera(CCamera* copy);

	/// <summary>
	/// �Ǐ]����^�[�Q�b�g��ݒ�
	/// </summary>
	/// <param name="target">�Ǐ]����^�[�Q�b�g</param>
	virtual void SetFollowTargetTf(CTransform* target);

	/// <summary>
	/// �Ǐ]���̃I�t�Z�b�g���W��ݒ�
	/// </summary>
	/// <param name="offset"></param>
	void SetFollowTargetOffset(const CVector& offset);

	/// <summary>
	/// ��������ʒu��ݒ�i�����_�̂݁j
	/// </summary>
	/// <param name="pos">��������ʒu</param>
	void LookAt(const CVector& pos);
	/// <summary>
	/// ��������ʒu��ݒ�i�����_ + ��x�N�g���j
	/// </summary>
	/// <param name="pos">��������ʒu</param>
	/// <param name="up">��x�N�g��</param>
	void LookAt(const CVector& pos, const CVector& up);
	/// <summary>
	/// ��������ʒu��ݒ�i���_ + �����_ + ��x�N�g���j
	/// </summary>
	/// <param name="eye">�J�����̈ʒu</param>
	/// <param name="at">��������ʒu</param>
	/// <param name="up">��x�N�g��</param>
	/// <param name="updateTargetEye">���_�̖ڕW�ʒu���X�V���邩�ǂ���</param>
	virtual void LookAt(const CVector& eye, const CVector& at, const CVector& up, bool updateTargetEye = true);

	/// <summary>
	/// ��ʃT�C�Y�ύX���̃R�[���o�b�N�֐�
	/// </summary>
	/// <param name="width">��ʂ̕�</param>
	/// <param name="height">��ʂ̍���</param>
	void Reshape(int width, int height);
	/// <summary>
	/// ��ʂ̕`��G���A���w��
	/// </summary>
	/// <param name="x">���̕`��J�n�ʒu</param>
	/// <param name="y">�c�̕`��J�n�ʒu</param>
	/// <param name="w">����</param>
	/// <param name="h">�c��</param>
	void Viewport(float x, float y, float w, float h);
	/// <summary>
	/// �������e�s���ݒ�
	/// </summary>
	/// <param name="fovy">��p</param>
	/// <param name="aspect">�A�X�y�N�g��</param>
	/// <param name="zNear">�`��ŒZ����</param>
	/// <param name="zFar">�`��Œ�����</param>
	void Perspective(float fovy, float aspect, float zNear, float zFar);

	/// <summary>
	/// �X�N���[�����W���烏�[���h���W�֕ϊ�
	/// </summary>
	/// <param name="screenPos">�X�N���[�����W</param>
	/// <returns>���[���h���W</returns>
	CVector ScreenToWorldPos(const CVector& screenPos);
	/// <summary>
	/// ���[���h���W����X�N���[�����W�֕ϊ�
	/// </summary>
	/// <param name="worldPos">���[���h���W</param>
	/// <returns>�X�N���[�����W</returns>
	CVector WorldToScreenPos(const CVector& worldPos);

	/// <summary>
	/// ���̃J���������݂̃J�����ɐݒ肷�邩�ǂ���
	/// �i���������ꍇ�̓��C���J���������݂̃J�����ɐݒ肳���j
	/// </summary>
	/// <param name="isCurrent">true�Ȃ�΁A���݂̃J�����ɐݒ�</param>
	void SetCurrent(bool isCurrent);

	/// <summary>
	/// �r���[�s����擾
	/// </summary>
	/// <returns>�r���[�s��</returns>
	const CMatrix& GetViewMatrix() const;
	/// <summary>
	/// �v���W�F�N�V�����s����擾
	/// </summary>
	/// <returns>�v���W�F�N�V�����s��</returns>
	const CMatrix& GetProjectionMatrix() const;
	/// <summary>
	/// �r���[�|�[�g�s����擾
	/// </summary>
	/// <returns></returns>
	const CMatrix& GetViewportMatrix() const;

	/// <summary>
	/// �Փ˔�����s���R���C�_�[�����X�g�ɒǉ�
	/// </summary>
	/// <param name="col"></param>
	void AddCollider(CCollider* col);
	/// <summary>
	/// �Փ˔�����s���R���C�_�[�����X�g�����菜��
	/// </summary>
	/// <param name="col"></param>
	void RemoveCollider(CCollider* col);

	/// <summary>
	/// �R���C�_�[�Ƃ̏Փ˔��莞�ɉ����߂������̊���
	/// </summary>
	/// <param name="ratio"></param>
	void SetHitColliderRatio(float ratio);

	// ��X�V
	void LateUpdate() override;

	// �폜
	void Kill() override;

	// �J�������f
	void Apply();

	// 2D�p�̃J�����s��𔽉f
	static void Start2DCamera();
	// 3D�p�̃J�����s��֖߂�
	static void End2DCamera();

protected:
	// �ݒ肳��Ă���R���C�_�[�Ƃ̏Փˌ��ʂ𔽉f����
	void ApplyCollision();

	CVector mTargetEye;	// ���_�̖ڕW�ʒu
	CVector mEye;		// ���_�̎��ۂ̈ʒu
	CVector mAt;		// �����_
	CVector mUp;		// ��x�N�g��
	CVector mEyeVec;	// ���_���璍���_�܂ł̃x�N�g��

	CTransform* mFollowTargetTf;	// �Ǐ]����^�[�Q�b�g��CTransfrom
	CVector mFollowOffsetPos;		// �Ǐ]�^�[�Q�b�g�̈ʒu���王�_�܂ł̃I�t�Z�b�g�l

private:
	// �J�������폜
	void DeleteCamera();

	static std::list<CCamera*> spCameraList;// ��������Ă���J�����̃��X�g
	static CCamera* spMainCamera;	// ���C���J����
	static CCamera* spCurrentCamera;// ���݂̃J����

	bool mIsMainCamera;				// ���C���J�������ǂ���

	CMatrix mViewMatrix;			// �r���[�s��
	CMatrix mProjectionMatrix;		// �v���W�F�N�V�����s��
	CMatrix mViewportMatrix;		// �r���[�|�[�g�s��
	float mFovy;		// �J�����̉�p
	float mZNear;		// �`��ŒZ����
	float mZFar;		// �`��Œ�����

	// �J�������Փ˔�����s���R���C�_�[�̃��X�g
	std::list<CCollider*> mColliders;
	// �J�������Փˎ��ɉ����߂�����
	float mHitColRatio;
};