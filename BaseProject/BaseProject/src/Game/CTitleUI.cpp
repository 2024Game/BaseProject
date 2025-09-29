#include "CTitleUI.h"
#include "CFont.h"
#include "CText.h"
#include "CImage.h"
#include "Maths.h"
#include "CInput.h"
#include "CFade.h"
#include "CExpandButton.h"
#include "Easing.h"

// �uCLICK TO START�v�̓_�Ŏ���
#define START_TEXT_BLINK_TIME 1.0f
// �uCLICK TO START�v�̓_�ŊԊu
#define START_TEXT_INTERVAL_TIME 3.0f
// �^�C�g�����j���[�̃A�j���[�V��������
#define OPEN_ANIM_TIME 0.25f
// �^�C�g�����j���[�̃A�j���[�V������̑҂�����
#define OPENED_WAIT_TIME 0.5f

// �R���X�g���N�^
CTitleUI::CTitleUI()
	: CTask(ETaskPriority::eUI, 0, ETaskPauseType::eDefault)
	, mState(EState::eIdle)
	, mStateStep(0)
	, mElapsedTime(0.0f)
	, mIsEnd(false)
{
	// �^�C�g�����S�̃t�H���g�f�[�^�𐶐�
	mpLogoFont = new CFont("res\\Font\\toroman.ttf");
	mpLogoFont->SetFontSize(128);
	mpLogoFont->SetAlignment(FTGL::TextAlignment::ALIGN_CENTER);
	mpLogoFont->SetLineLength(WINDOW_WIDTH);

	// �^�C�g�����S�̃e�L�X�g�𐶐�
	mpTitleLogo = new CText
	(
		mpLogoFont, 128,
		CVector2(0.0f, 32.0f),
		CVector2(WINDOW_WIDTH, WINDOW_HEIGHT),
		CColor(0.11f, 0.1f, 0.1f),
		ETaskPriority::eUI,
		0,
		ETaskPauseType::eDefault,
		false,
		false
	);
	mpTitleLogo->SetText("�^�C�g�����S");
	mpTitleLogo->SetEnableOutline(true);
	mpTitleLogo->SetOutlineColor(CColor(0.9f, 0.9f, 0.9f));

	// �^�C�g����ʂ̔w�i�C���[�W�𐶐�
	mpTitleBg = new CImage
	(
		"UI/title_bg.png",
		ETaskPriority::eUI,
		0,
		ETaskPauseType::eDefault,
		false,
		false
	);

	// �uCLICK TO START�v�̃e�L�X�g�𐶐�
	mpStartText = new CText
	(
		nullptr, 32,
		CVector2(0.0f, -64.0f),
		CVector2(WINDOW_WIDTH, WINDOW_HEIGHT),
		CColor(1.0f, 1.0f, 0.5f),
		ETaskPriority::eUI,
		0,
		ETaskPauseType::eDefault,
		false,
		false
	);
	mpStartText->SetText("- CLICK TO START -");
	mpStartText->SetTextAlignH(ETextAlignH::eCenter);
	mpStartText->SetTextAlignV(ETextAlignV::eBottom);
	mpStartText->SetEnableShadow(true);
	mpStartText->SetShadowColor(CColor(0.25f, 0.125f, 0.0f));
	mpStartText->SetEnableOutline(true);
	mpStartText->SetOutlineColor(CColor(0.0f, 0.0f, 0.0f));

	// [START]�{�^���𐶐�
	CExpandButton* btn1 = new CExpandButton
	(
		CVector2(WINDOW_WIDTH * 0.5f, 450.0f),
		CVector2(181.0f, 47.0f),
		ETaskPriority::eUI, 0, ETaskPauseType::eGame,
		false, false
	);
	// �{�^���̉摜��ǂݍ���
	btn1->LoadButtonImage("UI/title_start0.png", "UI/title_start1.png");
	// �{�^���N���b�N���ɌĂяo�����R�[���o�b�N�֐���ݒ�
	btn1->SetOnClickFunc(std::bind(&CTitleUI::OnClickStart, this));
	// �{�^���͍ŏ��͖��������āA�X�P�[���l��0�ɂ��Ă���
	btn1->SetEnable(false);
	btn1->SetScale(0.0f);
	// �{�^�����X�g�ɒǉ�
	mButtons.push_back(btn1);

	// [OPTION]�{�^���𐶐�
	CExpandButton* btn2 = new CExpandButton
	(
		CVector2(WINDOW_WIDTH * 0.5f, 550.0f),
		CVector2(181.0f, 47.0f),
		ETaskPriority::eUI, 0, ETaskPauseType::eGame,
		false, false
	);
	btn2->LoadButtonImage("UI/title_option0.png", "UI/title_option1.png");
	btn2->SetOnClickFunc(std::bind(&CTitleUI::OnClickOption, this));
	btn2->SetEnable(false);
	btn2->SetScale(0.0f);
	mButtons.push_back(btn2);

	// [QUIT]�{�^���𐶐�
	CExpandButton* btn3 = new CExpandButton
	(
		CVector2(WINDOW_WIDTH * 0.5f, 650.0f),
		CVector2(181.0f, 47.0f),
		ETaskPriority::eUI, 0, ETaskPauseType::eGame,
		false, false
	);
	btn3->LoadButtonImage("UI/title_quit0.png", "UI/title_quit1.png");
	btn3->SetOnClickFunc(std::bind(&CTitleUI::OnClickQuit, this));
	btn3->SetEnable(false);
	btn3->SetScale(0.0f);
	mButtons.push_back(btn3);
}

