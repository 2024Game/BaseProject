#ifndef CMODEL_H
#define CMODEL_H
//vector�̃C���N���[�h
#include <vector>
#include "CTriangle.h"
#include "CMaterial.h"
#include "CVertex.h"
#include "CResource.h"
#include "CColor.h"

/*
���f���N���X
���f���f�[�^�̓��͂�\��
*/
class CModel : public CResource
{
	friend CResourceManager;

public:
	std::vector<CTriangle> Triangles() const;

	// �J���[��ݒ�
	void SetColor(const CColor& color);
	// �J���[���擾
	const CColor& GetColor() const;
	// �A���t�@�l�ݒ�
	void SetAlpha(float alpha);
	// �A���t�@�l�擾
	float GetAlpha() const;

	// ���C�e�B���O�̗L����Ԃ�ݒ�
	void SetLighting(bool enable);
	// ���C�e�B���O�̗L����Ԃ��擾
	bool IsLighting() const;

	// �f�v�X�l�̏������݂��s�����ݒ�
	void SetDepthWrite(bool enable);
	// �f�v�X�l�̏������݂��s�����ǂ���
	bool IsDepthWrite() const;

	// �J�����O���s�����ǂ�����ݒ�
	void SetCullFace(bool enable);
	// �J�����O���s�����ǂ���
	bool IsCullFace() const;

	// �`�掞�̃u�����h������ݒ�
	void SetBlend(EBlend mode);
	// �`�掞�̃u�����h�������擾
	EBlend GetBlend() const;

	// �G�t�F�N�g�p�̐ݒ�
	void SetupEffectSettings();

	// �`��
	void Render();
	// �`��
	// Render(�s��)
	void Render(const CMatrix& m);
private:
	CModel();
	~CModel();
	// ���f���ǂݍ���
	bool Load(std::string path, bool dontDelete) override;
	// �}�e���A���ǂݍ���
	bool LoadMaterial(std::string path, bool dontDelete);
	void CreateVertexBuffer();

	// �}�e���A���|�C���^�̉ϒ��z��
	std::vector<CMaterial*> mpMaterials;
	// �O�p�`�̉ϒ��z��
	std::vector<CTriangle> mTriangles;
	// ���_�̔z��
	CVertex* mpVertexes;
	// ���f���̃J���[
	CColor mColor;

	// ���C�e�B���O���L�����ǂ���
	bool mIsLighting;
	// �f�v�X�l�������݂��s�����ǂ���
	bool mIsDepthWrite;
	// �J�����O���s�����ǂ���
	bool mIsCullFace;
	// �`�掞�̃u�����h����
	EBlend mBlendMode;
};

#endif
