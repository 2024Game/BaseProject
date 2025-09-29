#include "CModel.h"
#include "CVector.h"
#include "Maths.h"
#include "glut.h"

std::vector<CTriangle> CModel::Triangles() const
{
	return mTriangles;
}

//���f���ǂݍ���
bool CModel::Load(std::string path, bool dontDelete)
{
	//���_�f�[�^�̕ۑ�(CVector�^)
	std::vector<CVector> vertex;
	std::vector<CVector> normal;
	//�e�N�X�`���}�b�s���O�̕ۑ�(CVector�^)
	std::vector<CVector> uv;

	std::string dirPath = path;
	int index = dirPath.find_last_of('\\');
	if (index >= 0)
	{
		dirPath = dirPath.substr(0, index + 1);
	}

	std::string objPath = RES_DIR;
	objPath += path;

	//�t�@�C���|�C���^�ϐ��̍쐬
	FILE* fp;
	//�t�@�C������f�[�^�����
	//���̓G���A���쐬����
	char buf[256];

	//�t�@�C���̃I�[�v��
	//fopen(�t�@�C����,���[�h)
	//�I�[�v���ł��Ȃ�����NULL��Ԃ�
	fp = fopen(objPath.c_str(), "r");
	//�t�@�C���I�[�v���G���[�̔���
	//fp��NULL�̎��̓G���[
	if (fp == NULL) {
		//�R���\�[���ɃG���[�o�͂��Ė߂�
		printf("%s file open error��n", objPath.c_str());
		return false;
	}

	int matIdx = 0;

	//�t�@�C������1�s����
	//fgets(���̓G���A,�G���A�T�C�Y,�t�@�C���|�C���^)
	//�t�@�C���̍Ō�ɂȂ��NULL��Ԃ�
	while (fgets(buf, sizeof(buf), fp) != NULL) {
		//�f�[�^�𕪊�����
		char str[4][64] = { "", "", "", "" };
		//�����񂩂�f�[�^��4�ϐ��֑������
		//sscanf(������, �ϊ��w��q, �ϐ�)
		sscanf(buf, "%s %s %s %s", str[0], str[1], str[2], str[3]);
		//������̔�r
		//strcmp(������1, ������2)
		//������1�ƕ�����2��������0�A�قȂ鎞0�ȊO��Ԃ�
		//�擪��v�̎��A���_��vertex�ɒǉ�����
		if (strcmp(str[0], "v") == 0) {
			//�ϒ��z��vertex�ɒǉ�
			//atof(������)�@�����񂩂�float�^�̒l��Ԃ�
			vertex.push_back(CVector(atof(str[1]), atof(str[2]), atof(str[3])));
		}
		else if (strcmp(str[0], "vn") == 0) {
			//�ϒ��z��vertex�ɒǉ�
			//atof(������)�@�����񂩂�float�^�̒l��Ԃ�
			normal.push_back(CVector(atof(str[1]), atof(str[2]), atof(str[3])));
		}
		//�擪��f�̎��A�O�p�`���쐬���Ēǉ�����
		else if (strcmp(str[0], "f") == 0) {
			//���_�Ɩ@���̔ԍ��쐬
			int v[3], n[3];
			//�e�N�X�`���}�b�s���O�̗L���𔻒�
			if (strstr(str[1], "//")) {
				//���_�Ɩ@���̔ԍ��擾
				sscanf(str[1], "%d//%d", &v[0], &n[0]);
				sscanf(str[2], "%d//%d", &v[1], &n[1]);
				sscanf(str[3], "%d//%d", &v[2], &n[2]);
				//�O�p�`�쐬
				CTriangle t;
				t.Vertex(vertex[v[0] - 1], vertex[v[1] - 1], vertex[v[2] - 1]);
				t.Normal(normal[n[0] - 1], normal[n[1] - 1], normal[n[2] - 1]);
				//�}�e���A���ԍ��̐ݒ�
				t.MaterialIdx(matIdx);
				//�ϒ��z��mTriangles�ɎO�p�`��ǉ�
				mTriangles.push_back(t);
			}
			else {
				//�e�N�X�`���}�b�s���O�L��
				int u[3]; //�e�N�X�`���}�b�s���O�̔ԍ�
				//���_�Ɩ@���̔ԍ��擾�ƃ}�b�s���O�̔ԍ��擾
				sscanf(str[1], "%d/%d/%d", &v[0], &u[0], &n[0]);
				sscanf(str[2], "%d/%d/%d", &v[1], &u[1], &n[1]);
				sscanf(str[3], "%d/%d/%d", &v[2], &u[2], &n[2]);
				//�O�p�`�쐬
				CTriangle t;
				t.Vertex(vertex[v[0] - 1], vertex[v[1] - 1], vertex[v[2] - 1]);
				t.Normal(normal[n[0] - 1], normal[n[1] - 1], normal[n[2] - 1]);
				//�e�N�X�`���}�b�s���O�̐ݒ�
				t.UV(uv[u[0] - 1], uv[u[1] - 1], uv[u[2] - 1]);
				//�}�e���A���ԍ��̐ݒ�
				t.MaterialIdx(matIdx);
				//�ϒ��z��mTriangles�ɎO�p�`��ǉ�
				mTriangles.push_back(t);
			}
		}
		//�擪��mtllib�̎��A�}�e���A����ǂݍ���
		else if (strcmp(str[0], "mtllib") == 0) {
			std::string mtlPath = dirPath + "\\" + str[1];
			LoadMaterial(mtlPath, dontDelete);
		}
		//�擪��usemtl�̎��A�}�e���A���C���f�b�N�X���擾����
		else if (strcmp(str[0], "usemtl") == 0) {
			//�ϒ��z����ォ���r
			for (matIdx = mpMaterials.size() - 1; matIdx > 0; matIdx--) {
				//�������O�̃}�e���A��������΃��[�v�I��
				if (strcmp(mpMaterials[matIdx]->Name(), str[1]) == 0) {
					break; //���[�v����o��
				}
			}
		}
		//�擪��vt�̎��Auv�ɒǉ�����
		else if (strcmp(str[0], "vt") == 0) {
			//�ϒ��z��uv�ɒǉ�
			//atof(������)�@�����񂩂�float�^�̒l��Ԃ�
			uv.push_back(CVector(atof(str[1]), atof(str[2]), 0.0));
		}
	}

	//�t�@�C���̃N���[�Y
	fclose(fp);

	CreateVertexBuffer();

	return true;
}

