#pragma once
#include "CCamera.h"

// �Q�[�����̃J����
class CGameCamera : public CCamera
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="eye">�J�����̎��_</param>
	/// <param name="center">�J�����̒����_</param>
	/// <param name="isMainCamera">���C���J�������ǂ���</param>
	CGameCamera(const CVector& eye, const CVector& center, bool isMainCamera = true);
	// �f�X�g���N�^
	~CGameCamera();

	// ��X�V
	void LateUpdate() override;
};