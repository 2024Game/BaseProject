#pragma once
#include "CTask.h"
#include "CVector.h"
#include "CColor.h"

class CUIBase : public CTask
{
public:
	CUIBase(ETaskPriority prio = ETaskPriority::eUI, int sortOrder = 0,
		ETaskPauseType pause = ETaskPauseType::eGame,
		bool dontDelete = false, bool addTaskList = true);
	virtual ~CUIBase();

	//�\�����W��ݒ�iCVector2�Łj
	virtual void SetPos(const CVector2& pos);
	//�\�����W��ݒ�
	virtual void SetPos(const float& x, const float& y);
	//�\�����W���擾
	const CVector2& GetPos() const;

	//�X�P�[���l��ݒ�
	virtual void SetScale(const float& scale);
	//�X�P�[���l���擾
	float GetScale() const;

	//�\���T�C�Y��ݒ�iCVector2�Łj
	virtual void SetSize(const CVector2& size);
	//�\���T�C�Y��ݒ�
	virtual void SetSize(const float& x, const float& y);
	//�\���T�C�Y���擾
	const CVector2& GetSize() const;

	//���S�ʒu��ݒ�iCVector2�Łj
	virtual void SetCenter(const CVector2& center);
	//���S�ʒu��ݒ�
	virtual void SetCenter(const float x, const float y);
	//���S�ʒu���擾
	const CVector2& GetCenter() const;

	//�\���J���[��ݒ�iCColor�Łj
	virtual void SetColor(const CColor& color);
	//�\���J���[��ݒ�i�A���t�@�l�L��Łj
	virtual void SetColor(const float& r, const float& g, const float& b, const float& a);
	//�\���J���[��ݒ�
	virtual void SetColor(const float& r, const float& g, const float& b);
	//�\���J���[���擾
	const CColor& GetColor() const;

	//�A���t�@�l��ݒ�
	virtual void SetAlpha(const float& alpha);
	//�A���t�@�l���擾
	float GetAlpha() const;

	//�X�V
	void Update() override;

protected:
	bool CollisionPoint(const CVector2& point);

	virtual void OnPointerEnter(const CVector2& pos);
	virtual void OnPointerExit(const CVector2& pos);
	virtual void OnPointerDown(const CVector2& pos);
	virtual void OnPointerUp(const CVector2& pos);
	virtual void OnMove(const CVector2& move);

protected:
	bool mIsTouch;
	bool mIsEnter;
	CVector2 mLastMousePos;
	CVector2 mPosition;	//�\�����W
	CVector2 mSize;		//�\���T�C�Y
	CVector2 mCenter;	//���S�ʒu
	float mScale;
	CColor mColor;		//UI�̃J���[
};