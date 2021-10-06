#include "DXUT.h"
#include "cZako5.h"
#include "cItem.h"

void cZako5::Init()
{
	AddComponent<cMeshRenderer>()->m_Mesh = IMAGE->FindMesh("Enemy5");
	GetComponent<cMeshRenderer>()->m_Bounding = IMAGE->FindMesh("Enemy5_Bounding")->Mesh;
	GetComponent<cMeshRenderer>()->m_isReflected = true;
	GetComponent<cMeshRenderer>()->AddShader<cStandardShader>();
	AddComponent<cCollider>()->AddCollider(Vec3(0, 0, 0), 2);
	GetComponent<cCollider>()->m_CollWith.push_back(Obj_PlayerBullet);

	m_MaxHp = 200;
	m_Hp = m_MaxHp;
	m_Speed = 500;
	m_Friction = 0.985;
	m_Owner->m_Scale = Vec3(10, 10, 10);
	m_Owner->m_Rot = MakeQuaternion(Vec3(0, 180, 0));

	for (int x = -1; x <= 1; x += 2)
	{
		for (int y = -1; y <= 1; y += 2)
		{
			cWeapon* a = OBJECT->AddObject<cWeapon>("Minigun", Vec3(0.75 * x * m_Owner->m_Scale.x, 0.5 * y * m_Owner->m_Scale.y, 0.5 * m_Owner->m_Scale.z), Obj_Weapon)->GetComponent<cWeapon>();
			a->AddComponent<cMeshRenderer>()->m_Mesh = IMAGE->FindMesh("Minigun");
			a->GetComponent<cMeshRenderer>()->m_Bounding = IMAGE->FindMesh("Minigun_Bounding")->Mesh;
			a->GetComponent<cMeshRenderer>()->m_isReflected = true;
			a->GetComponent<cMeshRenderer>()->AddShader<cStandardShader>();
			a->m_Enemy = m_Owner;
			a->m_MaxHp = a->m_Hp = 100;
			a->m_OnFire = [](cWeapon* _This)->void {
				_This->m_Owner->SetAlarm("FireAble", 1);
				for (int i = 0; i < 10; i++)
				{
					cBullet* a = OBJECT->AddObject<cBullet>("EnemyBullet", _This->m_Owner->m_Pos, Obj_EnemyBullet)->GetComponent<cBullet>();
					a->m_Owner->m_Scale = Vec3(5, 5, 5);
					a->AddComponent<cSpriteRenderer>()->m_Sprite = IMAGE->FindTexture("Bullet4_1");
					a->GetComponent<cSpriteRenderer>()->m_Mode = BILL;
					a->GetComponent<cSpriteRenderer>()->m_isReflected = true;
					a->AddComponent<cCollider>()->AddCollider(Vec3(0, 0, 0), 0.25);
					a->m_Speed = 200;
					a->m_Damage = 10;
					a->m_Owner->m_Rot = LookAt(_This->m_Owner->m_Pos, OBJECT->m_Player->m_Pos + RandomVec3() * 10);
				}
			};
			m_Weapons.push_back(a);
		}
	}

	cWeapon* a = OBJECT->AddObject<cWeapon>("Canon", Vec3(0, 100000, 0), Obj_Weapon)->GetComponent<cWeapon>();
	a->AddComponent<cMeshRenderer>()->m_Mesh = IMAGE->FindMesh("Canon");
	a->GetComponent<cMeshRenderer>()->m_Bounding = IMAGE->FindMesh("MissileLauncher_Bounding")->Mesh;
	a->GetComponent<cMeshRenderer>()->m_isReflected = true;
	a->GetComponent<cMeshRenderer>()->AddShader<cStandardShader>();
	a->m_Enemy = m_Owner;
	a->m_MaxHp = a->m_Hp = 10000000;
	a->m_OnFire = [](cWeapon* _This)->void {
		_This->m_Owner->SetAlarm("FireAble", 7.5);
		float Offset = Random(0, 359);
		for (int i = 0; i < 72; i++)
		{
			for (int j = -7; j <= 7; j++)
			{
				cBullet* a = OBJECT->AddObject<cBullet>("EnemyBullet", _This->m_Enemy->m_Pos, Obj_EnemyBullet)->GetComponent<cBullet>();
				a->m_Owner->m_Scale = Vec3(5, 5, 5);
				a->AddComponent<cSpriteRenderer>()->m_Sprite = IMAGE->FindTexture("Bullet1_7");
				a->GetComponent<cSpriteRenderer>()->m_Mode = BILL;
				a->GetComponent<cSpriteRenderer>()->m_isReflected = true;
				a->AddComponent<cCollider>()->AddCollider(Vec3(0, 0, 0), 0.25);
				a->m_Speed = 150;
				a->m_Damage = 10;
				a->m_Owner->m_Rot = MakeQuaternion(Vec3(j * 2, i * 5 + Offset, 0));
			}
		}
	};
	m_Weapons.push_back(a);
}

void cZako5::Update()
{
	cEnemy::Update();
	Vec3 Dir;
	D3DXVec3Normalize(&Dir, &(m_Owner->m_Pos - OBJECT->m_Player->m_Pos));
	Dir.x = -Clamp(Dir.x, -0.4, 0.4);
	Dir.y = -Clamp(Dir.y, -0.4, 0.4);
	Dir.z = -1;
	D3DXVec3Normalize(&Dir, &Dir);
	m_Owner->m_Rot = LookAt(Vec3(0, 0, 0), Dir);
}

void cZako5::Render()
{
}

void cZako5::Release()
{
	cEnemy::Release();
}

void cZako5::OnCollision(cObject * _Other, Vec3 _CollPos)
{
	cEnemy::OnCollision(_Other, _CollPos);
}

void cZako5::OnAlarm(string _Key)
{
}

void cZako5::OnAnimationEnd(string _Key)
{
}

void cZako5::Death()
{
	for (int i = 0; i < 3; i++)
	{
		GAME->Explosion(m_Owner->m_Pos + RandomVec3() * 10, 15);
	}
	SOUND->Play("Explosion1_" + to_string(Random(1, 4)));
	switch (Random(0, 10))
	{
	case 0:
		OBJECT->AddObject<cItem>("Star", m_Owner->m_Pos, Obj_Item); break;
	case 1:
	case 2:
		OBJECT->AddObject<cItem>("Hp", m_Owner->m_Pos, Obj_Item); break;
	case 3:
	case 4:
	case 5:
		OBJECT->AddObject<cItem>("Booster", m_Owner->m_Pos, Obj_Item); break;
	}
}
