#include "DXUT.h"
#include "cGameManager.h"

void cGameManager::Init()
{
	m_TimeScale = 1;
	PlayerInit();
	ifstream File("Ranking.txt");
	string Name;
	int Score;
	for (int i = 0; i < 10; i++)
	{
		File >> Name;
		File >> Score;
		m_Ranking.push_back(Ranking{ Name, Score });
	}
	File.close();
	SortRanking();
}

void cGameManager::Update()
{
	m_NSDT = DXUTGetElapsedTime();
	if (m_Stop)
		m_NSDT = 0;
	m_DT = m_NSDT * m_TimeScale;
	m_HitStopTimer -= m_NSDT;
	if (m_HitStopTimer <= 0)
	{
		m_TimeScale = 1;
		m_HitStopTimer = 10000000;
	}

	if (INPUT->KeyDown(VK_F1))
	{
		m_Inv = !m_Inv;
	}
	if (INPUT->KeyDown(VK_F2))
	{
		m_Stop = !m_Stop;
	}
	if (INPUT->KeyDown(VK_F3))
	{
		m_TimerStop = !m_TimerStop;
	}
	if (INPUT->KeyDown(VK_F4))
	{
		SCENE->ChangeScene("TITLE");
	}
	if (INPUT->KeyDown(VK_F5))
	{
		SCENE->ChangeScene("STAGE1");
	}
	if (INPUT->KeyDown(VK_F6))
	{
		SCENE->ChangeScene("STAGE2");
	}
}

void cGameManager::Render()
{
}

void cGameManager::Release()
{
}

void cGameManager::SortRanking()
{
	m_Ranking.sort([](Ranking &a, Ranking &b)->bool {return a.Score > b.Score; });
}

void cGameManager::PlayerInit()
{
	m_Score = 0;
	m_Time = 0;
	m_TimerStop = false;
	m_Inv = false;
	m_Stop = false;
	m_Win = false;
}

void cGameManager::HitStop(float _Time, float _Speed)
{
	m_HitStopTimer = _Time;
	m_TimeScale = _Speed;
}

void cGameManager::Explosion(Vec3 _Pos, float _Scale)
{
	cObject* a;
	for (int i = 0; i < _Scale * 0.5; i++)
	{
		a = OBJECT->AddObject<cSpriteRenderer>("Particle", _Pos + RandomVec3() * _Scale * 0.5, Obj_Particle);
		a->AddComponent<cParticle>();
		a->GetComponent<cSpriteRenderer>()->m_Mode = BILL;
		a->GetComponent<cSpriteRenderer>()->m_isReflected = true;
		a->AddComponent<cSpriteAnimation>()->AddAnimation("Effect", "Explosion", Random(20.f, 30.f));
		a->m_Scale = Vec3(1, 1, 1) * Random(0.75f, 1.25f) * _Scale * 5;
		a->m_Rot = MakeQuaternion(Vec3(0, 0, Random(0, 359)));
	}
	for (int i = 0; i < _Scale * 0.2; i++)
	{
		a = OBJECT->AddObject<cSpriteRenderer>("Particle", _Pos, Obj_Particle);
		a->GetComponent<cSpriteRenderer>()->m_Sprite = IMAGE->FindTexture("Circle");
		a->GetComponent<cSpriteRenderer>()->m_isReflected = true;
		a->GetComponent<cSpriteRenderer>()->AddShader<c2DShader>();
		a->AddComponent<cParticle>()->SetScale(Vec3(1, 1, 1), Vec3(1, 1, 1) * _Scale * 25, 0.97);
		a->GetComponent<cParticle>()->SetAlpha(1, -0.02f, Random(1.05f, 1.1f));
		a->m_Rot = MakeQuaternion(RandomVec3() * 360);
	}
	for (int i = 0; i < _Scale; i++)
	{
		a = OBJECT->AddObject<cSpriteRenderer>("Particle", _Pos, Obj_Particle);
		a->GetComponent<cSpriteRenderer>()->m_Sprite = IMAGE->FindTexture("Debris" + to_string(Random(1, 6)));
		a->GetComponent<cSpriteRenderer>()->m_Mode = BILL;
		a->GetComponent<cSpriteRenderer>()->m_isReflected = true;
		a->m_Scale = Vec3(1, 1, 1) * Random(0.75f, 1.25f) * _Scale * 0.75;
		a->AddComponent<cParticle>()->SetPos(_Pos + RandomVec3() * _Scale * 0.3, RandomVec3() * _Scale * 10, 0.99);
		a->GetComponent<cParticle>()->SetRot(MakeQuaternion(Vec3(0, 0, Random(0, 359))), Vec3(0, 0, Random(-300, 300)), 0.99);
		a->GetComponent<cParticle>()->m_Gravity = a->m_Scale.x * 10;
		a->AddComponent<cLambda>()->m_OnUpate = [](cObject* _This)->void {
			if (_This->m_Pos.y < -2)
			{
				OBJECT->AddObject<cWave>("Wave", Vec3(_This->m_Pos.x, 0, _This->m_Pos.z), Obj_Particle)->GetComponent<cWave>()->Set(0.3 + _This->m_Scale.x * 0.05, 1, 75);
				_This->m_isDestroyed = true;
			}
		};
		a->GetComponent<cLambda>()->m_OnAlarm = [](cObject* _This)->void {
			cObject* a = OBJECT->AddObject<cParticle>("Particle", _This->m_Pos, Obj_Particle);
			a->AddComponent<cSpriteRenderer>();
			a->AddComponent<cSpriteAnimation>()->AddAnimation("Effect", "newsmoke" + to_string(Random(1, 2)) + "_", 60);
			a->GetComponent<cSpriteRenderer>()->m_Mode = BILL;
			a->GetComponent<cSpriteRenderer>()->m_isReflected = true;
			a->m_Scale = Vec3(1, 1, 1) * Random(1.f, 1.25f) * _This->m_Scale.x * 0.5;
			a->m_Rot = MakeQuaternion(Vec3(Random(0, 359), Random(0, 359), Random(0, 359)));
		};
		a->SetAlarm("", a->GetComponent<cLambda>()->m_AlarmTimer = 0.05);
	}
	a = OBJECT->AddObject<cParticle>("Particle", _Pos, Obj_Particle);
	a->AddComponent<cSpriteRenderer>()->m_Sprite = IMAGE->FindTexture("Part_Light");
	a->GetComponent<cSpriteRenderer>()->m_Mode = BILL;
	a->GetComponent<cSpriteRenderer>()->m_isReflected = true;
	a->GetComponent<cSpriteRenderer>()->AddShader<c2DShader>()->SetColor(1, 1, 0.9);
	a->GetComponent<cParticle>()->SetScale(Vec3(1, 1, 1) * Random(7.5f, 10.f) * _Scale, Vec3(0, 0, 0), 1);
	a->GetComponent<cParticle>()->SetAlpha(3, -1, 1.1);
}
