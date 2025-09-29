#define _USE_MATH_DEFINES
#include <cmath>
#include "CGamePause.h"
#include "CTaskManager.h"
#include "Maths.h"

//�R���X�g���N�^
CGamePause::CGamePause()
	: CTask(ETaskPriority::eSystem, 0, ETaskPauseType::eSystem, true)
	, mIsPaused(false)
	, mPauseStep(0)
	, mElapsedTime(0.0f)
	, mPauseTextAlpha(0.0f)
{
	//�t�F�[�h�w�i�p�̔��C���[�W��ǂݍ���
	mFadeBg = new CImage
	(
		"UI/white.png",
		ETaskPriority::eUI, 0,
		ETaskPauseType::eSystem,
		true, false
	);
	//����͍��w�i�ɂ���̂ŃJ���[����(0, 0, 0)�ɂ���
	//�ŏ��͔�\���̂܂܂ɂ���̂ŃA���t�@�l��0
	mFadeBg->SetColor(0.0f, 0.0f, 0.0f, 0.0f);
	//��ʑS�Ă𕢂��ݒ�
	//���W(0, 0)����T�C�Y(��ʕ�, ��ʍ���)�܂ł��w��
	mFadeBg->SetPos(0.0f, 0.0f);
	mFadeBg->SetSize(WINDOW_WIDTH, WINDOW_HEIGHT);

	//PAUSE�e�L�X�g�̃C���[�W�ǂݍ���
	mPauseText = new CImage
	(
		"UI/pause_text.png",
		ETaskPriority::eUI, 0,
		ETaskPauseType::eSystem,
		true, false
	);
	//��������ŏ��͔�\���Ȃ̂ŃA���t�@�l��0�ɂ��Ă���
	mPauseText->SetColor(1.0f, 1.0f, 1.0f, 0.0f);
	//��ʒ����ɔz�u
	const CVector2& size = mPauseText->GetSize();
	CVector2 pos = (CVector2(WINDOW_WIDTH, WINDOW_HEIGHT) - size) * 0.5f;
	mPauseText->SetPos(pos);
}

//�f�X�g���N�^
CGamePause::~CGamePause()
{
	//�t�F�[�h�w�i�̃C���[�W��ǂݍ���ł�����j��
	SAFE_DELETE(mFadeBg);
	//PAUSE�e�L�X�g�̃C���[�W��ǂݍ���ł�����j��
	SAFE_DELETE(mPauseText);
}

//�X�V
void CGamePause::Update()
{
	//�|�[�Y���ł͂Ȃ��ꍇ
	if (!mIsPaused)
	{
		//�uP�v�L�[�������ƁA�|�[�Y�J�n
		if (CInput::PushKey('P'))
		{
			//TaskManager����|�[�Y��ݒ肷��
			CTaskManager::Instance()->Pause(PAUSE_GAME);
			//�|�[�Y���̃t���O�����Ă�
			mIsPaused = true;
		}
	}
	//�|�[�Y��
	else
	{
		//�t�F�[�h�w�i�̕\������\���Ɋ|���鎞�ԁi�b�j
		static const float fadeTime = 0.25f;
		//�t�F�[�h�w�i�̍ő�A���t�@�l
		static const float fadeAlpha = 0.5f;
		//PAUSE�e�L�X�g�̃A�j���[�V�������ԁi�b�j
		static const float animTime = 1.0f;

		//�|�[�Y�X�e�b�v���Ƃɏ����𕪂���
		switch (mPauseStep)
		{
			//�|�[�Y�J�n�����̂Ńt�F�[�h�w�i��\��
			case 0:
			{
				//�t�F�[�h�w�i�̕\�����Ԃ��o�߂��Ă��Ȃ�
				if (mElapsedTime < fadeTime)
				{
					//�o�ߎ��Ԃɍ��킹�ăA���t�@�l��ݒ�
					float alpha = mElapsedTime / fadeTime;
					mFadeBg->SetAlpha(fadeAlpha * alpha);
					//1�t���[���̌o�ߎ��Ԃ����Z�i���j
					mElapsedTime += 0.016f;
				}
				//�t�F�[�h�w�i�̕\�����Ԃ��o�߂���
				else
				{
					//�t�F�[�h�w�i�����S�ɕ\������
					//���̃X�e�b�v��
					mFadeBg->SetAlpha(fadeAlpha);
					mElapsedTime = 0.0f;
					mPauseStep++;
				}
				break;
			}
			//PAUSE�e�L�X�g�̓_�ŃA�j���[�V�����Đ�
			case 1:
			{
				//�o�ߎ��Ԃɍ��킹��Sin�J�[�u��
				//�A���t�@�l��ݒ肵�ē_�ł�����
				float alpha = mElapsedTime / animTime;
				mPauseTextAlpha = sin(M_PI * 2.0f * alpha) * 0.5f + 0.5f;
				mPauseText->SetAlpha(mPauseTextAlpha);

				//�o�ߎ��Ԃ��A�j���[�V�������Ԃ𒴂�����A
				//0�ɖ߂��ă��[�v������
				mElapsedTime += 0.016f;
				if (mElapsedTime >= animTime) mElapsedTime -= animTime;

				//���̃X�e�b�v�ōēx�uP�v�L�[�������ꂽ��
				if (CInput::PushKey('P'))
				{
					//�|�[�Y�I���X�e�b�v�ֈڍs
					mElapsedTime = 0.0f;
					mPauseStep++;
				}
				break;
			}
			//�|�[�Y�I������̂Ń|�[�Y�w�i���\��
			case 2:
			{
				// �|�[�Y�w�i�̕\�������Ɣ��΂̏����Ȃ̂ŃR�����g�͊���
				if (mElapsedTime < fadeTime)
				{
					float alpha = mElapsedTime / fadeTime;
					//��\���ɂ��Ă����̂Łu1.0f-alpha�v
					mFadeBg->SetAlpha(fadeAlpha * (1.0f - alpha));
					mPauseText->SetAlpha(mPauseTextAlpha * (1.0f - alpha));
					mElapsedTime += 0.016f;
				}
				else
				{
					//�I�����̓A���t�@�l0�Ŕ�\��
					mFadeBg->SetAlpha(0.0f);
					mPauseText->SetAlpha(0.0f);
					mElapsedTime = 0.0f;
					mPauseStep = 0;
					//TaskManager����|�[�Y����������
					CTaskManager::Instance()->UnPause(PAUSE_GAME);
					//�|�[�Y�t���O�����낷
					mIsPaused = false;
				}
				break;
			}
		}
	}
}

//�`��
void CGamePause::Render()
{
	//�t�F�[�h�w�i��ǂݍ���ł�����`��
	if (mFadeBg != nullptr)
	{
		mFadeBg->Render();
	}
	//PAUSE�e�L�X�g��ǂݍ���ł�����`��
	if (mPauseText != nullptr)
	{
		mPauseText->Render();
	}
}