#include "CDebugPrint.h"
#include <stdarg.h>
#include "CText.h"
#include "CDebugInput.h"

// �C���X�^���X
CDebugPrint* CDebugPrint::mspInstance = nullptr;

// �R���X�g���N�^
CDebugPrint::CDebugPrint()
	: CTask(ETaskPriority::eDebug, 0, ETaskPauseType::eDefault, true)
	, mIsOn(true)
{
	// ������`��p�̃N���X�𐶐�
	mpText = new CText
	(
		nullptr, 24,
		CVector2::zero,
		CVector2(WINDOW_WIDTH, WINDOW_HEIGHT),
		CColor::red,
		ETaskPriority::eDebug,
		0,
		ETaskPauseType::eDefault,
		true,
		false
	);
}

// �`�悷��f�o�b�O�������ǉ�
void CDebugPrint::Print(const char* format, ...)
{
	if (mspInstance == nullptr)
	{
		mspInstance = new CDebugPrint();
	}
	if (!mspInstance->mIsOn) return;

	char buf[512];
	va_list ap;
	va_start(ap, format);
	vsprintf_s(buf, format, ap);
	va_end(ap);
	mspInstance->AddStr(buf);
}

// �`�悷��f�o�b�O�������ǉ��i�����\���p�j
void CDebugPrint::ForcePrint(const char* format, ...)
{
	if (mspInstance == nullptr)
	{
		mspInstance = new CDebugPrint();
	}

	char buf[512];
	va_list ap;
	va_start(ap, format);
	vsprintf_s(buf, format, ap);
	va_end(ap);
	mspInstance->AddStr(buf);
}

// �`�悷�镶��������X�g�ɒǉ�
void CDebugPrint::AddStr(std::string str)
{
	mPrintStrList.push_back(str);
}

// �X�V
void CDebugPrint::Update()
{
#if _DEBUG
	// �uSHIFT�v+�u8�v�Ńf�o�b�O�v�����g���I���I�t
	if (CDebugInput::Key(VK_SHIFT) && CDebugInput::PushKey('8'))
	{
		mIsOn = !mIsOn;
	}
#endif

	std::string str = "";
	for (std::string s : mPrintStrList)
	{
		str += s;
	}
	mpText->SetText(str.c_str());
	mpText->Update();
	mPrintStrList.clear();
}

// �`��
void CDebugPrint::Render()
{
	mpText->Render();
}