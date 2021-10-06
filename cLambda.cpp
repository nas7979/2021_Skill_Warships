#include "DXUT.h"
#include "cLambda.h"

void cLambda::Init()
{
}

void cLambda::Update()
{
	if (m_OnUpate)
		m_OnUpate(m_Owner);
}

void cLambda::Render()
{
}

void cLambda::Release()
{
	if (m_OnRelease)
		m_OnRelease(m_Owner);
}

void cLambda::OnCollision(cObject * _Other, Vec3 _CollPos)
{
}

void cLambda::OnAlarm(string _Key)
{
	if (m_OnAlarm)
	{
		m_OnAlarm(m_Owner);
		m_Owner->SetAlarm("", m_AlarmTimer);
	}
}

void cLambda::OnAnimationEnd(string _Key)
{
}
