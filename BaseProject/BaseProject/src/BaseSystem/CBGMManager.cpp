#include "CBGMManager.h"

// BGM�}�l�[�W���[�̃C���X�^���X
CBGMManager* CBGMManager::ms_instance = nullptr;

// �C���X�^���X���擾
CBGMManager* CBGMManager::Instance()
{
	if (ms_instance == nullptr)
	{
		ms_instance = new CBGMManager();
	}
	return ms_instance;
}

// �C���X�^���X��j��
void CBGMManager::ClearInstance()
{
	SAFE_DELETE(ms_instance);
}

// �R���X�g���N�^
CBGMManager::CBGMManager()
	: mState(EState::eIdle)
	, mStateStep(0)
	, mFadeTime(0.0f)
	, mElapsedTime(0.0f)
	, mIsFromBegin(true)
	, mpCurrentBGM(nullptr)
	, mpNextBGM(nullptr)
	, mLoadedBGMType(EBGMType::eNone)
{
	mNextBGMData.fromBegin = true;
	mNextBGMData.fadeTime = 0.0f;
	mNextBGMData.data.Reset();
}

// �f�X�g���N�^
CBGMManager::~CBGMManager()
{
	ms_instance = nullptr;
}

// ��Ԃ�؂�ւ�
void CBGMManager::ChangeState(EState state)
{
	mState = state;
	mStateStep = 0;
}

// BGM�f�[�^���擾
BGMData CBGMManager::GetBGMData(EBGMType type) const
{
	for (int i = 0; i < BGM_DATA_COUNT; i++)
	{
		const BGMData& data = BGM_DATA[i];
		if (data.type == type) return data;
	}

	BGMData ret;
	ret.Reset();
	return ret;
}

// BGM���Đ�
void CBGMManager::Play(EBGMType type, bool fromBegin, float fadeTime)
{
	// ���ɍĐ�����BGM�̃f�[�^��ݒ�
	mNextBGMData.fromBegin = fromBegin;
	mNextBGMData.fadeTime = fadeTime;
	mNextBGMData.data = GetBGMData(type);
	// ���݂��ҋ@��Ԃł���΁ABGM�ǂݍ��ݏ�Ԃֈڍs
	if (mState == EState::eIdle)
	{
		ChangeState(EState::eLoading);
	}
}

// �ҋ@��Ԃ̍X�V����
void CBGMManager::UpdateIdle()
{
}

// BGM�ǂݍ��ݒ��̍X�V����
void CBGMManager::UpdateLoading()
{
	// �X�e�b�v���Ƃɏ�����؂�ւ���
	switch (mStateStep)
	{
		// �X�e�b�v0�FBGM���L�����`�F�b�N
		case 0:
			mIsFromBegin = mNextBGMData.fromBegin;
			mFadeTime = mNextBGMData.fadeTime;
			mElapsedTime = 0.0f;
			mLoadedBGMType = mNextBGMData.data.type;
			mCurrBGMData = mNextBGMData.data;
			// �L���łȂ�BGM�ł���΁A���t�F�[�h��Ԃֈڍs
			if (mLoadedBGMType == EBGMType::eNone)
			{
				mpNextBGM = nullptr;
				ChangeState(EState::eFade);
			}
			// �L���ł���΁A���̃X�e�b�v��
			else
			{
				mStateStep++;
			}
			break;
		// �X�e�b�v1�FBGM��ǂݍ���
		case 1:
		{
			std::string path = mNextBGMData.data.path;
			mpNextBGM = CResourceManager::Load<CSound>(path, path, true);
			mpNextBGM->SetBaseVolume(mCurrBGMData.baseVolume);
			mStateStep++;
			break;
		}
		// �X�e�b�v2�FBGM�ǂݍ��ݑ҂�
		case 2:
			// BGM�̓ǂݍ��݂��I���΁A�t�F�[�h��Ԃֈڍs
			ChangeState(EState::eFade);
			break;
	}
}

