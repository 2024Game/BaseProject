#pragma once
#include <initializer_list>

// �I�u�W�F�N�g���ʗp�̃^�O
enum class ETag
{
	eNone = -1,

	eField,	// �t�B�[���h���\������I�u�W�F�N�g
	eRideableObject,	// ��邱�Ƃ��ł���I�u�W�F�N�g
	eItem,	// �A�C�e���̃I�u�W�F�N�g

	ePlayer,// �v���C���[
	eEnemy,	// �G�l�~�[

	eBullet,// �e��
	eFlame,	// ��
	eSlash,	// �a��

	eUI,

	eCamera,// �J����

	Num
};
// �I�u�W�F�N�g���ʗp�̃^�O�̏��������X�g
using Tags = std::initializer_list<ETag>;
