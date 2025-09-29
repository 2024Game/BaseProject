#include "CSoundManager.h"
#include "CSound.h"
#include "CBGMManager.h"

// �T�E���h�}�l�[�W���[�̃C���X�^���X
CSoundManager* CSoundManager::mpInstance = nullptr;

// �C���X�^���X���擾
CSoundManager* CSoundManager::Instance()
{
	// �C���X�^���X���Ȃ���΁A�V������������
	if (mpInstance == nullptr)
	{
		// �C���X�^���X����
		mpInstance = new CSoundManager();
		// ���������s������A�C���X�^���X��j��
		if (!mpInstance->mIsInitialized)
		{
			SAFE_DELETE(mpInstance);
		}
	}
	return mpInstance;
}

// �C���X�^���X��j��
void CSoundManager::ClearInstance()
{
	SAFE_DELETE(mpInstance);
}

// �R���X�g���N�^
CSoundManager::CSoundManager()
	: mIsInitialized(false)
	, mpXAudio(nullptr)
	, mpMasterVoice(nullptr)
{
	HRESULT hr;
	(void)CoInitializeEx(NULL, COINIT_MULTITHREADED);
	hr = XAudio2Create(&mpXAudio, 0, XAUDIO2_DEFAULT_PROCESSOR);
	if (FAILED(hr))
	{
		CoUninitialize();
		mpXAudio = nullptr;
		return;
	}

	hr = mpXAudio->CreateMasteringVoice(&mpMasterVoice, XAUDIO2_DEFAULT_CHANNELS, XAUDIO2_DEFAULT_SAMPLERATE, 0, 0, NULL);
	if (FAILED(hr))
	{
		if (mpXAudio != nullptr) mpXAudio->Release();
		mpXAudio = nullptr;
		mpMasterVoice = nullptr;
		CoUninitialize();
		return;
	}

	// �������I��
	mIsInitialized = true;
}

// �f�X�g���N�^
CSoundManager::~CSoundManager()
{
	if (mpMasterVoice != nullptr)
	{
		mpMasterVoice->DestroyVoice();
		mpMasterVoice = nullptr;
	}

	if (mpXAudio != nullptr)
	{
		mpXAudio->Release();
		mpXAudio = nullptr;
	}

	CoUninitialize();
}

// �����f�[�^���Đ����邽�߂̃\�[�X�{�C�X�𐶐�
IXAudio2SourceVoice* CSoundManager::CreateSourceVoice(const WAVEFORMATEX* format)
{
	if (mpXAudio == nullptr) return nullptr;

	// �\�[�X�{�C�X���쐬
	IXAudio2SourceVoice* ret = nullptr;
	HRESULT hr = mpXAudio->CreateSourceVoice
	(
		&ret, format,
		XAUDIO2_VOICE_NOPITCH /*| XAUDIO2_VOICE_MUSIC*/,
		XAUDIO2_DEFAULT_FREQ_RATIO,
		NULL,
		NULL,
		NULL
	);
	// �쐬�Ɏ��s�����ꍇ�́Anullptr��Ԃ�
	if (FAILED(hr)) return nullptr;

	return ret;
}

// �T�E���h�����X�g�ɒǉ�
void CSoundManager::Add(CSound* sound)
{
	mSounds.push_back(sound);
}

// �T�E���h�����X�g�����菜��
void CSoundManager::Remove(CSound* sound)
{
	mSounds.remove(sound);
}

// �X�V
void CSoundManager::Update()
{
	// ���X�g�ɓo�^����Ă���T�E���h���X�V
	for (CSound* sound : mSounds)
	{
		sound->Update();
	}

	// BGM�}�l�[�W���[�̍X�V
	CBGMManager::Instance()->Update();
}