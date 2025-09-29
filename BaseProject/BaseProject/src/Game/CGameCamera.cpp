#include "CGameCamera.h"
#include "CInput.h"

// �J�����̉�]���x
#define ROTATE_SPEED 0.1f

// �R���X�g���N�^
CGameCamera::CGameCamera(const CVector& eye, const CVector& center, bool isMainCamera)
	: CCamera(eye, center, isMainCamera)
{
}

// �f�X�g���N�^
CGameCamera::~CGameCamera()
{
}

// ��X�V
void CGameCamera::LateUpdate()
{
	// �}�E�X�̉������̈ړ��ʂɍ��킹�āA��]�l�i�N�H�[�^�j�I���j�����߂�
	CVector2 delta = CInput::GetDeltaMousePos();
	CQuaternion rot = CQuaternion(0.0f, delta.X() * ROTATE_SPEED, 0.0f);

	// �Ǐ]���̃I�t�Z�b�g�ʒu����]
	mFollowOffsetPos = rot * mFollowOffsetPos;

	// �J�����̌�������]
	CVector vec = -mEyeVec.Normalized();
	CVector forward = rot * vec;
	Rotation(CQuaternion::LookRotation(forward));

	// �J�����̃x�[�X�̍X�V����
	CCamera::LateUpdate();
}
