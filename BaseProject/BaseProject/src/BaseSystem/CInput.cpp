#include "CInput.h"
#include "CDebugInput.h"
#include <stdio.h>

GLFWwindow* CInput::spWindow = nullptr;	// �E�B���h�E�̃|�C���^
std::map<int, int> CInput::msInputBits;	// �L�[�̓��͏�Ԃ��Ǘ����郊�X�g
bool CInput::msIsShowCursor = true;		// �}�E�X�J�[�\����\�����邩
CVector2 CInput::msMousePos = CVector2(0.0f, 0.0f);	// ���݂̃}�E�X���W
CVector2 CInput::msLastMousePos = CVector2(0.0f, 0.0f);	// �O��̃}�E�X���W
int CInput::msMouseWheel = 0;		// �}�E�X�z�C�[���̉�]��
int CInput::msLastMouseWheel = 0;	// �O��̃}�E�X�z�C�[���̉�]��
int CInput::msDeltaMouseWheel = 0;	// �}�E�X�z�C�[���̉�]�ʂ̍���

#if _DEBUG
// �ʏ���͂������������O�̃}�E�X���W
CVector2 CInput::msDebugLastMousePos = CVector2(0.0f, 0.0f);
#endif

// �E�B���h�E��ݒ�
void CInput::Window(GLFWwindow* pwindow)
{
	spWindow = pwindow;
}

// �}�E�X���W���擾
CVector2 CInput::GetMousePos()
{
#if _DEBUG
	if (CDebugInput::IsOn()) return msDebugLastMousePos;
#endif
	return msMousePos;
}

// �}�E�X���W�̈ړ��ʂ��擾
CVector2 CInput::GetDeltaMousePos()
{
#if _DEBUG
	if (CDebugInput::IsOn()) return CVector2(0.0f, 0.0f);
#endif
	return msMousePos - msLastMousePos;
}

// �}�E�X�z�C�[���̉�]�ʂ����Z
void CInput::AddMouseWheel(int wheel)
{
	msMouseWheel += wheel;
}

// �}�E�X�z�C�[���̉�]�ʂ��擾
int CInput::GetDeltaMouseWheel()
{
#if _DEBUG
	if (CDebugInput::IsOn()) return 0;
#endif
	return msDeltaMouseWheel;
}

// �R���X�g���N�^
CInput::CInput()
{
	printf("���̓C���X�^���X�����܂�܂���\n");
}

// �L�[�����͂���Ă��邩
bool CInput::Key(int key)
{
#if _DEBUG
	if (CDebugInput::IsOn()) return false;
#endif
	return (msInputBits[key] & (1 << eInputBit_On)) != 0;
}

// �L�[���������u�Ԃ��ǂ���
bool CInput::PushKey(int key)
{
#if _DEBUG
	if (CDebugInput::IsOn()) return false;
#endif
	return (msInputBits[key] & (1 << eInputBit_Push)) != 0;
}

// �L�[�𗣂����u�Ԃ��ǂ���
bool CInput::PullKey(int key)
{
#if _DEBUG
	if (CDebugInput::IsOn()) return false;
#endif
	return (msInputBits[key] & (1 << eInputBit_Pull)) != 0;
}

// �}�E�X�J�[�\���̕\���ݒ�
void CInput::ShowCursor(bool isShow)
{
	msIsShowCursor = isShow;
}

// �L�[�̓��͏�Ԃ��X�V
void CInput::Update()
{
	int mode = glfwGetInputMode(spWindow, GLFW_CURSOR);
	if (msIsShowCursor && mode == GLFW_CURSOR_DISABLED ||
		!msIsShowCursor && mode == GLFW_CURSOR_NORMAL)
	{
		glfwSetInputMode
		(
			spWindow,
			GLFW_CURSOR,
			msIsShowCursor ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED
		);
	}

	auto itBegin = msInputBits.begin();
	auto itEnd = msInputBits.end();
	for (auto it = itBegin; it != itEnd; ++it)
	{
		int key = it->first;
		int bit = it->second;

		// ���݉����Ă����Ԃ��擾
		bool isOn = (GetAsyncKeyState(key) & 0x8000) != 0;
		// �O�񉟂��Ă����Ԃ��擾
		bool isLastOn = bit & (1 << eInputBit_On);

		// �r�b�g�t���O������
		bit = 0;

		// ���݉����Ă����Ԃł���΁A
		// eInputBit_On�̃r�b�g�𗧂Ă�
		if (isOn) bit |= (1 << eInputBit_On);
		// �O��͗����Ă��āA���݂͉����Ă����Ԃł���΁A
		// eInputBit_Push�̃r�b�g�𗧂Ă�
		if (!isLastOn && isOn) bit |= (1 << eInputBit_Push);
		// �O��͉����Ă��āA���݂͗����Ă����Ԃł���΁A
		// eInputBit_Pull�̃r�b�g�𗧂Ă�
		if (isLastOn && !isOn) bit |= (1 << eInputBit_Pull);

		// ���X�g���̃r�b�g�t���O���㏑��
		it->second = bit;
	}

	// �}�E�X���W�X�V
	msLastMousePos = msMousePos;
	double xpos, ypos;
	glfwGetCursorPos(spWindow, &xpos, &ypos);
	//	glfwGet
	msMousePos = CVector2((float)xpos, (float)ypos);
#if _DEBUG
	if (!CDebugInput::IsOn())
	{
		msDebugLastMousePos = msMousePos;
	}
#endif

	// �}�E�X�z�C�[���̉�]�ʂ��X�V
	msDeltaMouseWheel = msMouseWheel - msLastMouseWheel;
	msLastMouseWheel = msMouseWheel;
}
