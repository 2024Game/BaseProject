#include "CLineEffect.h"
#include <glut.h>
#include "Maths.h"

// �R���X�g���N�^
CLineEffect::CLineEffect(ETag tag)
	: CEffect(tag)
	, mpOwner(nullptr)
	, mpTexture(nullptr)
	, mpAnimData(nullptr)
	, mAnimNo(0)
	, mElapsedTime(0.0f)
	, mBaseUV(0.0f, 0.0f, 1.0f, 1.0f)
	, mUV(0.0f, 0.0f, 1.0f, 1.0f)
{
}

// �f�X�g���N�^
CLineEffect::~CLineEffect()
{
	// �����傪���݂���ꍇ�́A
	if (mpOwner != nullptr)
	{
		// ������Ɏ������폜���ꂽ���Ƃ�`����
		mpOwner->DeleteObject(this);
	}
}

// �������ݒ�
void CLineEffect::SetOwner(CObjectBase* owner)
{
	mpOwner = owner;
}

// �e�N�X�`���ݒ�
void CLineEffect::SetTexture(std::string texName)
{
	mpTexture = CResourceManager::Get<CTexture>(texName);
}

// UV�ݒ�
void CLineEffect::SetBaseUV(const CRect& uv)
{
	mBaseUV = uv;
}

// �A�j���[�V�������Đ��ł��邩�ǂ���
bool CLineEffect::IsEnableAnim() const
{
	if (mpAnimData == nullptr) return false;
	if (mpAnimData->frames.size() == 0) return false;
	return true;
}

// �A�j���[�V�����f�[�^��ݒ�
void CLineEffect::SetAnimData(TexAnimData* animData)
{
	mpAnimData = animData;
	mAnimNo = 0;
	mElapsedTime = 0.0f;

	// �A�j���[�V�������L���ł���΁A
	// ��ԍŏ��̉摜��UV��ݒ�
	if (IsEnableAnim())
	{
		mUV = mpTexture->CalcUV
		(
			mpAnimData->row,
			mpAnimData->col,
			0
		);
	}
}

// �A�j���[�V�������I��������ǂ���
bool CLineEffect::IsEndAnim() const
{
	if (mpAnimData == nullptr) return true;
	int size = mpAnimData->frames.size();
	return mAnimNo == size;
}

// ���̃|�C���g��ǉ�
int CLineEffect::AddPoint(const CVector& pos, float startWidth, float endWidth)
{
	CLinePoint* point = new CLinePoint(pos, startWidth, endWidth);
	mPoints.push_back(point);
	return mPoints.size() - 1;
}

// ���̃|�C���g���폜
void CLineEffect::RemovePoint(int index)
{
	if (!(0 <= index && index < mPoints.size())) return;
	CLinePoint* point = mPoints[index];
	mPoints[index] = nullptr;
	mPoints.erase(mPoints.begin() + index);
	delete point;
}

// ���̃|�C���g�����擾
int CLineEffect::GetPointCount() const
{
	return mPoints.size();
}

// ���̃|�C���g�̍��W��ݒ�
void CLineEffect::SetPos(int index, const CVector& pos)
{
	if (!(0 <= index && index < mPoints.size())) return;
	mPoints[index]->pos = pos;
}

// ���̃|�C���g�̍��W���擾
CVector CLineEffect::GetPos(int index) const
{
	if (!(0 <= index && index < mPoints.size())) return CVector();
	return mPoints[index]->pos;
}

// ���W���v�Z
CVector CLineEffect::CalcPos(const CVector& pos) const
{
	if (mpOwner == nullptr) return pos;
	return mpOwner->Position() + mpOwner->Rotation() * pos;
}

//// ���̃|�C���g�̕���ݒ�
//void CLineEffect::SetWidth(int index, float width)
//{
//	if (!(0 <= index && index < mPoints.size())) return;
//	mPoints[index]->width = width;
//}
//
//// ���̃|�C���g�̕����擾
//float CLineEffect::GetWidth(int index) const
//{
//	if (!(0 <= index && index < mPoints.size())) return 0.0f;
//	return mPoints[index]->width;
//}

