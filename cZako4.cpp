#include "DXUT.h"
#include "cZako4.h"
#include "cItem.h"

void cZako4::Init()
{
	AddComponent<cMeshRenderer>()->m_Mesh = IMAGE->FindMesh("Enemy4");
	GetComponent<cMeshRenderer>()->m_Bounding = IMAGE->FindMesh("Enemy4_Bounding")->Mesh;
	GetComponent<cMeshRenderer>()->m_isReflected = true;
	GetComponent<cMeshRenderer>()->AddShader<cStandardShader>();
	AddComponent<cCollider>()->AddCollider(Vec3(0, 0, 0), 2);
	GetComponent<cCollider>()->m_CollWith.push_back(Obj_PlayerBullet);

	m_MaxHp = 100;
	m_Hp = m_MaxHp;
	m_Speed = 125;
	m_Owner->m_Scale = Vec3(6, 6, 6);
	m_Owner->m_Rot = MakeQuaternion(Vec3(0, 180, 0));

	cWeapon* a = OBJECT->AddObject<cWeapon>("Canon", Vec3(0.3 * m_Owner->m_Scale.x, 0 * m_Owner->m_Scale.y, 0.5 * m_Owner->m_Scale.z), Obj_Weapon)->GetComponent<cWeapon>();
	a->AddComponent<cMeshRenderer>()->m_Mesh = IMAGE->FindMesh("Canon");
	a->GetComponent<cMeshRenderer>()->m_Bounding = IMAGE->FindMesh("Canon_Bounding")->Mesh;
	a->GetComponent<cMeshRenderer>()->m_isReflected = true;
	a->GetComponent<cMeshRenderer>()->AddShader<cStandardShader>();
	a->m_Enemy = m_Owner;
	a->m_MaxHp = a->m_Hp = 30;
	a->m_OnFire = [](cWeapon* _This)->void {
		_This->m_Owner->SetAlarm("FireAble", 2);
		Quaternion Rot = LookAt(_This->m_Owner->m_Pos, OBJECT->m_Player->m_Pos + RandomVec3() * 10);
		for (int i = 0; i < 10; i++)
		{
			_This->m_Schedules.push_back(new Schedule{ 0.05f * i, [=]()->void {
				cBullet* a = OBJECT->AddObject<cBullet>("EnemyBullet", _This->m_Owner->m_Pos, Obj_EnemyBullet)->GetComponent<cBullet>();
				a->m_Owner->m_Scale = Vec3(5, 5, 5);
				a->AddComponent<cSpriteRenderer>()->m_Sprite = IMAGE->FindTexture("Bullet2_3");
				a->GetComponent<cSpriteRenderer>()->m_Mode = BILL;
				a->GetComponent<cSpriteRenderer>()->m_isReflected = true;
				a->AddComponent<cCollider>()->AddCollider(Vec3(0, 0, 0), 0.25);
				a->m_Speed = 500;
				a->m_Damage = 10;
				a->m_Friction = 0.95;
				a->m_EndSpeed = 150;
				a->m_Owner->m_Rot = Rot;
			} });
		}
	};
	m_Weapons.push_back(a);

	a = OBJECT->AddObject<cWeapon>("Canon", Vec3(-0.3 * m_Owner->m_Scale.x, 0 * m_Owner->m_Scale.y, 0.5 * m_Owner->m_Scale.z), Obj_Weapon)->GetComponent<cWeapon>();
	a->AddComponent<cMeshRenderer>()->m_Mesh = IMAGE->FindMesh("Canon");
	a->GetComponent<cMeshRenderer>()->m_Bounding = IMAGE->FindMesh("Canon_Bounding")->Mesh;
	a->GetComponent<cMeshRenderer>()->m_isReflected = true;
	a->GetComponent<cMeshRenderer>()->AddShader<cStandardShader>();
	a->m_Enemy = m_Owner;
	a->m_MaxHp = a->m_Hp = 30;
	a->m_OnFire = [](cWeapon* _This)->void {
		_This->m_Owner->SetAlarm("FireAble", 2);
		Quaternion Rot = LookAt(_This->m_Owner->m_Pos, OBJECT->m_Player->m_Pos + RandomVec3() * 10);
		for (int i = 0; i < 10; i++)
		{
			_This->m_Schedules.push_back(new Schedule{ 0.05f * i, [=]()->void {
				cBullet* a = OBJECT->AddObject<cBullet>("EnemyBullet", _This->m_Owner->m_Pos, Obj_EnemyBullet)->GetComponent<cBullet>();
				a->m_Owner->m_Scale = Vec3(5, 5, 5);
				a->AddComponent<cSpriteRenderer>()->m_Sprite = IMAGE->FindTexture("Bullet2_3");
				a->GetComponent<cSpriteRenderer>()->m_Mode = BILL;
				a->GetComponent<cSpriteRenderer>()->m_isReflected = true;
				a->AddComponent<cCollider>()->AddCollider(Vec3(0, 0, 0), 0.25);
				a->m_Speed = 500;
				a->m_Damage = 10;
				a->m_Friction = 0.95;
				a->m_EndSpeed = 150;
				a->m_Owner->m_Rot = Rot;
			} });
		}
	};
	m_Weapons.push_back(a);

	a = OBJECT->AddObject<cWeapon>("Torpedo", Vec3(1.4 * m_Owner->m_Scale.x, -0.4 * m_Owner->m_Scale.y, 0.5 * m_Owner->m_Scale.z), Obj_Weapon)->GetComponent<cWeapon>();
	a->AddComponent<cMeshRenderer>()->m_Mesh = IMAGE->FindMesh("MissileLauncher");
	a->GetComponent<cMeshRenderer>()->m_Bounding = IMAGE->FindMesh("MissileLauncher_Bounding")->Mesh;
	a->GetComponent<cMeshRenderer>()->m_isReflected = true;
	a->GetComponent<cMeshRenderer>()->AddShader<cStandardShader>();
	a->m_Enemy = m_Owner;
	a->m_MaxHp = a->m_Hp = 30;
	a->m_OnFire = [](cWeapon* _This)->void {
		_This->m_Owner->SetAlarm("FireAble", 10);
		SOUND->Play("Enemy_Missile_Fire");
		for (int i = -1; i <= 1; i++)
		{
			cObject* a = OBJECT->AddObject<cBullet>("Torpedo", _This->m_Owner->m_Pos, Obj_EnemyBullet);
			a->m_Scale = Vec3(20, 20, 20);
			a->AddComponent<cMeshRenderer>()->m_Mesh = IMAGE->FindMesh("Torpedo");
			a->GetComponent<cMeshRenderer>()->m_isReflected = true;
			a->GetComponent<cBullet>()->m_Damage = 20;
			a->GetComponent<cBullet>()->m_Speed = 300;
			a->AddComponent<cCollider>()->AddCollider(Vec3(0, 1, 0), 0.25);
			a->AddComponent<cLambda>()->m_OnUpate = [](cObject* _This)->void {
				_This->m_Pos.y = Clamp(_This->m_Pos.y - 50 * DT, 1, 100);
			};
			a->GetComponent<cLambda>()->m_OnRelease = [](cObject* _This)->void {
				cObject* a = OBJECT->AddObject<cParticle>("Particle", _This->m_Pos, Obj_Particle);
				a->AddComponent<cSpriteRenderer>()->m_Sprite = IMAGE->FindTexture("Part_Light");
				a->GetComponent<cSpriteRenderer>()->AddShader<c2DShader>()->SetColor(1, 1, 0.9);
				a->GetComponent<cSpriteRenderer>()->m_Mode = BILL;
				a->GetComponent<cSpriteRenderer>()->m_isReflected = true;
				a->GetComponent<cParticle>()->SetAlpha(1, -0.01, 1.2);
				a->m_Scale = Vec3(1, 1, 1) * Random(50.f, 60.f);
				a = OBJECT->AddObject<cParticle>("Particle", _This->m_Pos, Obj_Particle);
				a->AddComponent<cSpriteRenderer>();
				a->GetComponent<cSpriteRenderer>()->m_Mode = BILL;
				a->GetComponent<cSpriteRenderer>()->m_isReflected = true;
				a->AddComponent<cSpriteAnimation>()->AddAnimation("Effect", "fireboom1_", 60);
				a->m_Scale = Vec3(15, 15, 15) * Random(1.f, 1.25f);
				SOUND->Play("Missile_Hit");
			};
			a->SetAlarm("TorpedoWave", 0);
			a->m_Rot = _This->m_Owner->m_Rot * MakeQuaternion(Vec3(0, Random(-10.f, 10.f), 0));
		}
	};
	m_Weapons.push_back(a);
	a = OBJECT->AddObject<cWeapon>("Torpedo", Vec3(-1.4 * m_Owner->m_Scale.x, -0.4 * m_Owner->m_Scale.y, 0.5 * m_Owner->m_Scale.z), Obj_Weapon)->GetComponent<cWeapon>();
	a->AddComponent<cMeshRenderer>()->m_Mesh = IMAGE->FindMesh("MissileLauncher");
	a->GetComponent<cMeshRenderer>()->m_Bounding = IMAGE->FindMesh("MissileLauncher_Bounding")->Mesh;
	a->GetComponent<cMeshRenderer>()->m_isReflected = true;
	a->GetComponent<cMeshRenderer>()->AddShader<cStandardShader>();
	a->m_Enemy = m_Owner;
	a->m_MaxHp = a->m_Hp = 30;
	a->m_OnFire = [](cWeapon* _This)->void {
		_This->m_Owner->SetAlarm("FireAble", 10);
		SOUND->Play("Enemy_Missile_Fire");
		for (int i = -1; i <= 1; i++)
		{
			cObject* a = OBJECT->AddObject<cBullet>("Torpedo", _This->m_Owner->m_Pos, Obj_EnemyBullet);
			a->m_Scale = Vec3(20, 20, 20);
			a->AddComponent<cMeshRenderer>()->m_Mesh = IMAGE->FindMesh("Torpedo");
			a->GetComponent<cMeshRenderer>()->m_isReflected = true;
			a->GetComponent<cBullet>()->m_Damage = 20;
			a->GetComponent<cBullet>()->m_Speed = 300;
			a->AddComponent<cCollider>()->AddCollider(Vec3(0, 1, 0), 0.25);
			a->AddComponent<cLambda>()->m_OnUpate = [](cObject* _This)->void {
				_This->m_Pos.y = Clamp(_This->m_Pos.y - 100 * DT, 1, 100);
			};
			a->GetComponent<cLambda>()->m_OnRelease = [](cObject* _This)->void {
				cObject* a = OBJECT->AddObject<cParticle>("Particle", _This->m_Pos, Obj_Particle);
				a->AddComponent<cSpriteRenderer>()->m_Sprite = IMAGE->FindTexture("Part_Light");
				a->GetComponent<cSpriteRenderer>()->AddShader<c2DShader>()->SetColor(1, 1, 0.9);
				a->GetComponent<cSpriteRenderer>()->m_Mode = BILL;
				a->GetComponent<cSpriteRenderer>()->m_isReflected = true;
				a->GetComponent<cParticle>()->SetAlpha(1, -0.01, 1.2);
				a->m_Scale = Vec3(1, 1, 1) * Random(50.f, 60.f);
				a = OBJECT->AddObject<cParticle>("Particle", _This->m_Pos, Obj_Particle);
				a->AddComponent<cSpriteRenderer>();
				a->GetComponent<cSpriteRenderer>()->m_Mode = BILL;
				a->GetComponent<cSpriteRenderer>()->m_isReflected = true;
				a->AddComponent<cSpriteAnimation>()->AddAnimation("Effect", "fireboom1_", 60);
				a->m_Scale = Vec3(15, 15, 15) * Random(1.f, 1.25f);
				SOUND->Play("Missile_Hit");
			};
			a->SetAlarm("TorpedoWave", 0);
			a->m_Rot = _This->m_Owner->m_Rot * MakeQuaternion(Vec3(0, Random(-10.f, 10.f), 0));
		}
	};
	m_Weapons.push_back(a);
}

void cZako4::Update()
{
	cEnemy::Update();
}

void cZako4::Render()
{
}

void cZako4::Release()
{
	cEnemy::Release();
}

void cZako4::OnCollision(cObject * _Other, Vec3 _CollPos)
{
	cEnemy::OnCollision(_Other, _CollPos);
}

void cZako4::OnAlarm(string _Key)
{
}

void cZako4::OnAnimationEnd(string _Key)
{
}

void cZako4::Death()
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
