#include "CEnemy.h"
#include "CEffect.h"
#include "CCollisionManager.h"
#include "CColliderCapsule.h"
#include "CGaugeUI3D.h"

#define GRAVITY 0.0625f

// �R���X�g���N�^
CEnemy::CEnemy()
	: CXCharacter(ETag::eEnemy, ETaskPriority::eEnemy)
	, mState(0)
	, mStateStep(0)
	, mElapsedTime(0.0f)
	, mMoveSpeed(CVector::zero)
	, mMoveSpeedY(0.0f)
	, mIsGrounded(false)
	, mGroundNormal(CVector::up)
	, mpBodyCol(nullptr)
	, mpHpGauge(nullptr)
{
	// HP�Q�[�W���쐬
	mpHpGauge = new CGaugeUI3D(this);
	mpHpGauge->SetMaxPoint(mMaxHp);
	mpHpGauge->SetCurrPoint(mHp);
}

// �f�X�g���N�^
CEnemy::~CEnemy()
{
	// �R���C�_�[�폜
	SAFE_DELETE(mpBodyCol);

	// HP�Q�[�W�����݂�����A�ꏏ�ɍ폜����
	if (mpHpGauge != nullptr)
	{
		mpHpGauge->SetOwner(nullptr);
		mpHpGauge->Kill();
	}
}

// �I�u�W�F�N�g�폜��`����֐�
void CEnemy::DeleteObject(CObjectBase* obj)
{
	// �폜���ꂽ�̂�HP�Q�[�W�ł���΁A
	// HP�Q�[�W�̃|�C���^����ɂ���
	if (obj == mpHpGauge)
	{
		mpHpGauge = nullptr;
	}
}

// �G�̏�����
void CEnemy::InitEnemy(std::string path, const std::vector<AnimData>* pAnimData)
{
	// �A�j���[�V�����f�[�^�e�[�u����ݒ�
	mpAnimData = pAnimData;

	// ���f���f�[�^��ǂݍ���
	CModelX* model = CResourceManager::Get<CModelX>(path);

	// �e�[�u�����̃A�j���[�V�����f�[�^��ǂݍ���
	int size = mpAnimData->size();
	for (int i = 0; i < size; i++)
	{
		const AnimData& data = (*mpAnimData)[i];
		if (data.path.empty()) continue;
		model->AddAnimationSet(data.path.c_str());
	}

	// CXCharacter�̏�����
	Init(model);
}

// �Փˏ���
void CEnemy::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// �{�̂��Փ˂���
	if (self == mpBodyCol)
	{
		// �t�B�[���h�ƏՓ˂���
		if (other->Layer() == ELayer::eField)
		{
			// �⓹�Ŋ���Ȃ��悤�ɁA�����߂��x�N�g����X��Z�̒l��0�ɂ���
			CVector adjust = hit.adjust;
			adjust.X(0.0f);
			adjust.Z(0.0f);

			Position(Position() + adjust * hit.weight);

			// �Փ˂����n�ʂ������V�䂩����ςŔ���
			CVector normal = hit.adjust.Normalized();
			float dot = CVector::Dot(normal, CVector::up);
			// ���ς̌��ʂ��v���X�ł���΁A���ƏՓ˂���
			if (dot >= 0.0f)
			{
				// �����Ȃǂŏ��ɏォ��Փ˂������i���ړ��j�̂�
				// �㉺�̈ړ����x��0�ɂ���
				if (mMoveSpeedY < 0.0f)
				{
					mMoveSpeedY = 0.0f;
				}

				// �ڒn����
				mIsGrounded = true;
				// �ڒn�����n�ʂ̖@�����L�����Ă���
				mGroundNormal = hit.adjust.Normalized();
			}
			// ���ς̌��ʂ��}�C�i�X�ł���΁A�V��ƏՓ˂���
			else
			{
				// �W�����v�ȂǂœV��ɉ�����Փ˂������i��ړ��j�̂�
				// �㉺�̈ړ����x��0�ɂ���
				if (mMoveSpeedY > 0.0f)
				{
					mMoveSpeedY = 0.0f;
				}
			}
		}
		// �v���C���[�ƏՓ˂����ꍇ
		else if (other->Layer() == ELayer::ePlayer)
		{
			// �������ɂ̂݉����߂����߁A
			// �����߂��x�N�g����Y�̒l��0�ɂ���
			CVector adjust = hit.adjust;
			adjust.Y(0.0f);
			Position(Position() + adjust * hit.weight);
		}
	}
}

// �A�j���[�V�����؂�ւ�
void CEnemy::ChangeAnimation(int type, bool restart)
{
	if (mpAnimData == nullptr) return;
	if (!(0 <= type && type < mpAnimData->size())) return;
	AnimData data = (*mpAnimData)[type];
	CXCharacter::ChangeAnimation
	(
		type,
		data.loop,
		data.frameLength,
		restart
	);
	CXCharacter::SetAnimationSpeed(data.speed);
}

// ��Ԑ؂�ւ�
void CEnemy::ChangeState(int state)
{
	// ������Ԃ̏ꍇ�͐؂�ւ��Ȃ�
	if (state == mState) return;

	// ��Ԃ�ύX���āA��Ԋ֘A�̕ϐ��̏�����
	mState = state;
	mStateStep = 0;
	mElapsedTime = 0.0f;
}

// �X�V
void CEnemy::Update()
{
	// �d��
	mMoveSpeedY -= GRAVITY;

	// �ړ�
	CVector moveSpeed = mMoveSpeed + CVector(0.0f, mMoveSpeedY, 0.0f);
	Position(Position() + moveSpeed);

	// �L�����N�^�[�̍X�V
	CXCharacter::Update();

	mIsGrounded = false;

	// HP�Q�[�W���X�V
	mpHpGauge->Position(Position() + mGaugeOffsetPos);
	mpHpGauge->SetMaxPoint(mMaxHp);
	mpHpGauge->SetCurrPoint(mHp);
}

// �`��
void CEnemy::Render()
{
	CXCharacter::Render();
}
