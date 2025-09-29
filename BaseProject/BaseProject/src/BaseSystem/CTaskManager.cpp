#include "CTaskManager.h"
#include "CCamera.h"
#include "CDebugCamera.h"
#include "CBillBoard.h"

//�^�X�N�}�l�[�W���̃C���X�^���X
CTaskManager* CTaskManager::mpInstance = nullptr;

// �C���X�^���X���擾
CTaskManager* CTaskManager::Instance()
{
	// �C���X�^���X��������΁A�V������������
	if (mpInstance == nullptr)
	{
		mpInstance = new CTaskManager();
	}
	return mpInstance;
}

// �C���X�^���X�̔j��
void CTaskManager::ClearInstance()
{
	delete mpInstance;
	mpInstance = nullptr;
}

// �R���X�g���N�^
CTaskManager::CTaskManager()
	: mPauseBit(0)
{
}

// �f�X�g���N�^
CTaskManager::~CTaskManager()
{
	for (int i = 0; i < 2; i++)
	{
		std::list<CTask*>& list = i == 0 ? m3dTasks : m2dTasks;
		// �擪����Ō�܂ŌJ��Ԃ�
		auto itr = list.begin();
		auto end = list.end();
		while (itr != end)
		{
			// �폜�^�X�N���L�����Ă���
			CTask* del = *itr;
			itr = list.erase(itr);
			delete del;
		}
	}
	m3dTasks.clear();
	m2dTasks.clear();
}

// �w�肵���^�X�N�����X�g�ɒǉ�
void CTaskManager::Add(CTask* task, bool isSort)
{
	if (!task->mAddTaskList) return;

	// �ǉ�����^�X�N��3D�^�X�N���ǂ���
	bool is3dTask = task->mPriority < ETaskPriority::Start2d;
	// 3D�^�X�N��2D�^�X�N���Œǉ����郊�X�g�؂�ւ���
	std::list<CTask*>& list = is3dTask ? m3dTasks : m2dTasks;

	// ���X�g�̐擪���珇�Ԃɒ��ׂ�
	auto itr = list.begin();
	auto end = list.end();
	while (itr != end)
	{
		CTask* current = *itr;
		// �ǉ�����^�X�N�ƌ������̃^�X�N�̗D��x�������ꍇ
		if (task->mPriority == current->mPriority)
		{
			// ���ёւ����̔ԍ������������ɕ��ׂ�
			if (task->mSortOrder < current->mSortOrder)
			{
				list.insert(itr, task);
				return;
			}
		}
		// �ǉ�����^�X�N�̗D��x��
		// �������̃^�X�N�̗D��x��菬�����ꍇ�́A
		// �������̃^�X�N�̑O�ɑ}��
		else if (task->mPriority < current->mPriority)
		{
			list.insert(itr, task);
			return;
		}
		// ����
		itr++;
	}

	// �����܂Œ��ׂă^�X�N���������ޏꏊ���Ȃ������ꍇ�́A
	// ���X�g�̍Ō���ɒǉ�
	list.push_back(task);
}

// �w�肵���^�X�N�����X�g�����菜��
void CTaskManager::Remove(CTask* task, bool isSort)
{
	if (!task->mAddTaskList) return;

	// ��菜���^�X�N��3D�^�X�N���ǂ���
	bool is3dTask = task->mPriority < ETaskPriority::Start2d;
	// 3D�^�X�N��2D�^�X�N���Ń��X�g��؂�ւ���
	std::list<CTask*>& list = is3dTask ? m3dTasks : m2dTasks;

	// ���X�g���Ɏ�菜���^�X�N�����݂��邩�ǂ���
	auto find = std::find(list.begin(), list.end(), task);
	if (find == list.end()) return;

	// ���݂����ꍇ�́A���X�g����^�X�N����菜��
	list.erase(find);
}

// �폜�t���O�������Ă���^�X�N��S�č폜
void CTaskManager::Delete()
{
	// 3D�^�X�N���X�g���𐸍�
	Delete(m3dTasks);
	// 2D�^�X�N���X�g���𐸍�
	Delete(m2dTasks);
}

