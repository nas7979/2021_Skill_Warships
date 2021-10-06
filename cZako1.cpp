#include "DXUT.h"
#include "cZako1.h"

void cZako1::Init()
{
	AddComponent<cMeshRenderer>()->m_Mesh = IMAGE->FindMesh("Enemy1");
	GetComponent<cMeshRenderer>()->m_Bounding = IMAGE->FindMesh("Enemy1_Bounding")->Mesh;
	GetComponent<cMeshRenderer>()->m_isReflected = true;
	GetComponent<cMeshRenderer>()->AddShader<cStandardShader>();
	AddComponent<cCollider>()->AddCollider(Vec3(0, 0, 0), 2);
	GetComponent<cCollider>()->m_CollWith.push_back(Obj_PlayerBullet);

	m_MaxHp = 30;
	m_Hp = m_MaxHp;
	m_Speed = 100;
	m_Owner->m_Scale = Vec3(5, 5, 5);
	m_Owner->m_Rot = MakeQuaternion(Vec3(0, 180, 0));

	cWeapon* a = OBJECT->AddObject<cWeapon>("Canon", Vec3(1 * m_Owner->m_Scale.x, 0 * m_Owner->m_Scale.y, 0.75 * m_Owner->m_Scale.z), Obj_Weapon)->GetComponent<cWeapon>();
	a->AddComponent<cMeshRenderer>()->m_Mesh = IMAGE->FindMesh("Canon");
	a->GetComponent<cMeshRenderer>()->m_Bounding = IMAGE->FindMesh("Canon_Bounding")->Mesh;
	a->GetComponent<cMeshRenderer>()->m_isReflected = true;
	a->GetComponent<cMeshRenderer>()->AddShader<cStandardShader>();
	a->m_Enemy = m_Owner;
	a->m_MaxHp = a->m_Hp = 30;
	a->m_OnFire = [](cWeapon* _This)->void {
		_This->m_Owner->SetAlarm("FireAble", 1);
		for (int i = 0; i < 1; i++)
		{
			cBullet* a = OBJECT->AddObject<cBullet>("EnemyBullet", _This->m_Owner->m_Pos, Obj_EnemyBullet)->GetComponent<cBullet>();
			a->m_Owner->m_Scale = Vec3(5, 5, 5);
			a->AddComponent<cSpriteRenderer>()->m_Sprite = IMAGE->FindTexture("Bullet1_1");
			a->GetComponent<cSpriteRenderer>()->m_Mode = BILL;
			a->GetComponent<cSpriteRenderer>()->m_isReflected = true;
			a->AddComponent<cCollider>()->AddCollider(Vec3(0, 0, 0), 0.25);
			a->m_Speed = 150;
			a->m_Damage = 10;
			a->m_Owner->m_Rot = _This->m_Owner->m_Rot * MakeQuaternion(RandomVec3(Vec3(0, -5, 0), Vec3(0, 5, 0)));
		}
	};
	m_Weapons.push_back(a);

	a = OBJECT->AddObject<cWeapon>("Canon", Vec3(-1 * m_Owner->m_Scale.x, 0 * m_Owner->m_Scale.y, 0.75 * m_Owner->m_Scale.z), Obj_Weapon)->GetComponent<cWeapon>();
	a->AddComponent<cMeshRenderer>()->m_Mesh = IMAGE->FindMesh("Canon");
	a->GetComponent<cMeshRenderer>()->m_Bounding = IMAGE->FindMesh("Canon_Bounding")->Mesh;
	a->GetComponent<cMeshRenderer>()->m_isReflected = true;
	a->GetComponent<cMeshRenderer>()->AddShader<cStandardShader>();
	a->m_Enemy = m_Owner;
	a->m_MaxHp = a->m_Hp = 30;
	a->m_OnFire = [](cWeapon* _This)->void {
		_This->m_Owner->SetAlarm("FireAble", 1);
		for (int i = 0; i < 1; i++)
		{
			cBullet* a = OBJECT->AddObject<cBullet>("EnemyBullet", _This->m_Owner->m_Pos, Obj_EnemyBullet)->GetComponent<cBullet>();
			a->m_Owner->m_Scale = Vec3(5, 5, 5);
			a->AddComponent<cSpriteRenderer>()->m_Sprite = IMAGE->FindTexture("Bullet1_1");
			a->GetComponent<cSpriteRenderer>()->m_Mode = BILL;
			a->GetComponent<cSpriteRenderer>()->m_isReflected = true;
			a->AddComponent<cCollider>()->AddCollider(Vec3(0, 0, 0), 0.25);
			a->m_Speed = 150;
			a->m_Damage = 10;
			a->m_Owner->m_Rot = _This->m_Owner->m_Rot * MakeQuaternion(RandomVec3(Vec3(0, -5, 0), Vec3(0, 5, 0)));
		}
	};
	m_Weapons.push_back(a);
}

void cZako1::Update()
{
	cEnemy::Update();
}

void cZako1::Render()
{
}

void cZako1::Release()
{
	cEnemy::Release();
}

void cZako1::OnCollision(cObject * _Other, Vec3 _CollPos)
{
	cEnemy::OnCollision(_Other, _CollPos);
}

void cZako1::OnAlarm(string _Key)
{
}

void cZako1::OnAnimationEnd(string _Key)
{
}

void cZako1::Death()
{
	GAME->Explosion(m_Owner->m_Pos, 10);
	SOUND->Play("Explosion1_" + to_string(Random(1, 4)));
}