// �f�X�g���N�^
CTitleUI::~CTitleUI()
{
	SAFE_DELETE(mpLogoFont);
	SAFE_DELETE(mpTitleLogo);
	SAFE_DELETE(mpTitleBg);
	SAFE_DELETE(mpStartText);

	int size = mButtons.size();
	for (int i = 0; i < size; i++)
	{
		CButton* btn = mButtons[i];
		mButtons[i] = nullptr;
		SAFE_DELETE(btn);
	}
	mButtons.clear();
}

// �^�C�g����ʏI����
bool CTitleUI::IsEnd() const
{
	return mIsEnd;
}

// �Q�[�����J�n���邩
bool CTitleUI::IsStartGame() const
{
	// �I�����ڂ�1�ڂȂ�΁A�Q�[���J�n
	return mSelectIndex == 0;
}

// �Q�[�����I�����邩
bool CTitleUI::IsExitGame() const
{
	// �I�����ڂ�3�ڂȂ�΁A�Q�[���I��
	return mSelectIndex == 2;
}

// ��Ԑ؂�ւ�
void CTitleUI::ChangeState(EState state)
{
	if (state == mState) return;
	mState = state;
	mStateStep = 0;
	mElapsedTime = 0.0f;
}

// [START]�N���b�N���̃R�[���o�b�N�֐�
void CTitleUI::OnClickStart()
{
	if (mIsEnd) return;

	mSelectIndex = 0;
	mIsEnd = true;
}

// [OPTION]�N���b�N���̃R�[���o�b�N�֐�
void CTitleUI::OnClickOption()
{
	if (mIsEnd) return;

	mSelectIndex = 1;
}

// [QUIT]�N���b�N���̃R�[���o�b�N�֐�
void CTitleUI::OnClickQuit()
{
	if (mIsEnd) return;

	mSelectIndex = 2;
	mIsEnd = true;
}

