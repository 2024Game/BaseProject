#pragma once
#include <Windows.h>
#include "GLFW/glfw3.h"
#include <map>
#include "CVector.h"
class CDebugInput;

// ���͔���N���X
class CInput
{
	friend CDebugInput;
public:
	/// <summary>
	/// �E�B���h�E��ݒ�
	/// </summary>
	/// <param name="pwindow"></param>
	static void Window(GLFWwindow* pwindow);

	// �}�E�X���W���擾
	static CVector2 GetMousePos();

	// �}�E�X���W�̈ړ��ʂ��擾
	static CVector2 GetDeltaMousePos();

	// �}�E�X�z�C�[���̉�]�ʂ����Z
	static void AddMouseWheel(int wheel);
	// �}�E�X�z�C�[���̉�]�ʂ̍������擾
	static int GetDeltaMouseWheel();

	/// <summary>
	/// �L�[�����͂���Ă��邩
	/// </summary>
	/// <param name="key">���肷��L�[</param>
	/// <returns>true�Ȃ�΁A������Ă���</returns>
	static bool Key(int key);

	/// <summary>
	/// �L�[���������u�Ԃ��ǂ���
	/// �O��̃t���[���͉����Ă��Ȃ���Ԃ�
	/// ����̃t���[���ŉ�������ԂɂȂ�������
	/// 1�t���[���̂�true��Ԃ�
	/// </summary>
	/// <param name="key">���肷��L�[</param>
	/// <returns>true�Ȃ�΁A�������u�Ԃł���</returns>
	static bool PushKey(int key);

	/// <summary>
	/// �L�[�𗣂����u�Ԃ��ǂ���
	/// �O��̃t���[���͉����Ă����Ԃ�
	/// ����̃t���[���ŗ�������ԂɂȂ�������
	/// 1�t���[���̂�true��Ԃ�
	/// </summary>
	/// <param name="key">���肷��L�[</param>
	/// <returns>true�Ȃ�΁A�������u�Ԃł���</returns>
	static bool PullKey(int key);

	/// <summary>
	/// �}�E�X�J�[�\���̕\���ݒ�
	/// </summary>
	/// <param name="isShow">true�Ȃ�΁A�\������</param>
	static void ShowCursor(bool isShow);

	/// <summary>
	/// �L�[�̓��͏�Ԃ��X�V
	/// �i�Ăяo����1�t���[����1��܂Łj
	/// </summary>
	static void Update();

private:
	// �R���X�g���N�^
	// �i�C���X�^���X�𐶐��ł��Ȃ��悤��private�j
	CInput();

	static GLFWwindow* spWindow;	// �E�B���h�E�̃|�C���^

	// �L�[�̓��͏�ԃr�b�g
	enum InputBit
	{
		eInputBit_On,	//�����Ă��邩�ǂ���
		eInputBit_Push,	//�������u�Ԃ��ǂ���
		eInputBit_Pull,	//�������u�Ԃ��ǂ���
		Num,
	};
	// �L�[�̓��͏�Ԃ��Ǘ����郊�X�g
	static std::map<int, int> msInputBits;
	// �}�E�X�J�[�\����\�����邩
	static bool msIsShowCursor;
	// ���݂̃}�E�X���W
	static CVector2 msMousePos;
	// �O��̃}�E�X���W
	static CVector2 msLastMousePos;
	// �}�E�X�z�C�[���̉�]��
	static int msMouseWheel;
	// �O��̃}�E�X�z�C�[���̉�]��
	static int msLastMouseWheel;
	// �}�E�X�z�C�[���̉�]�ʂ̍���
	static int msDeltaMouseWheel;

#if _DEBUG
	// �ʏ���͂������������O�̃}�E�X���W
	static CVector2 msDebugLastMousePos;
#endif
};