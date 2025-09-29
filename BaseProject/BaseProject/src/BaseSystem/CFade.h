#pragma once
#include "CTask.h"
#include "CColor.h"
#include "CImage.h"

// ��ʂ̃t�F�[�h����
class CFade : public CTask
{
public:
	// �C���X�^���X���擾
	static CFade* Instance();

	/// <summary>
	/// �t�F�[�h�J���[��ݒ�
	/// </summary>
	/// <param name="color">�ݒ肷��F</param>
	static void SetFadeColor(const CColor& color);

	/// <summary>
	/// �t�F�[�h�C���J�n
	/// </summary>
	/// <param name="time">�t�F�[�h����</param>
	static void FadeIn(float time = 0.25f);
	/// <summary>
	/// �t�F�[�h�A�E�g�J�n
	/// </summary>
	/// <param name="time">�t�F�[�h����</param>
	static void FadeOut(float time = 0.25f);

	/// <summary>
	/// �t�F�[�h�����ǂ���
	/// </summary>
	/// <returns>true�Ȃ�΁A�t�F�[�h��</returns>
	static bool IsFading();

private:
	// �R���X�g���N�^
	CFade();
	// �f�X�g���N�^
	~CFade();

	/// <summary>
	/// �t�F�[�h�J�n
	/// </summary>
	/// <param name="time">�t�F�[�h����</param>
	/// <param name="isFadeIn">�t�F�[�h�C�����ǂ���</param>
	void StartFade(float time, bool isFadeIn);

	// �X�V
	void Update() override;
	// �`��
	void Render() override;

	// �t�F�[�h�N���X�̃C���X�^���X
	static CFade* ms_instance;

	CImage* mpFadeImage;	// �t�F�[�h�摜
	CColor mFadeColor;		// �t�F�[�h�J���[
	float mFadeTime;		// �t�F�[�h����
	float mElapsedTime;		// �o�ߎ���
	bool mIsFadeIn;			// �t�F�[�h�C�����ǂ���
	bool mIsFading;			// �t�F�[�h�����ǂ���
};