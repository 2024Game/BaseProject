#include "CTrailEffect.h"

// �R���X�g���N�^
CTrailEffect::CTrailEffect(ETag tag,
	CObjectBase* owner, CMatrix* mtx, CVector offsetPos,
	float updateInterval, float updateDist,
	float startWidth, float endWidth, float showTime)
	: CLineEffect(tag)
	, mpOwner(owner)
	, mpMatrix(mtx)
	, mOffsetPos(offsetPos)
	, mUpdateInterval(updateInterval)
	, mUpdateDistance(updateDist)
	, mStartWidth(startWidth)
	, mEndWidth(endWidth)
	, mShowTime(showTime)
	, mLastPos(0.0f, 0.0f, 0.0f)
	, mElapsedTime(0.0f)
{
	CVector pos = GetCurrentPos();
	AddPoint(pos, mStartWidth, mEndWidth);
	AddPoint(pos, mStartWidth, mEndWidth);
	mLastPos = pos;
}

// �f�X�g���N�^
CTrailEffect::~CTrailEffect()
{
}

// �������ݒ�
void CTrailEffect::SetOwner(CObjectBase* owner)
{
	mpOwner = owner;
	UpdateLastPoint();
}

// �ǐՂ���s���ݒ�
void CTrailEffect::SetFollowMatrix(CMatrix* mtx)
{
	mpMatrix = mtx;
	UpdateLastPoint();
}

//// �ݒ肳�ꂽ������܂��͍s��̈ʒu����̃I�t�Z�b�g�l��ݒ�
//void CTrailEffect::SetOffsetPos(const CVector& pos)
//{
//	mOffsetPos = pos;
//	UpdateLastPoint();
//	if (mPoints.size() == 1)
//	{
//		mLastPos = mPoints[0]->pos;
//	}
//}

// ���ݒn���擾
CVector CTrailEffect::GetCurrentPos() const
{
	CVector pos = Position();

	// �s�񂪐ݒ肳��Ă�����A�s��̍��W��Ԃ�
	if (mpMatrix != nullptr)
	{
		pos = mpMatrix->Position();
	}
	// �����傪�ݒ肳��Ă�����A������̍��W��Ԃ�
	else if (mpOwner != nullptr)
	{
		pos = mpOwner->Position();
	}

	return pos + mOffsetPos;
}

// ���̍Ō�̃|�C���g�̈ʒu���X�V
void CTrailEffect::UpdateLastPoint()
{
	if (mpOwner == nullptr && mpMatrix == nullptr) return;

	int index = mPoints.size() - 1;
	if (index < 0) return;

	SetPos(index, GetCurrentPos());
}

// �X�V
void CTrailEffect::Update()
{
	if (IsKill()) return;

	CLineEffect::Update();

	// �s�񂩎����傪�ݒ肳��Ă���΁A���ݒn���X�V
	if (mpMatrix != nullptr || mpOwner != nullptr)
	{
		// ���ݒn���擾
		CVector pos = GetCurrentPos();
		// �O��̃|�C���g�����苗������Ă�����A
		float dist = (pos - mLastPos).Length();

		// �X�V�Ԋu���X�V�����𒴂�����A
		if (mElapsedTime >= mUpdateInterval || dist >= mUpdateDistance)
		{
			// �V�����|�C���g��ǉ�
			AddPoint(pos, mStartWidth, mEndWidth);
			mLastPos = pos;
			mElapsedTime = std::max(mElapsedTime - mUpdateInterval, 0.0f);
		}
		// �X�V�^�C�~���O�łȂ���΁A�Ō�̃|�C���g�̈ʒu���X�V
		else
		{
			UpdateLastPoint();
		}
		// �X�V�Ԋu�p�̌o�ߎ��Ԃ����Z
		mElapsedTime += Times::DeltaTime();
	}

	// ���̃|�C���g�̐���1�ȉ����A
	// ��������s����ݒ肳��Ă��Ȃ���Ԃł���΁A
	int size = mPoints.size();
	if (size <= 1 && mpOwner == nullptr && mpMatrix == nullptr)
	{
		// �G�t�F�N�g���폜
		Kill();
		return;
	}

	std::list<int> removes;
	// �e�|�C���g�̍X�V
	for (int i = 0; i < size; i++)
	{
		CLinePoint* point = mPoints[i];
		if (point == nullptr) continue;
		// �\�����Ԃɍ��킹�āA���X�ɓ��߂��Ă���
		if (point->elapsed < mShowTime)
		{
			float alpha = point->elapsed / mShowTime;
			point->alpha = 1.0f - alpha;

			// �\�����ԂɌo�ߎ��Ԃ����Z
			point->elapsed += Times::DeltaTime();
		}
		// �\�����Ԃ��I���΁A��菜�����X�g�ɒǉ�
		else
		{
			point->alpha = 0.0f;
			removes.push_back(i);
		}
	}

	// ��菜�����X�g�ɓo�^���ꂽ�|�C���g����菜��
	for (int i : removes)
	{
		RemovePoint(i);
	}
}
