#include "CCamera.h"
#include "glut.h"
#include "Maths.h"
#include "CDebugCamera.h"

std::list<CCamera*> CCamera::spCameraList;
CCamera* CCamera::spMainCamera = nullptr;
CCamera* CCamera::spCurrentCamera = nullptr;

// �R���X�g���N�^
CCamera::CCamera(const CVector& eye, const CVector& center, bool isMainCamera)
	: CObjectBase(ETag::eCamera, ETaskPriority::eCamera)
	, mIsMainCamera(isMainCamera)
	, mFollowTargetTf(nullptr)
	, mFollowOffsetPos(CVector::zero)
	, mFovy(CAMERA_FOVY)
	, mZNear(CAMERA_ZNEAR)
	, mZFar(CAMERA_ZFAR)
	, mHitColRatio(1.0f)
{
	mTargetEye = eye;
	LookAt(eye, center, CVector::up);
	Reshape(WINDOW_WIDTH, WINDOW_HEIGHT);

	// ���C���J�������ݒ肳��Ă��Ȃ���΁A
	if (mIsMainCamera && spMainCamera == nullptr)
	{
		// ���C���J�����ɐݒ�
		spMainCamera = this;
		// ���C���J���������݂̃J�����ɐݒ�
		spCurrentCamera = spMainCamera;
	}

	spCameraList.push_back(this);
}

// �f�X�g���N�^
CCamera::~CCamera()
{
	// �J�����폜����
	DeleteCamera();
}

// ���C���J�������擾
CCamera* CCamera::MainCamera()
{
	return spMainCamera;
}

// ���݂̃J�������擾
CCamera* CCamera::CurrentCamera()
{
#if _DEBUG
	// �f�o�b�O�J�������L���ł���΁A
	if (CDebugCamera::IsOn())
	{
		// �f�o�b�O�J������Ԃ�
		return CDebugCamera::DebugCamera();
	}
	// �f�o�b�O�J�����������ł���΁A
	else
#endif
	{
		// ���݂̃J������Ԃ�
		return spCurrentCamera;
	}
}

// �J�������폜
void CCamera::DeleteCamera()
{
	// �J�������X�g���玩�����g����菜��
	spCameraList.remove(this);

	// �폜���ꂽ�̂����C���J�����ł���΁A
	// �o�^���Ă��郁�C���J����������
	if (spMainCamera == this)
	{
		spMainCamera = nullptr;
	}

	// ���݂̃J�������폜���ꂽ�ꍇ��
	if (spCurrentCamera == this)
	{
		// ���݂̃J�����Ƃ͕ʂɃ��C���J���������݂���΁A
		// ���C���J���������݂̃J�����ɕύX
		if (spMainCamera != nullptr)
		{
			spCurrentCamera = spMainCamera;
		}
		// ���C���J���������݂��Ȃ��ꍇ
		else
		{
			spCurrentCamera = nullptr;
			// �J�������X�g�̐擪���璲�ׂ�
			// ���C���J���������݂���΁A
			// ���̃J���������݂̃J�����ɐݒ肷��
			for (CCamera* cam : spCameraList)
			{
				if (cam->mIsMainCamera)
				{
					spMainCamera = cam;
					spCurrentCamera = spMainCamera;
					break;
				}
			}
		}
	}
}

// �J�����̏�Ԃ��R�s�[
void CCamera::CopyCamera(CCamera* copy)
{
	if (copy == nullptr) return;

	Position(copy->Position());
	Rotation(copy->Rotation());
	mViewMatrix = copy->mViewMatrix;
	mProjectionMatrix = copy->mProjectionMatrix;
	mViewportMatrix = copy->mViewportMatrix;
	mFovy = copy->mFovy;
	mZNear = copy->mZNear;
	mZFar = copy->mZFar;
	mTargetEye = copy->mTargetEye;
	mEye = copy->mEye;
	mAt = copy->mAt;
	mUp = copy->mUp;
	mEyeVec = copy->mEyeVec;
}

// �Ǐ]����^�[�Q�b�g��ݒ�
void CCamera::SetFollowTargetTf(CTransform* target)
{
	mFollowTargetTf = target;
	if (mFollowTargetTf != nullptr)
	{
		mFollowOffsetPos = mTargetEye - mFollowTargetTf->Position();
	}
}

// �Ǐ]���̃I�t�Z�b�g���W��ݒ�
void CCamera::SetFollowTargetOffset(const CVector& offset)
{
	if (mFollowTargetTf != nullptr)
	{
		mFollowOffsetPos = offset;
	}
}

// ��������ʒu��ݒ�i�����_�̂݁j
void CCamera::LookAt(const CVector& pos)
{
	LookAt(mEye, pos, mUp);
}