// �t�F�[�h���̍X�V����
void CBGMManager::UpdateFade()
{
	switch (mStateStep)
	{
		// �X�e�b�v0�F���ݍĐ�����BGM���t�F�[�h�A�E�g
		case 0:
			// ���ݍĐ�����BGM�����݂��Ȃ��ꍇ�́A���̃X�e�b�v��
			if (mpCurrentBGM == nullptr)
			{
				mStateStep++;
			}
			else
			{
				// �t�F�[�h���Ԃ��o�߂��Ă��Ȃ�
				if (mElapsedTime < mFadeTime)
				{
					// �o�ߎ��Ԃɍ��킹�ă{�����[���������Ă���
					float alpha = mElapsedTime / mFadeTime;
					mpCurrentBGM->SetVolume(1.0f - alpha);
					// �o�ߎ��Ԃ����Z
					mElapsedTime += Times::CalcDeltaTime();
				}
				// �o�ߎ��Ԃ��o�߂���
				else
				{
					// �{�����[����0�ɂ��Ē�~
					mpCurrentBGM->SetVolume(0.0f);
					mpCurrentBGM->Stop(-1, 0.0f);
					mpCurrentBGM = nullptr;

					mElapsedTime = 0.0f;
					mStateStep++;
				}
			}
			break;
		// �X�e�b�v1�F���ɍĐ�����BGM�����݂���΁A�{�����[��0�ōĐ��J�n
		case 1:
			if (mpNextBGM != nullptr)
			{
				mpNextBGM->PlayLoop
				(
					mCurrBGMData.isLoop ? -1 : 0,
					mIsFromBegin,
					0.0f,
					true,
					0.0f
				);
			}
			mStateStep++;
			break;
		// �X�e�b�v2�F���ɍĐ�����BGM���t�F�[�h�C��
		case 2:
			// ���ɍĐ�����BGM�����݂��Ȃ��ꍇ�́A���̃X�e�b�v��
			if (mpNextBGM == nullptr)
			{
				mStateStep++;
			}
			else
			{
				// �t�F�[�h���Ԃ��o�߂��Ă��Ȃ�
				if (mElapsedTime < mFadeTime)
				{
					// �o�ߎ��Ԃɍ��킹�ă{�����[�����グ�Ă���
					float alpha = mElapsedTime / mFadeTime;
					mpNextBGM->SetVolume(alpha);
					// �o�ߎ��Ԃ����Z
					mElapsedTime += Times::CalcDeltaTime();
				}
				// �o�ߎ��Ԃ��o�߂���
				else
				{
					// �{�����[����1�ɐݒ�
					mpNextBGM->SetVolume(1.0f);
					// ���ݍĐ�����BGM�Ɏ���BGM��ݒ�
					mpCurrentBGM = mpNextBGM;
					// ����BGM����ɂ���
					mpNextBGM = nullptr;

					mElapsedTime = 0.0f;
					mStateStep++;
				}
			}
			break;
		case 3:
			// �ǂݍ���BGM�Ǝ��ɍĐ�����BGM�̎�ނ��Ⴄ�ꍇ�́A
			// BGM�ǂݍ��ݏ�Ԃֈڍs
			if (mLoadedBGMType != mNextBGMData.data.type)
			{
				ChangeState(EState::eLoading);
			}
			// ���ɍĐ�����BGM�����݂��Ȃ��ꍇ�́A�ҋ@��Ԃֈڍs
			else
			{
				ChangeState(EState::eIdle);
			}
			break;
	}
}

// �N���X�t�F�[�h���̍X�V����
void CBGMManager::UpdateCrossFade()
{
	// �t�F�[�h���Ԃ��o�߂��Ă��Ȃ�
	if (mElapsedTime < mFadeTime)
	{
		float alpha = mElapsedTime / mFadeTime;
		// ���ݍĐ�����BGM�����݂���΁A�{�����[����ݒ�
		if (mpCurrentBGM != nullptr)
		{
			mpCurrentBGM->SetVolume(1.0f - alpha);
		}
		// ���ɍĐ�����BGM�����݂���΁A�{�����[����ݒ�
		if (mpNextBGM != nullptr)
		{
			mpNextBGM->SetVolume(alpha);
		}
		// �o�ߎ��Ԃ����Z
		mElapsedTime += Times::CalcDeltaTime();
	}
	// �o�ߎ��Ԃ��o�߂���
	else
	{
		// ���ݍĐ�����BGM�����݂���΁A�{�����[����0�ɂ��Ē�~
		if (mpCurrentBGM != nullptr)
		{
			mpCurrentBGM->SetVolume(0.0f);
			mpCurrentBGM->Stop(-1, 0.0f);
		}
		// ���ɍĐ�����BGM�����݂���΁A
		if (mpNextBGM != nullptr)
		{
			// �{�����[����1�ɐݒ�
			mpNextBGM->SetVolume(1.0f);
			// ���ݍĐ�����BGM�Ɏ���BGM��ݒ�
			mpCurrentBGM = mpNextBGM;
			// ����BGM����ɂ���
			mpNextBGM = nullptr;
		}
		// ���ɍĐ�����BGM�����݂��Ȃ��Ȃ�΁A���݂�BGM����ɂ���
		else
		{
			mpCurrentBGM = nullptr;
		}

		// �ǂݍ���BGM�Ǝ��ɍĐ�����BGM�̎�ނ��Ⴄ�ꍇ�́A
		// BGM�ǂݍ��ݏ�Ԃֈڍs
		if (mLoadedBGMType != mNextBGMData.data.type)
		{
			ChangeState(EState::eLoading);
		}
		// ���ɍĐ�����BGM�����݂��Ȃ��ꍇ�́A�ҋ@��Ԃֈڍs
		else
		{
			ChangeState(EState::eIdle);
		}
	}
}

// �X�V
void CBGMManager::Update()
{
	// ���݂̏�Ԃɍ��킹�āA�X�V������؂�ւ���
	switch (mState)
	{
		// �ҋ@���
		case EState::eIdle:
			UpdateIdle();
			break;
		// BGM�ǂݍ��ݒ�
		case EState::eLoading:
			UpdateLoading();
			break;
		// �t�F�[�h��
		case EState::eFade:
			UpdateFade();
			//UpdateCrossFade();
			break;
	}
}