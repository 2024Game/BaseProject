#pragma once
#include "CTask.h"
#include <list>

/// <summary>
/// �^�X�N�Ǘ��N���X
/// </summary>
class CTaskManager
{
public:
	/// <summary>
	/// �C���X�^���X���擾
	/// </summary>
	/// <returns>�C���X�^���X</returns>
	static CTaskManager* Instance();
	// �C���X�^���X��j��
	static void ClearInstance();

	/// <summary>
	/// �w�肵���^�X�N�����X�g�ɒǉ�
	/// </summary>
	/// <param name="task">�ǉ�����^�X�N</param>
	/// <param name="isSort">�\�[�g���鎞�̒ǉ��������ǂ���</param>
	void Add(CTask* task, bool isSort = false);
	/// <summary>
	/// �w�肵���^�X�N�����X�g�����菜��
	/// </summary>
	/// <param name="task">��菜���^�X�N</param>
	/// <param name="isSort">�\�[�g���鎞�̎�菜���������ǂ���</param>
	void Remove(CTask* task, bool isSort = false);

	// �폜�t���O�������Ă���^�X�N��S�č폜
	void Delete();
	/// <summary>
	/// �w�肵���V�[���ɏ�������^�X�N��S�č폜
	/// </summary>
	/// <param name="scene">�폜����V�[���̎��</param>
	void DeleteInScene(EScene scene);

	// 3D�^�X�N�̃��X�g���擾
	const std::list<CTask*>& Get3dTasks() const;
	// 2D�^�X�N�̃��X�g���擾
	const std::list<CTask*>& Get2dTasks() const;

	/// <summary>
	/// �|�[�Y����
	/// </summary>
	/// <param name="pauseBit"></param>
	void Pause(int pauseBit);
	/// <summary>
	/// �|�[�Y����������
	/// </summary>
	/// <param name="pauseBit"></param>
	void UnPause(int pauseBit);
	/// <summary>
	/// �|�[�Y�����ǂ���
	/// </summary>
	/// <param name="pauseBit"></param>
	/// <returns></returns>
	bool IsPaused(int pauseBit = 0) const;

	// �X�V
	void Update();
	// ��X�V
	void LateUpdate();
	// �`��
	void Render();

private:
	// �R���X�g���N�^
	CTaskManager();
	// �f�X�g���N�^
	~CTaskManager();

	// �w�肳�ꂽ���X�g����
	// �폜�t���O�������Ă���^�X�N��S�č폜
	void Delete(std::list<CTask*>& list);

	// �^�X�N�̕��ёւ�
	void Sort();

	// �X�V���邩�ǂ���
	bool IsUpdate(CTask* task) const;

	// 3D�I�u�W�F�N�g�̕`�揈��
	void Render3d();
	// 2D�I�u�W�F�N�g�̕`�揈��
	void Render2d();

	// ���[�V�����u���[�L������3D�I�u�W�F�N�g�̕`�揈��
	void RenderMotionBlur();

	// �^�X�N�}�l�[�W���̃C���X�^���X
	static CTaskManager* mpInstance;

	// 3D�^�X�N�̃��X�g
	std::list<CTask*> m3dTasks;
	// 2D�^�X�N�̃��X�g
	std::list<CTask*> m2dTasks;

	int mPauseBit;	// �|�[�Y�̃r�b�g�t���O
};