// ��������ʒu��ݒ�i�����_ + ��x�N�g���j
void CCamera::LookAt(const CVector& pos, const CVector& up)
{
	LookAt(mEye, pos, up);
}

// ��������ʒu��ݒ�i���_ + �����_ + ��x�N�g���j
void CCamera::LookAt(const CVector& eye, const CVector& at, const CVector& up, bool updateTargetEye)
{
	if (updateTargetEye) mTargetEye = eye;
	mEye = eye; mAt = at; mUp = up;
	mEyeVec = mAt - mTargetEye;
	mViewMatrix.Identity();
	CVector f = (mEye - mAt).Normalized();
	CVector r = CVector::Cross(mUp, f).Normalized();
	CVector u = CVector::Cross(f, r).Normalized();
	mViewMatrix.M(0, 0, r.X()); mViewMatrix.M(1, 0, r.Y()); mViewMatrix.M(2, 0, r.Z());
	mViewMatrix.M(0, 1, u.X()); mViewMatrix.M(1, 1, u.Y()); mViewMatrix.M(2, 1, u.Z());
	mViewMatrix.M(0, 2, f.X()); mViewMatrix.M(1, 2, f.Y()); mViewMatrix.M(2, 2, f.Z());
	mViewMatrix.M(3, 0, -CVector::Dot(mEye, r));
	mViewMatrix.M(3, 1, -CVector::Dot(mEye, u));
	mViewMatrix.M(3, 2, -CVector::Dot(mEye, f));

	Position(mEye);
	Rotation(CQuaternion::LookRotation(f));

	if (mFollowTargetTf != nullptr)
	{
		mFollowOffsetPos = mTargetEye - mFollowTargetTf->Position();
	}
}

// ��ʃT�C�Y�ύX����
void CCamera::Reshape(int width, int height)
{
	//��ʂ̕`��G���A�̎w��
	Viewport(0.0f, 0.0f, width, height);

	Perspective
	(
		CAMERA_FOVY,
		(float)width / height,
		CAMERA_ZNEAR,
		CAMERA_ZFAR
	);
}

// ��ʂ̕`��G���A���w��
void CCamera::Viewport(float x, float y, float w, float h)
{
	glViewport(x, y, w, h);
	mViewportMatrix.Identity();
	mViewportMatrix.M(0, 0, w * 0.5f);
	mViewportMatrix.M(3, 0, x + w * 0.5f);
	mViewportMatrix.M(1, 1, -h * 0.5f);
	mViewportMatrix.M(3, 1, y + h * 0.5f);
}

