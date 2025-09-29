#pragma once
#include <list>

class CNavManager;
class CNavNode;

// �o�H�T���m�[�h�̐ڑ����
class CNavConnectData
{
public:
	bool enabled;		// �L�����ǂ���
	CNavNode* node;		// �q�����Ă���m�[�h
	float cost;			// �q�����Ă���m�[�h�܂ł̈ړ��R�X�g
	bool forced;		// �����I�Ɍq���m�[�h���ǂ���
	CNavConnectData(CNavNode* n, float c, bool f)
		: enabled(true), node(n), cost(c), forced(f) {}
};

// �o�H�T���m�[�h�N���X
class CNavNode
{
	friend CNavManager;

public:
	// �R���X�g���N�^
	CNavNode(const CVector& pos, bool isDestNode = false);

	// �L����Ԃ�ݒ�
	void SetEnable(bool enable);
	// ���ݗL�����ǂ���
	bool IsEnable() const;

	// �폜����
	void Kill();
	// �폜���邩�ǂ���
	bool IsKill() const;

	// �m�[�h�̍��W���擾
	const CVector& GetPos() const;
	// �Օ����`�F�b�N��\���p�ɁA������ɏグ���m�[�h�̍��W��Ԃ�
	CVector GetOffsetPos() const;
	// �m�[�h�̍��W��ݒ�
	void SetPos(const CVector& pos, bool isInit = false);

	// �w�肵���m�[�h�Ɛڑ����Ă��邩
	bool IsConnectNode(CNavNode* node) const;

	// �w�肵���m�[�h�Ƃ̐ڑ���Ԃ�ݒ�
	void SetEnableConnect(CNavNode* node, bool enable);
	// �w�肵���m�[�h�Ƃ̐ڑ���Ԃ�Ԃ�
	bool IsEnableConnect(CNavNode* node) const;

	// �ڑ�����m�[�h�ǉ�
	void AddConnect(CNavNode* node, bool forced = false);
	// �ڑ����Ă���m�[�h����菜��
	void RemoveConnect(CNavNode* node);
	// �ڑ����Ă���S�Ẵm�[�h������
	void ClearConnects(bool forced = false);

	// �����I�ɐڑ�����m�[�h��ǉ�
	void AddForcedConnectNode(CNavNode* node);
	// �����I�ɐڑ�����m�[�h����菜��
	void RemoveForcedConnectNode(CNavNode* node);
	// �w�肵���m�[�h�������I�ɐڑ�����m�[�h���ǂ���
	bool IsForcedConnectNode(CNavNode* node) const;

	// �ڑ����Ȃ��m�[�h��ǉ�
	void AddBlockedNode(CNavNode* node);
	// �ڑ����Ȃ��m�[�h����菜��
	void RemoveBlockedNode(CNavNode* node);
	// �w�肵���m�[�h���ڑ����Ȃ��m�[�h���ǂ���
	bool IsBlockedNode(CNavNode* node) const;

	// �ڑ����Ă���m�[�h���X�V
	void UpdateConnectNode(bool immediate = false);
	// �m�[�h�X�V����
	bool IsUpdating() const;

	// �m�[�h�̐F�ݒ�i�f�o�b�O�p�j
	void SetColor(const CColor& color);
	// �m�[�h���X�V
	void Update();
	// �m�[�h��`��i�f�o�b�O�p�j
	void Render();

private:
	// �f�X�g���N�^
	~CNavNode();

	// �ŒZ�o�H�v�Z�p�̃f�[�^�����Z�b�g
	void ResetCalcData();

	// �ڑ��m�[�h�̍X�V�J�n���̌Ăяo��
	void StartUpdateConnectNode();
	// �ڑ��m�[�h�̍X�V�I�����̌Ăяo��
	void EndUpdateConnectNode();

	// �m�[�h���L�����ǂ���
	bool mIsEnable;
	// �폜���邩�ǂ���
	bool mIsKill;

	// �o�H�T�����Ɍo�R�ł��Ȃ��ړI�n��p�̃m�[�h���ǂ���
	bool mIsDestNode;
	CVector mPosition;	// �m�[�h�̍��W
	// �ڑ����Ă���m�[�h�̏�񃊃X�g
	std::vector<CNavConnectData> mConnectData;
	// �Օ����`�F�b�N�Ɋ֌W�Ȃ��ڑ�����m�[�h�̃��X�g
	std::vector<CNavNode*> mForcedConnectNodes;
	// �ڑ����Ȃ��m�[�h�̃��X�g
	std::vector<CNavNode*> mBlockedNodes;

	// �ŒZ�o�H�v�Z���ɋL�����Ă������
	float mCalcMoveCost;		// �J�n�m�[�h���炱�̃m�[�h�܂łɊ|�������ړ��R�X�g
	CNavNode* mpCalcFromNode;	// �ŒZ�o�H�ł̑O�̃m�[�h�̃|�C���^
	bool mIsUpdateConnectNode;	// �ڑ��m�[�h���X�V����K�v�����邩
	bool mIsUpdaingConnectNode;	// �ڑ��m�[�h���X�V����

	// �f�o�b�O�֘A
	CColor mColor;		// �m�[�h�̐F
};