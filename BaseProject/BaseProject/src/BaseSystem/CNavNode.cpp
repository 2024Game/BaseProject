#include "CNavNode.h"
#include "CNavManager.h"
#include "Primitive.h"

// �m�[�h��Y���W�̃I�t�Z�b�g�l
#define NODE_OFFSET_Y 5.0f
// �T���m�[�h�̋����̌��E�l
#define FIND_NODE_DISTANCE 180.0f
// �m�[�h�̈ʒu���X�V���鋗��
#define UPDATE_DISTANCE 1.0f

// �R���X�g���N�^
CNavNode::CNavNode(const CVector& pos, bool isDestNode)
	: mIsEnable(true)
	, mIsKill(false)
	, mIsDestNode(isDestNode)
	, mPosition(pos)
	, mCalcMoveCost(-1.0f)
	, mpCalcFromNode(nullptr)
	, mIsUpdateConnectNode(false)
	, mIsUpdaingConnectNode(false)
	, mColor(0.0f, 1.0f, 0.0f, 1.0f)
{
	// �Ǘ��N���X�̃��X�g�Ɏ��g��ǉ�
	CNavManager* navMgr = CNavManager::Instance();
	if (navMgr != nullptr)
	{
		navMgr->AddNode(this);
	}

	// ���W��ݒ�
	SetPos(mPosition, true);
}

// �f�X�g���N�^
CNavNode::~CNavNode()
{
	// �T���m�[�h���폜����O�ɁA���g�Ɛڑ����Ă���m�[�h����菜��
	ClearConnects(true);
}

// �L����Ԃ�ݒ�
void CNavNode::SetEnable(bool enable)
{
	mIsEnable = enable;
}

// ���ݗL�����ǂ���
bool CNavNode::IsEnable() const
{
	return mIsEnable;
}

// �폜����
void CNavNode::Kill()
{
	mIsKill = true;
}

// �폜���邩�ǂ���
bool CNavNode::IsKill() const
{
	return mIsKill;
}

// �ŒZ�o�H�v�Z�p�̃f�[�^�����Z�b�g
void CNavNode::ResetCalcData()
{
	mCalcMoveCost = -1.0;
	mpCalcFromNode = nullptr;
}

// �m�[�h�̍��W���擾
const CVector& CNavNode::GetPos() const
{
	return mPosition;
}

// �Օ����`�F�b�N��\���p�ɁA������ɏグ���m�[�h�̍��W��Ԃ�
CVector CNavNode::GetOffsetPos() const
{
	// �n�ʂƏd�Ȃ�Ȃ��悤�ɁA�m�[�h�̈ʒu���班���グ���ʒu��Ԃ�
	return mPosition + CVector(0.0f, NODE_OFFSET_Y, 0.0f);
}

// �m�[�h�̍��W��ݒ�
void CNavNode::SetPos(const CVector& pos, bool isInit)
{
	// ���������łȂ����
	if (!isInit)
	{
		// �ʒu���ς���ĂȂ���΁A�X�V���Ȃ�
		if (pos == mPosition) return;

		// �m�[�h�̈ʒu�̕ω����X�V���鋗�����Z���ꍇ�́A�X�V���Ȃ�
		float dist = (pos - mPosition).LengthSqr();
		if (dist < UPDATE_DISTANCE * UPDATE_DISTANCE) return;
	}

	// �m�[�h�̍��W���X�V
	mPosition = pos;

	// ���������́A�����ڑ��m�[�h���X�V
	if (isInit)
	{
		UpdateConnectNode(true);
	}
	// ����ȍ~�́A�X�V�ł���^�C�~���O�ɍX�V
	else
	{
		// �ڑ��m�[�h�X�V�t���O�𗧂Ă�
		mIsUpdateConnectNode = true;
	}
}

