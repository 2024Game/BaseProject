#ifndef CCOLLIDERMESH_H
#define CCOLLIDERMESH_H
#include <vector>
#include "CModel.h"
#include "CCollider.h"

struct STDivMesh
{
	CBounds bounds;
	std::vector<STVertexData*> vertices;
};

/*
���f���f�[�^����O�p�R���C�_�̐���
*/
class CColliderMesh : public CCollider
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="owner">�R���C�_�[�̎�����</param>
	/// <param name="layer">�Փ˔���p���C���[</param>
	/// <param name="model">���f���f�[�^</param>
	/// <param name="isKinematic">true�Ȃ�΁A�Փˎ��ɉ����߂��̉e�����󂯂Ȃ�</param>
	/// <param name="weight">�R���C�_�[�̏d��</param>
	/// <param name="divX">�R���C�_�[��X���̕�����</param>
	/// <param name="divY">�R���C�_�[��Y���̕�����</param>
	/// <param name="divZ">�R���C�_�[��Z���̕�����</param>
	CColliderMesh(CObjectBase* owner, ELayer layer, CModel* model,
		bool isKinematic = false, float weight = 1.0f,
		int divX = 1, int divY = 1, int divZ = 1);
	// �f�X�g���N�^
	~CColliderMesh();

	void Set(CModel* model);
	const std::vector<STVertexData>& Get() const;
	const std::vector<STDivMesh>& GetDivMesh() const;

#if _DEBUG
	// �R���C�_�[�`��
	void Render() override;
	// �R���C�_�[�̃o�E���f�B���O�{�b�N�X��`��
	void RenderBounds() override;
	// �R���C�_�[�̕����͈͂�`��
	void RenderDivArea();
#endif

protected:
	// �R���C�_�[�̏����X�V
	void UpdateCol(bool isInit = false) override;

private:
	//�O�p�R���C�_�̔z��쐬
	std::vector<STVertexData> mVertices;
	std::vector<STDivMesh> mDivMesh;
	int mDivX;
	int mDivY;
	int mDivZ;
};

#endif
