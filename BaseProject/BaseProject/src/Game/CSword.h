#pragma once
#include "CWeapon.h"
#include "CModel.h"

// ���̃N���X
class CSword : public CWeapon
{
public:
	// �R���X�g���N�^
	CSword(CObjectBase* owner, ETag tag,
		Tags hitTags, Layers hitLayers);
	// �f�X�g���N�^
	~CSword();

	// �`��
	void Render() override;

private:
	CModel* mpModel;	// ���̃��f��
};
