#pragma once
class CColor;
class CVector;

namespace System
{
	/// <summary>
	/// �Q�[�����I��
	/// </summary>
	void ExitGame();
	/// <summary>
	/// �Q�[�����I�����邩�ǂ�����Ԃ�
	/// </summary>
	/// <returns>true�Ȃ�A�Q�[�����I��</returns>
	bool IsExitGame();

	/// <summary>
	/// ��ʂ̃N���A����\��ݒ�irgba�j
	/// </summary>
	/// <param name="r">�ݒ肷��F�̐ԗv�f</param>
	/// <param name="g">�ݒ肷��F�̗Ηv�f</param>
	/// <param name="b">�ݒ肷��F�̐v�f</param>
	/// <param name="a">�ݒ肷��F�̃A���t�@�l</param>
	void SetClearColor(float r, float g, float b, float a);
	/// <summary>
	/// ��ʂ̃N���A�J���[��ݒ�iCColor�j
	/// </summary>
	/// <param name="color">�ݒ肷��F</param>
	void SetClearColor(const CColor& color);

	/// <summary>
	/// ���[�V�����u���[�̗L�������ݒ�
	/// </summary>
	/// <param name="enable">true�Ȃ�΁A�L��</param>
	void SetEnableMotionBlur(bool enable);
	/// <summary>
	/// ���[�V�����u���[���L�����ǂ���
	/// </summary>
	/// <returns>true�Ȃ�΁A�L��</returns>
	bool IsEnableMotionBlur();
	/// <summary>
	/// ���[�V�����u���[�̃p�����[�^�ݒ�
	/// </summary>
	/// <param name="dir">�u���[���|�������</param>
	/// <param name="width">�u���[���|���镝</param>
	/// <param name="count">�u���[���|�����</param>
	void SetMotionBlur(const CVector& dir, float width, int count);
	/// <summary>
	/// ���[�V�����u���[�̃p�����[�^���擾
	/// </summary>
	/// <param name="outDir">�u���[�̕�����Ԃ��p�̃|�C���^</param>
	/// <param name="outWidth">�u���[�̕���Ԃ��p�̃|�C���^</param>
	/// <param name="outCount">�u���[�̉񐔂�Ԃ��p�̃|�C���^</param>
	void GetMotionBlurParam(CVector* outDir, float* outWidth, int* outCount);
}