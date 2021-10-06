#include "DXUT.h"
#include "cTrash.h"
#include "cPlayer.h"

void cTrash::Init()
{
	AddComponent<cMeshRenderer>()->m_Mesh = IMAGE->FindMesh("Trash");
	GetComponent<cMeshRenderer>()->m_Bounding = IMAGE->FindMesh("Trash_Bounding")->Mesh;
	GetComponent<cMeshRenderer>()->AddShader<cStandardShader>();
	GetComponent<cMeshRenderer>()->m_isReflected = true;
	AddComponent<cCollider>()->AddCollider(Vec3(0, 0, 0), 0.5);
	GetComponent<cCollider>()->m_CollWith.push_back(Obj_Player);
	GetComponent<cCollider>()->m_CollWith.push_back(Obj_PlayerBullet);
	m_Owner->m_Scale = Vec3(1, 1, 1) * Random(10.f, 12.5f);
	m_Owner->m_Rot = MakeQuaternion(RandomVec3(Vec3(-5, 0, 5), Vec3(5, 359, 5)));

	m_Speed = 100;
	m_MaxHp = m_Hp = 100;
}

void cTrash::Update()
{
	m_Owner->m_Pos.z -= m_Speed * DT;
	if (m_Hp <= 0)
	{
		m_Owner->m_isDestroyed = true;
		cObject* a;
		Vec3 Dir;
		for (int i = 0; i < 35; i++)
		{
			Dir = RandomVec3();
			a = OBJECT->AddObject<cParticle>("Particle", m_Owner->m_Pos, Obj_Particle);
			a->AddComponent<cSpriteRenderer>()->m_Sprite = IMAGE->FindTexture("Trash" + to_string(Random(1, 4)));
			a->GetComponent<cSpriteRenderer>()->m_Mode = BILL;
			a->GetComponent<cSpriteRenderer>()->m_isReflected = true;
			a->GetComponent<cSpriteRenderer>()->AddShader<c2DShader>();
			a->GetComponent<cParticle>()->SetPos(m_Owner->m_Pos + Dir * 7.5, Dir * Random(30, 60), 1);
			a->GetComponent<cParticle>()->SetScale(Vec3(1, 1, 1) * Random(4.f, 7.f), Vec3(1, 1, 1) * Random(20.f, 30.f), 1);
			a->GetComponent<cParticle>()->SetRot(MakeQuaternion(Vec3(0, 0, Random(0, 359))), Vec3(0, 0, Random(-90, 90)), 1);
			a->GetComponent<cParticle>()->SetAlpha(1, Random(-1.f, -1.5f), 1.5);
			a->GetComponent<cParticle>()->m_Gravity = a->m_Scale.x * 10;
		}
	}
	m_Hp = 100;

	float& White = GetComponent<cMeshRenderer>()->GetShader<cStandardShader>()->m_White;
	White = Clamp(White - 20 * DT, 0, 1);

	if (m_Owner->m_Pos.z <= -100)
	{
		m_Owner->m_isDestroyed = true;
	}
}

void cTrash::Render()
{
}

void cTrash::Release()
{
}

void cTrash::OnCollision(cObject * _Other, Vec3 _CollPos)
{
	cEnemy::OnCollision(_Other, _CollPos);
	if (_Other->m_Tag == Obj_Player)
	{
		_Other->GetComponent<cPlayer>()->m_Slow = 3;
		_Other->GetComponent<cPlayer>()->m_Speed = 25;
		m_Hp = 0;
	}
}

void cTrash::OnAlarm(string _Key)
{
}

void cTrash::OnAnimationEnd(string _Key)
{
}

void cTrash::Death()
{
}
