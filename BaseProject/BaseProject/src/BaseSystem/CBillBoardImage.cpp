#include "CBillBoardImage.h"

// �R���X�g���N�^
CBillBoardImage::CBillBoardImage(std::string path, ETag tag, ETaskPauseType pause)
	: CImage3D(path, tag, ETaskPriority::eBillboard, 0, pause)
{
	// �r���{�[�h���I��
	SetBillboard(true);
}

// �f�X�g���N�^
CBillBoardImage::~CBillBoardImage()
{
}
