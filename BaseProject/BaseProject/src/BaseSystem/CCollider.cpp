#include "CCollider.h"
#include "CCollisionManager.h"
#include "CColliderLine.h"
#include "CColliderSphere.h"
#include "CColliderTriangle.h"
#include "CColliderCapsule.h"
#include "CColliderMesh.h"
#include "CObjectBase.h"
#include "Maths.h"
#include "Primitive.h"

// �R���X�g���N�^
CCollider::CCollider(CObjectBase* owner, ELayer layer, EColliderType type,
	bool isKinematic, float weight)
	: mLastMtx(CMatrix::zero)
	, mLayer(layer)
	, mType(type)
	, mpOwner(owner)
	, mIsEnable(true)
	, mIsShow(true)
	, mIsKinematic(isKinematic)
	, mWeight(weight)
	, mCollisionLayers(~0)
	, mCollisionTags(~0)
	, mpAttachMtx(nullptr)
{
	// �R���W�������X�g�ɒǉ�
	CCollisionManager::Instance()->Add(this);
}

// �f�X�g���N�^
CCollider::~CCollider()
{
	// �R���W�������X�g����폜
	CCollisionManager::Instance()->Remove(this);
}

// �Փ˔��背�C���[���擾
ELayer CCollider::Layer() const
{
	return mLayer;
}

// �R���C�_�[�̎�ނ��擾
EColliderType CCollider::Type() const
{
	return mType;
}

// �R���C�_�[�̎������Ԃ�
CObjectBase* CCollider::Owner() const
{
	return mpOwner;
}

// �R���C�_�[�̎�����̃^�O���擾
ETag CCollider::Tag() const
{
	// �����傪���݂���΁A������̃^�O��Ԃ�
	if (mpOwner == nullptr) return ETag::eNone;
	return mpOwner->Tag();
}

// �R���C�_�[���L�����ǂ�����ݒ�
void CCollider::SetEnable(bool isEnable)
{
	mIsEnable = isEnable;
}

// �R���C�_�[���L�����ǂ������擾
bool CCollider::IsEnable() const
{
	return mIsEnable;
}

// �R���C�_�[���f�o�b�O�\�����邩�ǂ�����ݒ�
void CCollider::SetShow(bool isShow)
{
	mIsShow = isShow;
}

// �R���C�_�[���f�o�b�O�\�����邩�ǂ���
bool CCollider::IsShow() const
{
	return mIsShow;
}

// �Փˎ��̉����߂��̉e�����󂯂邩�ǂ�����ݒ�
void CCollider::SetKinematic(bool iskinematic)
{
	mIsKinematic = iskinematic;
}

// �Փˎ��̉����߂��̉e�����󂯂邩�ǂ���
bool CCollider::IsKinematic() const
{
	return mIsKinematic;
}

// �R���C�_�[�̏d�ʂ�ݒ�
void CCollider::SetWeight(float weight)
{
	mWeight = weight;
}

// �R���C�_�[�̏d�ʂ��擾
float CCollider::GetWeight() const
{
	return mWeight;
}

// �w�肵���R���C�_�[�ƏՓ˔�����s�����ǂ������擾
bool CCollider::IsCollision(CCollider* col) const
{
	// �������g�������͑���̃R���C�_�[���L���łȂ���΁A
	// �Փ˔�����s��Ȃ�
	if (col == nullptr) return false;
	if (!col->IsEnable()) return false;
	if (!mIsEnable) return false;

	// �����R���C�_�[�������́A�����傪�����ł���΁A
	// �Փ˔�����s��Ȃ�
	if (col == this) return false;
	if (col->Owner() != nullptr || mpOwner != nullptr)
	{
		if (col->Owner() == mpOwner) return false;
	}

	// �Փ˔�����s�����C���[��
	if (!IsCollisionLayer(col->Layer())) return false;
	// �Փ˔�����s���I�u�W�F�N�g�^�O��
	if (!IsCollisionTag(col->Tag())) return false;

	// �S�Ă̏����𖞂������ꍇ�͏Փ˔�����s��
	return true;
}

// �Փ˔�����s�����C���[��ݒ�
void CCollider::SetCollisionLayers(Layers layers)
{
	mCollisionLayers = 0;
	for (auto&& layer : layers)
	{
		mCollisionLayers |= 1 << (int)layer;
	}
}

// �w�肵�����C���[�Ƃ̏Փ˔�����s�����ǂ�����ݒ�
void CCollider::SetCollisionLayer(ELayer layer, bool isCollision)
{
	if (isCollision) mCollisionLayers |= 1 << (int)layer;
	else mCollisionLayers &= ~(1 << (int)layer);
}

// �w�肵�����C���[�ƏՓ˔�����s�����ǂ������擾
bool CCollider::IsCollisionLayer(ELayer layer) const
{
	return (mCollisionLayers & 1 << (int)layer) != 0;
}

// �Փ˔�����s���I�u�W�F�N�g�^�O��ݒ�
void CCollider::SetCollisionTags(Tags tags)
{
	mCollisionTags = 0;
	for (auto&& tag : tags)
	{
		mCollisionTags |= 1 << (int)tag;
	}
}

// �w�肵���I�u�W�F�N�g�^�O�Ƃ̏Փ˔�����s�����ǂ�����ݒ�
void CCollider::SetCollisionTag(ETag tag, bool isCollision)
{
	if (isCollision) mCollisionTags |= 1 << (int)tag;
	else mCollisionTags &= ~(1 << (int)tag);
}

// �w�肵���I�u�W�F�N�g�^�O�ƏՓ˔�����s�����ǂ������擾
bool CCollider::IsCollisionTag(ETag tag) const
{
	return (mCollisionTags & 1 << (int)tag) != 0;
}

// �w�肵���s��ɃR���C�_�[����������
void CCollider::SetAttachMtx(const CMatrix* mtx)
{
	mpAttachMtx = mtx;
}

// �s����擾
CMatrix CCollider::Matrix() const
{
	CMatrix m = CTransform::Matrix();
	// ��������s�񂪐ݒ肳��Ă����
	if (mpAttachMtx != nullptr)
	{
		// ���̍s��ɂ�������
		m = m * mAttachMtx;
	}
	// �����傪�ݒ肳��Ă����
	else if (mpOwner != nullptr)
	{
		// ������̍s��ɕ���
		m = m * mpOwner->Matrix();
	}
	return m;
}