//�}�e���A���ǂݍ���
bool CModel::LoadMaterial(std::string path, bool dontDelete)
{
	std::string mtlPath = RES_DIR + path;

	std::string dirPath = path;
	int index = dirPath.find_last_of('\\');
	if (index >= 0)
	{
		dirPath = dirPath.substr(0, index + 1);
	}

	//�t�@�C���|�C���^�ϐ��̍쐬
	FILE* fp;
	//�t�@�C������f�[�^�����
	//���̓G���A���쐬����
	char buf[256];

	//�t�@�C���̃I�[�v��
	//fopen(�t�@�C����,���[�h)
	//�I�[�v���ł��Ȃ�����NULL��Ԃ�
	fp = fopen(mtlPath.c_str(), "r");
	//�t�@�C���I�[�v���G���[�̔���
	//fp��NULL�̎��̓G���[
	if (fp == NULL) {
		//�R���\�[���ɃG���[�o�͂��Ė߂�
		printf("%s file open error��n", mtlPath.c_str());
		return false;
	}

	//�}�e���A���C���f�b�N�X
	int idx = 0;
	//�t�@�C������1�s����
	//fgets(���̓G���A,�G���A�T�C�Y,�t�@�C���|�C���^)
	//�t�@�C���̍Ō�ɂȂ��NULL��Ԃ�
	while (fgets(buf, sizeof(buf), fp) != NULL) {
		//�f�[�^�𕪊�����
		char str[4][64] = { "", "", "", "" };
		//�����񂩂�f�[�^��4�ϐ��֑������
		sscanf(buf, "%s %s %s %s", str[0], str[1], str[2], str[3]);
		//�擪��newmtl�̎��A�}�e���A����ǉ�����
		if (strcmp(str[0], "newmtl") == 0) {
			CMaterial* pm = new CMaterial();
			//�}�e���A�����̐ݒ�
			pm->Name(str[1]);
			//�}�e���A���̉ϒ��z��ɒǉ�
			mpMaterials.push_back(pm);
			//�z��̒������擾
			idx = mpMaterials.size() - 1;
		}
		//�擪��Kd�̎��ADiffuse��ݒ肷��
		else if (strcmp(str[0], "Kd") == 0) {
			mpMaterials[idx]->Diffuse()[0] = atof(str[1]);
			mpMaterials[idx]->Diffuse()[1] = atof(str[2]);
			mpMaterials[idx]->Diffuse()[2] = atof(str[3]);
		}
		//�擪��d�̎��A���l��ݒ肷��
		else if (strcmp(str[0], "d") == 0) {
			mpMaterials[idx]->Diffuse()[3] = atof(str[1]);
		}
		//�擪��map_Kd�̎��A�e�N�X�`������͂���
		else if (strcmp(str[0], "map_Kd") == 0) {
			std::string texPath = dirPath + str[1];
			mpMaterials[idx]->LoadTexture(str[1], texPath.c_str(), dontDelete);
		}

	}

	//�t�@�C���̃N���[�Y
	fclose(fp);

	return true;
}

