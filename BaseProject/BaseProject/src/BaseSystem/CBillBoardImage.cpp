#include "CBillboardImage.h"

// �R���X�g���N�^
CBillboardImage::CBillboardImage(std::string path, ETag tag, ETaskPauseType pause)
	: CImage3D(path, tag, ETaskPriority::eBillboard, 0, pause)
{
	// �r���{�[�h���I��
	SetBillboard(true);
}

// �f�X�g���N�^
CBillboardImage::~CBillboardImage()
{
}
