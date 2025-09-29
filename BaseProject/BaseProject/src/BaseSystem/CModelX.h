#ifndef CMODELX_H
#define CMODELX_H
#include <vector>		//vector�N���X�̃C���N���[�h
#include "CMatrix.h"	//�}�g���N�X�N���X�̃C���N���[�h
#include "CVector.h"	//Vector�N���X�̃C���N���[�h
#include "CMyShader.h"	//�V�F�[�_�[�N���X�̃C���N���[�h
#include "CResource.h"
#include "CColor.h"

class CModelX;			//CModel�N���X�̐錾
class CModelXFrame;		//CModelXFrame�N���X�̐錾
class CMesh;			//CMesh�N���X�̐錾
class CMaterial;		//�}�e���A���̐錾
class CSkinWeights;		//�X�L���E�F�C�g�N���X�̐錾
class CAnimationSet;	//�A�j���[�V�����Z�b�g�N���X�̐錾
class CAnimation;		//�A�j���[�V�����N���X�̐錾
class CAnimationKey;	//�A�j���[�V�����L�[�N���X�̐錾
class CXCharacter;		//CXCharacter�N���X�̐錾

/*
CModel
X�t�@�C���`����3D���f���f�[�^���v���O�����ŔF������
*/
class CModelX : public CResource
{
	friend CResourceManager;
	friend CModelXFrame;
	friend CMesh;
	friend CAnimationSet;
	friend CAnimation;
	friend CModelX;
	friend CMyShader;
private:
	//�R���X�g���N�^
	CModelX();
	//�f�X�g���N�^
	~CModelX();

	//�t�@�C���ǂݍ���
	bool Load(std::string path, bool dontDelete) override;

	std::vector<CMaterial*> mMaterial;	//�}�e���A���z��
	std::vector<CModelXFrame*> mFrame;  //�t���[���̔z��
	std::vector<CAnimationSet*> mAnimationSet;	//�A�j���[�V�����Z�b�g�z��

	CMatrix* mpSkinningMatrix;	//�V�F�[�_�[�p�X�L���}�g���b�N�X
	CMyShader mShader;	//�V�F�[�_�[�̃C���X�^���X

	char* mpPointer;	//�ǂݍ��݈ʒu
	char mToken[1024];	//���o�����P��̗̈�

	//c����؂蕶���Ȃ�true��Ԃ�
	bool IsDelimiter(char c);
	//�ǂݍ��ݍς݃t���O
	bool mLoaded;

	std::string mFilePath;	//���f���f�[�^�̃t�@�C���p�X
	std::string mDirPath;	//���f���f�[�^�̃f�B���N�g���p�X

	CColor mColor;		//���f���̃J���[

public:
	//�}�e���A���z��̎擾
	std::vector<CMaterial*>& Material();
	std::vector<CModelXFrame*>& Frames();
	std::vector<CAnimationSet*>& CModelX::AnimationSet();
	//�t���[�����ɊY������t���[���̃A�h���X��Ԃ�
	CModelXFrame* FinedFrame(const char* name);
	//�}�e���A���̌���
	CMaterial* FindMaterial(char* name);

	//�P��̎��o��
	char* GetToken();
	char* Token();

	//�g�[�N�����Ȃ��Ȃ�����true
	bool EOT();
	//�ǂݍ��ݍς݂�����
	bool IsLoaded();

	//�t�@�C���p�X���擾
	std::string FilePath() const;
	//�f�B���N�g���p�X���擾
	std::string DirPath() const;

	//�V�F�[�_�[���g�����`��
	void RenderShader(CMatrix* m);
	bool IsAddAnimationSet(std::string path) const;
	//�A�j���[�V�����Z�b�g�̒ǉ�
	void AddAnimationSet(const char* file);
	//�A�j���[�V�����𔲂��o��
	void SeparateAnimationSet(int idx, int start, int end, char* name);
	void AnimateVertex(CMatrix* mat);
	//���_�ɃA�j���[�V������K�p
	void AnimateVertex();
	//�X�L���E�F�C�g�̃t���[���ԍ��ݒ�
	void SetSkinWeightFrameIndex();
	//�t���[���̕ϊ��s����A�j���[�V�����f�[�^�ōX�V����
	void AnimateFrame();
	//�m�[�h�̓ǂݔ�΂�
	void SkipNode();

	// �J���[��ݒ�
	void SetColor(const CColor& color);
	// �J���[���擾
	const CColor& GetColor() const;
	// �A���t�@�l�ݒ�
	void SetAlpha(float alpha);
	// �A���t�@�l�擾
	float GetAlpha() const;

	//�`�揈��
	void Render();
};

//CModelXFrame�N���X�̒�`
class CModelXFrame {
	friend CModelX;
	friend CAnimation;
	friend CAnimationSet;
	friend CMyShader;
private:
	std::vector<CModelXFrame*> mChild; //�q�t���[���̔z��
	CMesh* mpMesh;  //Mesh�f�[�^
	CMatrix mTransformMatrix;  //�ϊ��s��
	CMatrix mCombinedMatrix;   //�����s��
	char* mpName;  //�t���[�����O
	int mIndex;  //�t���[���ԍ�

public:
	//�f�t�H���g�R���X�g���N�^
	CModelXFrame();
	//�R���X�g���N�^
	CModelXFrame(CModelX* model, bool dontDelete);
	//�f�X�g���N�^
	~CModelXFrame();

