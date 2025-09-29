#include "CDebugProfiler.h"
#include "CDebugTimer.h"
#include "Time.h"

// CDebugProfiler�̃C���X�^���X
CDebugProfiler* CDebugProfiler::mpInstance = nullptr;

// �R���X�g���N�^
CDebugProfiler::CDebugProfiler()
	: CTask(ETaskPriority::eDebug, 0, ETaskPauseType::eDefault, true)
{
}

// �f�X�g���N�^
CDebugProfiler::~CDebugProfiler()
{
}

// �C���X�^���X�擾
CDebugProfiler* CDebugProfiler::Instance()
{
	if (mpInstance == nullptr)
	{
		mpInstance = new CDebugProfiler();
	}
	return mpInstance;
}

// ���Ԍv���J�n
void CDebugProfiler::StartTimer(std::string name)
{
	CDebugProfiler* inst = Instance();
	auto& timers = inst->mTimers;

	CDebugTimer* timer = timers[name];
	if (timer == nullptr)
	{
		timer = new CDebugTimer();
		timers[name] = timer;
 	}

	timer->Start();
}

// ���Ԍv���I��
void CDebugProfiler::EndTimer(std::string name)
{
	CDebugProfiler* inst = Instance();
	auto& timers = inst->mTimers;

	CDebugTimer* timer = timers[name];
	if (timer == nullptr) return;

	timer->End();
}

// �v�����ʂ�`��
void CDebugProfiler::Print()
{
	CDebugProfiler* inst = Instance();
	auto& timers = inst->mTimers;

	if (timers.size() > 0)
	{
		CDebugPrint::Print("�������������� Profiler ��������������\n");
		CDebugPrint::Print("%.1fFPS( Delta:%f�b )\n", Times::FPS(), Times::DeltaTime());
		for (auto& timer : timers)
		{
			CDebugTimer* dt = timer.second;
			if (dt->IsEnd())
			{
				CDebugPrint::Print
				(
					"%s:�o�ߎ���[%f�b]:����[%f%%%%]\n",
					timer.first.c_str(),
					dt->Get(),
					(dt->Get() / Times::DeltaTime()) * 100.0f
				);
			}
			else
			{
				CDebugPrint::Print
				(
					"%s:�o�ߎ���[%f�b]\n",
					timer.first.c_str(),
					dt->Get()
				);
			}
			dt->Reset();
		}
		CDebugPrint::Print("��������������������������������������\n");
	}
}
