#pragma once
#include <stdio.h>
#include <Windows.h>
#include <mmsystem.h>
#include <xaudio2.h>
#include <vector>
#include <list>
#include "CResource.h"

#pragma comment(lib,"xaudio2.lib")
#pragma comment(lib,"winmm.lib")

class CAudioSource;
class CFadeSound;
class CSoundManager;

// �T�E���h�Đ��N���X
class CSound : public CResource
{
	friend CResourceManager;
	friend CSoundManager;
public:
	/// <summary>
	/// �T�E���h���Đ�
	/// </summary>
	/// <param name="volume">�{�����[��</param>
	/// <param name="force">�Đ��g������Ȃ����ɁA���ɍĐ����̉������~���čĐ����邩�ǂ���</param>
	/// <param name="fadeTime">�Đ����̃t�F�[�h�C������</param>
	/// <returns></returns>
	int Play(float volume = 1.0f, bool force = false, float fadeTime = 0.0f);
	/// <summary>
	/// �T�E���h�����[�v�Đ�
	/// </summary>
	/// <param name="loopCount">���[�v���i0:���[�v�Ȃ��A-1:�i�����[�v�j</param>
	/// <param name="fromBegin">�ŏ�����Đ����邩�ǂ���</param>
	/// <param name="volume">�{�����[��</param>
	/// <param name="force">�Đ��g������Ȃ����ɁA���ɍĐ����̉������~���čĐ����邩�ǂ���</param>
	/// <param name="fadeTime">�Đ����̃t�F�[�h�C������</param>
	/// <returns></returns>
	int PlayLoop(int loopCount = -1, bool fromBegin = true, float volume = 1.0f, bool force = false, float fadeTime = 0.0f);

	/// <summary>
	/// �T�E���h���~
	/// </summary>
	/// <param name="index">��~����C���f�b�N�X�i�w��Ȃ���΁A�S�Ē�~�j</param>
	/// <param name="fadeTime">��~���̃t�F�[�h�A�E�g����</param>
	void Stop(int index = -1, float fadeTime = 0.125f);

	/// <summary>
	/// �T�E���h���Đ������ǂ���
	/// </summary>
	/// <returns></returns>
	bool IsPlaying(int index = -1) const;

	/// <summary>
	/// �T�E���h�𓯎��Đ��ł��鐔��ݒ�
	/// </summary>
	/// <param name="count">�����Đ��ł��鐔</param>
	void SetSimultaneousPlayCount(int count);

	/// <summary>
	/// �T�E���h�̃x�[�X���ʂ�ݒ�
	/// </summary>
	/// <param name="volume">�x�[�X����</param>
	void SetBaseVolume(float volume);

	/// <summary>
	/// �T�E���h�̉��ʂ�ݒ�
	/// </summary>
	/// <param name="volume">�ݒ肷�鉹��</param>
	/// <param name="index">�ݒ肷��C���f�b�N�X�i�w��Ȃ���΁A�S�Ăɐݒ�j</param>
	void SetVolume(float volume, int index = -1);

	/// <summary>
	/// ���[�v�͈͂�ݒ�
	/// </summary>
	/// <param name="loopBegin">���[�v�J�n�ʒu</param>
	/// <param name="loopLength">���[�v�̒���</param>
	void SetLoopRange(int loopBegin, int loopLength);

	/// <summary>
	/// �T�E���h�̃T���v�������擾
	/// </summary>
	/// <returns></returns>
	int GetSampleCount();
	/// <summary>
	/// �Đ����̃T�E���h�̌��݂̃T���v�������擾
	/// </summary>
	/// <returns></returns>
	int GetCurrentSampleCount(int index = 0);

private:
	// �R���X�g���N�^
	CSound();
	// �f�X�g���N�^
	~CSound();

	/// <summary>
	/// �����t�@�C����ǂݍ���
	/// </summary>
	/// <param name="path">�ǂݍ��މ����t�@�C���̃p�X</param>
	/// <returns></returns>
	bool Load(std::string path, bool dontDelete) override;

	// �����Đ��p�̏��f�[�^���쐬
	bool CreateAudioSource();

	// �������Đ����Ă��Ȃ��C���f�b�N�X���擾
	int FindNotPlayingSourceVoice(bool force) const;

	/// <summary>
	/// �T�E���h���t�F�[�h�J�n
	/// </summary>
	/// <param name="voice">�t�F�[�h�������s���T�E���h</param>
	/// <param name="fadeTime">�t�F�[�h����</param>
	/// <param name="volume">�{�����[��</param>
	/// <param name="isFadeIn">�t�F�[�h�C�����ǂ���</param>
	void AddFadeSound(IXAudio2SourceVoice* voice, float fadeTime, float volume, bool isFadeIn);

	// �X�V
	void Update();

	// �������Đ�����\�[�X�{�C�X�̃��X�g
	std::vector<CAudioSource> mAudioSources;
	// �t�F�[�h���̃T�E���h�̃��X�g
	std::list<CFadeSound> mFadeSounds;

	HMMIO g_hmmio;
	MMIOINFO g_mmioinfo;
	MMCKINFO g_riffchunkinfo;
	MMCKINFO g_datachunkinfo;
	WAVEFORMATEX g_wfx;
	LONG g_readlen;
	unsigned char* mpBuf;
	int mSampleCount;
	int mLoopBegin;
	int mLoopLength;

	float mBaseVolume;	// �x�[�X����
};

// �����Đ��p�̏��
class CAudioSource
{
public:
	// �������Đ�����\�[�X�{�C�X
	IXAudio2SourceVoice* voice;
	// �����̍Đ����
	XAUDIO2_BUFFER info;
};

// �����t�F�[�h�p�̏��
class CFadeSound
{
public:
	IXAudio2SourceVoice* voice;
	float fadeTime;
	float elapsedTime;
	float volume;
	bool isFadeIn;
};
