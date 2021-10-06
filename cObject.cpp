#include "DXUT.h"
#include "cObject.h"

void cObject::Init()
{
}

void cObject::Update()
{
	for (auto& iter : m_Components)
	{
		if (iter->m_isEnable)
		{
			iter->Update();
		}
	}
	for (auto& iter = m_Alarms.begin(); iter != m_Alarms.end();)
	{
		if ((*iter)->Time <= 0)
		{
			for (auto& comp : m_Components)
			{
				comp->OnAlarm((*iter)->Key);
			}
			delete (*iter);
			iter = m_Alarms.erase(iter);
		}
		else
		{
			(*iter)->Time -= DT;
			iter++;
		}
	}
	m_Mat = MakeMatrix(m_Pos, m_Rot, m_Scale);
}

void cObject::Render()
{
	for (auto& iter : m_Components)
	{
		if (iter->m_isEnable)
		{
			iter->Render();
		}
	}
}

void cObject::Release()
{
	for (auto& iter : m_Components)
	{
		iter->Release();
		delete iter;
	}
	for (auto& iter : m_Alarms)
	{
		delete iter;
	}
}

void cObject::SetAlarm(string _Key, float _Time)
{
	m_Alarms.push_back(new Alarm{ _Key, _Time });
}