// �o�E���f�B���O�{�b�N�X���擾
const CBounds& CCollider::Bounds() const
{
	return mBounds;
}

// �R���C�_�[�X�V
void CCollider::Update()
{
	// ��������s�񂪐ݒ肳��Ă���΁A
	if (mpAttachMtx != nullptr)
	{
		// ��������s��������o�ϐ��ɃR�s�[
		mAttachMtx = *mpAttachMtx;
	}

	// �R���C�_�[�̏��̍X�V
	UpdateCol();
}

#if _DEBUG
// �R���C�_�[�̃o�E���f�B���O�{�b�N�X��`��
void CCollider::RenderBounds()
{
	CColor col = CColor::yellow;
	if (!IsEnable() ||
		(Owner() != nullptr && !Owner()->IsEnableCol()))
	{
		col = CColor::gray;
	}
	col.A(0.25f);
	Primitive::DrawWireBox
	(
		mBounds.Center(), mBounds.Size(),
		col, EBlend::eAlpha
	);
}
#endif

// ��`���m�̏Փ˔���
bool CCollider::CollisionRect(const CRect& rect1, const CRect& rect2)
{
	if (rect1.Left() <= rect2.Right() && rect1.Right() >= rect2.Left() &&
		rect1.Top() <= rect2.Bottom() && rect1.Bottom() >= rect2.Top())
	{
		return true;
	}
	return false;
}

// ��`�Ɠ_�̏Փ˔���
bool CCollider::CollisionRectPoint(const CRect& rect, const CVector2 point)
{
	if (rect.Left() <= point.X() && rect.Right() >= point.X() &&
		rect.Top() <= point.Y() && rect.Bottom() >= point.Y())
	{
		return true;
	}
	return false;
}

// �R���C�_�[�̐ݒ�
void CCollider::Set(CObjectBase* owner, ELayer layer)
{
	mpOwner = owner;
	mLayer = layer;
}

// �O�p�`�ƎO�p�`�̏Փ˔���
bool CCollider::CollisionTriangle(const CVector& t00, const CVector& t01, const CVector& t02,
	const CVector& t10, const CVector& t11, const CVector& t12,
	CHitInfo* hit)
{
	return false;
}

// �O�p�`�Ɛ����̏Փ˔���
bool CCollider::CollisionTriangleLine(
	const CVector& t0, const CVector& t1, const CVector& t2,
	const CVector& ls, const CVector& le,
	CHitInfo* h, bool isLeftMain)
{
	//�ʂ̖@�����A�O�ς𐳋K�����ċ��߂�
	CVector normal = CVector::Cross(t1 - t0, t2 - t0).Normalized();
	//�O�p�̒��_��������n�_�ւ̃x�N�g�������߂�
	CVector v0sv = ls - t0;
	//�O�p�̒��_��������I�_�ւ̃x�N�g�������߂�
	CVector v0ev = le - t0;
	//�������ʂƌ������Ă��邩���ςŊm�F����
	float dots = v0sv.Dot(normal);
	float dote = v0ev.Dot(normal);
	//�v���X�͌������ĂȂ�
	if (dots * dote >= 0.0f) {
		//�Փ˂��ĂȂ��i�����s�v�j
		h->adjust = CVector(0.0f, 0.0f, 0.0f);
		return false;
	}

	//�����͖ʂƌ������Ă���
	//�ʂƐ����̌�_�����߂�
	//��_�̌v�Z
	CVector cross = ls + (le - ls) * (abs(dots) / (abs(dots) + abs(dote)));

	//��_���O�p�`���Ȃ�Փ˂��Ă���
	if (!IsInsideTriangle(cross, t0, t1, t2, normal))
	{
		//�O�p�`�O�Ȃ̂ŁA�Փ˂��ĂȂ�
		h->adjust = CVector(0.0f, 0.0f, 0.0f);
		return false;
	}

	//�q�b�g���Ɍ�_��ݒ�
	h->cross = cross;

	//�����l�v�Z�i�Փ˂��Ȃ��ʒu�܂Ŗ߂��j
	if (dots < 0.0f) {
		//�n�_������
		h->adjust = normal * dots;
	}
	else {
		//�I�_������
		h->adjust = normal * dote;
	}
	if (!isLeftMain) h->adjust = -h->adjust;
	return true;
}

// �O�p�`�ƃ��C�̏Փ˔���
bool CCollider::CollisionTriangleRay(
	const CVector& t0, const CVector& t1, const CVector& t2,
	const CVector& rs, const CVector& re,
	CHitInfo* h, bool isLeftMain)
{
	//�ʂ̖@�����A�O�ς𐳋K�����ċ��߂�
	CVector normal = CVector::Cross(t1 - t0, t2 - t0).Normalized();
	//�ʂ̖@���ƃ��C�̌��������������������Ă�����
	if (CVector::Dot(normal, re - rs) > 0.0f)
	{
		//�Փ˂��ĂȂ��i�����s�v�j
		h->adjust = CVector(0.0f, 0.0f, 0.0f);
		return false;
	}

	//�O�p�̒��_��������n�_�ւ̃x�N�g�������߂�
	CVector v0sv = rs - t0;
	//�O�p�̒��_��������I�_�ւ̃x�N�g�������߂�
	CVector v0ev = re - t0;
	//�������ʂƌ������Ă��邩���ςŊm�F����
	float dots = v0sv.Dot(normal);
	float dote = v0ev.Dot(normal);
	//�v���X�͌������ĂȂ�
	if (dots * dote >= 0.0f) {
		//�Փ˂��ĂȂ��i�����s�v�j
		h->adjust = CVector(0.0f, 0.0f, 0.0f);
		return false;
	}

	//�����͖ʂƌ������Ă���
	//�ʂƐ����̌�_�����߂�
	//��_�̌v�Z
	CVector cross = rs + (re - rs) * (abs(dots) / (abs(dots) + abs(dote)));

	//��_���O�p�`���Ȃ�Փ˂��Ă���
	if (!IsInsideTriangle(cross, t0, t1, t2, normal))
	{
		//�O�p�`�O�Ȃ̂ŁA�Փ˂��ĂȂ�
		h->adjust = CVector(0.0f, 0.0f, 0.0f);
		return false;
	}

	//�q�b�g���Ɍ�_��ݒ�
	h->cross = cross;

	//�����l�v�Z�i�Փ˂��Ȃ��ʒu�܂Ŗ߂��j
	if (dots < 0.0f) {
		//�n�_������
		h->adjust = normal * dots;
	}
	else {
		//�I�_������
		h->adjust = normal * dote;
	}
	if (!isLeftMain) h->adjust = -h->adjust;
	return true;
}

