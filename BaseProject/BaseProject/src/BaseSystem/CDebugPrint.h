#pragma once
#include <vector>
#include <string>
#include "CTask.h"

class CText;

// ��ʏ�Ƀf�o�b�O������\������N���X
class CDebugPrint : public CTask
{
public:
	/// <summary>
	/// �`�悷��f�o�b�O�������ǉ�
	/// </summary>
	/// <param name="format">����������</param>
	/// <param name="��2�����ȍ~">������ɕϊ��������</param>
	static void Print(const char* format, ...);

	/// <summary>
	/// �`�悷��f�o�b�O�������ǉ��i�����\���p�j
	/// </summary>
	/// <param name="format">����������</param>
	/// <param name="��2�����ȍ~">������ɕϊ��������</param>
	static void ForcePrint(const char* format, ...);

private:
	// �R���X�g���N�^
	CDebugPrint();

	// �`�悷�镶��������X�g�ɒǉ�
	void AddStr(std::string str);

	// �X�V
	void Update();
	// �`��
	void Render();

	static CDebugPrint* mspInstance;		// �C���X�^���X
	bool mIsOn;		// �f�o�b�O�v�����g���L�����ǂ���
	std::vector<std::string> mPrintStrList;	// �`�敶����̃��X�g
	CText* mpText;	// ������`��p
};