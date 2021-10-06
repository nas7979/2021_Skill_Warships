#include "DXUT.h"
#include "cZako2.h"

void cZako2::Init()
{
	AddComponent<cMeshRenderer>()->m_Mesh = IMAGE->FindMesh("Enemy2");
	GetComponent<cMeshRenderer>()->m_Bounding = IMAGE->FindMesh("Enemy2_Bounding")->Mesh;
	GetComponent<cMeshRenderer>()->m_isReflected = true;
	GetComponent<cMeshRenderer>()->AddShader<cStandardShader>();
	AddComponent<cCollider>()->AddCollider(Vec3(0, 0, 0), 2);
	GetComponent<cCollider>()->m_CollWith.push_back(Obj_PlayerBullet);

	m_MaxHp = 50;
	m_Hp = m_MaxHp;
	m_Speed = 150;
	m_Owner->m_Scale = Vec3(5, 5, 5);
	m_Owner->m_Rot = MakeQuaternion(Vec3(0, 180, 0));

	cWeapon* a = OBJECT->AddObject<cWeapon>("Minigun", Vec3(0.5 * m_Owner->m_Scale.x, 0.75 * m_Owner->m_Scale.y, 0 * m_Owner->m_Scale.z), Obj_Weapon)->GetComponent<cWeapon>();
	a->AddComponent<cMeshRenderer>()->m_Mesh = IMAGE->FindMesh("Minigun");
	a->GetComponent<cMeshRenderer>()->m_Bounding = IMAGE->FindMesh("Minigun_Bounding")->Mesh;
	a->GetComponent<cMeshRenderer>()->m_isReflected = true;
	a->GetComponent<cMeshRenderer>()->AddShader<cStandardShader>();
	a->m_Enemy = m_Owner;
	a->m_MaxHp = a->m_Hp = 30;
	a->m_OnFire = [](cWeapon* _This)->void {
		_This->m_Owner->SetAlarm("FireAble", 0.15);
		for (int i = 0; i < 1; i++)
		{
			cBullet* a = OBJECT->AddObject<cBullet>("EnemyBullet", _This->m_Owner->m_Pos, Obj_EnemyBullet)->GetComponent<cBullet>();
			a->m_Owner->m_Scale = Vec3(5, 5, 5);
			a->AddComponent<cSpriteRenderer>()->m_Sprite = IMAGE->FindTexture("Bullet1_4");
			a->GetComponent<cSpriteRenderer>()->m_Mode = BILL;
			a->GetComponent<cSpriteRenderer>()->m_isReflected = true;
			a->AddComponent<cCollider>()->AddCollider(Vec3(0, 0, 0), 0.25);
			a->m_Speed = 200;
			a->m_Damage = 10;
			a->m_Owner->m_Rot = _This->m_Owner->m_Rot * MakeQuaternion(RandomVec3(Vec3(-5, -5, 0), Vec3(5, 5, 0)));
		}
	};
	m_Weapons.push_back(a);

	a = OBJECT->AddObject<cWeapon>("Minigun", Vec3(-0.5 * m_Owner->m_Scale.x, 0.75 * m_Owner->m_Scale.y, 0 * m_Owner->m_Scale.z), Obj_Weapon)->GetComponent<cWeapon>();
	a->AddComponent<cMeshRenderer>()->m_Mesh = IMAGE->FindMesh("Minigun");
	a->GetComponent<cMeshRenderer>()->m_Bounding = IMAGE->FindMesh("Minigun_Bounding")->Mesh;
	a->GetComponent<cMeshRenderer>()->m_isReflected = true;
	a->GetComponent<cMeshRenderer>()->AddShader<cStandardShader>();
	a->m_Enemy = m_Owner;
	a->m_MaxHp = a->m_Hp = 30;
	a->m_OnFire = [](cWeapon* _This)->void {
		_This->m_Owner->SetAlarm("FireAble", 0.15);
		for (int i = 0; i < 1; i++)
		{
			cBullet* a = OBJECT->AddObject<cBullet>("EnemyBullet", _This->m_Owner->m_Pos, Obj_EnemyBullet)->GetComponent<cBullet>();
			a->m_Owner->m_Scale = Vec3(5, 5, 5);
			a->AddComponent<cSpriteRenderer>()->m_Sprite = IMAGE->FindTexture("Bullet1_4");
			a->GetComponent<cSpriteRenderer>()->m_Mode = BILL;
			a->GetComponent<cSpriteRenderer>()->m_isReflected = true;
			a->AddComponent<cCollider>()->AddCollider(Vec3(0, 0, 0), 0.25);
			a->m_Speed = 200;
			a->m_Damage = 10;
			a->m_Owner->m_Rot = _This->m_Owner->m_Rot * MakeQuaternion(RandomVec3(Vec3(-5, -5, 0), Vec3(5, 5, 0)));
		}
	};
	m_Weapons.push_back(a);
}

void cZako2::Update()
{
	cEnemy::Update();
	m_Owner->m_Rot = LookAt(m_Owner->m_Pos, OBJECT->m_Player->m_Pos);
}

void cZako2::Render()
{
}

void cZako2::Release()
{
	cEnemy::Release();
}

void cZako2::OnCollision(cObject * _Other, Vec3 _CollPos)
{
	cEnemy::OnCollision(_Other, _CollPos);
}

void cZako2::OnAlarm(string _Key)
{
}

void cZako2::OnAnimationEnd(string _Key)
{
}

void cZako2::Death()
{
	GAME->Explosion(m_Owner->m_Pos, 10);
	SOUND->Play("Explosion1_" + to_string(Random(1, 4)));
}
