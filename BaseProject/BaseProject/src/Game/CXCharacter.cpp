#include "CXCharacter.h"
#include "Maths.h"

//�R���X�g���N�^
CXCharacter::CXCharacter(ETag tag, ETaskPriority prio, int sortOrder, ETaskPauseType pause)
	: CCharaBase(tag, prio, sortOrder, pause)
	, mpModel(nullptr)
	, mpCombinedMatrix(nullptr)
	, mAnimationLoopFlg(false)
	, mAnimationIndex(0)
	, mAnimationFrame(0.0f)
	, mAnimationFrameSize(0.0f)
	, mAnimationSpeed(1.0f)
{
}

//�f�X�g���N�^
CXCharacter::~CXCharacter()
{
	SAFE_DELETE_ARRAY(mpCombinedMatrix)
}

/*
Init
����������
*/
void CXCharacter::Init(CModelX* model)
{
	mpModel = model;
	//�����s��ޔ��G���A�̊m��
	mpCombinedMatrix = new CMatrix[model->Frames().size()];
	//�ŏ��̃A�j���[�V�����ɂ���
	mAnimationIndex = 0;
	//�J��Ԃ��Đ�����
	mAnimationLoopFlg = true;
	//1�A�j���[�V�����ڂ̍ő�t���[����
	mAnimationFrameSize = model->AnimationSet()[0]->MaxTime();
	//�A�j���[�V�����̃t���[�����ő�ɂ���
	mAnimationFrame = 0.0f;
	mpModel->AnimationSet()[mAnimationIndex]->Time(mAnimationFrame);
	//�A�j���[�V�����̏d�݂�1.0(100%)�ɂ���
	mpModel->AnimationSet()[mAnimationIndex]->Weight(1.0f);
}


/*
ChangeAnimation
�A�j���[�V������؂�ւ���
index:�A�j���[�V�����̔ԍ�
loop:true:�J��Ԃ�
framesize:�Ō�܂ōĐ�����̂Ɏg�p�����t���[����
*/
void CXCharacter::ChangeAnimation(int index, bool loop,
	float framesize, bool restart)
{
	//�ŏ�����J�n���Ȃ��ꍇ���A�����A�j���[�V�����̏ꍇ�͐؂�ւ��Ȃ�
	if (!restart && mAnimationIndex == index) return;
	//���̃A�j���[�V�����̏d�݂�0.0(0%)�ɂ���
	mpModel->AnimationSet()[mAnimationIndex]->Weight(0.0f);
	//�ԍ��A�J��Ԃ��A�t���[�������w��
	mAnimationIndex = index % mpModel->AnimationSet().size();
	mAnimationLoopFlg = loop;
	mAnimationFrameSize = framesize;
	//�A�j���[�V�����̃t���[�����ŏ��ɂ���
	mAnimationFrame = 0.0f;
	mpModel->AnimationSet()[mAnimationIndex]->Time(mAnimationFrame);
	//�A�j���[�V�����̏d�݂�1.0(100%)�ɂ���
	mpModel->AnimationSet()[mAnimationIndex]->Weight(1.0f);
}

/*
Update
�X�V����
matrix:�ړ��A��]�A�g��k���̍s��
*/
void CXCharacter::Update(const CMatrix& matrix)
{
	if (mpModel == nullptr) return;

	auto& animSet = mpModel->AnimationSet();
	CAnimationSet* currAnim = animSet[mAnimationIndex];

	for (size_t i = 0; i < animSet.size(); i++)
	{
		animSet[i]->mWeight = 0.0f;
		currAnim->mWeight = 1.0f;
	}
	//�Ō�܂ōĐ�����
	if (0.0f <= mAnimationFrame && mAnimationFrame <= mAnimationFrameSize)
	{
		//�A�j���[�V�����̎��Ԃ�ݒ�
		currAnim->Time(mAnimationFrame);
		//�t���[����i�߂�
		mAnimationFrame += mAnimationSpeed * 60.0f * Times::DeltaTime();
	}
	else
	{
		//�A�j���[�V�������J��Ԃ��ꍇ
		if (mAnimationLoopFlg)
		{
			//�A�j���[�V�����̃t���[���������[�v������
			float maxTime = currAnim->MaxTime();
			if (maxTime > 0.0f)
			{
				mAnimationFrame = fmodf(mAnimationFrame + maxTime, maxTime);
			}
			else
			{
				mAnimationFrame = 0.0f;
			}
			currAnim->Time(mAnimationFrame);
		}
		//�A�j���[�V�������J��Ԃ��Ȃ��ꍇ
		else
		{
			//�ŏI�t���[���ɐݒ�
			currAnim->TimeProgress(mAnimationFrame >= 0.0f ? 1.0f : 0.0f);
		}
	}
	//�t���[���̕ϊ��s����A�j���[�V�����ōX�V����
	mpModel->AnimateFrame();
	//�t���[���̍����s����v�Z����
	mpModel->Frames()[0]->AnimateCombined(&matrix);
	//�����s��̑ޔ�
	for (size_t i = 0; i < mpModel->Frames().size(); i++)
	{
		mpCombinedMatrix[i] =
			mpModel->Frames()[i]->CombinedMatrix();
	}
}

//�X�V����
void CXCharacter::Update()
{
	//�A�j���[�V�������X�V����
	Update(Matrix());
}

/*
Render
�`�悷��
*/
void CXCharacter::Render()
{
	if (mpModel == nullptr) return;
	mpModel->SetColor(mColor);
	mpModel->RenderShader(mpCombinedMatrix);
}

bool CXCharacter::IsAnimationFinished()
{
	return mAnimationFrame >= mAnimationFrameSize;
}

int CXCharacter::AnimationIndex()
{
	return mAnimationIndex;
}

// �Đ����̃A�j���[�V�����t���[�����擾
float CXCharacter::GetAnimationFrame() const
{
	return mAnimationFrame;
}

// �Đ����̃A�j���[�V�����̐i�s�x���擾
float CXCharacter::GetAnimationFrameRatio() const
{
	if (mAnimationFrameSize == 0.0f) return 0.0f;
	return Math::Clamp01(mAnimationFrame / mAnimationFrameSize);
}

// �w�肵���{�[���̍s����擾
const CMatrix* CXCharacter::GetFrameMtx(std::string name) const
{
	// ���f���f�[�^���ݒ肳��Ă��Ȃ�
	if (mpModel == nullptr) return nullptr;

	// �t���[������
	CModelXFrame* frame = mpModel->FinedFrame(name.c_str());

	//�w�肳�ꂽ�t���[�������݂��Ȃ�����
	if (frame == nullptr) return nullptr;

	// �t���[���̍s��c�Ԃ�
	return &frame->CombinedMatrix();
}

// �A�j���[�V�����̍Đ����x��ݒ�
void CXCharacter::SetAnimationSpeed(float speed)
{
	mAnimationSpeed = speed;
}

// �A�j���[�V�����̍Đ����x���擾
float CXCharacter::GetAnimationSpeed() const
{
	return mAnimationSpeed;
}
