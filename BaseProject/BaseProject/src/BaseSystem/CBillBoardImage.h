#pragma once
#include "CImage3D.h"

// ��ɃJ�����̕����������C���[�W
class CBillBoardImage : public CImage3D
{
public:
	// �R���X�g���N�^
	CBillBoardImage(std::string path,
		ETag tag = ETag::eNone,
		ETaskPauseType pause = ETaskPauseType::eDefault);
	// �f�X�g���N�^
	virtual ~CBillBoardImage();

private:
};