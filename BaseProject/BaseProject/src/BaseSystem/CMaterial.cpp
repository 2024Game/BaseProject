#include "CMaterial.h"
//memset,strncpy�̃C���N���[�h
#include <string.h>
#include "glut.h"
/*
* strncpy(char* str1, const char* str2, int len)
* �R�s�[��str1�ɃR�s�[��str2�̕�����len�������܂ŃR�s�[����
*/
char* strncpy(char* str1, const char* str2, int len)
{
	int i = 0;
	//i��len��菬�����A���A�R�s�[�����I���łȂ��ԌJ��Ԃ�
	while (i < len && *str2 != '\0')
	{
		*(str1 + i) = *str2; //�R�s�[��ɃR�s�[������
		str2++; //�R�s�[��������
		i++;
	}
	str1[i] = '\0'; //�R�s�[��̕�����ɏI���
	return str1; //�R�s�[��̐擪�A�h���X��ԋp
}

bool CMaterial::LoadTexture(std::string name, std::string path, bool dontDelete)
{
	mpTexture = CResourceManager::Load<CTexture>(path, path, dontDelete);
	return mpTexture != nullptr;
}

CTexture* CMaterial::Texture()
{
	return mpTexture;
}

void CMaterial::Disabled()
{
	//�u�����h�����𖳌�
	Blend::DisableBlend();
	//�e�N�X�`���L��
	if (mpTexture != nullptr && mpTexture->Id())
	{
		//�e�N�X�`���̃o�C���h������
		glBindTexture(GL_TEXTURE_2D, 0);
		//�e�N�X�`���𖳌��ɂ���
		glDisable(GL_TEXTURE_2D);
	}
}

//�f�t�H���g�R���X�g���N�^
CMaterial::CMaterial()
	: mVertexNum(0)
	, mPower(0)
	, mpTextureFilename(nullptr)
	, mpTexture(nullptr)
	, mBlendType(EBlend::eAlpha)
{
	//���O��0�Ŗ���
	memset(mName, 0, sizeof(mName));
	//Deffuse�J���[�̓f�t�H���g��(1, 1, 1, 1)
	int size = ARRAY_SIZE(mDiffuse);
	for (int i = 0; i < size; i++)
	{
		mDiffuse[i] = 1.0f;
	}
}

/*
Material�f�[�^�̓ǂݍ��݂Ɛݒ�
*/
CMaterial::CMaterial(CModelX *model, bool dontDelete)
	: mpTextureFilename(nullptr)
	, mBlendType(EBlend::eAlpha)
{
	//CModelX�Ƀ}�e���A����ǉ�����
	model->Material().push_back(this);
	model->GetToken(); // { ? Name
	if (strcmp(model->Token(), "{") != 0)
	{
		// {�łȂ����̓}�e���A����
		strcpy(mName, model->Token());
		model->GetToken(); //{
	}

	mDiffuse[0] = atof(model->GetToken());
	mDiffuse[1] = atof(model->GetToken());
	mDiffuse[2] = atof(model->GetToken());
	mDiffuse[3] = atof(model->GetToken());

	mPower = atof(model->GetToken());

	mSpecular[0] = atof(model->GetToken());
	mSpecular[1] = atof(model->GetToken());
	mSpecular[2] = atof(model->GetToken());

	mEmissive[0] = atof(model->GetToken());
	mEmissive[1] = atof(model->GetToken());
	mEmissive[2] = atof(model->GetToken());

	model->GetToken(); //TextureFilename or }

	if (strcmp(model->Token(), "TextureFilename") == 0)
	{
		//�e�N�X�`������̏ꍇ�A�e�N�X�`���t�@�C�����擾
		model->GetToken(); // {
		model->GetToken(); // filename
		mpTextureFilename = new char[strlen(model->Token()) + 1];
		strcpy(mpTextureFilename, model->Token());

		//�e�N�X�`���̓ǂݍ���
		std::string texPath = model->DirPath() + mpTextureFilename;
		LoadTexture(mpTextureFilename, texPath, dontDelete);

		model->GetToken(); // }
		model->GetToken(); // }
	}
}

CMaterial::~CMaterial()
{
	if (mpTextureFilename)
	{
		delete[] mpTextureFilename;
	}
	mpTextureFilename = nullptr;

	if (mpTexture != nullptr)
	{
	}
}

//�}�e���A����L���ɂ���
void CMaterial::Enabled(const CColor& color)
{
	glColor4fv((GLfloat*)&color);
	//�g�U���̐ݒ�
	float diffuse[4];
	diffuse[0] = mDiffuse[0] * color.R();
	diffuse[1] = mDiffuse[1] * color.G();
	diffuse[2] = mDiffuse[2] * color.B();
	diffuse[3] = mDiffuse[3] * color.A();
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);

	//�u�����h������L���ɂ���
	Blend::EnableBlend(mBlendType);
	//�e�N�X�`���L��
	if (mpTexture != nullptr && mpTexture->Id())
	{
		//�e�N�X�`�����g�p�\�ɂ���
		glEnable(GL_TEXTURE_2D);
		//�e�N�X�`�����o�C���h����
		glBindTexture(GL_TEXTURE_2D, mpTexture->Id());
	}
}
//�}�e���A���̖��O�̎擾
char* CMaterial::Name()
{
	return mName;
}

// �}�e���A���̖��O��ݒ肷��
// Name(�}�e���A���̖��O)
void CMaterial::Name(char* name)
{
	strncpy(mName, name, MATERIAL_NAME_LEN);
}
//mDiffuse�z��̎擾
float* CMaterial::Diffuse()
{
	return mDiffuse;
}

void CMaterial::VertexNum(int num)
{
	mVertexNum = num;
}

int CMaterial::VertexNum()
{
	return mVertexNum;
}

//�u�����h�����̃^�C�v��ݒ�
void CMaterial::SetBlendType(EBlend type)
{
	mBlendType = type;
}