void CModel::Render()
{
	// ���S�ɓ����ȏ�Ԃł���΁A�`�悵�Ȃ�
	if (mColor.A() == 0.0f) return;

	//�ϒ��z��̗v�f�������J��Ԃ�
	for (int i = 0; i < mTriangles.size(); i++) {
		//�}�e���A���̓K�p
		mpMaterials[mTriangles[i].MaterialIdx()]->Enabled(mColor);
		//�ϒ��z��ɓY�����ŃA�N�Z�X����
		mTriangles[i].Render();
		//�}�e���A���𖳌�
		mpMaterials[mTriangles[i].MaterialIdx()]->Disabled();
	}
}

CModel::CModel()
	: mpVertexes(nullptr)
	, mColor(CColor::white)
	, mIsLighting(true)
	, mIsDepthWrite(true)
	, mIsCullFace(true)
	, mBlendMode(EBlend::eAlpha)
{
}

CModel::~CModel()
{
	for (int i = 0; i < mpMaterials.size(); i++)
	{
		delete mpMaterials[i];
	}
	delete[] mpVertexes;
}

// �J���[��ݒ�
void CModel::SetColor(const CColor& color)
{
	mColor = color;
}

// �J���[���擾
const CColor& CModel::GetColor() const
{
	return mColor;
}

// �A���t�@�l�ݒ�
void CModel::SetAlpha(float alpha)
{
	mColor.A(Math::Clamp01(alpha));
}

// �A���t�@�l�擾
float CModel::GetAlpha() const
{
	return mColor.A();
}

// ���C�e�B���O�̗L����Ԃ�ݒ�
void CModel::SetLighting(bool enable)
{
	mIsLighting = enable;
}

// ���C�e�B���O�̗L����Ԃ��擾
bool CModel::IsLighting() const
{
	return mIsLighting;
}

// �f�v�X�l�̏������݂��s�����ݒ�
void CModel::SetDepthWrite(bool enable)
{
	mIsDepthWrite = enable;
}

// �f�v�X�l�̏������݂��s�����ǂ���
bool CModel::IsDepthWrite() const
{
	return mIsDepthWrite;
}

// �J�����O���s�����ǂ�����ݒ�
void CModel::SetCullFace(bool enable)
{
	mIsCullFace = enable;
}

// �J�����O���s�����ǂ���
bool CModel::IsCullFace() const
{
	return mIsCullFace;
}

// �`�掞�̃u�����h������ݒ�
void CModel::SetBlend(EBlend mode)
{
	mBlendMode = mode;
	for (CMaterial* mat : mpMaterials)
	{
		mat->SetBlendType(mBlendMode);
	}
}

