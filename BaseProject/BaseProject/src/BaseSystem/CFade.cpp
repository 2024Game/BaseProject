#include "CFade.h"

// �t�F�[�h�N���X�̃C���X�^���X
CFade* CFade::ms_instance = nullptr;

// �R���X�g���N�^
CFade::CFade()
	: CTask(ETaskPriority::eFade, 0, ETaskPauseType::eSystem, true, true)
	, mFadeColor(CColor::black)
	, mFadeTime(0.0f)
	, mElapsedTime(0.0f)
	, mIsFadeIn(false)
	, mIsFading(false)
{
	// �t�F�[�h�摜��ǂݍ���
	mpFadeImage = new CImage
	(
		"UI/white.png",
		ETaskPriority::eFade, 0,
		ETaskPauseType::eSystem,
		true, false // �t�F�[�h�摜�̓^�X�N���X�g�ɓo�^���Ȃ�
	);
	// �t�F�[�h�摜����ʑS�̂̃T�C�Y�ɕύX
	mpFadeImage->SetSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	// �t�F�[�h�J���[��ݒ�
	mpFadeImage->SetColor(mFadeColor);
}

// �f�X�g���N�^
CFade::~CFade()
{
	// �t�F�[�h�摜���^�X�N���X�g�ɓo�^���Ă��Ȃ��̂ŁA
	// �f�X�g���N�^�Ŏ����Ŕj������
	SAFE_DELETE(mpFadeImage);

	ms_instance = nullptr;
}

// �C���X�^���X���擾
CFade* CFade::Instance()
{
	if (ms_instance == nullptr)
	{
		ms_instance = new CFade();
	}
	return ms_instance;
}

// �t�F�[�h�J���[��ݒ�
void CFade::SetFadeColor(const CColor& color)
{
	CFade* inst = Instance();
	inst->mFadeColor.Set(color.R(), color.G(), color.B());
	inst->mpFadeImage->SetColor(inst->mFadeColor);
}

// �t�F�[�h�C���J�n
void CFade::FadeIn(float time)
{
	Instance()->StartFade(time, true);
}

// �t�F�[�h�A�E�g�J�n
void CFade::FadeOut(float time)
{
	Instance()->StartFade(time, false);
}

// �t�F�[�h�J�n
void CFade::StartFade(float time, bool isFadeIn)
{
	// �e�p�����[�^��ݒ�
	mFadeTime = time;
	mElapsedTime = 0.0f;
	mIsFadeIn = isFadeIn;

	// �t�F�[�h���Ԃ�0���ǂ����ŏ�����Ԃ̃A���t�@�l��ύX
	float alpha = mFadeTime <= 0.0f ? 1.0f : 0.0f;
	// ������Ԃ̃A���t�@�l��ݒ�
	mFadeColor.A(mIsFadeIn ? 1.0f - alpha : alpha);
	mpFadeImage->SetColor(mFadeColor);

	// �t�F�[�h���Ԃ�0��蒷���ꍇ�́A�t�F�[�h�t���O�𗧂Ă�
	mIsFading = mFadeTime > 0.0f;
}

// �t�F�[�h�����ǂ���
bool CFade::IsFading()
{
	return Instance()->mIsFading;
}

// �X�V
void CFade::Update()
{
	// �t�F�[�h���łȂ���΁A�������Ȃ�
	if (!mIsFading) return;

	// �t�F�[�h���Ԃ��o�߂��Ă��Ȃ�
	if (mElapsedTime < mFadeTime)
	{
		// �o�ߎ��Ԃɍ��킹�āA�t�F�[�h�C���[�W�̃A���t�@�l��ύX
		float alpha = mElapsedTime / mFadeTime;
		mFadeColor.A(mIsFadeIn ? 1.0f - alpha : alpha);
		mpFadeImage->SetColor(mFadeColor);
		// �o�ߎ��ԉ��Z
		mElapsedTime += Times::DeltaTime();
	}
	// �t�F�[�h���Ԃ��o�߂���
	else
	{
		// �t�F�[�h�I��
		mIsFading = false;
		mElapsedTime = mFadeTime;
		mFadeColor.A(mIsFadeIn ? 0.0f : 1.0f);
		mpFadeImage->SetColor(mFadeColor);
	}
}

// �`��
void CFade::Render()
{
	if (mFadeColor.A() == 0.0f) return;
	mpFadeImage->Render();
}
