#include "DXUT.h"
#include "cBullet.h"

void cBullet::Init()
{
}

void cBullet::Update()
{
	m_Speed *= pow(m_Friction, 60 * DT);
	if (m_Friction > 1)
	{
		if (m_EndSpeed < m_Speed)
		{
			m_Speed = m_EndSpeed;
			m_Friction = 1;
		}
	}
	else if (m_Friction < 1)
	{
		if (m_EndSpeed > m_Speed)
		{
			m_Speed = m_EndSpeed;
			m_Friction = 1;
		}
	}

	if (m_Target)
	{
		if (m_Target->m_isDestroyed)
		{
			m_Target = nullptr;
		}
		else
		{
			Vec3 To, Front;
			D3DXVec3Normalize(&To, &(m_Target->m_Pos - m_Owner->m_Pos));
			Front = RotateVec3(Vec3(0, 0, -1), m_Owner->m_Rot);
			D3DXVec3Cross(&To, &To, &Front);
			m_Owner->m_Rot *= *D3DXQuaternionRotationAxis(&Quaternion(), &To, D3DXToRadian(m_RotSpeed * DT));
			m_Dir = RotateVec3(Vec3(0, 0, 1), m_Owner->m_Rot);
		}
	}
	else if (m_Dir == Vec3(0, 0, 0))
	{
		m_Dir = RotateVec3(Vec3(0, 0, 1), m_Owner->m_Rot);
	}

	m_Owner->m_Pos += m_Dir * m_Speed * DT;

	if (m_Owner->m_Pos.z > 150 && m_Dir.z > 0 && m_Owner->m_Tag == Obj_EnemyBullet)
	{
		GetComponent<cCollider>()->m_isEnable = false;
	}
	if (m_Owner->m_Pos.z <= -50 || m_Owner->m_Pos.z >= 750 || abs(m_Owner->m_Pos.x) >= (m_Owner->m_Pos.z + 50) * 1.7)
	{
		m_Owner->m_isDestroyed = true;
	}
	if (m_Owner->m_Pos.y < -0.4 * m_Owner->m_Scale.x && m_Target == nullptr)
	{
		m_Owner->m_isDestroyed = true;
		OBJECT->AddObject<cWave>("Wave", Vec3(m_Owner->m_Pos.x, 0, m_Owner->m_Pos.z), Obj_None)->GetComponent<cWave>()->Set(0.49, 0.8, 100);
		//SOUND->Play("Water_Splash" + to_string(Random(1, 5)), -2500);
	}
}

void cBullet::Render()
{
}

void cBullet::Release()
{
	if (GetComponent<cSpriteRenderer>() != nullptr)
	{
		cObject* a = OBJECT->AddObject<cParticle>("Prticle", m_Owner->m_Pos, Obj_Particle);
		a->AddComponent<cSpriteRenderer>()->m_Sprite = GetComponent<cSpriteRenderer>()->m_Sprite;
		a->GetComponent<cSpriteRenderer>()->m_isReflected = true;
		a->GetComponent<cSpriteRenderer>()->m_Mode = BILL;
		a->GetComponent<cSpriteRenderer>()->AddShader<c2DShader>();
		a->GetComponent<cParticle>()->SetScale(m_Owner->m_Scale, Vec3(1,1,1) * 100, 0.9);
		a->GetComponent<cParticle>()->SetAlpha(1, -2.5, 1.02);
	}
}

void cBullet::OnCollision(cObject * _Other, Vec3 _CollPos)
{
}

void cBullet::OnAlarm(string _Key)
{
	if (_Key == "TorpedoWave")
	{
		cWave* Wave = OBJECT->AddObject<cWave>("Wave", Vec3(m_Owner->m_Pos.x, 0, m_Owner->m_Pos.z), Obj_Particle)->GetComponent<cWave>();
		Wave->Set(0.6, 1, 60, false);
		m_Owner->SetAlarm("TorpedoWave", 0.05);
		return;
	}
}

void cBullet::OnAnimationEnd(string _Key)
{
}
