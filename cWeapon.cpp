#include "DXUT.h"
#include "cWeapon.h"

void cWeapon::Init()
{
	m_LocalPos = m_Owner->m_Pos;
}

void cWeapon::Update()
{
	m_Owner->m_Pos = m_Enemy->m_Pos + RotateVec3(m_LocalPos, m_Enemy->m_Rot);
	m_Owner->m_Rot = m_Enemy->m_Rot;
	m_Owner->m_Scale = m_Enemy->m_Scale * 4;
	if (m_isFireAble)
	{
		m_isFireAble = false;
		m_OnFire(this);
	}

	float& White = GetComponent<cMeshRenderer>()->GetShader<cStandardShader>()->m_White;
	White = Clamp(White - 20 * DT, 0, 1);

	for (auto& iter = m_Schedules.begin(); iter != m_Schedules.end();)
	{
		(*iter)->Time -= DT;
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

void cWeapon::Render()
{
}

void cWeapon::Release()
{
	for (auto& iter : m_Schedules)
	{
		delete iter;
	}
}

void cWeapon::OnCollision(cObject * _Other, Vec3 _CollPos)
{
}

void cWeapon::OnAlarm(string _Key)
{
	if (_Key == "FireAble")
	{
		m_isFireAble = true;
		return;
	}
}

void cWeapon::OnAnimationEnd(string _Key)
{
}
