#pragma once

// �u�����h�����̃^�C�v
enum class EBlend
{
	eAlpha,		// �A���t�@�u�����h
	eAdd,		// ���Z�u�����h
	eMultiply,	// ��Z�u�����h
	eInvert,	// �F���]
};

namespace Blend
{
	/// <summary>
	/// �w�肵���u�����h������L����
	/// </summary>
	/// <param name="blend">�u�����h�����̃^�C�v</param>
	void EnableBlend(EBlend blend);
	/// <summary>
	/// �u�����h�����𖳌���
	/// </summary>
	void DisableBlend();
}