// �������e�s���ݒ�
void CCamera::Perspective(float fovy, float aspect, float zNear, float zFar)
{
	//�e�l��ݒ�
	mFovy = fovy;
	mZNear = zNear;
	mZFar = zFar;

	//�v���W�F�N�V�����s��֐؂�ւ�
	glMatrixMode(GL_PROJECTION);
	//�s������������āA�V�����v���W�F�N�V�����s���ݒ�
	glLoadIdentity();
	gluPerspective(mFovy, aspect, mZNear, mZFar);
	//�v���W�F�N�V�����s����L�����Ă���
	glGetFloatv(GL_PROJECTION_MATRIX, mProjectionMatrix.M());

	//���f���r���[�s��֖߂�
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

// �X�N���[�����W���烏�[���h���W�֕ϊ�
CVector CCamera::ScreenToWorldPos(const CVector& screenPos)
{
	float z = screenPos.Z();
	float d = ((z * (mZFar + mZNear) - 2.0f * mZFar * mZNear) / (mZFar - mZNear) / z);
	CVector4 v =
		mViewportMatrix.Inverse() *
		mProjectionMatrix.Inverse() *
		mViewMatrix.Inverse() *
		CVector4(screenPos.X(), screenPos.Y(), d, 1.0f);
	return CVector(v.X() / v.W(), v.Y() / v.W(), v.Z() / v.W());
}

// ���[���h���W����X�N���[�����W�֕ϊ�
CVector CCamera::WorldToScreenPos(const CVector& worldPos)
{
	CVector4 v =
		mViewMatrix *
		mProjectionMatrix *
		mViewportMatrix *
		CVector4(worldPos, 1.0f);
	return CVector(v.X() / v.W(), v.Y() / v.W(), v.Z() / fabsf(v.W()));
}

// ���̃J���������݂̃J�����ɐݒ肷�邩�ǂ���
void CCamera::SetCurrent(bool isCurrent)
{
	// ���̃J���������݂̃J�����ɕύX
	if (isCurrent)
	{
		spCurrentCamera = this;
	}
	// �������̓��C���J���������݂̃J�����ɂ���
	else
	{
		spCurrentCamera = spMainCamera;
	}
}

// �r���[�s����擾
const CMatrix& CCamera::GetViewMatrix() const
{
	return mViewMatrix;
}

// �v���W�F�N�V�����s����擾
const CMatrix& CCamera::GetProjectionMatrix() const
{
	return mProjectionMatrix;
}

// �r���[�|�[�g�s����擾
const CMatrix& CCamera::GetViewportMatrix() const
{
	return mViewportMatrix;
}

// �Փ˔�����s���R���C�_�[�����X�g�ɒǉ�
void CCamera::AddCollider(CCollider* col)
{
	mColliders.push_back(col);
}

// �Փ˔�����s���R���C�_�[�����X�g�����菜��
void CCamera::RemoveCollider(CCollider* col)
{
	mColliders.remove(col);
}

// �R���C�_�[�Ƃ̏Փ˔��莞�ɉ����߂������̊���
void CCamera::SetHitColliderRatio(float ratio)
{
	mHitColRatio = ratio;
}

// �ݒ肳��Ă���R���C�_�[�Ƃ̏Փˌ��ʂ𔽉f����
void CCamera::ApplyCollision()
{
	// �����_���王�_�܂Ń��C���΂��āA
	// �Ԃɏ�Q��������΁A���_����Q������O�ɃY����
	CHitInfo hit;
	CVector rayStart = mAt;
	CVector rayEnd = mEye;
	float nearDist = 0.0f;
	bool isHit = false;
	// �ݒ肳��Ă���R���C�_�[�����Ԃɒ��ׂ�
	for (CCollider* c : mColliders)
	{
		// ���C�ƃR���C�_�[�̏Փ˔�����s��
		if (CCollider::CollisionRay(c, rayStart, rayEnd, &hit))
		{
			// ��_���s���l�łȂ���΁A
			if (hit.cross.LengthSqr() != 0.0f)
			{
				// �Փˈʒu�܂ł̋����ň�ԋ߂����������߂�
				if (!isHit) nearDist = hit.dist;
				else nearDist = std::min(hit.dist, nearDist);
				isHit = true;
			}
		}
	}

	// ���C���Փ˂��Ă�����A
	// ���_���Փ˒n�_����O�ɉ����߂�
	if (isHit)
	{
		mEye = rayStart + (rayEnd - rayStart).Normalized() * nearDist * mHitColRatio;
	}
}

// ��X�V
void CCamera::LateUpdate()
{
	// �Ǐ]����^�[�Q�b�g���ݒ肳��Ă���΁A
	// �J�����̈ʒu��Ǐ]�^�[�Q�b�g�̈ʒu�ɍ��킹��
	if (mFollowTargetTf != nullptr)
	{
		CVector eye = mFollowTargetTf->Position() + mFollowOffsetPos;
		Position(eye);
		CVector diff = eye - mTargetEye;
		mTargetEye = eye;
		mEye = mTargetEye;
		mAt = mEye - VectorZ().Normalized() * mEyeVec.Length();
	}

	// �ݒ肳��Ă���R���C�_�[�ƏՓ˂���ꍇ�́A
	// �J�����̈ʒu�������o��
	ApplyCollision();

	// ���_�A�����_�A��x�N�g������e�s����X�V
	LookAt(mEye, mAt, mUp, false);
}

// �폜
void CCamera::Kill()
{
	CTask::Kill();
	DeleteCamera();
}

// �J�������f
void CCamera::Apply()
{
	glMultMatrixf(mViewMatrix.M());
}

// 2D�p�̃J�����s��𔽉f
void CCamera::Start2DCamera()
{
	// �v���W�F�N�V�����s��֐؂�ւ��āA
	// �s��̑ޔ��ƍs���������
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	// 2D�̕\���G���A��ݒ�
	gluOrtho2D(0.0, WINDOW_WIDTH, WINDOW_HEIGHT, 0.0);

	// ���f���r���[�s��֖߂��āA
	// �s��̑ޔ��ƍs���������
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	// 2D�`��ɕK�v�Ȑݒ���s��
	glDisable(GL_DEPTH_TEST);	// �f�v�X�e�X�g���I�t
	glDisable(GL_LIGHTING);		// ���C�e�B���O���I�t
	glDisable(GL_CULL_FACE);	// �J�����O����
}

// 3D�p�̃J�����s��֖߂�
void CCamera::End2DCamera()
{
	// 3D�`��ɕK�v�ȏ������I���ɂ���
	glEnable(GL_DEPTH_TEST);	// �f�v�X�e�X�g���I��
	glEnable(GL_LIGHTING);		// ���C�e�B���O���I��
	glEnable(GL_CULL_FACE);

	// �v���W�F�N�V�����s��֐؂�ւ��āA
	// 2D�`��J�n���ɑޔ������s��֖߂�
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	// ���f���r���[�s��֖߂��āA
	// ��������ޔ������s��֖߂�
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}
