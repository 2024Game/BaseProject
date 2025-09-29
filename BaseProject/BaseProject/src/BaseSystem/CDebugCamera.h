#pragma once
#include "CCamera.h"
#if _DEBUG

// �f�o�b�O�J����
class CDebugCamera : public CCamera
{
public:
	// �f�o�b�O�J�������擾
	static CDebugCamera* DebugCamera();

	// �f�o�b�O�J�������I���ɂȂ��Ă��邩�ǂ���
	static bool IsOn();
	/// <summary>
	/// �f�o�b�O�J�������I��
	/// </summary>
	/// <param name="disableInput">�ʏ�̓��͏����𖳌��ɂ��邩�ǂ���</param>
	static void On(bool disableInput = true);
	// �f�o�b�O�J�������I�t
	static void Off();

	// ��X�V
	void LateUpdate() override;

private:
	// �R���X�g���N�^
	CDebugCamera();
	// �f�X�g���N�^
	~CDebugCamera();

	// �f�o�b�O�J�����̃C���X�^���X
	static CDebugCamera* spDebugCamera;
	// �f�o�b�O�J�������I���ɂȂ��Ă��邩�ǂ���
	static bool msIsOn;
	// �J�����̈ړ����x
	CVector mMoveSpeed;
	// �J�����̈ړ����x�̔{��
	float mMoveSpeedRatio;
	// �J�����̉�]�p�x
	CVector mRotateAngle;
};

#endif
