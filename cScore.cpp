#include "DXUT.h"
#include "cScore.h"
#include "cPlayer.h"

void cScore::Init()
{
	m_Phase = 0;
	m_Life = OBJECT->m_Player->GetComponent<cPlayer>()->m_Hp * 100000;
	m_Time = GAME->m_Time * 100000;
	GAME->m_Score += m_Life + m_Time;
	for (int i = 1; i <= 2; i++)
	{
		SCHEDULE->AddSchedule(2 * i, [&]()->void {
			m_Phase++;
		});
	}
	m_Alpha = 255 * 7;
}

void cScore::Update()
{
	m_Alpha -= NSDT;
	if (m_Alpha <= 0)
	{
		SCENE->ChangeScene(SCENE->m_NextScene);
	}
}

void cScore::Render()
{
	if (m_Phase >= 0)
	{
		IMAGE->RenderText("LIFE BONUS", Vec2(960, 100), 0, Vec2(2, 2), 0, Vec2(0.5, 0.5), D3DCOLOR_ARGB((int)Clamp(m_Alpha, 0, 255), 255, 0, 0));
		IMAGE->RenderText(to_string(m_Life), Vec2(960, 230), 0, Vec2(2, 2), 0, Vec2(0.5, 0.5), D3DCOLOR_ARGB((int)Clamp(m_Alpha, 0, 255), 255, 255, 255));
	}
	if (m_Phase >= 1)
	{
		IMAGE->RenderText("TIME BONUS", Vec2(960, 400), 0, Vec2(2, 2), 0, Vec2(0.5, 0.5), D3DCOLOR_ARGB((int)Clamp(m_Alpha, 0, 255), 255, 0, 0));
		IMAGE->RenderText(to_string(m_Time), Vec2(960, 530), 0, Vec2(2, 2), 0, Vec2(0.5, 0.5), D3DCOLOR_ARGB((int)Clamp(m_Alpha, 0, 255), 255, 255, 255));
	}
	if (m_Phase >= 2)
	{
		IMAGE->RenderText("TOTAL SCORE", Vec2(960, 700), 0, Vec2(2, 2), 0, Vec2(0.5, 0.5), D3DCOLOR_ARGB((int)Clamp(m_Alpha, 0, 255), 255, 0, 0));
		IMAGE->RenderText(to_string(GAME->m_Score), Vec2(960, 830), 0, Vec2(2, 2), 0, Vec2(0.5, 0.5), D3DCOLOR_ARGB((int)Clamp(m_Alpha, 0, 255), 255, 255, 255));
	}
}

void cScore::Release()
{
}

void cScore::OnCollision(cObject * _Other, Vec3 _CollPos)
{
}

void cScore::OnAlarm(string _Key)
{
}

void cScore::OnAnimationEnd(string _Key)
{
}
