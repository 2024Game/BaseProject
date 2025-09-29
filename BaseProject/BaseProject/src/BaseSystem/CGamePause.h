#pragma once
#include "CTask.h"
#include "CInput.h"
#include "CImage.h"

//�Q�[���|�[�Y����N���X
class CGamePause : public CTask
{
private:
	bool mIsPaused;			//�|�[�Y�����ǂ���
	int mPauseStep;			//�|�[�Y�����̃X�e�b�v
	float mElapsedTime;		//�o�ߎ��Ԍv���p
	float mPauseTextAlpha;	//PAUSE�e�L�X�g�̃A���t�@�l�ۑ��p

	CImage* mFadeBg;		//�t�F�[�h�w�i�̃C���[�W
	CImage* mPauseText;		//PAUSE�e�L�X�g�̃C���[�W

public:
	//�R���X�g���N�^
	CGamePause();
	//�f�X�g���N�^
	~CGamePause();

	//�X�V
	void Update();
	//�`��
	void Render();
};