// �w�肳�ꂽ���X�g����
// �폜�t���O�������Ă���^�X�N��S�č폜
void CTaskManager::Delete(std::list<CTask*>& list)
{
	// �擪����Ō�܂ŌJ��Ԃ�
	auto itr = list.begin();
	auto end = list.end();
	while (itr != end)
	{
		// �폜�^�X�N���L�����Ă���
		CTask* del = *itr;
		// �폜�t���O�������Ă�����A�폜���Ď���
		if (del->IsKill())
		{
			itr = list.erase(itr);
			delete del;
		}
		// �폜�t���O�������Ă��Ȃ���΁A���̂܂܎���
		else
		{
			itr++;
		}
	}
}

// �w�肵���V�[���ɏ�������^�X�N��S�č폜
void CTaskManager::DeleteInScene(EScene scene)
{
	// 3D�^�X�N���X�g�𐸍�
	for (CTask* task : m3dTasks)
	{
		// ��������V�[������v������A
		// �폜�t���O�𗧂Ă�
		if (task->GetSceneType() == scene)
		{
			task->Kill();
		}
	}
	// 2D�^�X�N���X�g�𐸍�
	for (CTask* task : m2dTasks)
	{
		// ��������V�[������v������A
		// �폜�t���O�𗧂Ă�
		if (task->GetSceneType() == scene)
		{
			task->Kill();
		}
	}
}

// �^�X�N�̕��ёւ�
void CTaskManager::Sort()
{
	// �^�X�N���X�g�ɒǉ����Ȃ����^�X�N�̃��X�g
	std::list<CTask*> addTasks;

	// 3D�^�X�N���X�g���ŕ��ёւ���^�X�N������
	auto itr = m3dTasks.begin();
	auto end = m3dTasks.end();
	while (itr != end)
	{
		CTask* task = *itr;
		itr++;

		// ���ёւ��t���O�������Ă�����A
		if (task->mIsSort)
		{
			Remove(task, true);
			task->mIsSort = false;
			addTasks.push_back(task);
		}
	}
	// 2D�^�X�N���X�g���ŕ��ёւ���^�X�N������
	itr = m2dTasks.begin();
	end = m2dTasks.end();
	while (itr != end)
	{
		CTask* task = *itr;
		itr++;

		// ���ёւ��t���O�������Ă�����A
		if (task->mIsSort)
		{
			Remove(task, true);
			task->mIsSort = false;
			addTasks.push_back(task);
		}
	}

	// �^�X�N���X�g�����菜�����^�X�N���A
	// �^�X�N���X�g�ɒǉ����Ȃ���
	for (CTask* task : addTasks)
	{
		Add(task, true);
	}
}

// 3D�^�X�N�̃��X�g���擾
const std::list<CTask*>& CTaskManager::Get3dTasks() const
{
	return m3dTasks;
}

// 2D�^�X�N�̃��X�g���擾
const std::list<CTask*>& CTaskManager::Get2dTasks() const
{
	return m2dTasks;
}

// �|�[�Y����
void CTaskManager::Pause(int pauseBit)
{
	mPauseBit |= pauseBit;
}

// �|�[�Y����������
void CTaskManager::UnPause(int pauseBit)
{
	mPauseBit &= ~pauseBit;
}

// �|�[�Y�����ǂ���
bool CTaskManager::IsPaused(int pauseBit) const
{
	if (pauseBit == 0) pauseBit = ~pauseBit;
	return (mPauseBit & pauseBit) != 0;
}

// �X�V���邩�ǂ���
bool CTaskManager::IsUpdate(CTask* task) const
{
	if (task == nullptr) return false;

	// �|�[�Y���̃^�X�N�Ȃ�΁A�X�V���Ȃ�
	ETaskPauseType pause = task->GetPauseType();
	if (pause != ETaskPauseType::eNone && (mPauseBit & (int)pause) != 0) return false;

	// �L���t���O���I�t�Ȃ�΁A�X�V���Ȃ�
	if (!task->IsEnable()) return false;

	return true;
}

