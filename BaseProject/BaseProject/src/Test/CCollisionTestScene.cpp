#include "CCollisionTestScene.h"
#include "CColSphere.h"
#include "CColLine.h"
#include "CColTriangle.h"
#include "CColQuad.h"
#include "CColCapsule.h"
#include "CCamera.h"
#include "CMeasure.h"
#include "CDebugInput.h"

// �R���X�g���N�^
CCollisionTestScene::CCollisionTestScene()
	: CSceneBase(EScene::eColTest)
	, mpSelectCol(nullptr)
{
}

// �f�X�g���N�^
CCollisionTestScene::~CCollisionTestScene()
{
}

// �V�[���ǂݍ���
void CCollisionTestScene::Load()
{
	// �w�i�F�ݒ�
	System::SetClearColor(0.1921569f, 0.3019608f, 0.4745098f, 1.0f);

	// ���̔|���S������
	new CColQuad
	(
		CVector2(200.0f, 200.0f),
		CColor::white,
		true
	);

	// ����p�̕\�����𐶐�
	new CMeasure(100);

	// ���𐶐�
	//CColSphere* cs = new CColSphere(10.0f, CColor::gray);
	//cs->Scale(2.0f, 2.0f, 2.0f);
	//cs->Position(10.0f, 10.0f, 0.0f);
	//mColList.push_back(cs);

	//CColSphere* cs2 = new CColSphere(1.0f, CColor::gray);
	//cs2->Scale(5.0f, 5.0f, 5.0f);
	//cs2->Position(-10.0f, 10.0f, 0.0f);
	//mColList.push_back(cs2);


	// �����𐶐�
	//CColLine* cl = new CColLine
	//(
	//	CVector(0.0f, 11.0f, 0.0f),
	//	CVector(0.0f, 1.0f, 0.0f),
	//	CColor::gray
	//);
	//mColList.push_back(cl);

	// �J�v�Z���𐶐�
	CColCapsule* cc = new CColCapsule
	(
		CVector(5.0f, 15.0f, -5.0f),
		CVector(5.0f, 11.0f, 5.0f),
		2.0f,
		CColor::gray
	);
	mColList.push_back(cc);

	CColCapsule* cc2 = new CColCapsule
	(
		CVector(-10.0f, 15.0f, 2.0f),
		CVector(-10.0f, 6.0f, -1.0f),
		5.0f,
		CColor::gray
	);
	mColList.push_back(cc2);

	//mpColTriangle = new CColTriangle
	//(
	//	CVector(-100.0f, -10.0f,  100.0f),
	//	CVector( 100.0f, -10.0f,  100.0f),
	//	CVector(   0.0f, -10.0f, -100.0f)
	//);

	// �J��������
	CCamera* mainCamera = new CCamera
	(
		CVector(0.0f, 50.0f, 75.0f),
		CVector::zero
	);
}

// �R���C�_�[��I��
void CCollisionTestScene::Select(CColBase* col)
{
	// �I�𒆂̃R���C�_�[�̑I������
	if (mpSelectCol != nullptr)
	{
		mpSelectCol->SetSelect(false);
	}

	// �I�𒆂̃R���C�_�[���ēx�w�肳�ꂽ�ꍇ��
	// �I����������
	if (mpSelectCol == col)
	{
		mpSelectCol = nullptr;
	}
	// �I�𒆂̃R���C�_�[�ȊO
	else
	{
		mpSelectCol = col;

		// �w�肳�ꂽ�R���C�_�[��I��
		if (mpSelectCol != nullptr)
		{
			mpSelectCol->SetSelect(true);
		}
	}
}

// �X�V
void CCollisionTestScene::Update()
{
#if _DEBUG
	int size = mColList.size();
	for (int i = 0; i < size; i++)
	{
		if (CDebugInput::PushKey('1' + i))
		{
			Select(mColList[i]);
		}
	}
#endif
}
