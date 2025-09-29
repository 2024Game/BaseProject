#ifndef CMATRIX_H
#define CMATRIX_H

class CVector;

/*
�}�g���N�X�N���X
4�s4��̍s��f�[�^�������܂�
*/
class CMatrix {
private:
	//4�~4�̍s��f�[�^��ݒ�
	float mM[4][4];

public:
	// �R���X�g���N�^
	CMatrix();
	// �R���X�g���N�^
	CMatrix(float m00, float m01, float m02, float m03,
			float m10, float m11, float m12, float m13,
			float m20, float m21, float m22, float m23,
			float m30, float m31, float m32, float m33);

	//*���Z�q�̃I�[�o�[���[�h
	//CMatrix * float �̉��Z���ʂ�Ԃ�
	CMatrix operator*(const float& x);
	//+���Z�q�̃I�[�o�[���[�h
	//CMatrix1 + CMatrix2 �̉��Z���ʂ�Ԃ�
	CMatrix operator+(const CMatrix& m);
	//+=���Z�q�̃I�[�o�[���[�h
	//CMatrix1 += CMatrix2 �̉��Z���s��
	void operator+=(const CMatrix& m);
	// CMatrix == CMatrix �̌��ʂ�Ԃ�
	bool operator == (const CMatrix& m) const;
	// CMatrix != CMatrix �̌��ʂ�Ԃ�
	bool operator != (const CMatrix& m) const;
	//�N�I�[�^�j�I���ŉ�]�s���ݒ肷��
	CMatrix Quaternion(float x, float y, float z, float w);
	CVector Position() const;
	void Position(const CVector& pos);
	CVector VectorZ() const; //Z���x�N�g���̎擾
	CVector VectorX() const; //X���x�N�g���̎擾
	CVector VectorY() const; //Y���x�N�g���̎擾
	//�]�u�s��擾
	CMatrix Transpose() const;
	//�t�s��擾
	CMatrix Inverse() const;
	//�\���m�F�p
	//4�~4�̍s�����ʏo��
	void Print();
	//�P�ʍs��̍쐬
	CMatrix Identity();
	//�g��k���s��̍쐬
	//Scale(�{��X, �{��Y, �{��Z)
	CMatrix Scale(float sx, float sy, float sz);
	CMatrix Scale(const CVector& scale);
	//�s��l�̎擾
	//M(�s, ��)
	//mM[�s][��]���擾
	float M(int r, int c) const;
	//��]�s��iY���j�̍쐬
	//RotateY(�p�x)
	CMatrix RotateY(float degree);
	//��]�s��iZ���j�̍쐬
	//RotateZ(�p�x)
	CMatrix RotateZ(float degree);
	//��]�s��iX���j�̍쐬
	//RotateX(�p�x)
	CMatrix RotateX(float degree);
	//�ړ��s��̍쐬
	//Translate(�ړ���X, �ړ���Y, �ړ���Z)
	CMatrix Translate(float mx, float my, float mz);
	CMatrix Translate(const CVector& translation);
	//*���Z�q�̃I�[�o�[���[�h
	//CMatrix * CMatrix �̉��Z���ʂ�Ԃ�
	const CMatrix operator*(const CMatrix& m) const;
	//�s��l�̑��
	//M(�s��, ��, �l)
	void M(int row, int col, float value);
	float* M() const;
	//�v�f���̎擾
	int Size();

	/// <summary>
	/// ���ʃx�N�g���Ə�����x�N�g�������]�s����擾
	/// </summary>
	/// <param name="forward">���ʃx�N�g��</param>
	/// <param name="up">������x�N�g��</param>
	/// <returns>��]�s��</returns>
	static CMatrix LookRotation(const CVector& forward, const CVector& up);

	// ��s��
	// [0, 0, 0, 0]
	// [0, 0, 0, 0]
	// [0, 0, 0, 0]
	// [0, 0, 0, 0]
	const static CMatrix zero;

	// �P�ʍs��
	// [1, 0, 0, 0]
	// [0, 1, 0, 0]
	// [0, 0, 1, 0]
	// [0, 0, 0, 1]
	const static CMatrix identity;
};
#endif
