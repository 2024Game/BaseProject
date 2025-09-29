#pragma once
#include "CEffect.h"
#include "CTexture.h"

class CLinePoint
{
public:
	CVector pos;
	float startWidth;
	float endWidth;
	float elapsed;
	float alpha;
	CLinePoint(const CVector& p, float sw, float ew)
		: pos(p), startWidth(sw), endWidth(ew), elapsed(0.0f), alpha(1.0f) {};
};

// �����G�t�F�N�g
class CLineEffect : public CEffect
{
public:
	// �R���X�g���N�^
	CLineEffect(ETag tag);
	// �f�X�g���N�^
	virtual ~CLineEffect();

	// �������ݒ�
	void SetOwner(CObjectBase* owner);

	// �e�N�X�`���ݒ�
	void SetTexture(std::string texName);

	// UV�ݒ�
	void SetBaseUV(const CRect& uv);

	// �A�j���[�V�����f�[�^��ݒ�
	void SetAnimData(TexAnimData* animData);

	// �A�j���[�V�������I��������ǂ���
	bool IsEndAnim() const;

	// ���̃|�C���g��ǉ�
	int AddPoint(const CVector& pos, float startWidth, float endWidth);
	// ���̃|�C���g���폜
	void RemovePoint(int index);

	// ���̃|�C���g�����擾
	int GetPointCount() const;

	// ���̃|�C���g�̍��W��ݒ�
	void SetPos(int index, const CVector& pos);
	// ���̃|�C���g�̍��W���擾
	CVector GetPos(int index) const;

	//// ���̃|�C���g�̕���ݒ�
	//void SetWidth(int index, float width);
	//// ���̃|�C���g�̕����擾
	//float GetWidth(int index) const;

	// �X�V
	void Update() override;
	// �`��
	void Render() override;

private:
	// ���W���v�Z
	CVector CalcPos(const CVector& pos) const;
	// �A�j���[�V�������Đ��ł��邩�ǂ���
	bool IsEnableAnim() const;

protected:
	// ������
	CObjectBase* mpOwner;

	// ���̃e�N�X�`��
	CTexture* mpTexture;
	// ���̃|�C���g�̃��X�g
	std::vector<CLinePoint*> mPoints;

	// �A�j���[�V�����f�[�^�̃��X�g
	TexAnimData* mpAnimData;
	// ���݂̃A�j���[�V�����ԍ�
	int mAnimNo;
	// �A�j���[�V�����p�̌o�ߎ��Ԍv���ϐ�
	float mElapsedTime;

	CRect mBaseUV;
	CRect mUV;
};