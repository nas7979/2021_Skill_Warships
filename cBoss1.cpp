#include "DXUT.h"
#include "cBoss1.h"
#include "cBoss1_Timeline1.h"
#include "cBoss1_Timeline2.h"
#include "cBoss1_Timeline3.h"
#include "cPlayer.h"

void cBoss1::Init()
{
	OBJECT->m_Boss = m_Owner;
	AddComponent<cMeshRenderer>()->m_Mesh = IMAGE->FindMesh("Boss1");
	GetComponent<cMeshRenderer>()->m_Bounding = IMAGE->FindMesh("Boss1_Bounding")->Mesh;
	GetComponent<cMeshRenderer>()->m_isReflected = true;
	GetComponent<cMeshRenderer>()->AddShader<cStandardShader>();
	AddComponent<cCollider>()->AddCollider(Vec3(0, 0, 0), 1.5);
	GetComponent<cCollider>()->m_CollWith.push_back(Obj_PlayerBullet);
	GetComponent<cCollider>()->m_isEnable = false;

	AddComponent<cBoss1_Timeline1>()->m_isEnable = false;
	AddComponent<cBoss1_Timeline2>()->m_isEnable = false;
	AddComponent<cBoss1_Timeline3>()->m_isEnable = false;

	m_MaxHp = 3000;
	m_Hp = m_MaxHp;
	m_Speed = 150;
	m_Friction = 0.995;
	m_Owner->m_Scale = Vec3(20, 20, 20);
	m_Owner->m_Rot = MakeQuaternion(Vec3(0, 180, 0));
	m_Phase = 1;
}

void cBoss1::Update()
{
	cEnemy::Update();
	GAME->m_ScrollSpeed *= pow(0.995f, 60 * DT);
	if (m_TargetPos != Vec3(0, 0, 0))
	{
		Vec3 Dir;
		D3DXVec3Normalize(&Dir, &(m_TargetPos - m_Owner->m_Pos));
		m_TargetSpeed *= pow(m_TargetFriction, 60 * DT);
		m_Owner->m_Pos += Dir * m_TargetSpeed * DT;
		if (D3DXVec3Length(&(m_Owner->m_Pos - m_TargetPos)) <= m_TargetSpeed * DT)
		{
			m_TargetSpeed = 0;
			m_TargetFriction = 1;
			m_TargetPos = Vec3(0, 0, 0);
		}
	}

	if (m_Phase == 1 && m_Hp <= 2000)
	{
		GetComponent<cBoss1_Timeline1>()->m_isEnable = false;
		GetComponent<cBoss1_Timeline2>()->Start();
		m_Phase++;
		m_TargetPos = Vec3(0, 30, 200);
		m_TargetSpeed = 250;
		m_TargetFriction = 0.99;
		for (int i = 0; i < 5; i++)
		{
			SCHEDULE->AddSchedule(Random(0.f, 0.3f), [=]()->void {
				GAME->Explosion(m_Owner->m_Pos + RandomVec3() * 10, Random(10, 20));
			});
		}
		SOUND->Play("Explosion2_" + to_string(Random(1, 4)));
		CAMERA->Shake(5, 1);
	}
	if (m_Phase == 2 && m_Hp <= 1000)
	{
		GetComponent<cBoss1_Timeline2>()->m_isEnable = false;
		GetComponent<cBoss1_Timeline3>()->Start();
		m_Phase++;
		m_TargetPos = Vec3(0, 10, 200);
		m_TargetSpeed = 200;
		m_TargetFriction = 0.99;
		for (int i = 0; i < 5; i++)
		{
			SCHEDULE->AddSchedule(Random(0.f, 0.3f), [=]()->void {
				GAME->Explosion(m_Owner->m_Pos + RandomVec3() * 10, Random(10, 20));
			});
		}
		SOUND->Play("Explosion2_" + to_string(Random(1, 4)));
		CAMERA->Shake(5, 1);
	}
}

