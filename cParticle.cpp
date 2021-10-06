#include "DXUT.h"
#include "cParticle.h"

void cParticle::Init()
{
}

void cParticle::Update()
{
	m_PosVel *= pow(m_PosFri, 60 * DT);
	m_PosVel.y -= m_Gravity * DT;
	m_RotVel *= pow(m_RotFri, 60 * DT);
	m_ScaVel *= pow(m_ScaFri, 60 * DT);
	m_AlpVel *= pow(m_AlpFri, 60 * DT);

	m_Owner->m_Pos += (m_PosVel + Vec3(0, 0, -GAME->m_ScrollSpeed * 3000)) * DT;
	m_Owner->m_Rot *= MakeQuaternion(m_RotVel * DT);
	m_Owner->m_Scale += m_ScaVel * DT;

	cSpriteRenderer* Sprite = GetComponent<cSpriteRenderer>();
	if (Sprite)
	{
		c2DShader* Shader = Sprite->GetShader<c2DShader>();
		if (Shader)
		{
			Shader->m_A += m_AlpVel * DT;
			if (Shader->m_A <= 0)
				m_Owner->m_isDestroyed = true;
		}
	}
	if (m_Owner->m_Scale.x <= 0 || m_Owner->m_Scale.y <= 0 || m_Owner->m_Scale.x <= 0)
	{
		m_Owner->m_isDestroyed = true;
	}
}

void cParticle::Render()
{
}

void cParticle::Release()
{
}

void cParticle::OnCollision(cObject * _Other, Vec3 _CollPos)
{
}

void cParticle::OnAlarm(string _Key)
{
	if (_Key == "Destroy")
	{
		m_Owner->m_isDestroyed = true;
		return;
	}
}

void cParticle::OnAnimationEnd(string _Key)
{
	if (_Key == "Effect")
	{
		m_Owner->m_isDestroyed = true;
		return;
	}
}

void cParticle::SetPos(Vec3 _Start, Vec3 _Vel, float _Fri)
{
	m_Owner->m_Pos = _Start;
	m_PosVel = _Vel;
	m_PosFri = _Fri;
}

void cParticle::SetRot(Quaternion _Start, Vec3 _Vel, float _Fri)
{
	m_Owner->m_Rot = _Start;
	m_RotVel = _Vel;
	m_RotFri = _Fri;
}

void cParticle::SetScale(Vec3 _Start, Vec3 _Vel, float _Fri)
{
	m_Owner->m_Scale = _Start;
	m_ScaVel = _Vel;
	m_ScaFri = _Fri;
}

void cParticle::SetAlpha(float _Start, float _Vel, float _Fri)
{
	GetComponent<cSpriteRenderer>()->GetShader<c2DShader>()->m_A = _Start;
	m_AlpVel = _Vel;
	m_AlpFri = _Fri;
}