// �w�肵���m�[�h�Ɛڑ����Ă��邩
bool CNavNode::IsConnectNode(CNavNode* node) const
{
	// ���g���A�w�肵���m�[�h�������ȏ�Ԃł���΁A�ڑ����Ă��Ȃ�
	if (!IsEnable() || !node->IsEnable()) return false;

	// �w�肵���m�[�h���ڑ�����Ă��邩�m�F
	auto find = std::find_if
	(
		mConnectData.begin(), mConnectData.end(),
		[node](const CNavConnectData& x) { return x.node == node; }
	);
	if (find != mConnectData.end())
	{
		// �ڑ�����Ă��邩�A�ڑ���Ԃ��L���ł���΁A�ڑ����Ă���
		if (find->enabled) return true;
	}

	// �ڑ�����Ă��Ȃ�����
	return false;
}

// �w�肵���m�[�h�Ƃ̐ڑ���Ԃ�ݒ�
void CNavNode::SetEnableConnect(CNavNode* node, bool enable)
{
	// �w�肵���m�[�h���ڑ�����Ă��邩�m�F
	auto find = std::find_if
	(
		mConnectData.begin(), mConnectData.end(),
		[node](const CNavConnectData& x) { return x.node == node; }
	);
	// �ڑ�����Ă�����A�ڑ���Ԃ�ύX
	if (find != mConnectData.end())
	{
		find->enabled = enable;
	}
}

// �w�肵���m�[�h�Ƃ̐ڑ���Ԃ�Ԃ�
bool CNavNode::IsEnableConnect(CNavNode* node) const
{
	// �w�肵���m�[�h���ڑ�����Ă��邩�m�F
	auto find = std::find_if
	(
		mConnectData.begin(), mConnectData.end(),
		[node](const CNavConnectData& x) { return x.node == node; }
	);
	if (find == mConnectData.end()) return false;
	return find->enabled;
}

// �ڑ�����m�[�h�ǉ�
void CNavNode::AddConnect(CNavNode* node, bool forced)
{
	for (CNavConnectData& connect : mConnectData)
	{
		if (connect.node == node) return;
	}

	// �ڑ�����m�[�h�܂ł̋������R�X�g�Ƃ���
	float cost = (node->GetPos() - mPosition).Length();

	// ���g�Ƒ��肻�ꂼ��̐ڑ����Ă���m�[�h���X�g�ɂ��݂���ݒ�
	mConnectData.push_back(CNavConnectData(node, cost, forced));
	node->mConnectData.push_back(CNavConnectData(this, cost, forced));
}

// �ڑ����Ă���m�[�h����菜��
void CNavNode::RemoveConnect(CNavNode* node)
{
	auto result = std::remove_if
	(
		mConnectData.begin(), mConnectData.end(),
		[node](const CNavConnectData& x) { return x.node == node; }
	);
	mConnectData.erase(result, mConnectData.end());
}

// �ڑ����Ă���S�Ẵm�[�h������
void CNavNode::ClearConnects(bool forced)
{
	// �ڑ�����̐ڑ����X�g���玩�g����菜��
	for (CNavConnectData& connect : mConnectData)
	{
		if (!forced && connect.forced) continue;
		connect.node->RemoveConnect(this);
	}

	// �����I�ɐڑ�����������̂ł����
	if (forced)
	{
		// ���g�̐ڑ����X�g���N���A
		mConnectData.clear();
	}
	// �����łȂ����
	else
	{
		// �����I�ɐڑ�����m�[�h�ȊO�̐ڑ���Ԃ�����
		auto result = std::remove_if
		(
			mConnectData.begin(), mConnectData.end(),
			[](const CNavConnectData& x) { return !x.forced; }
		);
		mConnectData.erase(result, mConnectData.end());
	}
}

// �����I�ɐڑ�����m�[�h��ǉ�
void CNavNode::AddForcedConnectNode(CNavNode* node)
{
	mForcedConnectNodes.push_back(node);
	AddConnect(node, true);

	node->mForcedConnectNodes.push_back(this);
	node->AddConnect(this, true);
}

// �����I�ɐڑ�����m�[�h����菜��
void CNavNode::RemoveForcedConnectNode(CNavNode* node)
{
	auto result = std::remove(mForcedConnectNodes.begin(), mForcedConnectNodes.end(), node);
	mForcedConnectNodes.erase(result, mForcedConnectNodes.end());
	RemoveConnect(node);

	result = std::remove(node->mForcedConnectNodes.begin(), node->mForcedConnectNodes.end(), this);
	node->mForcedConnectNodes.erase(result, node->mForcedConnectNodes.end());
	node->RemoveConnect(this);
}

