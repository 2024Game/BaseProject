#pragma once
#include "CVector.h"
#include "CTexture.h"
#include "CObjectBase.h"
#include "CTriangle.h"
#include "CMaterial.h"

// �ʒu���킹�̎��
enum class EAlignment
{
	eUpperLeft,		// ����
	eUpperCenter,	// ��
	eUpperRight,	// �E��

	eMiddleLeft,	// ��
	eMiddleCenter,	// ����
	eMiddleRight,	// �E

	eLowerLeft,		// ����
	eLowerCenter,	// ��
	eLowerRight,	// �E��

	eDefault = eMiddleCenter,	// �f�t�H���g
};

// 3D��Ԃɔz�u����C���[�W�`��p�N���X
class CImage3D : public CObjectBase
{
public:
	// �R���X�g���N�^
	CImage3D(std::string path,
		ETag tag,
		ETaskPriority prio = ETaskPriority::eDefault,
		int sortOrder = 0,
		ETaskPauseType pause = ETaskPauseType::eDefault,
		bool dontDelete = false,
		bool addTaskList = true);
	// �f�X�g���N�^
	virtual ~CImage3D();

	// �e�N�X�`���ǂݍ���
	void Load(std::string path);

	// �C���[�W�̃A���C�����g���擾
	EAlignment GetAlignment() const;
	// �C���[�W�̃A���C�����g��ݒ�
	void SetAlignment(EAlignment align);

	// 2D��Ԃł̃I�t�Z�b�g���W���擾
	const CVector2& GetOffsetPos() const;
	// 2D��Ԃł̃I�t�Z�b�g���W��ݒ�
	void SetOffsetPos(const float& x, const float& y);
	// 2D��Ԃł̃I�t�Z�b�g���W��ݒ�iCVector2�Łj
	void SetOffsetPos(const CVector2& pos);

	// �T�C�Y�擾
	const CVector2& GetSize() const;
	// �T�C�Y�ݒ�
	void SetSize(const float& x, const float& y);
	// �T�C�Y�ݒ�iCVector2�Łj
	void SetSize(const CVector2& size);

	// UV�擾
	const CRect& GetUV() const;
	// UV�ݒ�
	void SetUV(const float& left, const float& top, const float& right, const float& bottom);
	// UV�ݒ�iCRect�Łj
	void SetUV(const CRect& uv);

	// 3D��Ԃł̏k�ڂ�ݒ�
	void SetWorldUnitPerPixel(float pixel);

	/// <summary>
	/// �A�j���[�V�����f�[�^��ݒ�
	/// </summary>
	/// <param name="animData"></param>
	void SetAnimData(TexAnimData* animData);
	// �A�j���[�V�������I��������ǂ���
	bool IsEndAnim() const;

	// �r���{�[�h�̃I���I�t��ݒ�
	void SetBillboard(bool enable);
	// �f�v�X�e�X�g�̃I���I�t��ݒ�
	void SetDepthTest(bool enable);
	// �f�v�X�e�X�g�̃I���I�t��ݒ�
	void SetDepthMask(bool enable);
	// ���C�e�B���O�̃I���I�t��ݒ�
	void SetLighting(bool enable);

	// �X�V
	void Update() override;
	// �`��
	void Render() override;

protected:
	// �ύX�𒸓_�ɔ��f
	void ApplyVertices();
	// �A�j���[�V�������Đ��ł��邩�ǂ���
	bool IsEnableAnim() const;
	// �`��i�}�e���A���w��Łj
	void Render(CMaterial* mpMaterial);

	// �O�p�`2��
	CTriangle mT[2];
	// �}�e���A��
	CMaterial mMaterial;
	// �C���[�W�̃A���C�����g
	EAlignment mAlignment;
	// 2D��Ԃł̃I�t�Z�b�g���W
	CVector2 mOffsetPos;
	// �T�C�Y�L���p
	CVector2 mSize;
	// �e�N�X�`����UV
	CRect mUV;
	// 3D��Ԃł̏k��
	float mWolrdUnitPerPixel;

	// �A�j���[�V�����f�[�^�̃��X�g
	TexAnimData* mpAnimData;
	// ���݂̃A�j���[�V�����ԍ�
	int mAnimNo;
	// �A�j���[�V�����p�̌o�ߎ��Ԍv���ϐ�
	float mElapsedTime;

	bool mIsBillboard;
	// �f�v�X�e�X�g���L�����ǂ���
	bool mIsDepthTest;
	// �f�v�X�������݂��L�����ǂ���
	bool mIsDepthMask;
	// ���C�e�B���O���L�����ǂ���
	bool mIsLighting;
};
