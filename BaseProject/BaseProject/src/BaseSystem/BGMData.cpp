#include "BGMData.h"

// BGM�f�[�^�̃e�[�u��
const BGMData BGM_DATA[] =
{
	//BGM�̎��			���\�[�X�p�X				�x�[�X����	���[�v	���[�v�J�n	���[�v�I��
	{ EBGMType::eTitle,	"Sound\\BGM\\title.wav",	0.0625f,		true,	0,			0		},
	{ EBGMType::eGame,	"Sound\\BGM\\game.wav",		0.0625f,		true,	0,			2801203	},
	{ EBGMType::eMenu,	"Sound\\BGM\\menu.wav",		0.0625f,		true,	0,			0		},
};

// BGM�f�[�^��
const int BGM_DATA_COUNT = ARRAY_SIZE(BGM_DATA);