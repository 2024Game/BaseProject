#pragma once
#include "CSound.h"
#include "BGMData.h"

// BGM�Ǘ��N���X
class CBGMManager
{
public:
	// �C���X�^���X���擾
	static CBGMManager* Instance();
	// �C���X�^���X��j��
	static void ClearInstance();

	/// <summary>
	/// BGM���Đ�
	/// </summary>
	/// <param name="type">�Đ�����BGM�̎�ށieNone��ݒ肷���BGM��~�j</param>
	/// <param name="fromBegin">true�Ȃ�΁ABGM���ŏ�����Đ�����</param>
	/// <param name="fadeTime">�t�F�[�h����</param>
	void Play(EBGMType type, bool fromBegin = true, float fadeTime = 0.5f);

	// �X�V
	void Update();

private:
	// �R���X�g���N�^
	CBGMManager();
	// �f�X�g���N�^
	~CBGMManager();

	// BGM�}�l�[�W���[�̏��
	enum class EState
	{
		eIdle,		// �ҋ@���
		eLoading,	// BGM�ǂݍ��ݒ�
		eFade,		// �t�F�[�h��
	};
	// ��Ԃ�؂�ւ�
	void ChangeState(EState state);

	// BGM�f�[�^���擾
	BGMData GetBGMData(EBGMType type) const;

	// �ҋ@��Ԃ̍X�V����
	void UpdateIdle();
	// BGM�ǂݍ��ݒ��̍X�V����
	void UpdateLoading();
	// �t�F�[�h���̍X�V����
	void UpdateFade();
	// �N���X�t�F�[�h���̍X�V����
	void UpdateCrossFade();

	// BGM�}�l�[�W���[�̃C���X�^���X
	static CBGMManager* ms_instance;
	EState mState;			// ���݂̏��
	int mStateStep;			// ���݂̏�ԓ��̃X�e�b�v
	float mFadeTime;		// �t�F�[�h����
	float mElapsedTime;		// �o�ߎ���
	bool mIsFromBegin;		// BGM���ŏ�����Đ����邩�ǂ���
	BGMData mCurrBGMData;

	CSound* mpCurrentBGM;	// ���ݍĐ�����BGM
	CSound* mpNextBGM;		// ���ɍĐ�����BGM

	// ���ɍĐ�����BGM�̃f�[�^�N���X
	class CNextBGMData
	{
	public:
		bool fromBegin;		// ���ɍĐ�����BGM���ŏ�����Đ����邩�ǂ���
		float fadeTime;		// ����BGM���Đ����鎞�̃t�F�[�h����
		BGMData data;		// ���ɍĐ�����BGM�̏��
	};
	// ���ɍĐ�����BGM�̃f�[�^
	CNextBGMData mNextBGMData;

	EBGMType mLoadedBGMType;// �ǂݍ���BGM�̎��
};
