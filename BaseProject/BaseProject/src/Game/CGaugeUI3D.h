#pragma once
#include "CObjectBase.h"

class CImage3D;

// 3D��Ԃɔz�u����C���[�W�N���X
class CGaugeUI3D : public CObjectBase
{
public:
	// �R���X�g���N�^
	CGaugeUI3D(CObjectBase* owner);
	// �f�X�g���N�^
	virtual ~CGaugeUI3D();

	// �������ݒ�
	void SetOwner(CObjectBase* owner);

	// �ő�l��ݒ�
	void SetMaxPoint(int point);
	// ���ݒl��ݒ�
	void SetCurrPoint(int point);
	// �|�C���g�c�ʂ̊�����ݒ�i0.0�`1.0�j
	void SetPercent(float per);

	// �X�V
	void Update() override;
	// �`��
	void Render() override;

private:
	// �|�C���g�c�ʂ𔽉f
	void ApplyPoint();

	CObjectBase* mpOwner;	// HP�Q�[�W�̎�����

	CImage3D* mpGaugeImg;	// �Q�[�W�̃C���[�W
	CImage3D* mpWhiteImg;	// ���C���[�W

	CVector2 mGaugeSize;	// �Q�[�W�̃C���[�W�̃T�C�Y
	int mMaxPoint;			// �ő�l
	int mCurrPoint;			// ���ݒl
	float mPercent;			// �|�C���g�c�ʂ̊���
};