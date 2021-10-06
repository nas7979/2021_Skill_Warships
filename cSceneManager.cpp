#include "DXUT.h"
#include "cSceneManager.h"

void cSceneManager::Init()
{
}

void cSceneManager::Update()
{
	if (m_New)
	{
		OBJECT->Release();
		SOUND->StopAll();
		SCHEDULE->Release();
		CAMERA->Release();
		if (m_Cur)
			m_Cur->Release();
		m_Cur = m_New;
		m_New = nullptr;
		m_Cur->Init();
	}
	if (m_Cur)
	{
		m_Cur->Update();
	}
}

void cSceneManager::Render()
{
	if (m_Cur)
	{
		m_Cur->Render();
	}
}

void cSceneManager::Release()
{
	if (m_Cur)
		m_Cur->Release();
	for (auto& iter : m_Scenes)
	{
		delete iter.second;
	}
}

void cSceneManager::ChangeScene(string _Key)
{
	m_New = m_Scenes[_Key];
}
