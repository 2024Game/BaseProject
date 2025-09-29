#pragma once

// BGM�̎��
enum class EBGMType
{
	eNone = -1,

	eTitle,	// �^�C�g����ʂ�BGM
	eGame,	// �Q�[����ʂ�BGM
	eMenu,	// ���j���[���J���Ă��鎞��BGM

	Num
};

// BGM�f�[�^
struct BGMData
{
	EBGMType type;		// BGM�̎��
	std::string path;	// BGM�̃��\�[�X�p�X
	float baseVolume;	// �x�[�X����
	bool isLoop;		// ���[�v���邩�ǂ���
	int loopBegin;		// ���[�v�J�n�ʒu
	int loopLength;		// ���[�v�I���ʒu
	// ���Z�b�g����
	void Reset()
	{
		type = EBGMType::eNone;
		path = "";
		baseVolume = 1.0f;
		isLoop = true;
		loopBegin = 0;
		loopLength = 0;
	}
};
// BGM�f�[�^�̃e�[�u��
extern const BGMData BGM_DATA[];
// BGM�f�[�^��
extern const int BGM_DATA_COUNT;