// �w�肵���_�ƎO�p�`�̍ŋߐړ_���v�Z
CVector CCollider::ClosestPointOnTriangle(const CVector& p, const CVector& t0, const CVector& t1, const CVector& t2)
{
	CVector v01 = t1 - t0;
	CVector v02 = t2 - t0;
	CVector v0p = p - t0;

	float d1 = CVector::Dot(v01, v0p);
	float d2 = CVector::Dot(v02, v0p);
	if (d1 <= 0.0f && d2 <= 0.0f) return t0;

	CVector v1p = p - t1;
	float d3 = CVector::Dot(v01, v1p);
	float d4 = CVector::Dot(v02, v1p);
	if (d3 >= 0.0f && d4 <= d3) return t1;

	float v2 = d1 * d4 - d3 * d2;
	if (v2 <= 0.0f && d1 >= 0.0f && d3 <= 0.0f)
	{
		float v = d1 / (d1 - d3);
		return t0 + v01 * v;
	}

	CVector v2p = p - t2;
	float d5 = CVector::Dot(v01, v2p);
	float d6 = CVector::Dot(v02, v2p);
	if (d6 >= 0.0f && d5 <= d6) return t2;

	float v1 = d5 * d2 - d1 * d6;
	if (v1 <= 0.0f && d2 >= 0.0f && d6 <= 0.0f)
	{
		float w = d2 / (d2 - d6);
		return t0 + v02 * w;
	}

	float v0 = d3 * d6 - d5 * d4;
	if (v0 <= 0.0f && (d4 - d3) >= 0.0f && (d5 - d6) >= 0.0f)
	{
		float w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
		return t1 + (t2 - t1) * w;
	}

	float denom = 1.0f / (v0 + v1 + v2);
	float v = v1 * denom;
	float w = v2 * denom;
	return t0 + v01 * v + v02 * w;
}

// �O�p�`�ƃJ�v�Z���̏Փ˔���
bool CCollider::CollisionTriangleCapsule(
	const CVector& t0, const CVector& t1, const CVector& t2,
	const CVector& cs, const CVector& ce, float cr,
	CHitInfo* h, bool isLeftMain)
{
	const int steps = 5;
	float minDist = FLT_MAX;
	CVector adjust = CVector::zero;

	for (int i = 0; i <= steps; i++)
	{
		float t = float(i) / steps;
		CVector p = CVector::Lerp(cs, ce, t);
		CVector q = ClosestPointOnTriangle(p, t0, t1, t2);

		CVector diff = p - q;
		float dist = diff.Length();

		if (dist < cr)
		{
			float len = cr - dist;
			CVector dir = (dist > EPSILON) ? diff.Normalized() : CVector::up;
			adjust = dir * len;
			minDist = dist;
		}
	}

	if (minDist < cr)
	{
		h->adjust = isLeftMain ? -adjust : adjust;
		return true;
	}

	h->adjust = CVector::zero;
	return false;
}

// �O�p�`�Ɠ_�̏Փ˔���
bool CCollider::CollisionTrianglePoint(const CVector& t0, const CVector& t1, const CVector& t2, const CVector& tn, const CVector& p)
{
	if (CVector::Dot(CVector::Cross(t1 - t0, p - t0), tn) < 0.0f) return false;
	if (CVector::Dot(CVector::Cross(t2 - t1, p - t1), tn) < 0.0f) return false;
	if (CVector::Dot(CVector::Cross(t0 - t2, p - t2), tn) < 0.0f) return false;
	return true;
}

// �O�p�`�Ɠ_�̏Փ˔���
bool CCollider::CollisionTrianglePoint(const CVector& t0, const CVector& t1, const CVector& t2, const CVector& p)
{
	CVector n = CVector::Cross(t1 - t0, t2 - t0).Normalized();
	return CollisionTrianglePoint(t0, t1, t2, n, p);
}

// �O�p�`�Ƌ��̏Փ˔���
bool CCollider::CollisionTriangleSphere(
	const CVector& t0, const CVector& t1, const CVector& t2,
	const CVector& sp, const float sr,
	CHitInfo* h, bool isLeftMain)
{
	// ���̒��S���W�ƎO�p�`�|���S���̍ŋߐړ_�����߂�
	CVector q = ClosestPointOnTriangle(sp, t0, t1, t2);

	// �ŋߐړ_�܂ł̋��������a��菬�����ƏՓ˂��Ă���
	CVector diff = sp - q;
	float dist = diff.Length();
	if (dist < sr)
	{
		// �����߂��ʂƉ����߂��������v�Z
		float len = sr - dist;
		CVector dir = (dist > EPSILON) ? diff.Normalized() : CVector::up;
		CVector adjust = dir * len;
		h->adjust = isLeftMain ? -adjust : adjust;
		return true;
	}

	// �Փ˂��Ă��Ȃ�
	return false;
}

// ���Ƌ��̏Փ˔���
bool CCollider::CollisionSphere(const CVector& sp0, const float sr0,
	const CVector& sp1, const float sr1, CHitInfo* hit)
{
	//���S���璆�S�ւ̃x�N�g�������߂�
	CVector vec = sp0 - sp1;
	float length = vec.Length();
	//���S�̋��������a�̍��v��菬�����ƏՓ�
	float sum = sr0 + sr1;
	if (sum > length) {
		hit->adjust = vec.Normalized() * (sum - length);
		//�Փ˂��Ă���
		return  true;
	}

	hit->adjust = CVector(0.0f, 0.0f, 0.0f);
	//�Փ˂��Ă��Ȃ�
	return false;
}

