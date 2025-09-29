#include "CSlash.h"

// �R���X�g���N�^
CSlash::CSlash(CObjectBase* owner, const CVector& pos, const CVector& dir,
	float speed, float dist)
	: CObjectBase(ETag::eSlash, ETaskPriority::eEffect, 0, ETaskPauseType::eGame)
	, mpOwner(owner)
	, mKillMoveDist(dist)
	, mMovedDist(0.0f)

{
	Position(pos);
	mMoveSpeed = dir.Normalized() * speed;
	Rotation(CQuaternion::LookRotation(mMoveSpeed, CVector::up));

	mpModel = CResourceManager::Get<CModel>("Slash");
	mpModel->SetupEffectSettings();
}

// �f�X�g���N�^
CSlash::~CSlash()
{
}

// �X�V
void CSlash::Update()
{
	CVector move = mMoveSpeed * Times::DeltaTime();
	float dist = move.Length();
	if (mMovedDist + dist >= mKillMoveDist)
	{
		dist = mKillMoveDist - mMovedDist;
		move = move.Normalized() * dist;
	}
	Position(Position() + move);

	mMovedDist += dist;
	if (mMovedDist >= mKillMoveDist)
	{
		Kill();
	}
}

// �`��
void CSlash::Render()
{
	mpModel->SetColor(mColor);
	mpModel->Render(Matrix());
}