#ifndef CMYSHADER_H
#define CMYSHADER_H

#include "CShader.h"

class CModelX;
class CMaterial;
class CMesh;
class CMatrix;
class CModel;

class CMyShader : public CShader {
private:
	//�}�e���A���̐ݒ�
	void SetShader(CModelX* model, CMaterial* material);
	//�`�揈��
	void Render(CModelX* model, CMesh* mesh, CMatrix* pCombinedMatrix);
public:
	//�`�揈��
	void Render(CModelX* model, CMatrix* combinedMatrix);
	//	void Render(CModel *mesh);
};

#endif