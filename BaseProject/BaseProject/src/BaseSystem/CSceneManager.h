#pragma once
#include "CSceneBase.h"
#include "SceneType.h"

// �V�[���̏��
enum class ESceneState
{
	eLoading,	// �V�[���ǂݍ��ݒ�
	eFadeIn,	// �V�[���J�n���̃t�F�[�h�C����
	eIdle,		// �ҋ@��
	eFadeOut,	// �V�[���I�����̃t�F�[�h�A�E�g��
};

// �V�[���Ǘ��N���X
class CSceneManager
{
public:
	/// <summary>
	/// �C���X�^���X���擾
	/// </summary>
	/// <returns>�C���X�^���X</returns>
	static CSceneManager* Instance();
	// �C���X�^���X��j��
	static void ClearInstance();

	// �V�[���̏����������������ǂ���
	bool IsReady() const;

	// ���݂̏�Ԃ��擾
	ESceneState GetState() const;

	// �V�[���ǂݍ���
	void LoadScene(EScene scene);
	// �ǂݍ���ł���V�[����j��
	void UnloadScene();

	/// <summary>
	/// ���ݓǂݍ���ł���V�[���̎�ނ��擾
	/// </summary>
	/// <returns>�V�[���̎�ށieNone�̏ꍇ�̓V�[�����ǂݍ��܂�Ă��Ȃ��j</returns>
	EScene GetCurrentScene() const;

	// �X�V
	void Update();

private:
	// �R���X�g���N�^
	CSceneManager();
	// �f�X�g���N�^
	~CSceneManager();

	// ���݂̏�Ԃ�؂�ւ�
	void ChangeState(ESceneState state);

	// ���̃V�[���ɐ؂�ւ�
	void ChangeNextScene();

	// �V�[���ǂݍ��ݏ�Ԃł̍X�V����
	void UpdateLoading();
	// �t�F�[�h�C����Ԃł̍X�V����
	void UpdateFadeIn();
	// �ҋ@��Ԃł̍X�V����
	void UpdateIdle();
	// �t�F�[�h�A�E�g��Ԃł̍X�V����
	void UpdateFadeOut();

	// �V�[���}�l�[�W���̃C���X�^���X
	static CSceneManager* mpInstance;
	// ���ݓǂݍ���ł���V�[��
	CSceneBase* mpScene;
	// ���ɓǂݍ��ރV�[��
	EScene mNextScene;
	// �V�[���̌��݂̏��
	ESceneState mState;
	// ��ԓ��ł̃X�e�b�v
	int mStateStep;
};