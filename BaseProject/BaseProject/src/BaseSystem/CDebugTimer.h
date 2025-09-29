#pragma once
#include <windows.h>

// �f�o�b�O�p�̎��Ԍv���N���X
class CDebugTimer
{
public:
	// �R���X�g���N�^
	CDebugTimer();

	// ���Ԍv���J�n
	void Start();
	// ���Ԍv���I��
	void End();
	// �v���������Ԃ����Z�b�g
	void Reset();
	// �v���������Ԃ��擾
	float Get() const;
	// ���Ԍv�����I�����Ă��邩
	bool IsEnd() const;

private:
	LARGE_INTEGER freq;
	LARGE_INTEGER startCount;
	LARGE_INTEGER endCount;
	bool isEnd;
	float time;
};
