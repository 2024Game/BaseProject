#include "CImage.h"

//�R���X�g���N�^
CImage::CImage(const char* path, ETaskPriority prio, int sortOrder,
	ETaskPauseType pause, bool dontDelete, bool addTaskList)
	: CUIBase(prio, sortOrder, pause, dontDelete, addTaskList)
	, mpTexture(nullptr)
	, mUV(0.0f, 0.0f, 1.0f, 1.0f)
{
	mPosition = CVector2(0.0f, 0.0f);
	mSize = CVector2(128.0f, 128.0f);
	mColor = CColor(1.0f, 1.0f, 1.0f, 1.0f);

	Load(path, dontDelete);
}

//�f�X�g���N�^
CImage::~CImage()
{
}

//�e�N�X�`���̓ǂݍ���
void CImage::Load(const char* path, bool dontDelete)
{
	mpTexture = CResourceManager::Load<CTexture>(path, path, dontDelete);
	//�ǂݍ��ݐ���
	if (mpTexture != nullptr)
	{
		//�\���T�C�Y���e�N�X�`���̃T�C�Y�ŏ�����
		const STgamHeader& header = mpTexture->Header();
		SetSize(header.width, header.height);
	}
}

//�\������͈͂�ݒ�iCVector4�Łj
void CImage::SetUV(const CVector4& uv)
{
	mUV = uv;
}

//�\������͈͂�ݒ�
void CImage::SetUV(const float& left, const float& top, const float& right, const float& bottom)
{
	SetUV(CVector4(left, top, right, bottom));
}

//�\������͈͂��擾
const CVector4& CImage::GetUV() const
{
	return mUV;
}

//�`��
void CImage::Render()
{
	//�e�N�X�`�����ǂݍ��܂�ĂȂ���΁A�`�揈���͍s��Ȃ�
	if (mpTexture == nullptr) return;

	glPushMatrix();

	glLoadIdentity();

	//�e�N�X�`����L���ɂ���
	glEnable(GL_TEXTURE_2D);
	//�A���t�@�u�����h��L���ɂ���
	glEnable(GL_BLEND);
	//�u�����h���@���w��
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//�e�N�X�`�����w��
	glBindTexture(GL_TEXTURE_2D, mpTexture->Id());

	glBegin(GL_TRIANGLES);
	// �\���J���[��ݒ�
	glColor4fv((GLfloat*)&mColor);

	float uvLeft = mUV.X();
	float uvTop = mUV.Y();
	float uvRight = mUV.Z();
	float uvBottom = mUV.W();

	float vtxLeft = mPosition.X() - mCenter.X();
	float vtxRight = vtxLeft + mSize.X();
	float vtxBottom = mPosition.Y() - mCenter.Y();
	float vtxTop = vtxBottom + mSize.Y();

	glTexCoord2f(uvLeft, uvTop);
	glVertex2d(vtxLeft, vtxTop);

	glTexCoord2f(uvLeft, uvBottom);
	glVertex2d(vtxLeft, vtxBottom);

	glTexCoord2f(uvRight, uvBottom);
	glVertex2d(vtxRight, vtxBottom);

	glTexCoord2f(uvLeft, uvTop);
	glVertex2d(vtxLeft, vtxTop);

	glTexCoord2f(uvRight, uvBottom);
	glVertex2d(vtxRight, vtxBottom);

	glTexCoord2f(uvRight, uvTop);
	glVertex2d(vtxRight, vtxTop);

	glEnd();

	//�e�N�X�`�������
	glBindTexture(GL_TEXTURE_2D, 0);
	//�A���t�@�u�����h�𖳌�
	glDisable(GL_BLEND);
	//�e�N�X�`���𖳌�
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();
}