	int Index();

	const CMatrix& CombinedMatrix();
	//�����s��̍쐬
	void AnimateCombined(const CMatrix* parent);
	//�`�揈��
	void Render(const CColor& color);
};

//Mesh�N���X�̒�`
class CMesh {
	friend CModelX;
	friend CModelXFrame;
	friend CSkinWeights;
	friend CMyShader;
private:
	int mFaceNum;          //�ʐ�
	int mVertexNum;	   	   //���_��
	int mNormalNum;        //�@����
	int mMaterialNum;      //�}�e���A����
	int mMaterialIndexNum; //�}�e���A���ԍ���(�ʐ�)
	int* mpMaterialIndex;  //�}�e���A���ԍ�
	int* mpVertexIndex;    //�ʂ��\�����钸�_�C���f�b�N�X
	float* mpTextureCoords; //�e�N�X�`�����W�f�[�^

	std::vector<int> mMaterialVertexCount;		//�}�e���A�����̖ʐ�
	std::vector<CSkinWeights*> mSkinWeights;	//�X�L���E�F�C�g
	std::vector<CMaterial*> mMaterial; //�}�e���A���f�[�^
	CVector* mpVertex;			//���_�f�[�^
	CVector* mpNormal;			//�@���x�N�g��
	CVector* mpAnimateVertex;	//�A�j���[�V�����p���_
	CVector* mpAnimateNormal;	//�A�j���[�V�����p�@��
	CSkinWeights* mpSkinWeghts;

	GLuint mMyVertexBufferId;	//���_�o�b�t�@���ʎq

public:
	//�R���X�g���N�^
	CMesh();
	//�f�X�g���N�^
	~CMesh();

	void CreateVertexBuffer();
	void AnimateVertex(CMatrix* mat);
	//���_�ɃA�j���[�V�����K�p
	void AnimateVertex(CModelX* model);
	//�X�L���E�F�C�g�Ƀt���[���ԍ���ݒ肷��
	void SetSkinWeightFrameIndex(CModelX* model);
	//�ǂݍ��ݏ���
	void Init(CModelX* model, bool dontDelete);
	//�`�揈��
	void Render(const CColor& color);
};

/*
CSkinWeights
�X�L���E�F�C�g�N���X
*/
class CSkinWeights
{
	friend CModelX;
	friend CMesh;
	friend CMyShader;
private:
	char* mpFrameName;	//�t���[����
	int mFrameIndex;	//�t���[���ԍ�
	int mIndexNum;		//���_�ԍ���
	int* mpIndex;		//���_�ԍ��z��
	float* mpWeight;	//���_�E�F�C�g�z��
	CMatrix mOffset;	//�I�t�Z�b�g�}�g���b�N�X

public:
	CSkinWeights(CModelX* model);
	~CSkinWeights();
	const int& FrameIndex();
	const CMatrix& Offset();
};

/*
CAnimationSet
�A�j���[�V�����Z�b�g
*/
class CAnimationSet
{
	friend CModelX;
	friend CXCharacter;
private:
	std::string mPath;
	char* mpName;	//�A�j���[�V�����Z�b�g��
	float mTime;	//���ݎ���
	float mWeight;	//�d��
	float mMaxTime;	//�ő厞��
	std::vector<CAnimation*> mAnimation;

public:
	//�R���X�g���N�^
	CAnimationSet();
	CAnimationSet(CModelX* model);
	//�f�X�g���N�^
	~CAnimationSet();

	float Time();
	float MaxTime();
	std::vector<CAnimation*>& Animation();
	void AnimateMarix(CModelX* model);
	/// <summary>
	/// �A�j���[�V�����̍Đ����Ԃ�ݒ�
	/// </summary>
	/// <param name="time">�ݒ肷�鎞��</param>
	void Time(float time);
	/// <summary>
	/// �A�j���[�V�����̍Đ����Ԃ�i�s�x�Őݒ�
	/// </summary>
	/// <param name="progress">�Đ����Ԃ̐i�s�x�i0�`1�j</param>
	void TimeProgress(float progress);
	void Weight(float weight);	//�d�݂̐ݒ�
};

/*
CAnimation
�A�j���[�V�����N���X
*/
class CAnimation
{
	friend CModelX;
	friend CAnimationSet;
private:
	char* mpFrameName;		//�t���[����
	int mFrameIndex;		//�t���[���ԍ�
	int mKeyNum;			//�L�[��(���Ԑ�)
	CAnimationKey* mpKey;	//�L�[�z��

public:
	//�R���X�g���N�^
	CAnimation();
	CAnimation(CModelX* model);
	//�f�X�g���N�^
	~CAnimation();
};

/*
CAnimationKey
�A�j���[�V�����L�[�N���X
*/
class CAnimationKey
{
	friend CModelX;
	friend CAnimation;
	friend CAnimationSet;
private:
	float mTime;		//����
	CMatrix mMatrix;	//�s��
};
#endif