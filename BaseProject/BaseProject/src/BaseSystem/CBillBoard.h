#ifndef CBILLBOARD_H
#define CBILLBOARD_H
#include "CObjectBase.h"
#include "CTriangle.h"
#include "CMaterial.h"
#include "CColor.h"

/*
�r���{�[�h�N���X
��ɃJ�����̕��������l�p�`
*/
class CBillBoard : public CObjectBase {
public:
	//�R���X�g���N�^
	CBillBoard(ETag tag = ETag::eNone,
		ETaskPauseType pause = ETaskPauseType::eDefault);

	void SetSize(const CVector2& size);

	//�X�V
	void Update() override;
	//�`��
	void Render() override;
	void Render(CMaterial* mpMaterial);

protected:
	//�O�p�`2��
	CTriangle mT[2];
	//�}�e���A��
	CMaterial mMaterial;
};
#endif
