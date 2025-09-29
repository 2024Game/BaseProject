#include <Windows.h>
#include "glew.h"
#include "glut.h"
#include "GLFW/glfw3.h"
#include "CApplication.h"
#include "CInput.h"
#include "CCamera.h"
#include "CGamePause.h"

// 1�b�ԂɎ��s����t���[����
int gFPS = 60;
// �O��̃t���[���̌o�ߎ���
float gDeltaTime = 0.0f;
// �Q�[���J�n���̎���
LARGE_INTEGER start_time;

CApplication gApplication;

/* display�֐�
1�b�Ԃ�60����s�����
*/
void display() {
	//�e�o�b�t�@�[���N���A
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	//�s��̃��[�h�����f���r���[�ɂ���
	glMatrixMode(GL_MODELVIEW);
	//���f���r���[�̍s���P�ʍs��ɂ���
	glLoadIdentity();

	CInput::Update();
	gApplication.Update();

}

/// <summary>
/// �}�E�X�z�C�[����]���̃R�[���o�b�N�֐�
/// </summary>
/// <param name="window"></param>
/// <param name="x"></param>
/// <param name="y"></param>
void wheel(GLFWwindow* window, double x, double y)
{
	CInput::AddMouseWheel((int)y);
}

/*�E�B���h�E�T�C�Y�ύX���̏���
void reshape(int width, int height)
width:��ʕ�
height:��ʍ���
*/
void reshape(GLFWwindow* window, int width, int height) {
	CCamera* cam = CCamera::CurrentCamera();
	if (cam == nullptr)
	{
		glViewport(0, 0, width, height);	//��ʂ̕`��G���A�̎w��

		glMatrixMode(GL_PROJECTION);	//�s����v���W�F�N�V�������[�h�֕ύX
		glLoadIdentity();				//�s���������
		//3D�̉�ʂ�ݒ�
		gluPerspective
		(
			CAMERA_FOVY,
			(double)width / (double)height,
			CAMERA_ZNEAR,
			CAMERA_ZFAR
		);

		glMatrixMode(GL_MODELVIEW);		//�s������f���r���[���[�h�֕ύX
		glLoadIdentity();				//�s���������
	}
	else
	{
		cam->Reshape(width, height);
	}
}
//
LARGE_INTEGER last_time;	//�O��̃J�E���^�l
//�P�b�ԂɂU�O��`�悷��悤�ɒ��߂���
void idle() {
	LARGE_INTEGER freq;		//��b������̃J�E���^��
	LARGE_INTEGER time;		//����̃J�E���^�l

	//��b�Ԃ̃J�E���^�����擾
	QueryPerformanceFrequency(&freq);

	if (last_time.QuadPart == 0) {
		QueryPerformanceCounter(&last_time);
	}
	do {
		//���݂̃V�X�e���̃J�E���g�����擾
		QueryPerformanceCounter(&time);

		//���̃J�E���g-�O��̃J�E���g < 1�b������̃J�E���g���Ŋ���(1/gFPS�b������̃J�E���g��)
	} while (time.QuadPart - last_time.QuadPart < freq.QuadPart / gFPS);
	gDeltaTime = (float)(time.QuadPart - last_time.QuadPart) / freq.QuadPart;
	last_time = time;

	//�`�悷��֐����Ă�
	display();
	// �������Ԃ̌v�����ʂ�`��
	CDebugProfiler::Print();
}

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
#ifndef FULL_SCREEN
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, GAME_TITLE, NULL, NULL);
#else
	//Full Screen
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, GAME_TITLE, glfwGetPrimaryMonitor(), NULL);
#endif
	if (!window)
	{
		glfwTerminate();
		return -1;
	}
	CInput::Window(window);
	CInput::ShowCursor(false);
	/* Make the window's context current */
	glfwMakeContextCurrent(window);


	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// GLEW ������������
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		// GLEW �̏������Ɏ��s����
//		std::cerr << "Can't initialize GLEW" << std::endl;
		return 1;
	}
	// ���������̃^�C�~���O��҂�  
	//glfwSwapInterval(1); 

	// �E�B���h�E�̃T�C�Y�ύX���ɌĂяo�������̓o�^
	glfwSetWindowSizeCallback(window, reshape);
	reshape(window, WINDOW_WIDTH, WINDOW_HEIGHT);

	// �}�E�X�z�C�[����]���̃R�[���o�b�N�֐���o�^
	glfwSetScrollCallback(window, wheel);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	//���C�g�̐ݒ�
	//�Œ�V�F�[�_�[�p
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	float lightPosition[] = { 0.0f, 100.0f, 100.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glEnable(GL_NORMALIZE);

	// �Q�[���J�n���̎��Ԃ��L�����Ă���
	QueryPerformanceCounter(&start_time);

	//��������
	gApplication.Start();

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{

		idle();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();

		int state = glfwGetKey(window, GLFW_KEY_ESCAPE);
		if (System::IsExitGame() || state == GLFW_PRESS) {
			//ESC�L�[�Ń��[�v�I��
			break;
		}
	}

	//�I������
	gApplication.End();

	glfwTerminate();
	return 0;
}

// �ڕW�t���[�����[�g���擾
int Times::TargetFPS()
{
	return gFPS;
}

// �v�Z��ł�1�t���[���̌o�ߎ��Ԃ��擾
float Times::CalcDeltaTime()
{
	if (gFPS == 0) return 0.0f;
	return 1.0f / gFPS;
}

// �O��̃t���[����FPS���擾
float Times::FPS()
{
	if (gDeltaTime == 0.0f) return 0.0f;
	return 1.0f / gDeltaTime;
}

// �O��̃t���[���̌o�ߎ��Ԃ��擾
float Times::DeltaTime()
{
	return gDeltaTime;
}

// �Q�[���N�����Ă���̎��Ԃ��擾
float Times::Time()
{
	LARGE_INTEGER freq, time;
	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&time);
	return (float)(time.QuadPart - start_time.QuadPart) / freq.QuadPart;
}
