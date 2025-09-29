#pragma once
#include <stdio.h>
#include <Windows.h>
#include <mmsystem.h>
#include <xaudio2.h>
#include <list>

#pragma comment(lib,"xaudio2.lib")
#pragma comment(lib,"winmm.lib")

class CSound;

// �T�E���h�Đ��Ǘ��N���X
class CSoundManager
{
public:
	// �C���X�^���X���擾
	static CSoundManager* Instance();
	// �C���X�^���X��j��
	static void ClearInstance();

	/// <summary>
	/// �����f�[�^���Đ����邽�߂̃\�[�X�{�C�X�𐶐�
	/// </summary>
	/// <param name="format">�����f�[�^�̃t�H�[�}�b�g</param>
	/// <returns>���������\�[�X�{�C�X�̃|�C���^�[</returns>
	IXAudio2SourceVoice* CreateSourceVoice(const WAVEFORMATEX* format);

	// �T�E���h�����X�g�ɒǉ�
	void Add(CSound* sound);
	// �T�E���h�����X�g�����菜��
	void Remove(CSound* sound);

	// �X�V
	void Update();

private:
	// �R���X�g���N�^
	CSoundManager();
	// �f�X�g���N�^
	~CSoundManager();

	// �T�E���h�}�l�[�W���̃C���X�^���X
	static CSoundManager* mpInstance;
	bool mIsInitialized;		// �������ς݂�
	IXAudio2* mpXAudio;
	IXAudio2MasteringVoice* mpMasterVoice;
	std::list<CSound*> mSounds;	// �����ς݂̃T�E���h�̃��X�g
};