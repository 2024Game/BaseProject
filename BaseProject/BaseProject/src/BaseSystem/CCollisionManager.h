#pragma once
#include <list>
#include "CCollider.h"

/// <summary>
/// �Փˏ����̊Ǘ��N���X
/// </summary>
class CCollisionManager
{
public:
	/// <summary>
	/// �C���X�^���X���擾
	/// </summary>
	/// <returns>�C���X�^���X</returns>
	static CCollisionManager* Instance();
	// �C���X�^���X�̔j��
	static void ClearInstance();

	/// <summary>
	/// �w�肵���R���C�_�[�����X�g�ɒǉ�
	/// </summary>
	/// <param name="col">�ǉ�����R���C�_�[</param>
	void Add(CCollider* col);
	/// <summary>
	/// �w�肵���R���C�_�[�����X�g�����菜��
	/// </summary>
	/// <param name="col">��菜���R���C�_�[</param>
	void Remove(CCollider* col);

	/// <summary>
	/// �w�肵��2�̃R���C�_�[�̏Փˏ������s��
	/// </summary>
	/// <param name="col0">�Փˏ������s���R���C�_�[ 1��</param>
	/// <param name="col1">�Փˏ������s���R���C�_�[ 2��</param>
	void Collision(CCollider* col0, CCollider* col1);
	/// <summary>
	/// �w�肵���R���C�_�[�Ƒ��̑S�ẴR���C�_�[�Ƃ̏Փˏ������s��
	/// </summary>
	/// <param name="col">�Փˏ������s���R���C�_�[</param>
	void Collision(CCollider* col);
	/// <summary>
	/// �S�ẴR���C�_�[�̏Փˏ������s��
	/// </summary>
	void CollisionAll();

#if _DEBUG
	// �S�R���C�_�[��`��
	void Render();
	// �S�R���C�_�[�̃o�E���f�B���O�{�b�N�X��`��
	void RenderBounds();
#endif

private:
	// �R���X�g���N�^
	CCollisionManager();
	// �f�X�g���N�^
	~CCollisionManager();

	// �R���W�����}�l�[�W���̃C���X�^���X
	static CCollisionManager* mpInstance;
	// �R���C�_�[�̃��X�g
	std::list<CCollider*> mColliderList;
	// �R���C�_�[��`�悷�邩�ǂ���
	bool mIsShowCollider;
	// �R���C�_�[�̃o�E���f�B���O�{�b�N�X��`�悷�邩�ǂ���
	bool mIsShowColliderBounds;
};
