#include "CResourceManager.h"
#include "CModel.h"
#include "CModelX.h"
#include "CTexture.h"
#include "CSound.h"
#include "CFontData.h"

//�e���v���[�g�̑O�錾
template CModel* CResourceManager::Load(std::string name, std::string path, bool dontDelete);
template CModelX* CResourceManager::Load(std::string name, std::string path, bool dontDelete);
template CTexture* CResourceManager::Load(std::string name, std::string path, bool dontDelete);
template CSound* CResourceManager::Load(std::string name, std::string path, bool dontDelete);
template CFontData* CResourceManager::Load(std::string name, std::string path, bool dontDelete);
template CModel* CResourceManager::Get(std::string name);
template CModelX* CResourceManager::Get(std::string name);
template CTexture* CResourceManager::Get(std::string name);
template CSound* CResourceManager::Get(std::string name);
template CFontData* CResourceManager::Get(std::string name);

//CResourceManager�̃C���X�^���X
CResourceManager* CResourceManager::mpInstance = nullptr;

//�R���X�g���N�^
CResourceManager::CResourceManager()
{
}

//�f�X�g���N�^
CResourceManager::~CResourceManager()
{
	//�ǂݍ��ݍς݂̃��f���f�[�^��j��
	for (auto& res : mResources)
	{
		delete res.second;
	}
	mResources.clear();

	mpInstance = nullptr;
}

//�C���X�^���X�擾
CResourceManager* CResourceManager::Instance()
{
	if (mpInstance == nullptr)
	{
		mpInstance = new CResourceManager();
	}
	return mpInstance;
}

//�C���X�^���X��j��
void CResourceManager::ClearInstance()
{
	SAFE_DELETE(mpInstance);
}

// ���\�[�X�ǂݍ���
template <class T>
T* CResourceManager::Load(std::string name, std::string path, bool dontDelete)
{
	// ���Ƀ��\�[�X���ǂݍ��ݍς݂ł���΁A
	// �ǂݍ��ݍς݂̃��\�[�X��Ԃ�
	auto& list = Instance()->mResources;
	auto find = list.find(name);
	if (find != list.end())
	{
		return dynamic_cast<T*>(find->second);
	}

	printf("Load Resource [%s] :\n  %s\n", name.c_str(), path.c_str());

	// ���\�[�X�ǂݍ���
	CResource* res = new T();
	if (res == nullptr) return nullptr;
	if (!res->Load(path, dontDelete))
	{
		printf("-> [Failed]\n");

		// �ǂݍ��ݎ��s
		delete res;
		return nullptr;
	}
	res->SetDontDelete(dontDelete);

	// ���\�[�X�̃��X�g�ɒǉ�
	list.insert(make_pair(name, res));

	printf("-> [Success]\n");

	// �ǂݍ��񂾃��\�[�X��Ԃ�
	return dynamic_cast<T*>(res);
}

// �ǂݍ��ݍς݂̃��\�[�X���擾
template <class T>
T* CResourceManager::Get(std::string name)
{
	// ���\�[�X���ǂݍ��ݍς݂łȂ���΁A
	// nullptr��Ԃ�
	auto& list = Instance()->mResources;
	auto find = list.find(name);
	if (find == list.end()) return nullptr;

	// �ǂݍ���ł��郊�\�[�X��Ԃ�
	return dynamic_cast<T*>(find->second);
}

// ���\�[�X�폜
void CResourceManager::Delete(std::string name)
{
	auto& list = Instance()->mResources;
	auto find = list.find(name);
	if (find == list.end()) return;

	printf("Delete Resource [%s]\n", find->first.c_str());

	CResource* res = find->second;
	list.erase(find);
	delete res;
}

// �w�肵���V�[���ɏ������郊�\�[�X��S�č폜
void CResourceManager::DeleteInScene(EScene scene)
{
	auto& list = Instance()->mResources;
	auto itr = list.begin();
	auto end = list.end();
	while (itr != end)
	{
		CResource* res = itr->second;
		if (res->mSceneType == scene)
		{
			printf("Delete Resource [%s]\n", itr->first.c_str());

			itr = list.erase(itr);
			delete res;
		}
		else
		{
			itr++;
		}
	}
}
