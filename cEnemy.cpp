#include "DXUT.h"
#include "cEnemy.h"
#include "cItem.h"

void cEnemy::Init()
{
}

void cEnemy::Update()
{
	for (auto& iter = m_Weapons.begin(); iter != m_Weapons.end();)
	{
		if ((*iter)->m_Hp <= 0)
		{
			if ((*iter)->m_Owner->m_Name != "Canon")
			{
				OBJECT->AddObject<cItem>((*iter)->m_Owner->m_Name, Vec3(m_Owner->m_Pos.x, 2, m_Owner->m_Pos.z), Obj_Item);
			}
			(*iter)->m_Owner->m_isDestroyed = true;
			iter = m_Weapons.erase(iter);
		}
		else
			iter++;
	}

	if (m_Hp <= 0 || m_Owner->m_Pos.z <= -100)
	{
		m_Owner->m_isDestroyed = true;
		for (auto& iter : m_Weapons)
		{
			iter->m_Owner->m_isDestroyed = true;
		}
		if (m_Hp <= 0)
		{
			GAME->m_Score += m_MaxHp * 10000;
			Death();
		}
	}

	m_Speed *= pow(m_Friction, 60 * DT);
	m_Owner->m_Pos += Vec3(0, 0, -m_Speed * DT);

	float& White = GetComponent<cMeshRenderer>()->GetShader<cStandardShader>()->m_White;
	White = Clamp(White - 20 * DT, 0, 1);
}

void cEnemy::Render()
{
}

void cEnemy::Release()
{
}

void cEnemy::OnCollision(cObject * _Other, Vec3 _CollPos)
{
	if (_Other->m_Tag == Obj_PlayerBullet)
	{
		m_Hp -= _Other->GetComponent<cBullet>()->m_Damage;
		_Other->m_isDestroyed = true;
	}
}

void cEnemy::OnAlarm(string _Key)
{
}

void cEnemy::OnAnimationEnd(string _Key)
{
}

float cEnemy::TryRayCast(Vec3* _RayPos, Vec3* _RayDir, bool _isEnemyOnly, Vec3 * _HitPos, cObject** _Target)
{
	float Dist = 100000;
	float DistTemp;
	Vec3 HitPos;
	if (RayCast(*_RayPos, *_RayDir, GetComponent<cMeshRenderer>()->m_Bounding, &m_Owner->m_Pos, &m_Owner->m_Rot, &(m_Owner->m_Scale * 1.5), &HitPos))
	{
		Dist = D3DXVec3Length(&(*_RayPos - HitPos));
		*_HitPos = HitPos;
		*_Target = m_Owner;
	}
	if (_isEnemyOnly)
		return Dist;
	for (auto& iter : m_Weapons)
	{
		if (RayCast(*_RayPos, *_RayDir, iter->GetComponent<cMeshRenderer>()->m_Bounding, &iter->m_Owner->m_Pos, &iter->m_Owner->m_Rot, &(iter->m_Owner->m_Scale * 1.5), &HitPos))
		{
			DistTemp = D3DXVec3Length(&(*_RayPos - HitPos));
			if (DistTemp <= Dist)
			{
				Dist = DistTemp;
				*_HitPos = HitPos;
				*_Target = iter->m_Owner;
			}
		}
	}
	return Dist;
}