// ���Ɛ����̏Փ˔���
bool CCollider::CollisionSphereLine(const CVector& sp, const float sr,
	const CVector& ls, const CVector& le,
	CHitInfo* hit, bool isLeftMain)
{
	CVector nearest;
	float length = CalcDistancePointToLine(sp, ls, le, &nearest);
	if (length < sr)
	{
		CVector n = (sp - nearest).Normalized() * (isLeftMain ? 1.0f : -1.0f);
		hit->adjust = n * (sr - length);

		CVector v = (le - ls).Normalized();
		CVector p = (sp - ls).Normalized();
		float a = CVector::Dot(v, v);
		float b = CVector::Dot(v, p);
		float c = CVector::Dot(p, p) - sr * sr;
		if (a != 0.0f) hit->cross = ls + v * ((b - sqrtf(b * b - a * c)) / a);
		else hit->cross = CVector::zero;
		return true;
	}

	hit->adjust = CVector(0.0f, 0.0f, 0.0f);
	return false;
}

// ���ƃJ�v�Z���̏Փ˔���
bool CCollider::CollisionSphereCapsule(
	const CVector& sp, const float sr,
	const CVector& cs, const CVector& ce, float cr,
	CHitInfo* hit, bool isLeftMain)
{
	CVector nearest;
	float length = CalcDistancePointToLine(sp, cs, ce, &nearest);
	if (length < sr + cr)
	{
		CVector n = (sp - nearest).Normalized() * (isLeftMain ? 1.0f : -1.0f);
		hit->adjust = n * ((sr + cr) - length);

		CVector v = (ce - cs).Normalized();
		CVector p = (sp - cs).Normalized();
		float a = CVector::Dot(v, v);
		float b = CVector::Dot(v, p);
		float c = CVector::Dot(p, p) - sr * sr;
		if (a != 0.0f) hit->cross = cs + v * ((b - sqrtf(b * b - a * c)) / a);
		else hit->cross = CVector::zero;
		return true;
	}

	hit->adjust = CVector(0.0f, 0.0f, 0.0f);
	return false;
}

// �����Ɛ����̏Փ˔���
bool CCollider::CollisionLine(const CVector& ls0, const CVector& le0,
	const CVector& ls1, const CVector& le1, CHitInfo* hit)
{
	//TODO:�����l�̑Ή�
	hit->adjust = CVector(0.0f, 0.0f, 0.0f);

	CVector V0 = le0 - ls0;
	CVector V1 = le1 - ls1;

	CVector S1E1 = le0 - ls0;
	CVector S2E2 = le1 - ls1;
	CVector CD = CVector::Cross(V0, V1).Normalized();

	CVector S1S2 = ls1 - ls0;
	CVector S1E2 = le1 - ls0;
	CVector S2S1 = ls0 - ls1;
	CVector S2E1 = le0 - ls1;

	float length = 0.0f;
	float d1 = S1E1.Cross(S1S2).Dot(S1E1.Cross(S1E2));
	float d2 = S2E2.Cross(S2S1).Dot(S2E2.Cross(S2E1));
	if (d1 < 0 && d2 < 0)
	{
		length = abs(S1S2.Dot(CD));
	}
	else
	{
		float length1 = CalcDistancePointToLine(ls0, ls1, le1);
		float length2 = CalcDistancePointToLine(le0, ls1, le1);
		float length3 = CalcDistancePointToLine(ls1, ls0, le0);
		float length4 = CalcDistancePointToLine(le1, ls0, le0);
		length = fminf(fminf(length1, length2), fminf(length3, length4));
	}

	if (length < 0.1f + 0.1f)
	{
		return true;
	}

	return false;
}

// �J�v�Z���Ɛ����̏Փ˔���
bool CCollider::CollisionCapsuleLine(
	const CVector& cs, const CVector& ce, float cr,
	const CVector& ls, const CVector& le,
	CHitInfo* hit, bool isLeftMain)
{
	hit->adjust = CVector(0.0f, 0.0f, 0.0f);

	CVector SE0 = ce - cs;
	CVector SE1 = le - ls;
	CVector S10 = cs - ls;

	float L0 = SE0.LengthSqr();
	float L1 = SE1.LengthSqr();
	float d0 = CVector::Dot(SE0, S10);
	float d1 = CVector::Dot(SE1, S10);

	float t0, t1;
	if (L0 <= EPSILON && L1 <= EPSILON)
	{
		t0 = 0.0f;
		t1 = 0.0f;
	}
	else if (L0 <= EPSILON)
	{
		t0 = 0.0f;
		t1 = Math::Clamp01(d1 / L1);
	}
	else if (L1 <= EPSILON)
	{
		t0 = Math::Clamp01(-d0 / L0);
		t1 = 0.0f;
	}
	else
	{
		float d01 = CVector::Dot(SE0, SE1);
		float dn = L0 * L1 - d01 * d01;

		t0 = 0.0f;
		if (dn != 0.0f)
		{
			t0 = Math::Clamp01((d01 * d1 - d0 * L1) / dn);
		}

		t1 = (d01 * t0 + d1) / L1;

		if (t1 < 0.0f)
		{
			t1 = 0.0f;
			t0 = Math::Clamp01(-d0 / L0);
		}
		else if (t1 > 1.0f)
		{
			t1 = 1.0f;
			t0 = Math::Clamp01((d01 - d0) / L0);
		}
	}

	CVector C1 = cs + SE0 * t0;
	CVector C2 = ls + SE1 * t1;

	float dist = CVector::Distance(C1, C2);
	if (dist < cr)
	{
		CVector n = (C1 - C2).Normalized();
		float length = cr - dist;
		hit->adjust = n * length;

		return true;
	}

	return false;
}