// �ҋ@���
void CTitleUI::UpdateIdle()
{
	// �ҋ@��Ԃ��X�e�b�v�Ǘ�
	switch (mStateStep)
	{
		// �X�e�b�v0�F�uCLICK TO START�v�̓_�ő҂�
		case 0:
			mpStartText->SetAlpha(1.0f);
			if (mElapsedTime < START_TEXT_INTERVAL_TIME)
			{
				mElapsedTime += Times::DeltaTime();
			}
			else
			{
				mElapsedTime = 0.0f;
				mStateStep = 1;
			}
			break;
		// �X�e�b�v1�F�uCLICK TO START�v�̓_��
		case 1:
			if (mElapsedTime < START_TEXT_BLINK_TIME)
			{
				// �T�C���J�[�u�œ_��
				float per = sinf(M_PI * mElapsedTime / START_TEXT_BLINK_TIME);
				mpStartText->SetAlpha(1.0f - per);
				mElapsedTime += Times::DeltaTime();
			}
			else
			{
				mElapsedTime = 0.0f;
				mpStartText->SetAlpha(1.0f);
				mStateStep = 0;
			}
			break;
	}

	// �t�F�[�h���I����Ă���
	if (!CFade::IsFading())
	{
		// ���N���b�N����ƁA���j���[���J��
		if (CInput::PushKey(VK_LBUTTON))
		{
			ChangeState(EState::eOpen);
		}
	}
}

// ���j���[���J��
void CTitleUI::UpdateOpen()
{
	switch (mStateStep)
	{
		// �X�e�b�v0�F���j���[�̓���A�j���[�V����
		case 0:
			if (mElapsedTime < OPEN_ANIM_TIME)
			{
				// �X�P�[���l����U1.0���傫�����āA1.0�֖߂�C�[�W���O�A�j���[�V����
				float scale = Easing::BackOut(mElapsedTime, OPEN_ANIM_TIME, 0.0f, 1.0f, 2.0f);
				for (CExpandButton* btn : mButtons)
				{
					btn->SetScale(scale);
				}
				mElapsedTime += Times::DeltaTime();
			}
			else
			{
				for (CExpandButton* btn : mButtons)
				{
					btn->SetScale(1.0f);
				}
				mStateStep++;
				mElapsedTime = 0.0f;
			}
			break;
		// �X�e�b�v1�F���j���[�����̑҂�
		case 1:
			if (mElapsedTime < OPENED_WAIT_TIME)
			{
				mElapsedTime += Times::DeltaTime();
			}
			else
			{
				// ��莞�ԑ҂�����A�{�^�����I���ɂ��ă^�b�`�ł���悤�ɂ���
				// �i��^�b�`��h�����߂̑҂����ԁj
				for (CExpandButton* btn : mButtons)
				{
					btn->SetEnable(true);
				}
				ChangeState(EState::eSelect);
			}
			break;
	}
}

// ���j���[�I��
void CTitleUI::UpdateSelect()
{
}

// �t�F�[�h�A�E�g
void CTitleUI::UpdateFadeOut()
{
}

// �X�V
void CTitleUI::Update()
{
	switch (mState)
	{
		// �ҋ@���
		case EState::eIdle:
			UpdateIdle();
			break;
		// ���j���[���J��
		case EState::eOpen:
			UpdateOpen();
			break;
		// ���j���[�I��
		case EState::eSelect:
			UpdateSelect();
			break;
		// �t�F�[�h�A�E�g
		case EState::eFadeOut:
			UpdateFadeOut();
			break;
	}

	mpTitleLogo->Update();
	mpTitleBg->Update();
	mpStartText->Update();
	for (CButton* btn : mButtons)
	{
		btn->Update();
	}
}

// �`��
void CTitleUI::Render()
{
	// �`�揇�F
	// �w�i���^�C�g�����S���uCLICK TO START�v�����j���[�{�^��

	// �w�i�`��
	mpTitleBg->Render();
	// �^�C�g�����S�`��
	mpTitleLogo->Render();

	// �ҋ@��ԂȂ�΁A�uCLICK TO START�v��\��
	if (mState == EState::eIdle)
	{
		mpStartText->Render();
	}
	// �ҋ@��ԈȊO�́A���j���[�{�^����\��
	else
	{
		for (CButton* btn : mButtons)
		{
			btn->Render();
		}
	}
}
