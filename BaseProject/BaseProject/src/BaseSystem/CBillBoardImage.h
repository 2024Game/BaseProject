#pragma once
#include "CImage3D.h"

// ��ɃJ�����̕����������C���[�W
class CBillboardImage : public CImage3D
{
public:
	// �R���X�g���N�^
	CBillboardImage(std::string path,
		ETag tag = ETag::eNone,
		ETaskPauseType pause = ETaskPauseType::eDefault);
	// �f�X�g���N�^
	virtual ~CBillboardImage();

private:
};