// �J�v�Z���ƃJ�v�Z���̏Փ˔���
bool CCollider::CollisionCapsule(const CVector& cs0, const CVector& ce0, float cr0, const CVector& cs1, const CVector& ce1, float cr1, CHitInfo* hit)
{
	hit->adjust = CVector(0.0f, 0.0f, 0.0f);

	CVector SE0 = ce0 - cs0;
	CVector SE1 = ce1 - cs1;
	CVector S10 = cs0 - cs1;

	float L0 = SE0.LengthSqr();
	float L1 = SE1.LengthSqr();
	float d0 = CVector::Dot(SE0, S10);
	float d1 = CVector::Dot(SE1, S10);

	// �ŋߓ_�܂ł̋��������߂�
	float t0, t1;
	// ���J�v�Z�����_�������ꍇ
	if (L0 <= EPSILON && L1 <= EPSILON)
	{
		t0 = 0.0f;
		t1 = 0.0f;
	}
	// �J�v�Z��1���_�������ꍇ
	else if (L0 <= EPSILON)
	{
		t0 = 0.0f;
		t1 = Math::Clamp01(d1 / L1);
	}
	// �J�v�Z��2���_�������ꍇ
	else if (L1 <= EPSILON)
	{
		t0 = Math::Clamp01(-d0 / L0);
		t1 = 0.0f;
	}
	// �����Ƃ��J�v�Z���������ꍇ
	else
	{
		float d01 = CVector::Dot(SE0, SE1);
		float dn = L0 * L1 - d01 * d01;

		t0 = 0.0f;
		if (dn != 0.0f)
		{
			t0 = Math::Clamp01((d01 * d1 - d0 * L1) / dn);
		}

		t1 = (d01 * t0 + d1) / L1;

		if (t1 < 0.0f)
		{
			t1 = 0.0f;
			t0 = Math::Clamp01(-d0 / L0);
		}
		else if (t1 > 1.0f)
		{
			t1 = 1.0f;
			t0 = Math::Clamp01((d01 - d0) / L0);
		}
	}

	// �ŋߓ_�����߂�
	CVector C1 = cs0 + SE0 * t0;
	CVector C2 = cs1 + SE1 * t1;

	// �ŒZ���������߁A���J�v�Z���̔��a�̍��v�l���
	// �������ꍇ�͏Փ˂��Ă���
	float dist = CVector::Distance(C1, C2);
	if (dist < cr0 + cr1)
	{
		// ���߂��ŋߓ_�̕����։����߂�
		CVector n = (C1 - C2).Normalized();
		float length = cr0 + cr1 - dist;
		hit->adjust = n * length;

		return true;
	}

	// �Փ˂��Ȃ�����
	return false;
}

// ���b�V���Ɛ����̏Փ˔���
bool CCollider::CollisionMeshLine(const std::vector<STVertexData>& tris,
	const CVector& ls, const CVector& le, const CBounds& lb,
	CHitInfo* hit, bool isLeftMain)
{
	bool ret = false;
	CVector adjust = CVector::zero;
	CVector cross = CVector::zero;
	float nearDist = 0.0f;
	bool isFirst = true;
	for (auto& v : tris)
	{
		if (!CBounds::Intersect(v.bounds, lb)) continue;
		if (CollisionTriangleLine(v.wv.V[0], v.wv.V[1], v.wv.V[2], ls, le, hit, isLeftMain))
		{
			hit->tris.push_back(v.wv);

			CVector adj = hit->adjust;
			adjust.X(abs(adjust.X()) > abs(adj.X()) ? adjust.X() : adj.X());
			adjust.Y(abs(adjust.Y()) > abs(adj.Y()) ? adjust.Y() : adj.Y());
			adjust.Z(abs(adjust.Z()) > abs(adj.Z()) ? adjust.Z() : adj.Z());

			if (isFirst)
			{
				cross = hit->cross;
				nearDist = (cross - ls).Length();
				isFirst = false;
			}
			else
			{
				float dist = (hit->cross - ls).Length();
				if (dist < nearDist)
				{
					cross = hit->cross;
					nearDist = dist;
				}
			}

			ret = true;
		}
	}
	hit->adjust = adjust;
	hit->cross = cross;
	hit->dist = nearDist;
	return ret;
}

// ���b�V���ƃ��C�̏Փ˔���
bool CCollider::CollisionMeshRay(CColliderMesh* mesh,
	const CVector& rs, const CVector& re, const CBounds& rb,
	CHitInfo* hit, bool isLeftMain)
{
	bool ret = false;
	CVector adjust = CVector::zero;
	float nearDist = -1.0f;
	CVector start = rs;
	CVector end = re;
	CBounds lb = rb;
	const std::vector<STDivMesh>& divMesh = mesh->GetDivMesh();
	for (const STDivMesh& dm : divMesh)
	{
		if (!CBounds::Intersect(dm.bounds, lb)) continue;
		for (STVertexData* v : dm.vertices)
		{
			if (!CBounds::Intersect(v->bounds, lb)) continue;
			if (CollisionTriangleRay(v->wv.V[0], v->wv.V[1], v->wv.V[2], start, end, hit, isLeftMain))
			{
				hit->tris.push_back(v->wv);

				CVector adj = hit->adjust;
				adjust.X(abs(adjust.X()) > abs(adj.X()) ? adjust.X() : adj.X());
				adjust.Y(abs(adjust.Y()) > abs(adj.Y()) ? adjust.Y() : adj.Y());
				adjust.Z(abs(adjust.Z()) > abs(adj.Z()) ? adjust.Z() : adj.Z());

				if (nearDist < 0.0f)
				{
					end = hit->cross;
					nearDist = (end - start).LengthSqr();
					lb = CBounds::GetLineBounds(start, end);
				}
				else
				{
					float dist = (end - start).LengthSqr();
					if (dist < nearDist)
					{
						end = hit->cross;
						nearDist = dist;
					}
				}

				ret = true;
			}
		}
	}
	hit->adjust = adjust;
	hit->cross = end;
	hit->dist = sqrtf(nearDist);
	return ret;
}

