#include "CSceneManager.h"
#include "CFade.h"
#include "CTitleScene.h"
#include "CGameScene.h"

#include "Test/CBootMenuScene.h"
#include "Test/CCollisionTestScene.h"

// �V�[���}�l�[�W���̃C���X�^���X
CSceneManager* CSceneManager::mpInstance = nullptr;

// �C���X�^���X���擾
CSceneManager* CSceneManager::Instance()
{
	// �C���X�^���X�����݂��Ȃ��ꍇ�͐V������������
	if (mpInstance == nullptr)
	{
		mpInstance = new CSceneManager();
	}
	return mpInstance;
}

// �C���X�^���X��j��
void CSceneManager::ClearInstance()
{
	delete mpInstance;
	mpInstance = nullptr;
}

// �R���X�g���N�^
CSceneManager::CSceneManager()
	: mpScene(nullptr)
	, mNextScene(EScene::eNone)
	, mState(ESceneState::eIdle)
	, mStateStep(0)
{
}

// �f�X�g���N�^
CSceneManager::~CSceneManager()
{
	// �ǂݍ���ł���V�[��������΁A�j������
	UnloadScene();
}

// �V�[���̏����������������ǂ���
bool CSceneManager::IsReady() const
{
	// �ҋ@��Ԃł���΁A�����������Ă���
	return mState == ESceneState::eIdle;
}

// ���݂̏�Ԃ��擾
ESceneState CSceneManager::GetState() const
{
	return mState;
}

// ���݂̏�Ԃ�؂�ւ�
void CSceneManager::ChangeState(ESceneState state)
{
	mState = state;
	mStateStep = 0;
}

// �V�[���ǂݍ���
void CSceneManager::LoadScene(EScene scene)
{
	// ���ɃV�[�����ǂݍ��܂�Ă���ꍇ�́A
	// �ҋ@��ԂłȂ���Ύ��̃V�[����ǂݍ��߂Ȃ�
	if (mpScene != nullptr && mState != ESceneState::eIdle) return;

	// ���̃V�[����ݒ�
	mNextScene = scene;

	// ���ɃV�[�����ǂݍ��܂�Ă���ꍇ�́A
	// �t�F�[�h�A�E�g��Ԃֈڍs
	if (mpScene != nullptr)
	{
		ChangeState(ESceneState::eFadeOut);
	}
	// �ǂݍ��܂�Ă��Ȃ��ꍇ�́A���̂܂܃V�[���ǂݍ��ݏ�Ԃֈڍs
	else
	{
		ChangeState(ESceneState::eLoading);
	}
}

// �ǂݍ���ł���V�[����j��
void CSceneManager::UnloadScene()
{
	delete mpScene;
	mpScene = nullptr;
}

// ���̃V�[���ɐ؂�ւ�
void CSceneManager::ChangeNextScene()
{
	//���ɃV�[�����ǂݍ��܂�Ă�����A
	//���̃V�[����j��
	UnloadScene();

	//�ǂݍ��ރV�[���̎�ނɍ��킹��
	//��������V�[���̃N���X��ύX����
	switch (mNextScene)
	{
		//�^�C�g���V�[��
		case EScene::eTitle:	mpScene = new CTitleScene();	break;
		//�Q�[���V�[��
		case EScene::eGame:		mpScene = new CGameScene();		break;

		//�N�������j���[
		case EScene::eBootMenu:	mpScene = new CBootMenuScene();	break;
		//�Փ˔���e�X�g
		case EScene::eColTest:	mpScene = new CCollisionTestScene(); break;
	}

	//�V�[����V���������ł�����A
	//�V�[���ǂݍ��݊J�n
	if (mpScene != nullptr)
	{
		mpScene->Load();
	}

	// ���ɓǂݍ��ރV�[����������
	mNextScene = EScene::eNone;
}

// ���ݓǂݍ���ł���V�[���̎�ނ��擾
EScene CSceneManager::GetCurrentScene() const
{
	if (mpScene == nullptr) return EScene::eNone;
	return mpScene->GetSceneType();
}

// �V�[���ǂݍ��ݏ�Ԃł̍X�V����
void CSceneManager::UpdateLoading()
{
	// �X�e�b�v���Ƃɏ�����؂�ւ���
	switch (mStateStep)
	{
		// �X�e�b�v0�F���̃V�[����ǂݍ���
		case 0:
			ChangeNextScene();
			mStateStep++;
			break;
		// �X�e�b�v1�F�ǂݍ��ݏI���҂�
		case 1:
			// �񓯊��ǂݍ��ݖ��Ή��Ȃ��߁A
			// ���t�F�[�h�C����Ԃֈڍs
			ChangeState(ESceneState::eFadeIn);
			break;
	}
}

// �t�F�[�h�C����Ԃł̍X�V����
void CSceneManager::UpdateFadeIn()
{
	// �X�e�b�v���Ƃɏ�����؂�ւ���
	switch (mStateStep)
	{
		// �X�e�b�v0�F�t�F�[�h�C���J�n
		case 0:
			CFade::FadeIn();
			mStateStep++;
			break;
		// �X�e�b�v1�F�t�F�[�h�C���I���҂�
		case 1:
			// �t�F�[�h�C�����I��������A�ҋ@��Ԃֈڍs
			if (!CFade::IsFading())
			{
				ChangeState(ESceneState::eIdle);
			}
			break;
	}
}

// �ҋ@��Ԃł̍X�V����
void CSceneManager::UpdateIdle()
{
	//�V�[�����ǂݍ��܂�Ă��Ȃ��ꍇ�́A�������Ȃ�
	if (mpScene == nullptr) return;
	//�V�[���X�V
	mpScene->Update();
}

// �t�F�[�h�A�E�g��Ԃł̍X�V����
void CSceneManager::UpdateFadeOut()
{
	// �X�e�b�v���Ƃɏ�����؂�ւ���
	switch (mStateStep)
	{
		// �X�e�b�v0�F�t�F�[�h�A�E�g�J�n
		case 0:
			CFade::FadeOut();
			mStateStep++;
			break;
		// �X�e�b�v1�F�t�F�[�h�A�E�g�I���҂�
		case 1:
			// �t�F�[�h�A�E�g���I��������A�V�[���ǂݍ��ݏ�Ԃֈڍs
			if (!CFade::IsFading())
			{
				ChangeState(ESceneState::eLoading);
			}
			break;
	}
}

// �X�V
void CSceneManager::Update()
{
	// ���݂̏�Ԃɍ��킹�čX�V������؂�ւ���
	switch (mState)
	{
		// �V�[���ǂݍ��ݏ��
		case ESceneState::eLoading:
			UpdateLoading();
			break;
		// �t�F�[�h�C�����
		case ESceneState::eFadeIn:
			UpdateFadeIn();
			break;
		// �ҋ@���
		case ESceneState::eIdle:
			UpdateIdle();
			break;
		// �t�F�[�h�A�E�g���
		case ESceneState::eFadeOut:
			UpdateFadeOut();
			break;
	}
}
