#include "CDebugInput.h"

#if _DEBUG

// �f�o�b�O���̓��[�h���I���ɂȂ��Ă��邩�ǂ���
bool CDebugInput::msIsOn = false;

// �R���X�g���N�^
CDebugInput::CDebugInput()
{
}

// �}�E�X���W���擾
CVector2 CDebugInput::GetMousePos()
{
	return CInput::msMousePos;
}

// �}�E�X���W�̈ړ��ʂ��擾
CVector2 CDebugInput::GetDeltaMousePos()
{
	return CInput::msMousePos - CInput::msLastMousePos;
}

// �}�E�X�z�C�[���̉�]�ʂ̍������擾
int CDebugInput::GetDeltaMouseWheel()
{
	return CInput::msDeltaMouseWheel;
}

// �L�[�����͂���Ă��邩
bool CDebugInput::Key(int key)
{
	return (CInput::msInputBits[key] & (1 << CInput::eInputBit_On)) != 0;
}

// �L�[���������u�Ԃ��ǂ���
bool CDebugInput::PushKey(int key)
{
	return (CInput::msInputBits[key] & (1 << CInput::eInputBit_Push)) != 0;
}

// �L�[�𗣂����u�Ԃ��ǂ���
bool CDebugInput::PullKey(int key)
{
	return (CInput::msInputBits[key] & (1 << CInput::eInputBit_Pull)) != 0;
}

// �f�o�b�O���̓��[�h���I���ɂȂ��Ă��邩�ǂ���
bool CDebugInput::IsOn()
{
	return msIsOn;
}

// �f�o�b�O���̓��[�h���I��
void CDebugInput::On()
{
	msIsOn = true;
}

// �f�o�b�O���̓��[�h���I�t
void CDebugInput::Off()
{
	msIsOn = false;
}

#endif