// ���b�V���Ƌ��̏Փ˔���
bool CCollider::CollisionMeshSpehre(const std::vector<STVertexData>& tris,
	CColliderSphere* sphereCol, CHitInfo* hit, bool isLeftMain)
{
	CVector sp;
	float sr;
	sphereCol->Get(&sp, &sr);

	bool ret = false;
	CVector adjust = CVector::zero;
	for (auto& v : tris)
	{
		if (!CBounds::Intersect(v.bounds, sphereCol->Bounds())) continue;
		if (CollisionTriangleSphere(v.wv.V[0], v.wv.V[1], v.wv.V[2], sp, sr, hit, isLeftMain))
		{
			adjust.Y(fabsf(adjust.Y()) > fabsf(hit->adjust.Y()) ? adjust.Y() : hit->adjust.Y());
			adjust.X(fabsf(adjust.X()) > fabsf(hit->adjust.X()) ? adjust.X() : hit->adjust.X());
			adjust.Z(fabsf(adjust.Z()) > fabsf(hit->adjust.Z()) ? adjust.Z() : hit->adjust.Z());
			hit->tris.push_back(v.wv);
			ret = true;
		}
	}
	if (ret)
	{
		hit->adjust = adjust;
	}
	return ret;
}

// ���b�V���ƎO�p�`�̏Փ˔���
bool CCollider::CollisionMeshTriangle(const std::vector<STVertexData>& tris,
	CColliderTriangle* triCol, CHitInfo* hit, bool isLeftMain)
{
	CVector t0, t1, t2;
	triCol->Get(&t0, &t1, &t2);

	bool ret = false;
	for (auto& v : tris)
	{
		if (!CBounds::Intersect(v.bounds, triCol->Bounds())) continue;
		CVector v00 = isLeftMain ? v.wv.V[0] : t0;
		CVector v01 = isLeftMain ? v.wv.V[1] : t1;
		CVector v02 = isLeftMain ? v.wv.V[2] : t2;
		CVector v10 = isLeftMain ? t0 : v.wv.V[0];
		CVector v11 = isLeftMain ? t1 : v.wv.V[1];
		CVector v12 = isLeftMain ? t2 : v.wv.V[2];
		if (CollisionTriangle(v00, v01, v02, v10, v11, v12, hit))
		{
			hit->tris.push_back(v.wv);
			ret = true;
		}
	}
	return ret;
}

// ���b�V���ƃJ�v�Z���̏Փ˔���
bool CCollider::CollisionMeshCapsule(const std::vector<STVertexData>& tris,
	CColliderCapsule* capsuleCol, CHitInfo* hit, bool isLeftMain)
{
	CVector cs, ce;
	capsuleCol->Get(&cs, &ce);
	float cr = capsuleCol->Radius();

	bool ret = false;
	CVector adjust = CVector::zero;
	CVector cross = CVector::zero;
	float nearDist = 0.0f;
	bool isFirst = true;
	for (auto& v : tris)
	{
		if (!CBounds::Intersect(v.bounds, capsuleCol->Bounds())) continue;
		if (CollisionTriangleCapsule(v.wv.V[0], v.wv.V[1], v.wv.V[2], cs, ce, cr, hit, isLeftMain))
		{
			hit->tris.push_back(v.wv);

			CVector adj = hit->adjust;
			adjust.X(abs(adjust.X()) > abs(adj.X()) ? adjust.X() : adj.X());
			adjust.Y(abs(adjust.Y()) > abs(adj.Y()) ? adjust.Y() : adj.Y());
			adjust.Z(abs(adjust.Z()) > abs(adj.Z()) ? adjust.Z() : adj.Z());

			if (isFirst)
			{
				cross = hit->cross;
				nearDist = (cross - cs).Length();
				isFirst = false;
			}
			else
			{
				float dist = (hit->cross - cs).Length();
				if (dist < nearDist)
				{
					cross = hit->cross;
					nearDist = dist;
				}
			}

			ret = true;
		}
	}
	hit->adjust = adjust;
	hit->cross = cross;
	hit->dist = nearDist;
	return ret;
}

float CCollider::CalcDistancePointToLine(const CVector& point, const CVector& lineS, const CVector& lineE, CVector* nearest)
{
	CVector SE = lineE - lineS;
	float t = (point - lineS).Dot(SE.Normalized());

	CVector C;
	if (t < 0) C = lineS;
	else if (t > SE.Length()) C = lineE;
	else
	{
		C = lineS + SE.Normalized() * t;
	}

	if (nearest != nullptr) *nearest = C;

	return (C - point).Length();
}

float CCollider::CalcDistanceLine(const CVector& s1, const CVector& e1, const CVector& s2, const CVector& e2)
{
	// �������m���������邩�ǂ����ōŒZ�����̔����ς���
	CVector SE1 = e1 - s1;
	CVector SE2 = e2 - s2;
	float d1 = CVector::Dot(CVector::Cross(SE1, s2 - s1), CVector::Cross(SE1, e2 - s1));
	float d2 = CVector::Dot(CVector::Cross(SE2, s1 - s2), CVector::Cross(SE2, e1 - s2));
	// �������m���������Ă���ꍇ
	if (d1 < 0.0f && d2 < 0.0f)
	{
		CVector cv = CVector::Cross(SE1, SE2).Normalized();
		return fabsf(CVector::Dot(cv, s2 - s1));
	}
	// �������m���������Ă��Ȃ��ꍇ
	else
	{
		float l1 = CalcDistancePointToLine(s1, s2, e2);
		float l2 = CalcDistancePointToLine(e1, s2, e2);
		float l3 = CalcDistancePointToLine(s2, s1, e1);
		float l4 = CalcDistancePointToLine(e2, s1, e1);
		return std::min(std::min(l1, l2), std::min(l3, l4));
	}
}

bool CCollider::IsInsideTriangle(const CVector& p, const CVector& t0, const CVector& t1, const CVector& t2, const CVector& n)
{
	// �O�p�`�̊e�ӂƎO�p�`�̊e���_����_�܂ł̃x�N�g����
	// �����ȃx�N�g�����O�ρiCVector::Cross�j�ŋ��߂�B

	// �O�p�`�̖@���Ƌ��߂��x�N�g���̓��ρiCVector::Dot�j��
	// ���x�N�g���̌����𒲂ׂē��������������Ă��邩�𒲂ׂ�B

	// ������O�p�`�̎O�ӕ��s���A�S�ď����𖞂������ꍇ�́A
	// �O�p�`�̓����Ɣ��f����B
	if (CVector::Dot(CVector::Cross(t1 - t0, p - t0), n) < 0.0f) return false;
	if (CVector::Dot(CVector::Cross(t2 - t1, p - t1), n) < 0.0f) return false;
	if (CVector::Dot(CVector::Cross(t0 - t2, p - t2), n) < 0.0f) return false;
	return true;
}

