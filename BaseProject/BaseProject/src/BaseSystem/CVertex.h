#ifndef CVERTEX_H
#define CVERTEX_H
#include "CVector.h"
/*
���_�f�[�^�N���X
���_���Ƀf�[�^���܂Ƃ߂܂�
*/
class CVertex {
public:
	//�ʒu
	CVector mPosition;
	//�@��
	CVector mNormal;
	//�e�N�X�`���}�b�s���O
	CVector mTextureCoords;

	//�X�L���E�F�C�g
	float mBoneWeight[4];
	//�X�L���C���f�b�N�X
	float mBoneIndex[4];

	CVertex() {
		for (int i = 0; i < 4; i++) {
			mBoneIndex[i] = 0;
			mBoneWeight[i] = 0.0f;
		}
		mBoneWeight[0] = 1.0f;
	}
};

// �O�p�`�|���S���̒��_�f�[�^
struct STVertex
{
	CVector V[3];
	STVertex() : STVertex(CVector(), CVector(), CVector()) {}
	STVertex(const CVector& v0, const CVector& v1, const CVector& v2)
	{
		Set(v0, v1, v2);
	}
	void Set(const CVector& v0, const CVector& v1, const CVector& v2)
	{
		V[0] = v0;
		V[1] = v1;
		V[2] = v2;
	}
};

#endif


