#include "CTitleScene.h"
#include "CSceneManager.h"
#include "CGameScene.h"
#include "CCamera.h"
#include "CSound.h"
#include "CBGMManager.h"
#include "CTitleUI.h"

//�R���X�g���N�^
CTitleScene::CTitleScene()
	: CSceneBase(EScene::eTitle)
{
}

//�f�X�g���N�^
CTitleScene::~CTitleScene()
{
}

//�V�[���ǂݍ���
void CTitleScene::Load()
{
	// �^�C�g����ʂ̓J�[�\���\��
	CInput::ShowCursor(true);
	// �w�i�F�ݒ�
	System::SetClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	//�����Ń^�C�g����ʂɕK�v��
	//���\�[�X�̓ǂݍ��݂�N���X�̐������s��

	// �^�C�g��BGM���Đ�
	CBGMManager::Instance()->Play(EBGMType::eTitle);

	//mBgImage = new CImage("white.png");
	//mBgImage->SetColor(0.0f, 0.0f, 0.0f, 1.0f);
	//mBgImage->SetPos(0.0f, 0.0f);
	//mBgImage->SetSize(WINDOW_WIDTH, WINDOW_HEIGHT);

	CCamera* mainCamera = new CCamera
	(
		CVector(0.0f, 50.0f, 75.0f),
		CVector::zero
	);

	mpTitleUI = new CTitleUI();
	AddTask(mpTitleUI);
}

//�V�[���̍X�V����
void CTitleScene::Update()
{
	// �^�C�g����ʂ�
	if (mpTitleUI->IsEnd())
	{
		// �Q�[���J�n�Ȃ�΁A�Q�[���V�[����ǂݍ���
		if (mpTitleUI->IsStartGame())
		{
			CSceneManager::Instance()->LoadScene(EScene::eGame);
		}
		// �Q�[���I���Ȃ�΁A�A�v�������
		else if (mpTitleUI->IsExitGame())
		{
			System::ExitGame();
		}
	}
}
