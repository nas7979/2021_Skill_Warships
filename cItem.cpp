#include "DXUT.h"
#include "cItem.h"
#include "cPlayer.h"

void cItem::Init()
{
	m_Owner->m_Scale = Vec3(1, 1, 1) * 10;
	if (m_Owner->m_Name == "Minigun")
	{
		AddComponent<cMeshRenderer>()->m_Mesh = IMAGE->FindMesh("Minigun_Ammo");
	}
	else if (m_Owner->m_Name == "Torpedo")
	{
		AddComponent<cMeshRenderer>()->m_Mesh = IMAGE->FindMesh("Torpedo");
		m_Owner->m_Rot = MakeQuaternion(Vec3(45, 0, 0));
	}
	else if (m_Owner->m_Name == "Missile")
	{
		AddComponent<cMeshRenderer>()->m_Mesh = IMAGE->FindMesh("Missile");
		m_Owner->m_Rot = MakeQuaternion(Vec3(45, 0, 0));
		m_Owner->m_Scale *= 2;
	}
	else if (m_Owner->m_Name == "Booster")
	{
		AddComponent<cMeshRenderer>()->m_Mesh = IMAGE->FindMesh("Booster");
	}
	else if (m_Owner->m_Name == "Hp")
	{
		AddComponent<cMeshRenderer>()->m_Mesh = IMAGE->FindMesh("Hp");
	}
	else if (m_Owner->m_Name == "Star")
	{
		AddComponent<cMeshRenderer>()->m_Mesh = IMAGE->FindMesh("Star");
	}
	GetComponent<cMeshRenderer>()->m_isReflected = true;

	AddComponent<cCollider>()->AddCollider(Vec3(0, 0, 0), 2);
	GetComponent<cCollider>()->m_CollWith.push_back(Obj_Player);
}

void cItem::Update()
{
	m_Owner->m_Pos.z -= 50 * DT;
	m_Owner->m_Pos.y = 5 + cos(m_Owner->m_Pos.z * 0.1) * 3;
	m_Owner->m_Rot *= MakeQuaternion(Vec3(0, 90 * DT, 0));
}

void cItem::Render()
{
}

void cItem::Release()
{
}

void cItem::OnCollision(cObject * _Other, Vec3 _CollPos)
{
	cPlayer* Player = _Other->GetComponent<cPlayer>();
	if (m_Owner->m_Name == "Minigun")
	{
		Player->m_Ammo[1] += 50;
	}
	else if (m_Owner->m_Name == "Torpedo")
	{
		Player->m_Ammo[2] += 3;
	}
	else if (m_Owner->m_Name == "Missile")
	{
		Player->m_Ammo[3] += 15;
	}
	else if (m_Owner->m_Name == "Booster")
	{
		Player->m_Speed += 125;
		Player->m_Booster = 10;
	}
	else if (m_Owner->m_Name == "Hp")
	{
		Player->m_Hp = Clamp(Player->m_Hp + 50, 0, Player->m_MaxHp);
	}
	else if (m_Owner->m_Name == "Star")
	{
		Player->GetComponent<cCollider>()->m_isEnable = false;
		Player->GetComponent<cMeshRenderer>()->GetShader<cStandardShader>()->m_White = 1;
		Player->m_Star = 10;
	}
	m_Owner->m_isDestroyed = true;
}

void cItem::OnAlarm(string _Key)
{
}

void cItem::OnAnimationEnd(string _Key)
{
}
