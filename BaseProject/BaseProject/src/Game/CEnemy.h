#ifndef CENEMY_H
#define CENEMY_H
#include "CXCharacter.h"
#include "CCollider.h"
#include "CModel.h"

class CGaugeUI3D;

/*
�G�l�~�[�N���X
�L�����N�^�N���X���p��
*/
class CEnemy : public CXCharacter
{
public:
	// �A�j���[�V�����f�[�^
	struct AnimData
	{
		std::string path;	// �A�j���[�V�����f�[�^�̃p�X
		bool loop;			// ���[�v���邩�ǂ���
		float frameLength;	// �A�j���[�V�����̃t���[����
		float speed;		// �A�j���[�V�����̍Đ����x�i1.0�œ��{�j
	};

	// �R���X�g���N�^
	CEnemy();
	// �f�X�g���N�^
	virtual ~CEnemy();

	// �I�u�W�F�N�g�폜��`����֐�
	void DeleteObject(CObjectBase* obj) override;

	/// <summary>
	/// �Փˏ���
	/// </summary>
	/// <param name="self">�Փ˂������g�̃R���C�_�[</param>
	/// <param name="other">�Փ˂�������̃R���C�_�[</param>
	/// <param name="hit">�Փ˂������̏��</param>
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

	// �X�V
	void Update() override;
	// �`��
	void Render() override;

protected:
	// �G�̏�����
	void InitEnemy(std::string path, const std::vector<AnimData>* pAnimData);

	// ��Ԑ؂�ւ�
	virtual void ChangeState(int state);

	// �A�j���[�V�����؂�ւ�
	void ChangeAnimation(int type, bool restart = false);


	int mState;				// ���
	int mStateStep;			// ��ԓ��̃X�e�b�v�Ǘ��p
	float mElapsedTime;		// �o�ߎ��Ԍv���p

	// �A�j���[�V�����f�[�^�̃e�[�u��
	const std::vector<AnimData>* mpAnimData;

	CVector mMoveSpeed;		// �O�㍶�E�̈ړ����x
	float mMoveSpeedY;		// �d�͂�W�����v�ɂ��㉺�̈ړ����x

	bool mIsGrounded;		// �ڒn���Ă��邩�ǂ���
	CVector mGroundNormal;	// �ڒn���Ă���n�ʂ̖@��

	CCollider* mpBodyCol;	// �{�̂̃R���C�_�[

	CGaugeUI3D* mpHpGauge;	// HP�Q�[�W
	CVector mGaugeOffsetPos;// �Q�[�W�̃I�t�Z�b�g���W
};

#endif
