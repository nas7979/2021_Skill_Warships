#include "DXUT.h"
#include "cZako3.h"
#include "cItem.h"
#include "cMine.h"

void cZako3::Init()
{
	AddComponent<cMeshRenderer>()->m_Mesh = IMAGE->FindMesh("Enemy3");
	GetComponent<cMeshRenderer>()->m_Bounding = IMAGE->FindMesh("Enemy3_Bounding")->Mesh;
	GetComponent<cMeshRenderer>()->m_isReflected = true;
	GetComponent<cMeshRenderer>()->AddShader<cStandardShader>();
	AddComponent<cCollider>()->AddCollider(Vec3(0, 0, 0), 2);
	GetComponent<cCollider>()->m_CollWith.push_back(Obj_PlayerBullet);

	m_MaxHp = 100;
	m_Hp = m_MaxHp;
	m_Speed = 75;
	m_Owner->m_Scale = Vec3(6, 6, 6);
	m_Owner->m_Rot = MakeQuaternion(Vec3(0, 180, 0));

	cWeapon* a = OBJECT->AddObject<cWeapon>("Canon", Vec3(0.9 * m_Owner->m_Scale.x, 0 * m_Owner->m_Scale.y, 0 * m_Owner->m_Scale.z), Obj_Weapon)->GetComponent<cWeapon>();
	a->AddComponent<cMeshRenderer>()->m_Mesh = IMAGE->FindMesh("Canon");
	a->GetComponent<cMeshRenderer>()->m_Bounding = IMAGE->FindMesh("Canon_Bounding")->Mesh;
	a->GetComponent<cMeshRenderer>()->m_isReflected = true;
	a->GetComponent<cMeshRenderer>()->AddShader<cStandardShader>();
	a->m_Enemy = m_Owner;
	a->m_MaxHp = a->m_Hp = 30;
	a->m_OnFire = [](cWeapon* _This)->void {
		_This->m_Owner->SetAlarm("FireAble", 2);
		for (int i = 0; i < 3; i++)
		{
			_This->m_Schedules.push_back(new Schedule{ 0.3f * i, [=]()->void {
				cBullet* a = OBJECT->AddObject<cBullet>("EnemyBullet", _This->m_Owner->m_Pos, Obj_EnemyBullet)->GetComponent<cBullet>();
				a->m_Owner->m_Scale = Vec3(10, 10, 10);
				a->AddComponent<cSpriteRenderer>()->m_Sprite = IMAGE->FindTexture("Bullet5_5");
				a->GetComponent<cSpriteRenderer>()->m_Mode = BILL;
				a->GetComponent<cSpriteRenderer>()->m_isReflected = true;
				a->AddComponent<cCollider>()->AddCollider(Vec3(0, 0, 0), 0.25);
				a->m_Speed = 10;
				a->m_Damage = 20;
				a->m_Friction = 1.05;
				a->m_EndSpeed = 600;
				a->m_Owner->m_Rot = _This->m_Owner->m_Rot * MakeQuaternion(RandomVec3(Vec3(-5, -5, 0), Vec3(5, 5, 0)));
			} });
		}
	};
	m_Weapons.push_back(a);

	a = OBJECT->AddObject<cWeapon>("Canon", Vec3(-0.5 * m_Owner->m_Scale.x, 0.75 * m_Owner->m_Scale.y, 0 * m_Owner->m_Scale.z), Obj_Weapon)->GetComponent<cWeapon>();
	a->AddComponent<cMeshRenderer>()->m_Mesh = IMAGE->FindMesh("Canon");
	a->GetComponent<cMeshRenderer>()->m_Bounding = IMAGE->FindMesh("Canon_Bounding")->Mesh;
	a->GetComponent<cMeshRenderer>()->m_isReflected = true;
	a->GetComponent<cMeshRenderer>()->AddShader<cStandardShader>();
	a->m_Enemy = m_Owner;
	a->m_MaxHp = a->m_Hp = 30;
	a->m_OnFire = [](cWeapon* _This)->void {
		_This->m_Owner->SetAlarm("FireAble", 2);
		for (int i = 0; i < 3; i++)
		{
			_This->m_Schedules.push_back(new Schedule{ 0.3f * i, [=]()->void {
				cBullet* a = OBJECT->AddObject<cBullet>("EnemyBullet", _This->m_Owner->m_Pos, Obj_EnemyBullet)->GetComponent<cBullet>();
				a->m_Owner->m_Scale = Vec3(10, 10, 10);
				a->AddComponent<cSpriteRenderer>()->m_Sprite = IMAGE->FindTexture("Bullet5_5");
				a->GetComponent<cSpriteRenderer>()->m_Mode = BILL;
				a->GetComponent<cSpriteRenderer>()->m_isReflected = true;
				a->AddComponent<cCollider>()->AddCollider(Vec3(0, 0, 0), 0.25);
				a->m_Speed = 10;
				a->m_Damage = 20;
				a->m_Friction = 1.05;
				a->m_EndSpeed = 600;
				a->m_Owner->m_Rot = _This->m_Owner->m_Rot * MakeQuaternion(RandomVec3(Vec3(-5, -5, 0), Vec3(5, 5, 0)));
			} });
		}
	};
	m_Weapons.push_back(a);

	a = OBJECT->AddObject<cWeapon>("Torpedo", Vec3(0.5 * m_Owner->m_Scale.x, -0.5 * m_Owner->m_Scale.y, 2 * m_Owner->m_Scale.z), Obj_Weapon)->GetComponent<cWeapon>();
	a->AddComponent<cMeshRenderer>()->m_Mesh = IMAGE->FindMesh("MissileLauncher");
	a->GetComponent<cMeshRenderer>()->m_Bounding = IMAGE->FindMesh("MissileLauncher_Bounding")->Mesh;
	a->GetComponent<cMeshRenderer>()->m_isReflected = true;
	a->GetComponent<cMeshRenderer>()->AddShader<cStandardShader>();
	a->m_Enemy = m_Owner;
	a->m_MaxHp = a->m_Hp = 30;
	a->m_OnFire = [](cWeapon* _This)->void {
		_This->m_Owner->SetAlarm("FireAble", Random(7.5f, 15.f));
		cObject* a = OBJECT->AddObject<cBullet>("Torpedo", Vec3(_This->m_Owner->m_Pos.x, 0, _This->m_Owner->m_Pos.z), Obj_EnemyBullet);
		a->m_Scale = Vec3(10, 10, 10);
		a->AddComponent<cMeshRenderer>()->m_Mesh = IMAGE->FindMesh("Torpedo");
		a->GetComponent<cMeshRenderer>()->m_Surface = "Reflect";
		a->GetComponent<cBullet>()->m_Damage = 30;
		a->GetComponent<cBullet>()->m_Speed = 150;
		a->GetComponent<cBullet>()->m_RotSpeed = 50;
		a->GetComponent<cBullet>()->m_Target = OBJECT->m_Player;
		a->AddComponent<cCollider>()->AddCollider(Vec3(0, 1, 0), 0.2);
		a->AddComponent<cLambda>()->m_OnUpate = [](cObject* _This)->void {
			_This->m_Pos.y = 0;
		};
		a->GetComponent<cLambda>()->m_OnRelease = [](cObject* _This)->void {
			OBJECT->AddObject<cWave>("Wave", Vec3(_This->m_Pos.x, 0, _This->m_Pos.z), Obj_Particle)->GetComponent<cWave>()->Set(1.5, 2, 100);
			SOUND->Play("Torpedo_Hit");
		};
		a->SetAlarm("TorpedoWave", 0);
		a->m_Rot = _This->m_Owner->m_Rot;
	};
	m_Weapons.push_back(a);
	a = OBJECT->AddObject<cWeapon>("Torpedo", Vec3(-0.5 * m_Owner->m_Scale.x, -0.5 * m_Owner->m_Scale.y, 2 * m_Owner->m_Scale.z), Obj_Weapon)->GetComponent<cWeapon>();
	a->AddComponent<cMeshRenderer>()->m_Mesh = IMAGE->FindMesh("MissileLauncher");
	a->GetComponent<cMeshRenderer>()->m_Bounding = IMAGE->FindMesh("MissileLauncher_Bounding")->Mesh;
	a->GetComponent<cMeshRenderer>()->m_isReflected = true;
	a->GetComponent<cMeshRenderer>()->AddShader<cStandardShader>();
	a->m_Enemy = m_Owner;
	a->m_MaxHp = a->m_Hp = 30;
	a->m_OnFire = [](cWeapon* _This)->void {
		_This->m_Owner->SetAlarm("FireAble", Random(7.5f, 15.f));
		cObject* a = OBJECT->AddObject<cBullet>("Torpedo", Vec3(_This->m_Owner->m_Pos.x, 0, _This->m_Owner->m_Pos.z), Obj_EnemyBullet);
		a->m_Scale = Vec3(10, 10, 10);
		a->AddComponent<cMeshRenderer>()->m_Mesh = IMAGE->FindMesh("Torpedo");
		a->GetComponent<cMeshRenderer>()->m_Surface = "Reflect";
		a->GetComponent<cBullet>()->m_Damage = 30;
		a->GetComponent<cBullet>()->m_Speed = 150;
		a->GetComponent<cBullet>()->m_RotSpeed = 50;
		a->GetComponent<cBullet>()->m_Target = OBJECT->m_Player;
		a->AddComponent<cCollider>()->AddCollider(Vec3(0, 1, 0), 0.2);
		a->AddComponent<cLambda>()->m_OnUpate = [](cObject* _This)->void {
			_This->m_Pos.y = 0;
		};
		a->GetComponent<cLambda>()->m_OnRelease = [](cObject* _This)->void {
			OBJECT->AddObject<cWave>("Wave", Vec3(_This->m_Pos.x, 0, _This->m_Pos.z), Obj_Particle)->GetComponent<cWave>()->Set(1.5, 2, 100);
			SOUND->Play("Torpedo_Hit");
		};
		a->SetAlarm("TorpedoWave", 0);
		a->m_Rot = _This->m_Owner->m_Rot;
	};
	m_Weapons.push_back(a);
}

void cZako3::Update()
{
	cEnemy::Update();
	if (Random(0.f, 0.016666667f / DT) <= 0.002)
	{
		OBJECT->AddObject<cMine>("Mine", Vec3(m_Owner->m_Pos.x, 0, m_Owner->m_Pos.z), Obj_None);
	}
}

void cZako3::Render()
{
}

void cZako3::Release()
{
	cEnemy::Release();
}

void cZako3::OnCollision(cObject * _Other, Vec3 _CollPos)
{
	cEnemy::OnCollision(_Other, _CollPos);
}

void cZako3::OnAlarm(string _Key)
{
}

void cZako3::OnAnimationEnd(string _Key)
{
}

void cZako3::Death()
{
	GAME->Explosion(m_Owner->m_Pos, 15);
	SOUND->Play("Explosion1_" + to_string(Random(1, 4)));
	switch (Random(0, 30))
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
