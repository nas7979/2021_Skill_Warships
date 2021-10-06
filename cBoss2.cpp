#include "DXUT.h"
#include "cBoss2.h"
#include "cBoss2_Timeline1.h"
#include "cPlayer.h"

void cBoss2::Init()
{
	OBJECT->m_Boss = m_Owner;
	AddComponent<cMeshRenderer>()->m_Mesh = IMAGE->FindMesh("Boss1");
	GetComponent<cMeshRenderer>()->m_Bounding = IMAGE->FindMesh("Boss1_Bounding")->Mesh;
	GetComponent<cMeshRenderer>()->m_isReflected = false;
	GetComponent<cMeshRenderer>()->AddShader<cStandardShader>();
	AddComponent<cCollider>()->AddCollider(Vec3(0, 0, 0), 1.5);
	GetComponent<cCollider>()->m_CollWith.push_back(Obj_PlayerBullet);
	GetComponent<cCollider>()->m_isEnable = false;

	AddComponent<cBoss2_Timeline1>()->m_isEnable = false;

	m_MaxHp = 3000;
	m_Hp = m_MaxHp;
	m_Speed = 0;
	m_Friction = 1;
	m_Owner->m_Scale = Vec3(20, 20, 20);
	m_Owner->m_Rot = MakeQuaternion(Vec3(0, 180, 0));
	m_Phase = 1;
	m_Owner->SetAlarm("Phase1Wave", 0.05);
}

void cBoss2::Update()
{
	cEnemy::Update();

	if (m_Phase == 1)
	{
		m_Owner->m_Pos.z += 50 * DT;
		if (m_Owner->m_Pos.z > 200)
		{
			m_Phase = 2;
			//CAMERA->m_LookAt = m_Owner;
			m_TargetFriction = 0.9;
			m_TargetSpeed = 500;
			CAMERA->Shake(10, 2);
			OBJECT->AddObject<cWave>("Wave", Vec3(m_Owner->m_Pos.x, 0, m_Owner->m_Pos.z), Obj_Particle)->GetComponent<cWave>()->Set(1, 0.5, 150);
			GetComponent<cMeshRenderer>()->m_isReflected = true;
			SOUND->PlayBGM("BGM_Stage2_1");
		}
	}
	if (m_Phase == 2)
	{
		Vec3 Dir;
		D3DXVec3Normalize(&Dir, &(Vec3(0, 30, 200) - m_Owner->m_Pos));
		m_TargetSpeed *= pow(m_TargetFriction, 60 * DT);
		m_Owner->m_Pos += Dir * m_TargetSpeed * DT;
		if (D3DXVec3Length(&(m_Owner->m_Pos - Vec3(0, 30, 200))) <= m_TargetSpeed * DT)
		{
			m_TargetSpeed = 0;
			m_TargetFriction = 1;
			m_TargetPos = Vec3(0, 0, 0);
			m_Phase = 3;
			SCHEDULE->AddSchedule(2, [&]()->void {
				Start();
				m_Phase = 4;
			});
		}
	}
	if (m_Phase == 4)
	{
		if (m_Hp > 10)
			m_Hp -= 125 * DT;
		else
			OBJECT->m_Player->GetComponent<cPlayer>()->m_isAttackAble = true;
	}
	if (m_Phase != 1)
	{
		if (Random(0.f, 0.01666667f / DT) <= 0.02)
		{
			GAME->Explosion(m_Owner->m_Pos + RandomVec3() * 10, Random(5, 8));
			SOUND->Play("Explosion2_" + to_string(Random(1, 4)));
		}
	}
}

void cBoss2::Render()
{
	if (m_isStarted)
	{
		RECT Rect{ 0,0,(long)((m_Hp / m_MaxHp) * 847.f), 167 };
		IMAGE->RenderUI(IMAGE->FindTexture("UI_Pannel"), Vec2(960, 1050), 0, Vec2(1.5, 0.2), 0.1);
		IMAGE->RenderUI(IMAGE->FindTexture("UI_HpBar_Enemy"), Vec2(960, 1052), 0, Vec2(1.7, 0.2), 0.1, Vec2(0.5, 0.5), 0xffffffff, Rect);
	}
}

void cBoss2::Release()
{
	cEnemy::Release();
	OBJECT->m_Boss = nullptr;
}

void cBoss2::OnCollision(cObject * _Other, Vec3 _CollPos)
{
	cEnemy::OnCollision(_Other, _CollPos);
}

void cBoss2::OnAlarm(string _Key)
{
	if (_Key == "Phase1Wave")
	{
		if (m_Phase != 1)
			return;
		OBJECT->AddObject<cWave>("Wave", Vec3(m_Owner->m_Pos.x, 0, m_Owner->m_Pos.z), Obj_Particle)->GetComponent<cWave>()->Set(m_Owner->m_Pos.z * 0.004, 0.75, 100);
		m_Owner->SetAlarm("Phase1Wave", 0.05);
		return;
	}
}

void cBoss2::OnAnimationEnd(string _Key)
{
}

void cBoss2::Start()
{
	GetComponent<cCollider>()->m_isEnable = true;
	GetComponent<cBoss2_Timeline1>()->Start();
	m_isStarted = true;
}

void cBoss2::Death()
{
	m_isEnable = false;
	m_Owner->m_isDestroyed = false;
	GetComponent<cMeshRenderer>()->GetShader<cStandardShader>()->m_White = 0;
	GetComponent<cCollider>()->m_isEnable = false;
	GetComponent<cBoss2_Timeline1>()->m_isEnable = false;
	OBJECT->m_Player->GetComponent<cPlayer>()->m_isEnable = false;
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
		m_Owner->m_isDestroyed = true;
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
		CAMERA->Fade(0, 255);
	});
	SCHEDULE->AddSchedule(9, []()->void {
		CAMERA->Fade(255, -255);
		CAMERA->m_TargetPos = CAMERA->m_Pos = Vec3(0, 30, -30);
		CAMERA->m_Rot = MakeQuaternion(Vec3(30, 0, 0));
		OBJECT->m_Player->GetComponent<cPlayer>()->m_isEnable = true;
		SOUND->PlayBGM("BGM_Stage2_2");
	});
}
