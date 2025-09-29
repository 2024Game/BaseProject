#include "CColLine.h"
#include "Primitive.h"

// �R���X�g���N�^
CColLine::CColLine(const CVector& start, const CVector& end, const CColor& defaultColor,
	bool isKinematic, float weight)
	: CColBase(defaultColor)
	, mStartPos(start)
	, mEndPos(end)
{
	mpCollider = new CColliderLine
	(
		this, ELayer::eTest,
		mStartPos, mEndPos,
		isKinematic, weight
	);
	mpCollider->SetCollisionLayers({ ELayer::eTest });
}

// �f�X�g���N�^
CColLine::~CColLine()
{
	SAFE_DELETE(mpCollider);
}

// �Փˏ���
void CColLine::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	CColBase::Collision(self, other, hit);

	//if (mIsSelect)
	{
		//CColliderSphere* cs = dynamic_cast<CColliderSphere*>(other);
		//if (cs != nullptr)
		{
			Position(Position() + hit.adjust * hit.weight);
		}
	}
}

// �`��
void CColLine::Render()
{
	CMatrix m = Matrix();
	CVector s = mStartPos * m;
	CVector e = mEndPos * m;
	Primitive::DrawLine
	(
		s, e,
		GetColor(), 3.0f
	);
}