// �`�掞�̃u�����h�������擾
EBlend CModel::GetBlend() const
{
	return mBlendMode;
}

// �G�t�F�N�g�p�̐ݒ�
void CModel::SetupEffectSettings()
{
	SetLighting(false);
	SetDepthWrite(false);
	SetCullFace(false);
	SetBlend(EBlend::eAdd);
}

//�`��
//Render(�s��)
void CModel::Render(const CMatrix& m)
{
	// ���S�ɓ����ȏ�Ԃł���΁A�`�悵�Ȃ�
	if (mColor.A() == 0.0f) return;

	//�f�v�X�l�̏������ݐݒ�
	glDepthMask(mIsDepthWrite);
	//���C�e�B���O�ݒ�
	if (!mIsLighting) glDisable(GL_LIGHTING);
	//�J�����O�ݒ�
	if (!mIsCullFace) glDisable(GL_CULL_FACE);

	//�s��̑ޔ�
	glPushMatrix();
	//�����s����|����
	glMultMatrixf(m.M());

	//���_���W�̈ʒu��ݒ�
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, sizeof(CVertex), (void*)&mpVertexes[0].mPosition);
	//�@���x�N�g���̈ʒu��ݒ�
	glEnableClientState(GL_NORMAL_ARRAY);
	glNormalPointer(GL_FLOAT, sizeof(CVertex), (void*)&mpVertexes[0].mNormal);
	//�e�N�X�`���}�b�s���O�̈ʒu��ݒ�
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, sizeof(CVertex), (void*)&mpVertexes[0].mTextureCoords);

	int first = 0; //�`��ʒu
	//�}�e���A�����ɕ`�悷��
	for (size_t i = 0; i < mpMaterials.size(); i++) {
		//�}�e���A����K�p����
		mpMaterials[i]->Enabled(mColor);
		//�`��ʒu����̃f�[�^�ŎO�p�`��`�悵�܂�
		glDrawArrays(GL_TRIANGLES, first, mpMaterials[i]->VertexNum());
		//�}�e���A���𖳌��ɂ���
		mpMaterials[i]->Disabled();
		//�`��ʒu���ړ�
		first += mpMaterials[i]->VertexNum();
	}
	//�s���߂�
	glPopMatrix();

	//���_���W�̔z��𖳌��ɂ���
	glDisableClientState(GL_VERTEX_ARRAY);
	//�@���̔z��𖳌��ɂ���
	glDisableClientState(GL_NORMAL_ARRAY);
	//�e�N�X�`���}�b�s���O�̔z��𖳌��ɂ���
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	//�f�v�X�l�̏������ݏ�Ԃ�߂�
	glDepthMask(true);
	//���C�e�B���O�ݒ�
	glEnable(GL_LIGHTING);
	//�J�����O�ݒ�
	glEnable(GL_CULL_FACE);
}

void CModel::CreateVertexBuffer()
{
	mpVertexes = new CVertex[mTriangles.size() * 3];
	int idx = 0;
	for (int i = 0; i < mpMaterials.size(); i++)
	{
		for (int j = 0; j < mTriangles.size(); j++)
		{
			if (i == mTriangles[j].MaterialIdx())
			{
				mpMaterials[i]->VertexNum(mpMaterials[i]->VertexNum() + 3);
				mpVertexes[idx].mPosition = mTriangles[j].V0();
				mpVertexes[idx].mNormal = mTriangles[j].N0();
				mpVertexes[idx++].mTextureCoords = mTriangles[j].U0();
				mpVertexes[idx].mPosition = mTriangles[j].V1();
				mpVertexes[idx].mNormal = mTriangles[j].N1();
				mpVertexes[idx++].mTextureCoords = mTriangles[j].U1();
				mpVertexes[idx].mPosition = mTriangles[j].V2();
				mpVertexes[idx].mNormal = mTriangles[j].N2();
				mpVertexes[idx++].mTextureCoords = mTriangles[j].U2();
			}
		}
	}
}