bool CCollider::Collision(CCollider* c0, CCollider* c1, CHitInfo* hit)
{
	switch (c0->Type())
	{
	case EColliderType::eLine:
	{
		CColliderLine* line0 = dynamic_cast<CColliderLine*>(c0);
		CVector ls0, le0;
		line0->Get(&ls0, &le0);

		switch (c1->Type())
		{
		case EColliderType::eLine:
		{
			CColliderLine* line1 = dynamic_cast<CColliderLine*>(c1);
			CVector ls1, le1;
			line1->Get(&ls1, &le1);
			return CollisionLine(ls0, le0, ls1, le1, hit);
		}
		case EColliderType::eSphere:
		{
			CColliderSphere* sphere = dynamic_cast<CColliderSphere*>(c1);
			CVector sp;
			float sr;
			sphere->Get(&sp, &sr);
			return CollisionSphereLine(sp, sr, ls0, le0, hit, false);
		}
		case EColliderType::eTriangle:
		{
			CColliderTriangle* triangle = dynamic_cast<CColliderTriangle*>(c1);
			CVector t0, t1, t2;
			triangle->Get(&t0, &t1, &t2);
			return CollisionTriangleLine(t0, t1, t2, ls0, le0, hit, false);
		}
		case EColliderType::eCapsule:
		{
			CColliderCapsule* capsule = dynamic_cast<CColliderCapsule*>(c1);
			CVector cs, ce;
			capsule->Get(&cs, &ce);
			float cr = capsule->Radius();
			return CollisionCapsuleLine(cs, ce, cr, ls0, le0, hit, false);
		}
		case EColliderType::eMesh:
		{
			CColliderMesh* mesh = dynamic_cast<CColliderMesh*>(c1);
			auto tris = mesh->Get();
			return CollisionMeshLine(tris, ls0, le0, line0->Bounds(), hit, false);
		}
		}
		break;
	}
	case EColliderType::eSphere:
	{
		CColliderSphere* sphere0 = dynamic_cast<CColliderSphere*>(c0);
		CVector sp0;
		float sr0;
		sphere0->Get(&sp0, &sr0);

		switch (c1->Type())
		{
		case EColliderType::eLine:
		{
			CColliderLine* line = dynamic_cast<CColliderLine*>(c1);
			CVector ls, le;
			line->Get(&ls, &le);
			return CollisionSphereLine(sp0, sr0, ls, le, hit, true);
		}
		case EColliderType::eSphere:
		{
			CColliderSphere* sphere1 = dynamic_cast<CColliderSphere*>(c1);
			CVector sp1;
			float sr1;
			sphere1->Get(&sp1, &sr1);
			return CollisionSphere(sp0, sr0, sp1, sr1, hit);
		}
		case EColliderType::eTriangle:
		{
			CColliderTriangle* triangle = dynamic_cast<CColliderTriangle*>(c1);
			CVector t0, t1, t2;
			triangle->Get(&t0, &t1, &t2);
			return CollisionTriangleSphere(t0, t1, t2, sp0, sr0, hit, false);
		}
		case EColliderType::eCapsule:
		{
			CColliderCapsule* capsule = dynamic_cast<CColliderCapsule*>(c1);
			CVector cs, ce;
			capsule->Get(&cs, &ce);
			float cr = capsule->Radius();
			return CollisionSphereCapsule(sp0, sr0, cs, ce, cr, hit, true);
		}
		break;
		case EColliderType::eMesh:
		{
			CColliderMesh* mesh = dynamic_cast<CColliderMesh*>(c1);
			auto tris = mesh->Get();
			return CollisionMeshSpehre(tris, sphere0, hit, false);
		}
		}
		break;
	}
	case EColliderType::eTriangle:
	{
		CColliderTriangle* triangle0 = dynamic_cast<CColliderTriangle*>(c0);
		CVector t00, t01, t02;
		triangle0->Get(&t00, &t01, &t02);
		switch (c1->Type())
		{
		case EColliderType::eLine:
		{
			CColliderLine* line = dynamic_cast<CColliderLine*>(c1);
			CVector ls, le;
			line->Get(&ls, &le);
			return CollisionTriangleLine(t00, t01, t02, ls, le, hit, true);
		}
		case EColliderType::eSphere:
		{
			CColliderSphere* sphere = dynamic_cast<CColliderSphere*>(c1);
			CVector sp;
			float sr;
			sphere->Get(&sp, &sr);
			return CollisionTriangleSphere(t00, t01, t02, sp, sr, hit, true);
		}
		case EColliderType::eTriangle:
		{
			CColliderTriangle* triangle1 = dynamic_cast<CColliderTriangle*>(c1);
			CVector t10, t11, t12;
			triangle1->Get(&t10, &t11, &t12);
			return CollisionTriangle(t00, t01, t02, t10, t11, t12, hit);
		}
		case EColliderType::eCapsule:
		{
			CColliderCapsule* capsule = dynamic_cast<CColliderCapsule*>(c1);
			CVector cs, ce;
			capsule->Get(&cs, &ce);
			float cr = capsule->Radius();
			return CollisionTriangleCapsule(t00, t01, t02, cs, ce, cr, hit, true);
		}
		case EColliderType::eMesh:
		{
			CColliderMesh* mesh = dynamic_cast<CColliderMesh*>(c1);
			auto tris = mesh->Get();
			return CollisionMeshTriangle(tris, triangle0, hit, false);
		}
		}
		break;
	}
	case EColliderType::eCapsule:
	{
		CColliderCapsule* capsule0 = dynamic_cast<CColliderCapsule*>(c0);
		CVector cs0, ce0;
		capsule0->Get(&cs0, &ce0);
		float cr0 = capsule0->Radius();

		switch (c1->Type())
		{
		case EColliderType::eLine:
		{
			CColliderLine* line1 = dynamic_cast<CColliderLine*>(c1);
			CVector ls1, le1;
			line1->Get(&ls1, &le1);
			return CollisionCapsuleLine(cs0, ce0, cr0, ls1, le1, hit, false);
		}
		case EColliderType::eSphere:
		{
			CColliderSphere* sphere = dynamic_cast<CColliderSphere*>(c1);
			CVector sp;
			float sr;
			sphere->Get(&sp, &sr);
			return CollisionSphereCapsule(sp, sr, cs0, ce0, cr0, hit, false);
		}
		case EColliderType::eTriangle:
		{
			CColliderTriangle* triangle = dynamic_cast<CColliderTriangle*>(c1);
			CVector t0, t1, t2;
			triangle->Get(&t0, &t1, &t2);
			return CollisionTriangleCapsule(t0, t1, t2, cs0, ce0, cr0, hit, false);
		}
		case EColliderType::eCapsule:
		{
			CColliderCapsule* capsule1 = dynamic_cast<CColliderCapsule*>(c1);
			CVector cs1, ce1;
			capsule1->Get(&cs1, &ce1);
			float cr1 = capsule1->Radius();
			return CollisionCapsule(cs0, ce0, cr0, cs1, ce1, cr1, hit);
		}
		case EColliderType::eMesh:
		{
			CColliderMesh* mesh = dynamic_cast<CColliderMesh*>(c1);
			auto tris = mesh->Get();
			return CollisionMeshCapsule(tris, capsule0, hit, false);
		}
		}
		break;
	}
	case EColliderType::eMesh:
	{
		CColliderMesh* mesh = dynamic_cast<CColliderMesh*>(c0);
		auto tris = mesh->Get();
		switch (c1->Type())
		{
		case EColliderType::eLine:
		{
			CColliderLine* line = dynamic_cast<CColliderLine*>(c1);
			CVector ls, le;
			line->Get(&ls, &le);
			return CollisionMeshLine(tris, ls, le, line->Bounds(), hit, true);
		}
		case EColliderType::eSphere:
		{
			CColliderSphere* sphere = dynamic_cast<CColliderSphere*>(c1);
			return CollisionMeshSpehre(tris, sphere, hit, true);
		}
		case EColliderType::eTriangle:
		{
			CColliderTriangle* triangle = dynamic_cast<CColliderTriangle*>(c1);
			return CollisionMeshTriangle(tris, triangle, hit, true);
		}
		case EColliderType::eCapsule:
		{
			CColliderCapsule* capsule = dynamic_cast<CColliderCapsule*>(c1);
			return CollisionMeshCapsule(tris, capsule, hit, true);
		}
		case EColliderType::eMesh:
		{
			// ���b�V���ƃ��b�V���͏Փ˔�����s��Ȃ�
			return false;
		}
		}
		break;
	}
	}
	return false;
}

