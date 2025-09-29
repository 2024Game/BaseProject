#pragma once
#include "CSceneBase.h"
class CColBase;
class CColSphere;
class CColLine;
class CColTriangle;
class CColQuad;

// �Փ˔���e�X�g�V�[��
class CCollisionTestScene : public CSceneBase
{
public:
	// �R���X�g���N�^
	CCollisionTestScene();
	// �f�X�g���N�^
	~CCollisionTestScene();

	// �V�[���ǂݍ���
	void Load() override;

	// �R���C�_�[��I��
	void Select(CColBase* col);

	// �X�V
	void Update() override;

private:
	CColBase* mpSelectCol;			// �I�����Ă���R���C�_�[
	std::vector<CColBase*> mColList;// �����ς݂̃R���C�_�[���X�g
};