// �X�V
void CLineEffect::Update()
{
	if (IsEnableAnim())
	{
		int size = mpAnimData->frames.size();
		if (mAnimNo < size)
		{
			TexAnimFrameData data = mpAnimData->frames[mAnimNo];
			// ���݂̉摜�̕\�����Ԃ𒴂�����
			if (mElapsedTime >= data.time)
			{
				// ���̉摜�֐؂�ւ�
				mElapsedTime -= data.time;

				mAnimNo++;
				if (mAnimNo >= size)
				{
					if (mpAnimData->loop)
					{
						mAnimNo -= size;
					}
					else
					{
						mAnimNo = size;
					}
				}
				mUV = mpTexture->CalcUV
				(
					mpAnimData->row,
					mpAnimData->col,
					mAnimNo < size ? mAnimNo : mAnimNo - 1
				);
			}
			// �o�ߎ��Ԃ��v��
			mElapsedTime += Times::DeltaTime();
		}
	}
}

// �`��
void CLineEffect::Render()
{
	int size = mPoints.size();

	// �|�C���g��2�ȏ�Ȃ��Ȃ�΁A�`�悵�Ȃ�
	if (size < 2) return;

	glDisable(GL_LIGHTING);
	glDisable(GL_CULL_FACE);
	glDepthMask(GL_FALSE);
	glColor3f(mColor.R(), mColor.G(), mColor.B());

	// �e�N�X�`�����ݒ肳��Ă�����A�e�N�X�`���𔽉f
	if (mpTexture != nullptr)
	{
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBlendFunc(GL_ONE, GL_ONE);
		glBindTexture(GL_TEXTURE_2D, mpTexture->Id());
	}

	for (int i = 0; i < size - 1; i++)
	{
		CLinePoint* start = mPoints[i];
		CLinePoint* end = mPoints[i + 1];
		if (start == nullptr || end == nullptr) continue;
		const CVector& startPos = start->pos;
		const CVector& endPos = end->pos;
		float startWidth = Math::Lerp(start->startWidth, start->endWidth, 1.0f - start->alpha);
		float endWidth = Math::Lerp(end->startWidth, end->endWidth, 1.0f - end->alpha);

		CVector v = endPos - startPos;
		CVector crossUp = CVector::up;
		CVector side = CVector::Cross(v.Normalized(), crossUp);
		if (abs(side.LengthSqr()) <= EPSILON)
		{
			crossUp = CVector::forward;
			side = CVector::Cross(v.Normalized(), crossUp);
		}
		CVector up = CVector::Cross(v.Normalized(), side);

		bool prev = i > 0;
		CLinePoint* prevS = prev ? mPoints[i - 1] : nullptr;
		CLinePoint* prevE = prev ? mPoints[i] : nullptr;
		if (prev && prevS == nullptr) prev = false;
		CVector prevV = prev ? prevE->pos - prevS->pos : CVector::zero;
		CVector prevSide = prev ? CVector::Cross(prevV.Normalized(), crossUp) : CVector::zero;
		CVector prevUp = prev ? CVector::Cross(prevV.Normalized(), prevSide) : CVector::zero;
		bool next = i + 1 < size - 1;
		CLinePoint* nextS = next ? mPoints[i + 1] : nullptr;
		CLinePoint* nextE = next ? mPoints[i + 2] : nullptr;
		CVector nextV = next ? nextE->pos - nextS->pos : CVector::zero;
		CVector nextSide = next ? CVector::Cross(nextV.Normalized(), crossUp) : CVector::zero;
		CVector nextUp = next ? CVector::Cross(nextV.Normalized(), nextSide) : CVector::zero;

		// �c��`��
		{
			CVector vtx0 = startPos + up * startWidth * 0.5f;
			CVector vtx1 = startPos - up * startWidth * 0.5f;
			if (prev)
			{
				float width = Math::Lerp(prevE->startWidth, prevE->endWidth, 1.0f - prevE->alpha);
				CVector prevVtx0 = prevE->pos + prevUp * width * 0.5f;
				CVector prevVtx1 = prevE->pos - prevUp * width * 0.5f;
				vtx0 = (vtx0 + prevVtx0) * 0.5f;
				vtx1 = (vtx1 + prevVtx1) * 0.5f;
			}

			CVector vtx2 = endPos + up * endWidth * 0.5f;
			CVector vtx3 = endPos - up * endWidth * 0.5f;
			if (next)
			{
				float width = Math::Lerp(nextS->startWidth, nextS->endWidth, 1.0f - nextS->alpha);
				CVector nextVtx0 = nextS->pos + nextUp * width * 0.5f;
				CVector nextVtx1 = nextS->pos - nextUp * width * 0.5f;
				vtx2 = (vtx2 + nextVtx0) * 0.5f;
				vtx3 = (vtx3 + nextVtx1) * 0.5f;
			}

			vtx0 = CalcPos(vtx0);
			vtx1 = CalcPos(vtx1);
			vtx2 = CalcPos(vtx2);
			vtx3 = CalcPos(vtx3);

			glBegin(GL_TRIANGLE_STRIP);
			glTexCoord2f(mUV.Right(), mUV.Top());
			glVertex3fv((const float*)&vtx0);
			glTexCoord2f(mUV.Left(), mUV.Top());
			glVertex3fv((const float*)&vtx1);
			glTexCoord2f(mUV.Right(), mUV.Bottom());
			glVertex3fv((const float*)&vtx2);
			glTexCoord2f(mUV.Left(), mUV.Bottom());
			glVertex3fv((const float*)&vtx3);
			glEnd();
		}
		// ����`��
		{
			CVector vtx0 = startPos + side * startWidth * 0.5f;
			CVector vtx1 = startPos - side * startWidth * 0.5f;
			if (prev)
			{
				float width = Math::Lerp(prevE->startWidth, prevE->endWidth, 1.0f - prevE->alpha);
				CVector prevVtx0 = prevE->pos + prevSide * width * 0.5f;
				CVector prevVtx1 = prevE->pos - prevSide * width * 0.5f;
				vtx0 = (vtx0 + prevVtx0) * 0.5f;
				vtx1 = (vtx1 + prevVtx1) * 0.5f;
			}

			CVector vtx2 = endPos + side * endWidth * 0.5f;
			CVector vtx3 = endPos - side * endWidth * 0.5f;
			if (next)
			{
				float width = Math::Lerp(nextS->startWidth, nextS->endWidth, 1.0f - nextS->alpha);
				CVector nextVtx0 = nextS->pos + nextSide * width * 0.5f;
				CVector nextVtx1 = nextS->pos - nextSide * width * 0.5f;
				vtx2 = (vtx2 + nextVtx0) * 0.5f;
				vtx3 = (vtx3 + nextVtx1) * 0.5f;
			}

			vtx0 = CalcPos(vtx0);
			vtx1 = CalcPos(vtx1);
			vtx2 = CalcPos(vtx2);
			vtx3 = CalcPos(vtx3);

			glBegin(GL_TRIANGLE_STRIP);
			glTexCoord2f(mUV.Right(), mUV.Top());
			glVertex3fv((const float*)&vtx0);
			glTexCoord2f(mUV.Left(), mUV.Top());
			glVertex3fv((const float*)&vtx1);
			glTexCoord2f(mUV.Right(), mUV.Bottom());
			glVertex3fv((const float*)&vtx2);
			glTexCoord2f(mUV.Left(), mUV.Bottom());
			glVertex3fv((const float*)&vtx3);
			glEnd();
		}
	}

	// �e�N�X�`�����ݒ肳��Ă�����A�e�N�X�`���𖳌���Ԃɖ߂�
	if (mpTexture != nullptr)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_BLEND);
		glDisable(GL_TEXTURE_2D);
	}

	glDepthMask(GL_TRUE);
	glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHTING);
}