#pragma once
#include <vector>
#include "CTransform.h"
#include "CollisionLayer.h"
#include "ColliderType.h"
#include "CVertex.h"
#include "ObjectTag.h"
#include "CBounds.h"
#include "CRect.h"

class CObjectBase;
class CCollisionManager;
class CColliderLine;
class CColliderSphere;
class CColliderTriangle;
class CColliderCapsule;
class CColliderMesh;

struct STVertexData
{
	STVertex lv;
	STVertex wv;
	CBounds bounds;
};

struct STDivMesh;

// �q�b�g���
class CHitInfo
{
public:
	CVector adjust;	// �����߂��x�N�g��
	float weight;	// �����߂�����
	CVector cross;	// �Փˈʒu
	float dist;		// �Փˈʒu�܂ł̋���
	std::list<STVertex> tris;	// �Փ˂��Ă���O�p�`���X�g
};

/// <summary>
/// �R���C�_�[�̃x�[�X�N���X
/// </summary>
class CCollider : public CTransform
{
	friend CCollisionManager;
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="owner">�R���C�_�[�̎�����</param>
	/// <param name="layer">�Փ˔���p�̃��C���[</param>
	/// <param name="type">�R���C�_�[�̎��</param>
	CCollider(CObjectBase* owner, ELayer layer, EColliderType type, bool isKinematic, float weight);
	// �f�X�g���N�^
	virtual ~CCollider();

	// �Փ˔��背�C���[���擾
	ELayer Layer() const;
	// �R���C�_�[�̎�ނ��擾
	EColliderType Type() const;
	// �R���C�_�[�̎�������擾
	CObjectBase* Owner() const;
	// �R���C�_�[�̎�����̃^�O���擾
	ETag Tag() const;

	/// <summary>
	/// �R���C�_�[���L�����ǂ�����ݒ�
	/// </summary>
	/// <param name="isEnable">�L�����ǂ���</param>
	void SetEnable(bool isEnable);
	/// <summary>
	/// �R���C�_�[���L�����ǂ������擾
	/// </summary>
	/// <returns>true�Ȃ�ΗL��</returns>
	bool IsEnable() const;

	/// <summary>
	/// �R���C�_�[���f�o�b�O�\�����邩�ǂ�����ݒ�
	/// </summary>
	/// <param name="isShow"></param>
	void SetShow(bool isShow);
	/// <summary>
	/// �R���C�_�[���f�o�b�O�\�����邩�ǂ���
	/// </summary>
	/// <returns></returns>
	bool IsShow() const;

	/// <summary>
	/// �Փˎ��̉����߂��̉e�����󂯂邩�ǂ�����ݒ�
	/// </summary>
	/// <param name="isKinematic">true�Ȃ�΁A�e�����󂯂Ȃ�</param>
	void SetKinematic(bool isKinematic);
	/// <summary>
	/// �Փˎ��̉����߂��̉e�����󂯂邩�ǂ���
	/// </summary>
	/// <returns>true�Ȃ�΁A�e�����󂯂Ȃ�</returns>
	bool IsKinematic() const;

	/// <summary>
	/// �R���C�_�[�̏d�ʂ�ݒ�
	/// </summary>
	/// <param name="weight"></param>
	void SetWeight(float weight);
	/// <summary>
	/// �R���C�_�[�̏d�ʂ��擾
	/// </summary>
	/// <returns></returns>
	float GetWeight() const;

	/// <summary>
	/// �w�肵���R���C�_�[�ƏՓ˔�����s�����ǂ������擾
	/// </summary>
	/// <param name="col">�Փ˔�����s�����m�F����R���C�_�[</param>
	/// <returns>true�Ȃ�ΏՓ˔�����s��</returns>
	bool IsCollision(CCollider* col) const;

	/// <summary>
	/// �Փ˔�����s�����C���[��ݒ�
	/// �i�ݒ�O�̏�Ԃ̓��Z�b�g����j
	/// </summary>
	/// <param name="layers">�Փ˔�����s�����C���[�̃��X�g</param>
	void SetCollisionLayers(Layers layers);
	/// <summary>
	/// �w�肵�����C���[�Ƃ̏Փ˔�����s�����ǂ�����ݒ�
	/// </summary>
	/// <param name="layer">�ݒ肷�郌�C���[</param>
	/// <param name="isCollision">�Փ˔�����s�����ǂ���</param>
	void SetCollisionLayer(ELayer layer, bool isCollision);
	/// <summary>
	/// �w�肵�����C���[�ƏՓ˔�����s�����ǂ������擾
	/// </summary>
	/// <param name="layer">���肷�郌�C���[</param>
	/// <returns>true�Ȃ�ΏՓ˔�����s��</returns>
	bool IsCollisionLayer(ELayer layer) const;

	/// <summary>
	/// �Փ˔�����s���I�u�W�F�N�g�^�O��ݒ�
	/// �i�ݒ�O�̏�Ԃ̓��Z�b�g����j
	/// </summary>
	/// <param name="tags">�Փ˔�����s���I�u�W�F�N�g�^�O�̃��X�g</param>
	void SetCollisionTags(Tags tags);
	/// <summary>
	/// �w�肵���I�u�W�F�N�g�^�O�Ƃ̏Փ˔�����s�����ǂ�����ݒ�
	/// </summary>
	/// <param name="tag">�ݒ肷��I�u�W�F�N�g�^�O</param>
	/// <param name="isCollision">�Փ˔�����s�����ǂ���</param>
	void SetCollisionTag(ETag tag, bool isCollision);
	/// <summary>
	/// �w�肵���I�u�W�F�N�g�^�O�ƏՓ˔�����s�����ǂ������擾
	/// </summary>
	/// <param name="tag">���肷��I�u�W�F�N�g�^�O</param>
	/// <returns>true�Ȃ�ΏՓ˔�����s��</returns>
	bool IsCollisionTag(ETag tag) const;

	// �w�肵���s��ɃR���C�_�[����������
	void SetAttachMtx(const CMatrix* mtx);

	// �s����擾
	CMatrix Matrix() const;

	// �o�E���f�B���O�{�b�N�X���擾
	const CBounds& Bounds() const;

	// �R���C�_�[�X�V
	void Update();

#if _DEBUG
	// �R���C�_�[�`��
	virtual void Render() = 0;
	// �R���C�_�[�̃o�E���f�B���O�{�b�N�X��`��
	virtual void RenderBounds();
#endif

	/// <summary>
	/// ��`���m�̏Փ˔���
	/// </summary>
	/// <param name="rect1">��`1</param>
	/// <param name="rect2">��`2</param>
	/// <returns>true�Ȃ�΁A�Փ˂��Ă���</returns>
	static bool CollisionRect(const CRect& rect1, const CRect& rect2);

	/// <summary>
	/// ��`�Ɠ_�̏Փ˔���
	/// </summary>
	/// <param name="rect">��`</param>
	/// <param name="point">�_</param>
	/// <returns>true�Ȃ�΁A�Փ˂��Ă���</returns>
	static bool CollisionRectPoint(const CRect& rect, const CVector2 point);

	/// <summary>
	/// �O�p�`�ƎO�p�`�̏Փ˔���
	/// </summary>
	/// <param name="t00">�O�p�`1�̒��_1</param>
	/// <param name="t01">�O�p�`1�̒��_2</param>
	/// <param name="t02">�O�p�`1�̒��_3</param>
	/// <param name="t10">�O�p�`2�̒��_1</param>
	/// <param name="t11">�O�p�`2�̒��_2</param>
	/// <param name="t12">�O�p�`2�̒��_3</param>
	/// <param name="hit">�Փ˂������̏��</param>
	/// <returns>true�Ȃ�΁A�Փ˂��Ă���</returns>
	static bool CollisionTriangle(const CVector& t00, const CVector& t01, const CVector& t02,
		const CVector& t10, const CVector& t11, const CVector& t12,
		CHitInfo* hit);

	/// <summary>
	/// �O�p�`�Ɛ����̏Փ˔���
	/// </summary>
	/// <param name="t0">�O�p�`�̒��_1</param>
	/// <param name="t1">�O�p�`�̒��_2</param>
	/// <param name="t2">�O�p�`�̒��_3</param>
	/// <param name="ls">�����̎n�_</param>
	/// <param name="le">�����̏I�_</param>
	/// <param name="hit">�Փ˂������̏��</param>
	/// <returns>true�Ȃ�΁A�Փ˂��Ă���</returns>
	static bool CollisionTriangleLine(const CVector& t0, const CVector& t1, const CVector& t2,
		const CVector& ls, const CVector& le,
		CHitInfo* hit, bool isLeftMain);

	/// <summary>
	/// �O�p�`�ƃ��C�̏Փ˔���
	/// </summary>
	/// <param name="t0">�O�p�`�̒��_1</param>
	/// <param name="t1">�O�p�`�̒��_2</param>
	/// <param name="t2">�O�p�`�̒��_3</param>
	/// <param name="rs">���C�̎n�_</param>
	/// <param name="re">���C�̏I�_</param>
	/// <param name="hit">�Փ˂������̏��</param>
	/// <returns>true�Ȃ�΁A�Փ˂��Ă���</returns>
	static bool CollisionTriangleRay(const CVector& t0, const CVector& t1, const CVector& t2,
		const CVector& rs, const CVector& re,
		CHitInfo* hit, bool isLeftMain);

	/// <summary>
	/// �O�p�`�ƃJ�v�Z���̏Փ˔���
	/// </summary>
	/// <param name="t0">�O�p�`�̒��_1</param>
	/// <param name="t1">�O�p�`�̒��_2</param>
	/// <param name="t2">�O�p�`�̒��_3</param>
	/// <param name="cs">�J�v�Z�����\����������̎n�_</param>
	/// <param name="ce">�J�v�Z�����\����������̏I�_</param>
	/// <param name="cr">�J�v�Z���̔��a</param>
	/// <param name="hit">�Փ˂������̏��</param>
	/// <returns>true�Ȃ�΁A�Փ˂��Ă���</returns>
	static bool CollisionTriangleCapsule(const CVector& t0, const CVector& t1, const CVector& t2,
		const CVector& cs, const CVector& ce, float cr,
		CHitInfo* hit, bool isLeftMain);

	/// <summary>
	/// �O�p�`�Ɠ_�̏Փ˔���
	/// </summary>
	/// <param name="t0">�O�p�`�̒��_1</param>
	/// <param name="t1">�O�p�`�̒��_2</param>
	/// <param name="t2">�O�p�`�̒��_3</param>
	/// <param name="tn">�O�p�`�̖@��</param>
	/// <param name="p">�_�̍��W</param>
	/// <returns>true�Ȃ�΁A�Փ˂��Ă���</returns>
	static bool CollisionTrianglePoint(const CVector& t0, const CVector& t1, const CVector& t2, const CVector& tn, const CVector& p);
	/// <summary>
	/// �O�p�`�Ɠ_�̏Փ˔���
	/// </summary>
	/// <param name="t0">�O�p�`�̒��_1</param>
	/// <param name="t1">�O�p�`�̒��_2</param>
	/// <param name="t2">�O�p�`�̒��_3</param>
	/// <param name="p">�_�̍��W</param>
	/// <returns>true�Ȃ�΁A�Փ˂��Ă���</returns>
	static bool CollisionTrianglePoint(const CVector& t0, const CVector& t1, const CVector& t2, const CVector& p);

	/// <summary>
	/// �O�p�`�Ƌ��̏Փ˔���
	/// </summary>
	/// <param name="t0">�O�p�`�̒��_1</param>
	/// <param name="t1">�O�p�`�̒��_2</param>
	/// <param name="t2">�O�p�`�̒��_3</param>
	/// <param name="sp">���̍��W</param>
	/// <param name="sr">���̔��a</param>
	/// <param name="hit">�Փ˂������̏��</param>
	/// <returns>true�Ȃ�΁A�Փ˂��Ă���</returns>
	static bool CollisionTriangleSphere(const CVector& t0, const CVector& t1, const CVector& t2,
		const CVector& sp, const float sr,
		CHitInfo* hit, bool isLeftMain);

	/// <summary>
	/// ���Ƌ��̏Փ˔���
	/// </summary>
	/// <param name="sp0">��1�̍��W</param>
	/// <param name="sr0">��1�̔��a</param>
	/// <param name="sp1">��2�̍��W</param>
	/// <param name="sr1">��2�̔��a</param>
	/// <param name="hit">�Փ˂������̏��</param>
	/// <returns>true�Ȃ�΁A�Փ˂��Ă���</returns>
	static bool CollisionSphere(const CVector& sp0, const float sr0,
		const CVector& sp1, const float sr1, CHitInfo* hit);

	/// <summary>
	/// ���Ɛ����̏Փ˔���
	/// </summary>
	/// <param name="sp">���̍��W</param>
	/// <param name="sr">���̔��a</param>
	/// <param name="ls">�����̎n�_</param>
	/// <param name="le">�����̏I�_</param>
	/// <param name="hit">�Փ˂������̏��</param>
	/// <returns>true�Ȃ�΁A�Փ˂��Ă���</returns>
	static bool CollisionSphereLine(const CVector& sp, const float sr,
		const CVector& ls, const CVector& le,
		CHitInfo* hit, bool isLeftMain);

	/// <summary>
	/// ���ƃJ�v�Z���̏Փ˔���
	/// </summary>
	/// <param name="sp">���̍��W</param>
	/// <param name="sr">���̔��a</param>
	/// <param name="cs">�J�v�Z�����\����������̎n�_</param>
	/// <param name="ce">�J�v�Z�����\����������̏I�_</param>
	/// <param name="cr">�J�v�Z���̔��a</param>
	/// <param name="hit">�Փ˂������̏��</param>
	/// <returns>true�Ȃ�΁A�Փ˂��Ă���</returns>
	static bool CollisionSphereCapsule(const CVector& sp, const float sr,
		const CVector& cs, const CVector& ce, float cr,
		CHitInfo* hit, bool isLeftMain);

	/// <summary>
	/// �����Ɛ����̏Փ˔���
	/// </summary>
	/// <param name="ls0">����1�̎n�_</param>
	/// <param name="le0">����1�̏I�_</param>
	/// <param name="ls1">����2�̎n�_</param>
	/// <param name="le1">����2�̏I�_</param>
	/// <param name="hit">�Փ˂������̏��</param>
	/// <returns>true�Ȃ�΁A�Փ˂��Ă���</returns>
	static bool CollisionLine(const CVector& ls0, const CVector& le0,
		const CVector& ls1, const CVector& le1, CHitInfo* hit);

	/// <summary>
	/// �J�v�Z���ƃJ�v�Z���̏Փ˔���
	/// </summary>
	/// <param name="cs0">�J�v�Z��1���\����������̎n�_</param>
	/// <param name="ce0">�J�v�Z��1���\����������̏I�_</param>
	/// <param name="cr0">�J�v�Z��1�̔��a</param>
	/// <param name="cs1">�J�v�Z��2���\����������̎n�_</param>
	/// <param name="ce1">�J�v�Z��2���\����������̏I�_</param>
	/// <param name="cr1">�J�v�Z��2�̔��a</param>
	/// <param name="hit">�Փ˂������̏��</param>
	/// <returns>true�Ȃ�΁A�Փ˂��Ă���</returns>
	static bool CollisionCapsule(const CVector& cs0, const CVector& ce0, float cr0,
		const CVector& cs1, const CVector& ce1, float cr1,
		CHitInfo* hit);

	/// <summary>
	/// �J�v�Z���Ɛ����̏Փ˔���
	/// </summary>
	/// <param name="cs">�J�v�Z�����\����������̎n�_</param>
	/// <param name="ce">�J�v�Z�����\����������̏I�_</param>
	/// <param name="cr">�J�v�Z���̔��a</param>
	/// <param name="ls">�����̎n�_</param>
	/// <param name="le">�����̏I�_</param>
	/// <param name="hit">�Փ˂������̏��</param>
	/// <returns>true�Ȃ�΁A�Փ˂��Ă���</returns>
	static bool CollisionCapsuleLine(const CVector& cs, const CVector& ce, float cr,
		const CVector& ls, const CVector& le,
		CHitInfo* hit, bool isLeftMain);

	/// <summary>
	/// ���b�V���Ɛ����̏Փ˔���
	/// </summary>
	/// <param name="tris">���b�V�����\������O�p�`�|���S���̃��X�g</param>
	/// <param name="ls">�����̎n�_</param>
	/// <param name="le">�����̏I�_</param>
	/// <param name="lb">�����̃o�E���f�B���O�{�b�N�X</param>
	/// <param name="hit">�Փ˂������̏��</param>
	/// <returns>true�Ȃ�΁A�Փ˂��Ă���</returns>
	static bool CollisionMeshLine(const std::vector<STVertexData>& tris,
		const CVector& ls, const CVector& le, const CBounds& lb,
		CHitInfo* hit, bool isLeftMain);

	/// <summary>
	/// ���b�V���ƃ��C�̏Փ˔���
	/// </summary>
	/// <param name="mesh">���b�V���f�[�^</param>
	/// <param name="rs">���C�̎n�_</param>
	/// <param name="re">���C�̏I�_</param>
	/// <param name="rb">���C�̃o�E���f�B���O�{�b�N�X</param>
	/// <param name="hit">�Փ˂������̏��</param>
	/// <returns>true�Ȃ�΁A�Փ˂��Ă���</returns>
	static bool CollisionMeshRay(CColliderMesh* mesh,
		const CVector& rs, const CVector& re, const CBounds& rb,
		CHitInfo* hit, bool isLeftMain);

	/// <summary>
	/// ���b�V���Ƌ��̏Փ˔���
	/// </summary>
	/// <param name="tris">���b�V�����\������O�p�`�|���S���̃��X�g</param>
	/// <param name="sphereCol">���R���C�_�[</param>
	/// <param name="hit">�Փ˂������̏��</param>
	/// <returns>true�Ȃ�΁A�Փ˂��Ă���</returns>
	static bool CollisionMeshSpehre(const std::vector<STVertexData>& tris,
		CColliderSphere* sphereCol, CHitInfo* hit, bool isLeftMain);

	/// <summary>
	/// ���b�V���ƎO�p�`�̏Փ˔���
	/// </summary>
	/// <param name="tris">���b�V�����\������O�p�`�|���S���̃��X�g</param>
	/// <param name="triCol">�O�p�`�R���C�_�[</param>
	/// <param name="hit">�Փ˂������̏��</param>
	/// <returns>true�Ȃ�΁A�Փ˂��Ă���</returns>
	static bool CollisionMeshTriangle(const std::vector<STVertexData>& tris,
		CColliderTriangle* triCol, CHitInfo* hit, bool isLeftMain);

	/// <summary>
	/// ���b�V���ƃJ�v�Z���̏Փ˔���
	/// </summary>
	/// <param name="tris">���b�V�����\������O�p�`�|���S���̃��X�g</param>
	/// <param name="capsuleCol">�J�v�Z���R���C�_�[</param>
	/// <param name="hit">�Փ˂������̏��</param>
	/// <returns>true�Ȃ�΁A�Փ˂��Ă���</returns>
	static bool CollisionMeshCapsule(const std::vector<STVertexData>& tris,
		CColliderCapsule* capsuleCol, CHitInfo* hit, bool isLeftMain);

	/// <summary>
	/// �_��������܂ł̍ŒZ���������߂�
	/// </summary>
	/// <param name="point">�_�̍��W</param>
	/// <param name="lineS">�����̎n�_</param>
	/// <param name="lineE">�����̏I�_</param>
	/// <param name="nearest">�ŋߓ_�ԋp�p�̕ϐ��̃|�C���^</param>
	/// <returns>�ŒZ����</returns>
	static float CalcDistancePointToLine(const CVector& point, const CVector& lineS, const CVector& lineE, CVector* nearest = nullptr);

	/// <summary>
	/// �����Ɛ����̍ŒZ���������߂�
	/// </summary>
	/// <param name="s1">����1�̎n�_</param>
	/// <param name="e1">����1�̏I�_</param>
	/// <param name="s2">����2�̎n�_</param>
	/// <param name="e2">����2�̏I�_</param>
	/// <returns></returns>
	static float CalcDistanceLine(const CVector& s1, const CVector& e1, const CVector& s2, const CVector& e2);

	/// <summary>
	/// �w�肵���_���O�p�`�̓����ɂ��邩�ǂ���
	/// </summary>
	/// <param name="point">�_�̍��W</param>
	/// <param name="t0">�O�p�`��1�ڂ̒��_</param>
	/// <param name="t1">�O�p�`��2�ڂ̒��_</param>
	/// <param name="t2">�O�p�`��3�ڂ̒��_</param>
	/// <param name="n">�O�p�`�̖@��</param>
	/// <returns>true�Ȃ�΁A�O�p�`�̓����ɂ���</returns>
	static bool IsInsideTriangle(const CVector& p, const CVector& t0, const CVector& t1, const CVector& t2, const CVector& n);

	/// <summary>
	/// �w�肵���_�ƎO�p�`�̍ŋߐړ_���v�Z
	/// </summary>
	/// <param name="p"></param>
	/// <param name="t0"></param>
	/// <param name="t1"></param>
	/// <param name="t2"></param>
	/// <returns></returns>
	static CVector ClosestPointOnTriangle(const CVector& p, const CVector& t0, const CVector& t1, const CVector& t2);

	/// <summary>
	/// �Փ˔���
	/// </summary>
	/// <param name="c0">�R���C�_1</param>
	/// <param name="c1">�R���C�_2</param>
	/// <param name="hit">�q�b�g�������̏��</param>
	/// <returns>true�Ȃ�΁A�Փ˂��Ă���</returns>
	static bool Collision(CCollider* c0, CCollider* c1, CHitInfo* hit);

	/// <summary>
	/// ���C�ƃR���C�_�[�̏Փ˔���
	/// </summary>
	/// <param name="c">�R���C�_�[</param>
	/// <param name="start">���C�̊J�n�ʒu</param>
	/// <param name="end">���C�̏I���ʒu</param>
	/// <param name="hit">�q�b�g�������̏��</param>
	/// <returns>true�Ȃ�΁A�Փ˂��Ă���</returns>
	static bool CollisionRay(CCollider* c, const CVector& start, const CVector& end, CHitInfo* hit);

	/// <summary>
	/// �Փˎ��̉����߂��������Z�o
	/// </summary>
	/// <param name="self">���g�̃R���C�_�[</param>
	/// <param name="other">����̃R���C�_�[</param>
	/// <returns>�����߂������i0.0f �` 1.0f�j</returns>
	static float CalcPushBackRatio(CCollider* self, CCollider* other);

protected:
	// �R���C�_�[�̏����X�V
	virtual void UpdateCol(bool isInit = false) = 0;

	/// <summary>
	/// �R���C�_�[�̐ݒ�
	/// </summary>
	/// <param name="owner">�R���C�_�[�̎�����</param>
	/// <param name="layer">�Փ˔���p���C���[</param>
	void Set(CObjectBase* owner, ELayer layer);

	CBounds mBounds;		// �o�E���f�B���O�{�b�N�X
	CMatrix mLastMtx;		// �O�񔽉f���̍s��

private:
	ELayer mLayer;			// �Փ˔��背�C���[
	EColliderType mType;	// �R���C�_�[�̎��
	CObjectBase* mpOwner;	// �R���C�_�[�̎�����
	bool mIsEnable;			// �L�����ǂ���
	bool mIsShow;			// �f�o�b�O�\�����邩�ǂ���
	bool mIsKinematic;		// �Փˎ��̉����߂��̉e�����󂯂Ȃ����ǂ���
	float mWeight;			// �R���C�_�[�̏d��
	int mCollisionLayers;	// �Փ˔�����s�����C���[�̃r�b�g�t���O
	int mCollisionTags;		// �Փ˔�����s���I�u�W�F�N�g�^�O�̃r�b�g�t���O

	const CMatrix* mpAttachMtx;	// ��������s��̃|�C���^�[
	CMatrix mAttachMtx;			// ��������s��̖{��
};
