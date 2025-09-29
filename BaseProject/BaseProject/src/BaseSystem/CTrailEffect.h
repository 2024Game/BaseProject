#pragma once
#include "CLineEffect.h"

// �O�ՃG�t�F�N�g
class CTrailEffect : public CLineEffect
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="tag">�I�u�W�F�N�g�^�O</param>
	/// <param name="owner">������</param>
	/// <param name="mtx">�s��</param>
	/// <param name="offsetPos">���݈ʒu���Y�����I�t�Z�b�g�l</param>
	/// <param name="updateInterval">�X�V�Ԋu�i���ԁj</param>
	/// <param name="updateDist">�X�V�Ԋu�i�����j</param>
	/// <param name="startWidth">�J�n���̋O�Ղ̕�</param>
	/// <param name="endWidth">�I�����̋O�Ղ̕�</param>
	/// <param name="showTime">�O�Ղ̕\������</param>
	CTrailEffect(ETag tag,
		CObjectBase* owner, CMatrix* mtx, CVector offsetPos,
		float updateInterval, float updateDist,
		float startWidth, float endWidth,
		float showTime);
	// �f�X�g���N�^
	~CTrailEffect();

	// �������ݒ�
	void SetOwner(CObjectBase* obj);
	// �ǐՂ���s���ݒ�
	void SetFollowMatrix(CMatrix* mtx);
	//// �ݒ肳�ꂽ������܂��͍s��̈ʒu����̃I�t�Z�b�g�l��ݒ�
	//void SetOffsetPos(const CVector& pos);

	// �X�V
	void Update() override;

private:
	// ���ݒn���擾
	CVector GetCurrentPos() const;
	// ���̍Ō�̃|�C���g�̈ʒu���X�V
	void UpdateLastPoint();

	CObjectBase* mpOwner;	// ������̃|�C���^�[
	CMatrix* mpMatrix;		// �ǐՂ���s��̃|�C���^�[
	CVector mOffsetPos;		// ���ݒn����Y�����I�t�Z�b�g�l
	float mUpdateInterval;	// �X�V�Ԋu�i���ԁj
	float mUpdateDistance;	// �X�V�Ԋu�i�����j
	float mStartWidth;		// �J�n���̐��̕�
	float mEndWidth;		// �I�����̐��̕�
	float mShowTime;		// �\������

	CVector mLastPos;		// �O��̈ʒu
	float mElapsedTime;		// �o�ߎ��Ԍv���p
};