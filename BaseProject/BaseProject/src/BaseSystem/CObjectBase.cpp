#include "CObjectBase.h"
#include "CCollider.h"
#include "Maths.h"

// �R���X�g���N�^
CObjectBase::CObjectBase(ETag tag,
	ETaskPriority prio, int sortOrder,
	ETaskPauseType pause,
	bool dontDelete, bool addTaskList)
	: CTask(prio, sortOrder, pause, dontDelete, addTaskList)
	, mTag(tag)
	, mIsEnableCol(true)
	, mDepth(0.0f)
	, mColor(CColor::white)
{
}

// �f�X�g���N�^
CObjectBase::~CObjectBase()
{
}

// �I�u�W�F�N�g�폜��`����֐�
void CObjectBase::DeleteObject(CObjectBase* obj)
{
}

// �I�u�W�F�N�g�^�O���擾
ETag CObjectBase::Tag() const
{
	return mTag;
}

// �Փ˔�����s�����ݒ�
void CObjectBase::SetEnableCol(bool isEnable)
{
	mIsEnableCol = isEnable;
}

// �Փ˔�����s�����ǂ���
bool CObjectBase::IsEnableCol() const
{
	return mIsEnableCol && IsEnable();
}

// �J���[��ݒ�
void CObjectBase::SetColor(const CColor& color)
{
	mColor = color;
}

// �J���[���擾
const CColor& CObjectBase::GetColor() const
{
	return mColor;
}

// �A���t�@�l�ݒ�
void CObjectBase::SetAlpha(float alpha)
{
	mColor.A(Math::Clamp01(alpha));
}

// �A���t�@�l�擾
float CObjectBase::GetAlpha() const
{
	return mColor.A();
}

// �J�����܂ł̋������v�Z
void CObjectBase::CalcDepth()
{
	CCamera* camera = CCamera::CurrentCamera();
	if (camera == nullptr) return;

	CVector camPos = camera->Position();
	CVector pos = Position();

	mDepth = (camPos - pos).LengthSqr();
}

// �J��������̋������擾
float CObjectBase::GetDepth() const
{
	return mDepth;
}

// �Փˏ���
void CObjectBase::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
}

// ���C�ƃI�u�W�F�N�g�̏Փ˔���
bool CObjectBase::CollisionRay(const CVector& start, const CVector& end, CHitInfo* hit)
{
	return false;
}

// �U������
bool CObjectBase::IsAttacking() const
{
	return false;
}

// �U���J�n
void CObjectBase::AttackStart()
{
	// �U�����q�b�g�����I�u�W�F�N�g�̃��X�g��������
	mAttackHitObjects.clear();
}

// �U���I��
void CObjectBase::AttackEnd()
{
}

// �U�����q�b�g�����I�u�W�F�N�g��ǉ�
void CObjectBase::AddAttackHitObj(CObjectBase* obj)
{
	mAttackHitObjects.push_back(obj);
}

// ���ɍU�����q�b�g���Ă���I�u�W�F�N�g���ǂ���
bool CObjectBase::IsAttackHitObj(CObjectBase* obj) const
{
	auto find = std::find
	(
		mAttackHitObjects.begin(),
		mAttackHitObjects.end(),
		obj
	);
	return find != mAttackHitObjects.end();
}