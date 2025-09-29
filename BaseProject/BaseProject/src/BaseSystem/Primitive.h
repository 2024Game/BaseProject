#pragma once
#include "CVector.h"
#include "CColor.h"

// ��{�}�`�̕`��֘A
namespace Primitive
{
	/// <summary>
	/// ������`��i3D�j
	/// </summary>
	/// <param name="s">�n�_</param>
	/// <param name="e">�I�_</param>
	/// <param name="color">�F</param>
	/// <param name="lineWidth">���̑���</param>
	/// <param name="blend">�u�����h�^�C�v</param>
	void DrawLine(const CVector& s, const CVector& e,
		const CColor& color, float lineWidth = 1.0f, EBlend blend = EBlend::eAlpha);
	/// <summary>
	/// ������`��i2D�j
	/// </summary>
	/// <param name="s">�n�_</param>
	/// <param name="e">�I�_</param>
	/// <param name="color">�F</param>
	/// <param name="lineWidth">���̑���</param>
	/// <param name="blend">�u�����h�^�C�v</param>
	void DrawLine2D(const CVector2& s, const CVector2& e,
		const CColor& color, float lineWidth = 1.0f, EBlend blend = EBlend::eAlpha);

	/// <summary>
	/// ����`��
	/// </summary>
	/// <param name="pos">�`��s��</param>
	/// <param name="rad">���a</param>
	/// <param name="color">�F</param>
	/// <param name="blend">�u�����h�^�C�v</param>
	void DrawSphere(const CMatrix& m, float rad,
		const CColor& color, EBlend blend = EBlend::eAlpha);
	/// <summary>
	/// ���C���[�t���[���̋���`��
	/// </summary>
	/// <param name="pos">�`��s��</param>
	/// <param name="rad">���a</param>
	/// <param name="color">�F</param>
	/// <param name="blend">�u�����h�^�C�v</param>
	void DrawWireSphere(const CMatrix& m, float rad,
		const CColor& color, EBlend blend = EBlend::eAlpha);

	/// <summary>
	/// �O�p�`��`��
	/// </summary>
	/// <param name="v0">1�ڂ̒��_</param>
	/// <param name="v1">2�ڂ̒��_</param>
	/// <param name="v2">3�ڂ̒��_</param>
	/// <param name="color">�F</param>
	/// <param name="blend">�u�����h�^�C�v</param>
	void DrawTriangle(const CVector& v0, const CVector& v1, const CVector& v2,
		const CColor& color, EBlend blend = EBlend::eAlpha);

	/// <summary>
	/// �|���S����`��
	/// </summary>
	/// <param name="m">�`��s��</param>
	/// <param name="size">�c���̃T�C�Y</param>
	/// <param name="color">�F</param>
	/// <param name="blend">�u�����h�^�C�v</param>
	void DrawQuad(const CMatrix& m, const CVector2& size,
		const CColor& color, EBlend blend = EBlend::eAlpha);

	/// <summary>
	/// �J�v�Z����`��
	/// </summary>
	/// <param name="sp">�J�v�Z�����\����������̎n�_</param>
	/// <param name="ep">�J�v�Z�����\����������̏I�_</param>
	/// <param name="rad">�J�v�Z���̔��a</param>
	/// <param name="color">�F</param>
	/// <param name="blend">�u�����h�^�C�v</param>
	void DrawCapsule(const CVector& sp, const CVector& ep, float rad,
		const CColor& color, EBlend blend = EBlend::eAlpha);

	/// <summary>
	/// ���C���[�t���[���̃J�v�Z����`��
	/// </summary>
	/// <param name="sp">�J�v�Z�����\����������̎n�_</param>
	/// <param name="ep">�J�v�Z�����\����������̏I�_</param>
	/// <param name="rad">�J�v�Z���̔��a</param>
	/// <param name="color">�F</param>
	/// <param name="blend">�u�����h�^�C�v</param>
	void DrawWireCapsule(const CVector& sp, const CVector& ep, float rad,
		const CColor& color, EBlend blend = EBlend::eAlpha);

	/// <summary>
	/// ��`��`��
	/// </summary>
	/// <param name="m">�`��s��</param>
	/// <param name="startAngle">��`�̊J�n�p�x�i�f�B�O���[�j</param>
	/// <param name="endAngle">��`�̏I���p�x�i�f�B�O���[�j</param>
	/// <param name="rad">��`�̔��a</param>
	/// <param name="color">��`�̐F</param>
	/// <param name="blend">�u�����h�^�C�v</param>
	void DrawSector(const CMatrix& m, float startAngle, float endAngle,
		float rad, const CColor& color, EBlend blend = EBlend::eAlpha);

	/// <summary>
	/// �{�b�N�X��`��
	/// </summary>
	/// <param name="center">���S���W</param>
	/// <param name="size">�e���̃T�C�Y</param>
	/// <param name="color">�{�b�N�X�̐F</param>
	/// <param name="blend">�u�����h�^�C�v</param>
	void DrawBox(const CVector& center, const CVector& size,
		const CColor& color, EBlend blend = EBlend::eAlpha);

	/// <summary>
	/// ���C���[�t���[���̃{�b�N�X��`��
	/// </summary>
	/// <param name="center">���S���W</param>
	/// <param name="size">�e���̃T�C�Y</param>
	/// <param name="color">�{�b�N�X�̐F</param>
	/// <param name="blend">�u�����h�^�C�v</param>
	void DrawWireBox(const CVector& center, const CVector& size,
		const CColor& color, EBlend blend = EBlend::eAlpha);
};