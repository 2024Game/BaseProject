#pragma once

// �o�E���f�B���O�{�b�N�X���
class CBounds
{
public:
	// �R���X�g���N�^
	CBounds();
	// �f�X�g���N�^
	~CBounds();

	/// <summary>
	/// �ŏ��ʒu�ƍő�ʒu�Őݒ�
	/// </summary>
	/// <param name="min">�ŏ��ʒu</param>
	/// <param name="max">�ő�ʒu</param>
	void SetRange(const CVector& min, const CVector& max);	/// <summary>
	/// ���S�ʒu�ƃT�C�Y�Őݒ�
	/// </summary>
	/// <param name="center">���S�ʒu</param>
	/// <param name="size">�T�C�Y</param>
	void SetPos(const CVector& center, const CVector& size);

	// �ŏ��ʒu��ݒ�
	void Min(const CVector& min);
	// �ŏ��ʒu���擾
	const CVector& Min() const;
	// �ő�ʒu��ݒ�
	void Max(const CVector& max);
	// �ő�ʒu���擾
	const CVector& Max() const;

	// ���S�ʒu��ݒ�
	void Center(const CVector& center);
	// ���S�ʒu���擾
	CVector Center() const;
	// �T�C�Y��ݒ�
	void Size(const CVector& size);
	// �T�C�Y���擾
	CVector Size() const;

	/// <summary>
	/// 2�̃o�E���f�B���O�{�b�N�X���������邩�ǂ���
	/// </summary>
	/// <param name="b0">1�ڂ̃o�E���f�B���O�{�b�N�X</param>
	/// <param name="b1">2�ڂ̃o�E���f�B���O�{�b�N�X</param>
	/// <returns>true�Ȃ�΁A�������Ă���</returns>
	static bool Intersect(const CBounds& b0, const CBounds& b1);

	/// <summary>
	/// �����̃o�E���f�B���O�{�b�N�X���擾
	/// </summary>
	/// <param name="ls">�����̎n�_</param>
	/// <param name="le">�����̏I�_</param>
	/// <returns>�o�E���f�B���O�{�b�N�X</returns>
	static CBounds GetLineBounds(const CVector& ls, const CVector& le);
	/// <summary>
	/// ���̃o�E���f�B���O�{�b�N�X���擾
	/// </summary>
	/// <param name="pos">���̍��W</param>
	/// <param name="rad">���̔��a</param>
	/// <returns>�o�E���f�B���O�{�b�N�X</returns>
	static CBounds GetSphereBounds(const CVector& pos, float rad);
	/// <summary>
	/// �O�p�`�̃o�E���f�B���O�{�b�N�X���擾
	/// </summary>
	/// <param name="t0">�O�p�`��1�ڂ̒��_</param>
	/// <param name="t1">�O�p�`��2�ڂ̒��_</param>
	/// <param name="t2">�O�p�`��3�ڂ̒��_</param>
	/// <returns>�o�E���f�B���O�{�b�N�X</returns>
	static CBounds GetTriangleBounds(const CVector& t0, const CVector& t1, const CVector& t2);
	/// <summary>
	/// �J�v�Z���̃o�E���f�B���O�{�b�N�X���擾
	/// </summary>
	/// <param name="cs">�J�v�Z�����\����������̎n�_</param>
	/// <param name="ce">�J�v�Z�����\����������̏I�_</param>
	/// <param name="cr">�J�v�Z���̔��a</param>
	/// <returns>�o�E���f�B���O�{�b�N�X</returns>
	static CBounds GetCapsuleBounds(const CVector& cs, const CVector& ce, float cr);

private:
	CVector mMin;	// �ŏ��ʒu
	CVector mMax;	// �ő�ʒu
};
