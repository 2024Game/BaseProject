#include "CDebugCamera.h"
#if _DEBUG
#include "CDebugInput.h"
#include "glut.h"
#include "Maths.h"

// �����x
#define MOVE_ADD_SPEED 0.0625f
// �ő呬�x
#define MOVE_MAX_SPEED 10.0f
// �ړ����x�̌�����
#define MOVE_ATTEN_RATIO 0.95f
// �ړ����x�̔{���̍ŏ��l
#define MOVE_SPEED_RATIO_MIN 0.1f
// �ړ����x�̔{���̍ő�l
#define MOVE_SPEED_RATIO_MAX 3.0f
// �ړ����x�̔{���̕ϊ���
#define MOVE_SPEED_RATIO_VALUE 0.05f
// ��]���x
#define ROTATE_SPEED 0.125f
// ���s�ړ����x
#define TRANSLATE_SPEED 0.125f

// �f�o�b�O�J�����̃C���X�^���X
CDebugCamera* CDebugCamera::spDebugCamera = nullptr;
// �f�o�b�O�J�������I���ɂȂ��Ă��邩�ǂ���
bool CDebugCamera::msIsOn = false;

// �R���X�g���N�^
CDebugCamera::CDebugCamera()
	: CCamera(CVector(0.0f, 0.0f, 10.0f), CVector(0.0f, 0.0f, 0.0f), false)
	, mMoveSpeed(0.0f, 0.0f, 0.0f)
	, mMoveSpeedRatio(1.0f)
	, mRotateAngle(0.0f, 0.0f, 0.0f)
{
}

// �f�X�g���N�^
CDebugCamera::~CDebugCamera()
{
	spDebugCamera = nullptr;
}

// �f�o�b�O�J�������擾
CDebugCamera* CDebugCamera::DebugCamera()
{
	if (spDebugCamera == nullptr)
	{
		spDebugCamera = new CDebugCamera();
	}
	return spDebugCamera;
}

// �f�o�b�O�J�������I���ɂȂ��Ă��邩�ǂ���
bool CDebugCamera::IsOn()
{
	return msIsOn;
}

// �f�o�b�O�J�������I��
void CDebugCamera::On(bool disableInput)
{
	// �ʏ�̓��͏����𖳌��ɂ���ꍇ�́A
	// �f�o�b�O���̓��[�h���I���ɂ���
	if (disableInput)
	{
		CDebugInput::On();
	}

	// �f�o�b�O�J�����̗L���t���O�𗧂Ă�
	msIsOn = true;

	// �f�o�b�O�J�������I���ɂ�������
	// �f�o�b�O�J�����̏�Ԃ����C���J�����ɍ��킹��
	CCamera* mainCamera = CCamera::MainCamera();
	if (mainCamera != nullptr)
	{
		spDebugCamera->CopyCamera(mainCamera);
		spDebugCamera->mMoveSpeed = CVector::zero;
		spDebugCamera->mMoveSpeedRatio = 1.0f;
		spDebugCamera->mRotateAngle = spDebugCamera->EulerAngles();
		spDebugCamera->mRotateAngle.Z(0.0f);
	}

	spDebugCamera->Reshape(WINDOW_WIDTH, WINDOW_HEIGHT);
}

// �f�o�b�O�J�������I�t
void CDebugCamera::Off()
{
	CDebugInput::Off();
	msIsOn = false;

	CCamera* mainCamera = CCamera::MainCamera();
	if (mainCamera != nullptr)
	{
		mainCamera->Reshape(WINDOW_WIDTH, WINDOW_HEIGHT);
	}
}

// ��X�V
void CDebugCamera::LateUpdate()
{
	// [SHIFT] + [0]�Ńf�o�b�O�J�����̃I���I�t�؂�ւ�
	if (CDebugInput::Key(VK_SHIFT))
	{
		if (CDebugInput::PushKey('0'))
		{
			if (IsOn()) Off();
			else On();
		}
	}

	// �f�o�b�O�J�������L���łȂ���΁A�ȍ~�������Ȃ�
	if (!msIsOn) return;

	// �}�E�X�z�C�[���̉�]�ňړ����x�̔{����ύX
	mMoveSpeedRatio += MOVE_SPEED_RATIO_VALUE * CDebugInput::GetDeltaMouseWheel();
	mMoveSpeedRatio = Math::Clamp(mMoveSpeedRatio, MOVE_SPEED_RATIO_MIN, MOVE_SPEED_RATIO_MAX);

	// �L�[�̓��̓x�N�g�����擾
	CVector input;
	if (CDebugInput::Key('W'))		input.Z(-1.0f);
	else if (CDebugInput::Key('S'))	input.Z(1.0f);
	if (CDebugInput::Key('A'))		input.X(-1.0f);
	else if (CDebugInput::Key('D'))	input.X(1.0f);

	// ���̓x�N�g���̒����œ��͂���Ă��邩����
	if (input.LengthSqr() > 0.0f)
	{
		// �ړ����x������
		float addSpeed = MOVE_ADD_SPEED * mMoveSpeedRatio;
		CVector move = CQuaternion(mRotateAngle) * input;
		mMoveSpeed += move.Normalized() * addSpeed;
		// �ړ����x���ő�l�ŃN�����v
		float maxSpeed = MOVE_MAX_SPEED * mMoveSpeedRatio;
		if (mMoveSpeed.Length() > maxSpeed)
		{
			mMoveSpeed = mMoveSpeed.Normalized() * maxSpeed;
		}
	}

	// �ړ����x������
	mMoveSpeed *= MOVE_ATTEN_RATIO;
	// �J�����̈ʒu���X�V
	Position(Position() + mMoveSpeed);

	// ���h���b�O�ŃJ�����𕽍s�ړ�
	if (CDebugInput::Key(VK_LBUTTON))
	{
		CVector2 delta = CDebugInput::GetDeltaMousePos();
		Position(Position() + VectorX() * delta.X() * TRANSLATE_SPEED);
		Position(Position() + VectorY() * delta.Y() * TRANSLATE_SPEED);
	}
	// �E�h���b�O�ŃJ��������]
	else if (CDebugInput::Key(VK_RBUTTON))
	{
		CVector2 delta = CDebugInput::GetDeltaMousePos();
		float x = Math::Repeat(mRotateAngle.X() + delta.Y() * ROTATE_SPEED, 360.0f);
		float y = Math::Repeat(mRotateAngle.Y() + delta.X() * ROTATE_SPEED, 360.0f);
		mRotateAngle.X(x);
		mRotateAngle.Y(y);
	}
	Rotation(CQuaternion(mRotateAngle));

	mEye = Position();
	mAt = mEye + -VectorZ().Normalized();
	mUp = VectorY();

	// �f�o�b�O�\��
	CVector pos = Position();
	CDebugPrint::Print("������������ �f�o�b�O�J�����F�I�� ������������\n");
	CDebugPrint::Print("[W][A][S][D]�F�J�����ړ�\n");
	CDebugPrint::Print("���h���b�O�F�J�������s�ړ�\n");
	CDebugPrint::Print("�E�h���b�O�F�J������]\n");
	CDebugPrint::Print("�z�C�[���F�J�����ړ����x���� x %.2f\n", mMoveSpeedRatio);
	CDebugPrint::Print("�J�������W�F%f, %f, %f\n", pos.X(), pos.Y(), pos.Z());
	CDebugPrint::Print("����������������������������������������������\n");

	// �J�����̃x�[�X�X�V
	CCamera::LateUpdate();
}

#endif
