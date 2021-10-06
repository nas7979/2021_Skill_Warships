#include "DXUT.h"
#include "cObjectManager.h"

void cObjectManager::Init()
{
}

void cObjectManager::Update()
{
	for (int Tag = 0; Tag < Obj_End; Tag++)
	{
		for (auto& iter = m_Objects[Tag].begin(); iter != m_Objects[Tag].end();)
		{
			(*iter)->Update();
			iter++;
		}
	}
}

void cObjectManager::Render()
{
	for (int Tag = 0; Tag < Obj_End; Tag++)
	{
		for (auto& iter = m_Objects[Tag].begin(); iter != m_Objects[Tag].end();)
		{
			if ((*iter)->m_isDestroyed)
			{
				(*iter)->Release();
				delete *iter;
				iter = m_Objects[Tag].erase(iter);
			}
			else
			{
				(*iter)->Render();
				iter++;
			}
		}
	}
}

void cObjectManager::Release()
{
	for (int Tag = 0; Tag < Obj_End; Tag++)
	{
		for (auto& iter : m_Objects[Tag])
		{
			iter->Release();
			delete iter;
		}
		m_Objects[Tag].clear();
	}
}
