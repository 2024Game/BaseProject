#pragma once
#include <map>
#include <string>
#include "CTask.h"
class CDebugTimer;

// �f�o�b�O�p�̏������Ԃ��v�����ĕ\������N���X
class CDebugProfiler : public CTask
{
public:
	// �f�X�g���N�^
	~CDebugProfiler();

	// ���Ԍv���J�n
	static void StartTimer(std::string name);
	// ���Ԍv���I��
	static void EndTimer(std::string name);

	// �v�����ʂ�`��
	static void Print();

private:
	// CDebugProfiler�̃C���X�^���X
	static CDebugProfiler* mpInstance;
	// �������Ԍv�����̃^�C�}�[�̃��X�g
	std::map<std::string, CDebugTimer*> mTimers;

	// �R���X�g���N�^
	CDebugProfiler();
	// �C���X�^���X�擾
	static CDebugProfiler* Instance();
};