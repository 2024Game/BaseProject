#include "CUIBase.h"
#include "CUIManager.h"
#include "CInput.h"

CUIBase::CUIBase(ETaskPriority prio, int sortOrder, ETaskPauseType pause, bool dontDelete, bool addTaskList)
	: CTask(prio, sortOrder, pause, dontDelete, addTaskList)
	, mIsTouch(false)
	, mLastMousePos(CVector2::zero)
	, mPosition(0.0f, 0.0f)
	, mSize(100.0f, 100.0f)
	, mCenter(0.0f, 0.0f)
	, mScale(1.0f)
	, mColor(1.0f, 1.0f, 1.0f, 1.0f)
{
	CUIManager::Instance()->Add(this);
}

CUIBase::~CUIBase()
{
	CUIManager::Instance()->Remove(this);
}

//�\�����W��ݒ�iCVector2�Łj
void CUIBase::SetPos(const CVector2& pos)
{
	mPosition = pos;
}

//�\�����W��ݒ�
void CUIBase::SetPos(const float& x, const float& y)
{
	SetPos(CVector2(x, y));
}

//�\�����W���擾
const CVector2& CUIBase::GetPos() const
{
	return mPosition;
}

//�X�P�[���l��ݒ�
void CUIBase::SetScale(const float& scale)
{
	mScale = scale;
}

//�X�P�[���l���擾
float CUIBase::GetScale() const
{
	return mScale;
}

//�\���T�C�Y��ݒ�iCVector2�Łj
void CUIBase::SetSize(const CVector2& size)
{
	mSize = size;
}

//�\���T�C�Y��ݒ�
void CUIBase::SetSize(const float& x, const float& y)
{
	SetSize(CVector2(x, y));
}

//�\���T�C�Y���擾
const CVector2& CUIBase::GetSize() const
{
	return mSize;
}

//���S�ʒu��ݒ�iCVector2�Łj
void CUIBase::SetCenter(const CVector2& center)
{
	mCenter = center;
}

//���S�ʒu��ݒ�
void CUIBase::SetCenter(const float x, const float y)
{
	SetCenter(CVector2(x, y));
}

//���S�ʒu���擾
const CVector2& CUIBase::GetCenter() const
{
	return mCenter;
}

//�\���J���[��ݒ�iCColor�Łj
void CUIBase::SetColor(const CColor& color)
{
	mColor = color;
}

//�\���J���[��ݒ�i�A���t�@�l�L��Łj
void CUIBase::SetColor(const float& r, const float& g, const float& b, const float& a)
{
	mColor.Set(r, g, b, a);
}

//�\���J���[��ݒ�
void CUIBase::SetColor(const float& r, const float& g, const float& b)
{
	mColor.Set(r, g, b);
}

//�\���J���[���擾
const CColor& CUIBase::GetColor() const
{
	return mColor;
}

//�A���t�@�l��ݒ�
void CUIBase::SetAlpha(const float& alpha)
{
	mColor.A(alpha);
}

//�A���t�@�l���擾
float CUIBase::GetAlpha() const
{
	return mColor.A();
}

bool CUIBase::CollisionPoint(const CVector2& point)
{
	return CCollider::CollisionRectPoint(CRect(mPosition - mCenter, mSize), point);
}

void CUIBase::OnPointerEnter(const CVector2& pos)
{
}

void CUIBase::OnPointerExit(const CVector2& pos)
{
}

void CUIBase::OnPointerDown(const CVector2& pos)
{
}

void CUIBase::OnPointerUp(const CVector2& pos)
{
}

void CUIBase::OnMove(const CVector2& move)
{
}

void CUIBase::Update()
{
	CVector2 mousePos = CInput::GetMousePos();
	if (IsEnable() && !mIsTouch)
	{
		if (CInput::PushKey(VK_LBUTTON))
		{
			if (CollisionPoint(mousePos))
			{
				mIsTouch = true;
				OnPointerDown(mousePos);
				mLastMousePos = mousePos;
			}
		}
	}
	else
	{
		if (!IsEnable() || CInput::PullKey(VK_LBUTTON))
		{
			mIsTouch = false;
			OnPointerUp(mousePos);
		}
		else
		{
			CVector2 move = mousePos - mLastMousePos;
			if (move.LengthSqr() > 0.0f)
			{
				OnMove(move);
			}
			mLastMousePos = mousePos;
		}
	}

	if (!mIsEnter)
	{
		if (CollisionPoint(mousePos))
		{
			mIsEnter = true;
			OnPointerEnter(mousePos);
		}
	}
	else
	{
		if (!CollisionPoint(mousePos))
		{
			mIsEnter = false;
			OnPointerExit(mousePos);
		}
	}
}
