#ifndef CXCHARACTER_H
#define CXCHARACTER_H

#include "CModelX.h"
#include "CMaterial.h"
#include "CCharaBase.h"

class CXCharacter : public CCharaBase
{
public:
	//�R���X�g���N�^
	CXCharacter(ETag tag, ETaskPriority prio, int sortOrder = 0,
		ETaskPauseType pause = ETaskPauseType::eGame);
	//�f�X�g���N�^
	virtual ~CXCharacter();

	//����������
	void Init(CModelX* model);
	//�A�j���[�V�����̕ύX
	void ChangeAnimation(int index, bool loop, float framesize,
		bool restart = false);
	//�X�V����
	void Update(const CMatrix& m);
	void Update();
	//�`�揈��
	void Render();

	//�A�j���[�V�����̍Đ��I������
	//true:�I�� false:�Đ���
	bool IsAnimationFinished();

	int AnimationIndex();	//�A�j���[�V�����̔ԍ��̎擾

	// �Đ����̃A�j���[�V�����t���[�����擾
	float GetAnimationFrame() const;
	// �Đ����̃A�j���[�V�����̐i�s�x���擾
	float GetAnimationFrameRatio() const;

	// �w�肵���{�[���̍s����擾
	const CMatrix* GetFrameMtx(std::string name) const;

	// �A�j���[�V�����̍Đ����x��ݒ�
	void SetAnimationSpeed(float speed);
	// �A�j���[�V�����̍Đ����x���擾
	float GetAnimationSpeed() const;

protected:
	CModelX* mpModel;				//���f���f�[�^
	CMatrix* mpCombinedMatrix;		//�����s��ޔ�

	bool mAnimationLoopFlg;			//true:�A�j���[�V�������J��Ԃ�

	int mAnimationIndex;			//�A�j���[�V�����ԍ�

	float mAnimationFrame;			//�A�j���[�V�����̍Đ��t���[��
	float mAnimationFrameSize;		//�A�j���[�V�����̍Đ��t���[����
	float mAnimationSpeed;			//�A�j���[�V�����̍Đ����x

};
#endif