// �X�V
void CTaskManager::Update()
{
	// �^�X�N�̍X�V�O�Ƀ^�X�N�̕��ёւ����s��
	Sort();

	// 3D�^�X�N���X�g���̃^�X�N�����ԂɍX�V
	for (CTask* task : m3dTasks)
	{
		// �X�V����^�X�N�ł���΁A�^�X�N���X�V
		if (IsUpdate(task))
		{
			task->Update();
		}
	}
	// 2D�^�X�N���X�g���̃^�X�N�����ԂɍX�V
	for (CTask* task : m2dTasks)
	{
		// �X�V����^�X�N�ł���΁A�^�X�N���X�V
		if (IsUpdate(task))
		{
			task->Update();
		}
	}
}

// ��X�V
void CTaskManager::LateUpdate()
{
	// 3D�^�X�N���X�g���̃^�X�N�����ԂɍX�V
	for (CTask* task : m3dTasks)
	{
		// �X�V����^�X�N�ł���΁A�^�X�N�̌�X�V�����s
		if (IsUpdate(task))
		{
			task->LateUpdate();
		}
	}
	// 2D�^�X�N���X�g���̃^�X�N�����ԂɍX�V
	for (CTask* task : m2dTasks)
	{
		// �X�V����^�X�N�ł���΁A�^�X�N�̌�X�V�����s
		if (IsUpdate(task))
		{
			task->LateUpdate();
		}
	}
}

// 3D�I�u�W�F�N�g�̕`�揈��
void CTaskManager::Render3d()
{
	for (CTask* task : m3dTasks)
	{
		// �\���t���O���I���Ȃ�΁A
		if (task->IsShow())
		{
			// �^�X�N��`��
			task->Render();
		}
	}
}

// 2D�I�u�W�F�N�g�̕`�揈��
void CTaskManager::Render2d()
{
	for (CTask* task : m2dTasks)
	{
		// �\���t���O���I���Ȃ�΁A
		if (task->IsShow())
		{
			// �^�X�N��`��
			task->Render();
		}
	}
}

// ���[�V�����u���[�L������3D�I�u�W�F�N�g�̕`�揈��
void CTaskManager::RenderMotionBlur()
{
	// ���[�V�����u���[�̃p�����[�^���擾
	CVector dir;
	float width;
	int count;
	System::GetMotionBlurParam(&dir, &width, &count);

	// 1��̃u���[�Ŕ��f����F�̊���
	float ratio = 1.0f / (count + 1);
	// �w�肳�ꂽ�񐔕��u���[���|����
	for (int i = 0; i <= count; i++)
	{
		glPushMatrix();

		glClear(GL_DEPTH_BUFFER_BIT);

		// �u���[�̈ړ��ʂ����߂�
		CVector move = -dir * width * i;
		glTranslatef(move.X(), move.Y(), move.Z());

		// 3D�֘A�̃I�u�W�F�N�g��`��
		Render3d();

		// ����́A�J���[�o�b�t�@�̓��e��
		// �A�L�������[�V�����o�b�t�@�ɏ㏑��
		if (i == 0)
		{
			glAccum(GL_LOAD, ratio);
		}
		// ����ȍ~�́A�J���[�o�b�t�@�̓��e��
		// ��芄���ŉ��Z����
		else
		{
			glAccum(GL_ACCUM, ratio);
		}

		glPopMatrix();
	}
	// ���[�V�����u���[���|�����`�挋�ʂ�
	// �J���[�o�b�t�@�ɔ��f
	glAccum(GL_RETURN, 1.0f);
}

// �`��
void CTaskManager::Render()
{
	// ���݂̃J���������݂����
	CCamera* current = CCamera::CurrentCamera();
	if (current != nullptr)
	{
		// ���݂̃J�����𔽉f
		current->Apply();

		// ���[�V�����u���[���L��
		if (System::IsEnableMotionBlur())
		{
			// 3D�֘A�̕`��Ƀ��[�V�����u���[���|����
			RenderMotionBlur();
		}
		// ���[�V�����u���[���L���łȂ�
		else
		{
			// �ʏ��3D�֘A�̕`��
			Render3d();
		}
	}

	// 2D�`��p�̃J�����ɐ؂�ւ���
	CCamera::Start2DCamera();
	// 2D�֘A�̕`��
	Render2d();
	// 3D�`��p�̃J�����֖߂�
	CCamera::End2DCamera();
}
