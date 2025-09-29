#pragma once
#include <vector>
#include "CTask.h"

class CNavNode;

// �o�H�T�������̊Ǘ��N���X
class CNavManager : public CTask
{
public:
	// �o�H�Ǘ��N���X�̃C���X�^���X���擾
	static CNavManager* Instance();

	// �R���X�g���N�^
	CNavManager();
	// �f�X�g���N�^
	~CNavManager();

	// �o�H�P�T�b�N�p�̃m�[�h��ǉ�
	void AddNode(CNavNode* node);
	// �o�H�T���p�̃m�[�h����菜��
	void RemoveNode(CNavNode* node);

	/// <summary>
	///	�w�肵���m�[�h�ɐڑ��ł���m�[�h���������Đݒ�
	/// (�ԂɎՕ���������m�[�h�ɂ͐ڑ��ł��Ȃ�)
	/// </summary>
	/// <param name="node">�ڑ��ł���m�[�h����������m�[�h</param>
	/// <param name="distance">�ڑ��ł��鋗���̌��E�l</param>
	/// <returns>�ڑ������m�[�h�̐�</returns>
	void FindConnectNavNodes(CNavNode* node, float distance);

	/// <summary>
	/// �w�肵���m�[�h�ɐڑ��ł���m�[�h���������Đݒ�i���������Łj
	/// </summary>
	/// <param name="node">�ڑ��ł���m�[�h����������m�[�h</param>
	/// <param name="distance">�ڑ��ł��鋗���̌��E�l</param>
	/// <returns>�ڑ������m�[�h�̐�</returns>
	int FindConnectNavNodesImmediate(CNavNode* node, float distance);

	/// <summary>
	/// �w�肵���J�n�m�[�h����ړI�n�m�[�h�܂ł̍ŒZ�o�H�����߂�
	/// </summary>
	/// <param name="start">�J�n�m�[�h</param>
	/// <param name="goal">�ړI�n�m�[�h</param>
	/// <param name="route">�ŒZ�o�H�ԋp�p</param>
	/// <returns>�o�H���q�����Ă���ꍇ�́Atrue��Ԃ�</returns>
	bool Navigate(CNavNode* start, CNavNode* goal, std::vector<CNavNode*>& route);

	/// <summary>
	/// �w�肵���o�H���q�����Ă��邩�ǂ���
	/// </summary>
	/// <param name="route">�q�����Ă��邩�m�F����o�H</param>
	/// <returns>true�Ȃ�΁A�o�H���q�����Ă���</returns>
	bool IsRouteValid(const std::vector<CNavNode*>& route) const;

	/// <summary>
	/// �Օ����`�F�b�N�Ɏg�p����R���C�_�[��ǉ�
	/// </summary>
	/// <param name="col"></param>
	void AddCollider(CCollider* col);
	/// <summary>
	/// �Օ����`�F�b�N�Ɏg�p����R���C�_�[����菜��
	/// </summary>
	/// <param name="col"></param>
	void RemoveCollider(CCollider* col);

	// �S�Ẵm�[�h���X�V
	void Update() override;
	// �S�Ẵm�[�h�ƌo�H��`��
	void Render() override;

private:
	// �ŒZ�o�H�v�Z�p�̃f�[�^�����Z�b�g
	void ResetCalcData();
	/// <summary>
	/// �w�肵���m�[�h���玟�̃m�[�h�ւ̈ړ��R�X�g���v�Z
	/// </summary>
	/// <param name="node">�ړ��R�X�g���v�Z����J�n�m�[�h</param>
	/// <param name="goal">�ړI�n�̃m�[�h</param>
	void CalcNextMoveCost(CNavNode* node, CNavNode* goal);
	// �ڑ��ł���m�[�h���ǂ���
	bool CanConnectNavNode(CNavNode* node, CNavNode* other, float distance) const;

	// �w�肵���m�[�h��ڑ���Ԃ̍X�V�҂����X�g�ɒǉ�
	void AddUpdateConnectNavNode(CNavNode* node, float distance);
	// �w�肵���m�[�h��ڑ���Ԃ̍X�V�҂����X�g�����菜��
	void RemoveUpdateConnectNavNode(CNavNode* node);

	// �ڑ��m�[�h���X�V
	void UpdateConnectNavNode();

	// �폜����m�[�h�𒲂ׂ�
	void CheckKillNode();

	// �o�H�Ǘ��N���X�̃C���X�^���X�ւ̃|�C���^
	static CNavManager* spInstance;

	// �o�H�T���p�̃m�[�h�̃��X�g
	std::vector<CNavNode*> mNodes;

	// �o�H�T���̃f�o�b�O�\�����s�����ǂ���
	bool mIsRender;
	// �Ō�Ɍv�Z�����ŒZ�o�H�̃f�o�b�O�\���p
	std::vector<CNavNode*> mLastCalcRoute;

	// �Օ����`�F�b�N�Ɏg�p����R���C�_�[�̃��X�g
	std::vector<CCollider*> mColliders;

	// ���ݍX�V���̃m�[�h
	CNavNode* mpUpdateNode;
	float mFindNodeDistance;
	// ���̐ڑ����Ă��邩���ׂ�m�[�h�̃C���f�b�N�X�l
	int mNextFindNodeIndex;

	// �ڑ���ԍX�V�m�[�h�̃f�[�^
	struct UpdateConnectNodeData
	{
		CNavNode* node;	// �X�V����m�[�h
		float distance;	// ���肷��m�[�h�Ԃ̋���
	};
	// �ڑ���Ԃ̍X�V�҂����X�g
	std::vector<UpdateConnectNodeData> mUpdateConnectNodes;
};