void cBoss1::Render()
{
	if (m_isStarted)
	{
		RECT Rect{ 0,0,(long)((m_Hp / m_MaxHp) * 847.f), 167 };
		IMAGE->RenderUI(IMAGE->FindTexture("UI_Pannel"), Vec2(960, 1050), 0, Vec2(1.5, 0.2), 0.1);
		IMAGE->RenderUI(IMAGE->FindTexture("UI_HpBar_Enemy"), Vec2(960, 1052), 0, Vec2(1.7, 0.2), 0.1, Vec2(0.5,0.5), 0xffffffff, Rect);
	}
}

void cBoss1::Release()
{
	cEnemy::Release();
	OBJECT->m_Boss = nullptr;
}

void cBoss1::OnCollision(cObject * _Other, Vec3 _CollPos)
{
	cEnemy::OnCollision(_Other, _CollPos);
}

void cBoss1::OnAlarm(string _Key)
{
}

void cBoss1::OnAnimationEnd(string _Key)
{
}

void cBoss1::Start()
{
	GetComponent<cCollider>()->m_isEnable = true;
	GetComponent<cBoss1_Timeline1>()->Start();
	m_isStarted = true;
}

void cBoss1::Death()
{
	m_isEnable = false;
	m_Owner->m_isDestroyed = false;
	GetComponent<cMeshRenderer>()->GetShader<cStandardShader>()->m_White = 0;
	GetComponent<cCollider>()->m_isEnable = false;
	GetComponent<cBoss1_Timeline3>()->m_isEnable = false;
	OBJECT->m_Player->GetComponent<cPlayer>()->m_isEnable = false;
	for (auto& iter : OBJECT->m_Objects[Obj_Enemy])
	{
		if (iter != m_Owner)
			iter->GetComponent<cEnemy>()->m_Hp = 0;
	}
	for (auto& iter : OBJECT->m_Objects[Obj_EnemyBullet])
	{
		iter->m_isDestroyed = true;
	}
	for (int i = 0; i < 10; i++)
	{
		SCHEDULE->AddSchedule(Random(0.f, 5.f), [=]()->void {
			GAME->Explosion(m_Owner->m_Pos + RandomVec3() * 10, Random(10.f, 20.f));
			SOUND->Play("Explosion1_" + to_string(Random(1, 4)));
			CAMERA->Shake(1, 0.5);
		});
	}
	SCHEDULE->AddSchedule(1, []()->void {
		CAMERA->Fade(0, 510);
	});
	SCHEDULE->AddSchedule(1.5, [=]()->void {
		CAMERA->Fade(255, -510);
		CAMERA->m_LookAt = m_Owner;
		CAMERA->m_Pos = m_Owner->m_Pos + Vec3(0, 50, -30);
	});
	SCHEDULE->AddSchedule(4, [=]()->void {
		CAMERA->m_TargetPos = m_Owner->m_Pos + Vec3(0, 10, -70);
	});
	SCHEDULE->AddSchedule(5, [&]()->void {
		AddComponent<cLambda>()->m_OnUpate = [](cObject* _This)->void {
			_This->m_Pos.y -= 30 * DT;
		};
		CAMERA->m_Rot = LookAt(CAMERA->m_Pos, m_Owner->m_Pos);
		CAMERA->m_LookAt = nullptr;
		for (int i = 0; i < 10; i++)
		{
			GAME->Explosion(m_Owner->m_Pos + RandomVec3() * 20, Random(15.f, 30.f));
			CAMERA->Shake(2, 3);
		}
		SOUND->Play("Explosion2_" + to_string(Random(1, 4)));
		SOUND->Play("Explosion3_" + to_string(Random(1, 4)));
	});
	SCHEDULE->AddSchedule(8, []()->void {
		CAMERA->Fade(0, 128);
		OBJECT->m_Player->m_Pos = Vec3(0, 3, 50);
		OBJECT->m_Player->m_Rot = MakeQuaternion(Vec3(0, 0, 0));
		OBJECT->AddObject<cLambda>("Lambda", Vec3(0, 0, 0), Obj_None)->GetComponent<cLambda>()->m_OnUpate = [](cObject* _This)->void {
			OBJECT->m_Player->m_Pos.z += 100 * DT;
		};
	});
	SCHEDULE->AddSchedule(10, []()->void {
		SCENE->ChangeScene("STAGE2");
	});
}
