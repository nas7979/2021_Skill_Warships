#include "DXUT.h"
#include "cMine.h"
#include "cPlayer.h"

void cMine::Init()
{
	AddComponent<cMeshRenderer>()->m_Mesh = IMAGE->FindMesh("Mine");
	GetComponent<cMeshRenderer>()->AddShader<cStandardShader>();
	GetComponent<cMeshRenderer>()->m_isReflected = true;
	AddComponent<cCollider>()->AddCollider(Vec3(0, 0, 0), 3);
	GetComponent<cCollider>()->m_CollWith.push_back(Obj_Player);
	m_Owner->m_Scale = Vec3(1, 1, 1) * 10;

	m_Speed = 100;
	m_MaxHp = m_Hp = 1000;
}

void cMine::Update()
{
	m_Owner->m_Pos.z -= 100 * DT;
}

void cMine::Render()
{
	IMAGE->RenderSprite(IMAGE->FindTexture("Warning_Circle"), MakeMatrix(m_Owner->m_Pos + Vec3(0,0,0), MakeQuaternion(Vec3(0, 0, 0)), Vec3(1, 1, 1) * 60), BILL, nullptr, "Default", 1);
}

void cMine::Release()
{
}

void cMine::OnCollision(cObject * _Other, Vec3 _CollPos)
{
	_Other->GetComponent<cPlayer>()->DealDamage(50);
	m_Owner->m_isDestroyed = true;

	//cObject* a = OBJECT->AddObject<cSpriteRenderer>("Particle", m_Owner->m_Pos + RandomVec3() * 10, Obj_Particle);
	//a->AddComponent<cSpriteAnimation>()->AddAnimation("Effect", "explosiona1_orange_", Random(50.f, 70.f));
	//a->GetComponent<cSpriteRenderer>()->m_Mode = BILL;
	//a->GetComponent<cSpriteRenderer>()->m_isReflected = true;
	//a->m_Scale = Vec3(1, 1, 1) * Random(10.f, 15.f);
	//a->m_Rot = MakeQuaternion(Vec3(0, 0, Random(0, 359)));
	//a = OBJECT->AddObject<cParticle>("Particle", m_Owner->m_Pos, Obj_Particle);
	//a->AddComponent<cSpriteRenderer>()->m_Sprite = IMAGE->FindTexture("Part_Light");
	//a->GetComponent<cSpriteRenderer>()->m_Mode = BILL;
	//a->GetComponent<cSpriteRenderer>()->m_isReflected = true;
	//a->GetComponent<cSpriteRenderer>()->AddShader<c2DShader>();
	//a->GetComponent<cParticle>()->SetScale(Vec3(1, 1, 1) * Random(80, 120), Vec3(0, 0, 0), 1);
	//a->GetComponent<cParticle>()->SetAlpha(3, -1, 1.1);

	GAME->Explosion(m_Owner->m_Pos, 10);
	SOUND->Play("Explosion1_" + to_string(Random(1, 4)));
}

void cMine::OnAlarm(string _Key)
{
}

void cMine::OnAnimationEnd(string _Key)
{
}

void cMine::Death()
{
}