// �w�肵���m�[�h�������I�ɐڑ�����m�[�h���ǂ���
bool CNavNode::IsForcedConnectNode(CNavNode* node) const
{
	auto find = std::find(mForcedConnectNodes.begin(), mForcedConnectNodes.end(), node);
	return find != mForcedConnectNodes.end();
}

// �ڑ����Ȃ��m�[�h��ǉ�
void CNavNode::AddBlockedNode(CNavNode* node)
{
	mBlockedNodes.push_back(node);
	node->mBlockedNodes.push_back(this);
}

// �ڑ����Ȃ��m�[�h����菜��
void CNavNode::RemoveBlockedNode(CNavNode* node)
{
	auto result = std::remove(mBlockedNodes.begin(), mBlockedNodes.end(), node);
	mBlockedNodes.erase(result, mBlockedNodes.end());

	result = std::remove(node->mBlockedNodes.begin(), node->mBlockedNodes.end(), this);
	node->mBlockedNodes.erase(result, node->mBlockedNodes.end());
}

// �ڑ����Ȃ��m�[�h���ǂ���
bool CNavNode::IsBlockedNode(CNavNode* node) const
{
	auto find = std::find(mBlockedNodes.begin(), mBlockedNodes.end(), node);
	return find != mBlockedNodes.end();
}

// �ڑ����Ă���m�[�h���X�V
void CNavNode::UpdateConnectNode(bool immediate)
{
	CNavManager* navMgr = CNavManager::Instance();
	if (navMgr == nullptr) return;

	if (immediate)
	{
		navMgr->FindConnectNavNodesImmediate(this, FIND_NODE_DISTANCE);
	}
	else
	{
		if (mIsUpdaingConnectNode) return;
		navMgr->FindConnectNavNodes(this, FIND_NODE_DISTANCE);
	}

	mIsUpdateConnectNode = false;
}


// �ڑ��m�[�h�̍X�V�J�n���̌Ăяo��
void CNavNode::StartUpdateConnectNode()
{
	mIsUpdaingConnectNode = true;
}

// �ڑ��m�[�h�̍X�V�I�����̌Ăяo��
void CNavNode::EndUpdateConnectNode()
{
	mIsUpdaingConnectNode = false;
}

// �m�[�h�X�V����
bool CNavNode::IsUpdating() const
{
	if (mIsUpdateConnectNode) return true;
	if (mIsUpdaingConnectNode) return true;
	return false;
}

// �m�[�h�̐F�ݒ�i�f�o�b�O�p�j
void CNavNode::SetColor(const CColor& color)
{
	mColor.Set(color.R(), color.G(), color.B());
}

// �m�[�h���X�V
void CNavNode::Update()
{
	// �ڑ��m�[�h�X�V�t���O�������Ă�����A�X�V
	if (mIsUpdateConnectNode)
	{
		UpdateConnectNode();
	}
}

// �m�[�h��`��i�f�o�b�O�p�j
void CNavNode::Render()
{
	// �m�[�h���L����ԏo�Ȃ���΁A�`�悵�Ȃ�
	if (!mIsEnable) return;

	// �ڑ���̃m�[�h�܂ł̃��C����`��
	for (CNavConnectData& connect : mConnectData)
	{
		// �ڑ���̃m�[�h�������ł���΁A�X���[
		if (!connect.node->IsEnable()) continue;

		CColor color = CColor(0.11f, 0.1f, 0.1f, 1.0f);
		// �ڑ����Ă��邪�A���ݐڑ���Ԃ���������Ă���ꍇ�́A�ԐF�̃��C���ɕύX
		if (!connect.enabled) color = CColor(1.0f, 0.0f, 0.0f, 1.0f);
		Primitive::DrawLine
		(
			GetOffsetPos(),
			connect.node->GetOffsetPos(),
			color,
			2.0f
		);
	}

	// �m�[�h���W�Ɏl�p�`��`��
	Primitive::DrawWireBox
	(
		GetOffsetPos(),
		CVector::one,
		mColor
	);
}

