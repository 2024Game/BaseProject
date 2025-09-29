#pragma once

#define GAME_TITLE "GameTitle"	//�Q�[���^�C�g��

#define WINDOW_WIDTH 1280		//�E�B���h�E�̕�
#define WINDOW_HEIGHT 720		//�E�B���h�E�̍���

#define CAMERA_FOVY 60.0f		//�J�����̉�p
#define CAMERA_ZNEAR 1.0f		//�J�����̕`��ŒZ����
#define CAMERA_ZFAR 1000.0f		//�J�����̕`��Œ�����

//#define FULL_SCREEN	//�t���X�N���[���ɂ���ꍇ�͒�`����

//�z��̃T�C�Y�擾���}�N����
#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

//���\�[�X�t�@�C���t�H���_
#define RES_DIR "res\\"
//�̈������}�N����
#define SAFE_DELETE(del) {if(del != nullptr) delete del; del = nullptr;}
//���I�z��̗̈������}�N����
#define SAFE_DELETE_ARRAY(ary) {if(ary != nullptr) delete[] ary; ary = nullptr;}



#include "System.h"
#include "Times.h"
#include "Blend.h"
#include "CDebugCamera.h"
#include "CDebugPrint.h"
#include "CDebugProfiler.h"
#include "CResourceManager.h"
