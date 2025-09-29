#include "CCollisionManager.h"
#include "CCollider.h"
#include "CObjectBase.h"
#include "CDebugInput.h"

// �R���W�����}�l�[�W���̃C���X�^���X
CCollisionManager* CCollisionManager::mpInstance = nullptr;

// �C���X�^���X���擾
CCollisionManager* CCollisionManager::Instance()
{
	if (mpInstance == nullptr)
	{
		mpInstance = new CCollisionManager();
	}
	return mpInstance;
}

// �C���X�^���X�̔j��
void CCollisionManager::ClearInstance()
{
	delete mpInstance;
	mpInstance = nullptr;
}

// �R���X�g���N�^
CCollisionManager::CCollisionManager()
	: mIsShowCollider(false)
	, mIsShowColliderBounds(false)
{
}

// �f�X�g���N�^
CCollisionManager::~CCollisionManager()
{
}

// �w�肵���R���C�_�[�����X�g�ɒǉ�
void CCollisionManager::Add(CCollider* col)
{
	// ���Ƀ��X�g�ɒǉ�����Ă�����A�ǉ����Ȃ�
	auto it = std::find(mColliderList.begin(), mColliderList.end(), col);
	if (it != mColliderList.end()) return;

	// ���X�g�̍Ō���ɒǉ�
	mColliderList.push_back(col);
}

// �w�肵���R���C�_�[�����X�g�����菜��
void CCollisionManager::Remove(CCollider* col)
{
	mColliderList.remove(col);
}

// �w�肵��2�̃R���C�_�[�̏Փˏ������s��
void CCollisionManager::Collision(CCollider* col0, CCollider* col1)
{
	// �w�肵���R���C�_�[���L���łȂ���΁A�Փ˔�����s��Ȃ�
	if (col0 == nullptr || col1 == nullptr) return;
	if (!col0->IsEnable() || !col1->IsEnable()) return;
	// ���R���C�_�[�Ɏ����傪���݂��Ȃ��ꍇ�́A�Փ˔�����s��Ȃ�
	if (col0->Owner() == nullptr && col1->Owner() == nullptr) return;
	// ����������̃R���C�_�[�ł���΁A�Փ˔�����s��Ȃ�
	if (col0->Owner() == col1->Owner()) return;
	// 1�ڂ̃R���C�_�[�̎����傪�Փ˔�����s��Ȃ���Ԃł���΁A�Փ˔�����s��Ȃ�
	if (col0->Owner() != nullptr && !col0->Owner()->IsEnableCol()) return;
	// 2�ڂ̃R���C�_�[�̎����傪�Փ˔�����s��Ȃ���Ԃł���΁A�Փ˔�����s��Ȃ�
	if (col1->Owner() != nullptr && !col1->Owner()->IsEnableCol()) return;

	// ����̃R���C�_�[�ƏՓ˔�����s���R���C�_�[�łȂ���΁A�Փ˔�����s��Ȃ�
	if (!col0->IsCollision(col1)) return;
	if (!col1->IsCollision(col0)) return;

	// �o�E���f�B���O�{�b�N�X���m���������Ă��Ȃ��ꍇ�́A�Փ˔�����s��Ȃ�
	if (!CBounds::Intersect(col0->Bounds(), col1->Bounds())) return;

	// �Փ˔�����s��
	CHitInfo hit;
	bool collision = CCollider::Collision(col0, col1, &hit);
	// �Փ˂��Ă��Ȃ���΁A�Փˏ������s��Ȃ�
	if (!collision) return;

	// �����߂��̉e���������d�ʂŌv�Z
	hit.weight = CCollider::CalcPushBackRatio(col0, col1);
	
	// �e�R���C�_�[�̎�����ɏՓ˂������Ƃ�`����
	if (col0->Owner() != nullptr)
	{
		col0->Owner()->Collision(col0, col1, hit);
	}
	if (col1->Owner() != nullptr)
	{
		// �����߂������Ɖe�������𔽓]
		hit.adjust = -hit.adjust;
		hit.weight = 1.0f - hit.weight;
		col1->Owner()->Collision(col1, col0, hit);
	}
}

// �w�肵���R���C�_�[�Ƒ��̑S�ẴR���C�_�[�Ƃ̏Փˏ������s��
void CCollisionManager::Collision(CCollider* col)
{
	// �w�肵���R���C�_�[���L���łȂ���΁A�Փ˔���͍s��Ȃ�
	if (col == nullptr) return;
	if (!col->IsEnable()) return;

	// �R���C�_�[���X�g���̑S�ăR���C�_�[�ƏՓˏ������s��
	for (auto& other : mColliderList)
	{
		// 2�̃R���C�_�[�̏Փˏ������s��
		Collision(col, other);
	}
}

// �S�ẴR���C�_�[�̏Փˏ������s��
void CCollisionManager::CollisionAll()
{
	// �Փ˔�����s���O�ɑS�ẴR���C�_�[�̏����X�V
	for (CCollider* c : mColliderList)
	{
		c->UpdateCol();
	}

	// ���X�g�̐擪���珇�ԂɏՓˏ������s��
	auto it = mColliderList.begin();
	auto end = mColliderList.end();
	for (; it != end; ++it)
	{
		CCollider* current = *it;

		// current�̎��̃R���C�_�[���珇�ԂɏՓˏ������s��
		// �icurrent���O�̃R���C�_�[�Ƃ͊��ɏՓˏ������s���Ă��邽�߁j
		auto nextIt = it;
		nextIt++;
		for (; nextIt != end; ++nextIt)
		{
			CCollider* next = *nextIt;
			// 2�̃R���C�_�[�̏Փˏ������s��
			Collision(current, next);
		}
	}
}

#if _DEBUG
// �S�R���C�_�[��`��
void CCollisionManager::Render()
{
	// �uSHIFT�v+�u9�v�ŃR���C�_�[�\���@�\�I���I�t
	if (CDebugInput::Key(VK_SHIFT) && CDebugInput::PushKey('9'))
	{
		mIsShowCollider = !mIsShowCollider;
		if (mIsShowCollider) mIsShowColliderBounds = true;
	}
	// �R���C�_�[�\���t���O���I�t�Ȃ�A�ȍ~�������Ȃ�
	if (!mIsShowCollider) return;

	// ���X�g���̑S�ẴR���C�_�[��`��
	for (auto& col : mColliderList)
	{
		if (!col->IsShow()) continue;
		col->Render();
	}

	// �o�E���f�B���O�{�b�N�X��`��
	RenderBounds();
}

// �S�R���C�_�[�̃o�E���f�B���O�{�b�N�X��`��
void CCollisionManager::RenderBounds()
{
	// �R���C�_�[�\�����Ɂu9�v�������ƁA
	// �R���C�_�[�̃o�E���f�B���O�{�b�N�X�\���@�\�I���I�t
	if (mIsShowCollider && CDebugInput::PushKey('9'))
	{
		mIsShowColliderBounds = !mIsShowColliderBounds;
	}
	// �R���C�_�[�\���t���O���A
	// �o�E���f�B���O�{�b�N�X�\���t���O���I�t�Ȃ�A�ȍ~�������Ȃ�
	if (!mIsShowCollider || !mIsShowColliderBounds) return;

	// ���X�g���̑S�ẴR���C�_�[�̃o�E���f�B���O�{�b�N�X��`��
	for (auto& col : mColliderList)
	{
		if (!col->IsShow()) continue;
		col->RenderBounds();
	}
}
#endif
