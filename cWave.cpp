#include "DXUT.h"
#include "cWave.h"
#include "c2DShader.h"

void cWave::Init()
{
	AddComponent<cSpriteRenderer>()->m_Sprite = IMAGE->FindTexture("Wave_Normal");
	GetComponent<cSpriteRenderer>()->AddShader<c2DShader>();
	GetComponent<cSpriteRenderer>()->m_Surface = "Wave";
	GetComponent<cSpriteRenderer>()->m_isReflected = false;
}

void cWave::Update()
{
	m_Owner->m_Pos.z -= GAME->m_ScrollSpeed * 3000 * DT;
	m_Owner->m_Scale += Vec3(1, 1, 1) * m_SizeSpeed * DT;
	GetComponent<cSpriteRenderer>()->GetShader<c2DShader>()->m_A -= m_AlphaSpeed * DT;
	if (GetComponent<cSpriteRenderer>()->GetShader<c2DShader>()->m_A <= 0)
	{
		m_Owner->m_isDestroyed = true;
	}
}

void cWave::Render()
{
}

void cWave::Release()
{
}

void cWave::OnCollision(cObject * _Other, Vec3 _CollPos)
{
}

void cWave::OnAlarm(string _Key)
{
}

void cWave::OnAnimationEnd(string _Key)
{
}

void cWave::Set(float _StartAlpha, float _AlphaSpeed, float _SizeSpeed, bool _WithEffect)
{
	GetComponent<cSpriteRenderer>()->GetShader<c2DShader>()->m_A = _StartAlpha;
	m_AlphaSpeed = _AlphaSpeed;
	m_SizeSpeed = _SizeSpeed;

	if (!_WithEffect)
		return;
	cObject* a;
	Vec3 Dir;
	for (int i = 0; i < max(_StartAlpha - 0.3, 0) * 50; i++)
	{
		Dir = RandomVec3(Vec3(-1, 0, -1), Vec3(1, 1, 1));
		a = OBJECT->AddObject<cParticle>("Particle", m_Owner->m_Pos, Obj_Particle);
		a->AddComponent<cSpriteRenderer>()->m_Sprite = IMAGE->FindTexture("Water_Splat" + to_string(Random(1, 4)));
		a->GetComponent<cSpriteRenderer>()->m_Mode = BILL;
		a->GetComponent<cSpriteRenderer>()->m_isReflected = true;
		a->GetComponent<cSpriteRenderer>()->AddShader<c2DShader>();
		a->GetComponent<cParticle>()->SetPos(m_Owner->m_Pos + Dir * _SizeSpeed * 0.1, Dir * _SizeSpeed * _StartAlpha * 0.5, 1);
		a->GetComponent<cParticle>()->SetScale(Vec3(10, 10, 10) * _StartAlpha, Vec3(1, 1, 1) * Random(0.05f, 0.1f) * _SizeSpeed, 1);
		a->GetComponent<cParticle>()->SetRot(MakeQuaternion(Vec3(0, 0, Random(0, 359))), Vec3(0, 0, Random(-180, 180)), 1);
		a->GetComponent<cParticle>()->m_Gravity = _SizeSpeed * _StartAlpha;
		a->GetComponent<cParticle>()->m_AlpVel = -(_AlphaSpeed / _StartAlpha) * 2;
	}
	for (int i = 0; i < max(_StartAlpha - 0.5, 0) * 30; i++)
	{
		Dir = RandomVec3(Vec3(-1, -0.1, -1), Vec3(1, 0, 1));
		a = OBJECT->AddObject<cParticle>("Particle", m_Owner->m_Pos, Obj_Particle);
		a->AddComponent<cSpriteRenderer>()->m_Sprite = IMAGE->FindTexture("Water_Splash");
		a->GetComponent<cSpriteRenderer>()->m_isReflected = true;
		a->GetComponent<cSpriteRenderer>()->AddShader<c2DShader>();
		a->GetComponent<cParticle>()->SetPos(m_Owner->m_Pos + Dir * _SizeSpeed * 0.1, Vec3(0,0,0), 1);
		a->GetComponent<cParticle>()->SetScale(Vec3(10, Random(20.f,30.f), 10) * _StartAlpha * 4, Vec3(1, 1, 1.5) * Random(60, 75) * _StartAlpha, 1);
		a->GetComponent<cParticle>()->SetRot(MakeQuaternion(Vec3(90 + Random(-30, 30), Random(0, 359), Random(-30, 30))), Vec3(0, 0, 0), 1);
		a->GetComponent<cParticle>()->m_AlpVel = -(_AlphaSpeed / _StartAlpha) * 3;
	}
}