// ���C�ƃR���C�_�[�̏Փ˔���
bool CCollider::CollisionRay(CCollider* c, const CVector& start, const CVector& end, CHitInfo* hit)
{
	// �R���C�_�[��null�Ȃ�΁A�Փ˂��Ă��Ȃ�
	if (c == nullptr) return false;
	// ���C�̒�����0�Ȃ�΁A�Փ˂��Ă��Ȃ�
	CVector v = end - start;
	if (v.LengthSqr() == 0.0f) return false;
	// �R���C�_�[�ƃ��C�̃o�E���f�B���O�{�b�N�X���d�Ȃ��ĂȂ���΁A�Փ˂��Ȃ�
	CBounds lb = CBounds::GetLineBounds(start, end);
	if (!CBounds::Intersect(c->Bounds(), lb)) return false;

	// �R���C�_�[�̎�ނɂ���ďՓ˔����؂�ւ���
	switch (c->Type())
	{
		// ���R���C�_�[�Ƃ̏Փ�
	case EColliderType::eLine:
	{
		CColliderLine* line = dynamic_cast<CColliderLine*>(c);
		CVector ls, le;
		line->Get(&ls, &le);
		return CollisionLine(start, end, ls, le, hit);
	}
	// ���R���C�_�[�Ƃ̏Փ�
	case EColliderType::eSphere:
	{
		CColliderSphere* sphere = dynamic_cast<CColliderSphere*>(c);
		CVector sp;
		float sr;
		sphere->Get(&sp, &sr);
		return CollisionSphereLine(sp, sr, start, end, hit, false);
	}
	// �O�p�`�R���C�_�[�Ƃ̏Փ�
	case EColliderType::eTriangle:
	{
		CColliderTriangle* triangle = dynamic_cast<CColliderTriangle*>(c);
		CVector t0, t1, t2;
		triangle->Get(&t0, &t1, &t2);
		return CollisionTriangleRay(t0, t1, t2, start, end, hit, false);
	}
	// ���b�V���R���C�_�[�Ƃ̏Փ�
	case EColliderType::eMesh:
	{
		CColliderMesh* mesh = dynamic_cast<CColliderMesh*>(c);
		CBounds bounds = CBounds::GetLineBounds(start, end);
		return CollisionMeshRay(mesh, start, end, bounds, hit, false);
	}
	}

	// ����ȊO�͎��s
	return false;
}

// �Փˎ��̉����߂��������Z�o
float CCollider::CalcPushBackRatio(CCollider* self, CCollider* other)
{
	// ���g�̃R���C�_�[�������߂��̉e�����󂯂Ȃ�
	if (self->IsKinematic()) return 0.0f;
	// ����̃R���C�_�[�������߂��̉e�����󂯂Ȃ�
	if (other->IsKinematic()) return 1.0f;

	// �����̃R���C�_�[�������߂��̉e�����󂯂�ꍇ�́A
	// �����̃R���C�_�[�̏d�ʂŉ����߂��������Z�o
	float sw = self->GetWeight();
	float ow = other->GetWeight();
	// �����d�ʂȂ�΁A50%�̉e�����󂯂�
	if (sw == ow) return 0.5f;
	// ���g�̏d�ʂ�0�Ȃ�΁A100%�̉e�����󂯂�
	if (sw <= 0.0f) return 1.0f;
	// ����̏d�ʂ�0�Ȃ�΁A�e���͎󂯂Ȃ�
	if (ow <= 0.0f) return 0.0f;

	// �d�ʂ̊������v�Z���ĕԂ�
	return 1.0f - Math::Clamp01(sw / (sw + ow));
}
