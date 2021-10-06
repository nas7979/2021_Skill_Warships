#include "DXUT.h"
#include "cScheduleManager.h"

void cScheduleManager::Init()
{
}

void cScheduleManager::Update()
{
	for (auto& iter = m_Schedules.begin(); iter != m_Schedules.end();)
	{
		(*iter)->Time -= NSDT;
		if ((*iter)->Time <= 0)
		{
			(*iter)->Func();
			delete *iter;
			iter = m_Schedules.erase(iter);
		}
		else
			iter++;
	}
}

void cScheduleManager::Render()
{
}

void cScheduleManager::Release()
{
	for (auto& iter : m_Schedules)
	{
		delete iter;
	}
	m_Schedules.clear();
}

void cScheduleManager::AddSchedule(float _Time, function<void()> _Func)
{
	m_Schedules.push_back(new Schedule{ _Time, _Func });
}
