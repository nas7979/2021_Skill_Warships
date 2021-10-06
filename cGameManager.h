#pragma once
struct Ranking
{
	string Name;
	int Score;
};
class cGameManager : public cSingleton<cGameManager>
{
public:
	void Init();
	void Update();
	void Render();
	void Release();

	float m_DT;
	float m_NSDT;
	float m_TimeScale;
	float m_HitStopTimer;
	float m_ScrollSpeed;
	int m_Score;
	bool m_TimerStop;
	bool m_Inv;
	bool m_Stop;
	bool m_Win;
	float m_Time;
	list<Ranking> m_Ranking;

	void SortRanking();
	void PlayerInit();
	void HitStop(float _Time, float _Speed);
	void Explosion(Vec3 _Pos, float _Scale);
};

#define GAME cGameManager::GetInstance()