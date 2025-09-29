#include "CDebugTimer.h"

// �R���X�g���N�^
CDebugTimer::CDebugTimer()
	: freq({ 0 })
	, startCount({ 0 })
	, endCount({ 0 })
	, isEnd(true)
	, time(0.0f)
{
}

// ���Ԍv���J�n
void CDebugTimer::Start()
{
	if (!isEnd) return;

	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&startCount);
	isEnd = false;
}

// ���Ԍv���I��
void CDebugTimer::End()
{
	if (isEnd) return;

	QueryPerformanceCounter(&endCount);
	isEnd = true;

	time += (float)(endCount.QuadPart - startCount.QuadPart) / freq.QuadPart;
	startCount = { 0 };
	endCount = { 0 };
}

// �v���������Ԃ����Z�b�g
void CDebugTimer::Reset()
{
	isEnd = true;
	time = 0.0f;
	startCount = { 0 };
	endCount = { 0 };
}

// �v���������Ԃ��擾
float CDebugTimer::Get() const
{
	if (isEnd)
	{
		return time;
	}
	else
	{
		LARGE_INTEGER tempCount;
		QueryPerformanceCounter(&tempCount);
		return time + (float)(tempCount.QuadPart - startCount.QuadPart) / freq.QuadPart;
	}
}

// ���Ԍv�����I�����Ă��邩
bool CDebugTimer::IsEnd() const
{
	return isEnd;
}
