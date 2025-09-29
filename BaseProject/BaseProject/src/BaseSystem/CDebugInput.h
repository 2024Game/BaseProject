#pragma once
#if _DEBUG
#include "CInput.h"

// �ʏ�̓��͔���𖳌��ɂ���
// �f�o�b�O���[�h���̓��͔�����s���N���X
class CDebugInput
{
public:
	// �}�E�X���W���擾
	static CVector2 GetMousePos();

	// �}�E�X���W�̈ړ��ʂ��擾
	static CVector2 GetDeltaMousePos();

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
	/// �f�o�b�O���̓��[�h���I���ɂȂ��Ă��邩�ǂ���
	/// </summary>
	/// <returns></returns>
	static bool IsOn();
	/// <summary>
	/// �f�o�b�O���̓��[�h���I��
	/// </summary>
	static void On();
	/// <summary>
	/// �f�o�b�O���̓��[�h���I�t
	/// </summary>
	static void Off();

private:
	CDebugInput();

	// �f�o�b�O���̓��[�h���ǂ���
	static bool msIsOn;
};
#endif