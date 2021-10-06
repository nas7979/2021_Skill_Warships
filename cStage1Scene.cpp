#include "DXUT.h"
#include "cStage1Scene.h"
#include "cPlayer.h"
#include "cWater.h"
#include "cWave.h"
#include "cZako1.h"
#include "cZako2.h"
#include "cZako3.h"
#include "cZako4.h"
#include "cZako5.h"
#include "cTrash.h"
#include "cMine.h"
#include "cBoss1.h"

void cStage1Scene::Init()
{
	CAMERA->m_Pos = Vec3(0, 30, -30);
	CAMERA->m_Rot = MakeQuaternion(Vec3(30, 0, 0));
	GAME->m_ScrollSpeed = 0.03;
	GAME->m_Time = 180;
	CAMERA->Fade(255, -255);
	SOUND->PlayBGM("BGM_Stage1_1");
	OBJECT->AddObject<cPlayer>("Player", Vec3(0, 0, 0), Obj_Player);

	OBJECT->AddObject<cWater>("Water", Vec3(0, 0, 100), Obj_Map)->m_Scale = Vec3(150, 1, 150);
	OBJECT->AddObject<cWater>("Water", Vec3(0, -30, 1500), Obj_Map)->m_Scale = Vec3(3000, 1, 2000);

	cObject * a = OBJECT->AddObject<cMeshRenderer>("SkyBox", Vec3(0, 0, 0), Obj_None);
	a->GetComponent<cMeshRenderer>()->m_Mesh = IMAGE->FindMesh("Skybox");
	a->GetComponent<cMeshRenderer>()->m_isReflected = true;
	a->m_Scale = Vec3(3000, 3000, 3000);

	m_Time = 0;
	m_Last = 0;
}

void cStage1Scene::Update()
{
	float StartZ = 750;
	m_Time += DT * 10;
	if ((int)(m_Time) != m_Last)
	{
		m_Last = m_Time;
		switch (m_Last)
		{
		case 30:
			OBJECT->AddObject<cZako1>("Zako1", Vec3(50, 2, StartZ), Obj_Enemy);
			OBJECT->AddObject<cZako1>("Zako1", Vec3(100, 2, StartZ), Obj_Enemy);
			OBJECT->AddObject<cZako1>("Zako1", Vec3(-50, 2, StartZ), Obj_Enemy);
			OBJECT->AddObject<cZako1>("Zako1", Vec3(-100, 2, StartZ), Obj_Enemy);
			break;

		case 60:
			OBJECT->AddObject<cZako2>("Zako2", Vec3(0, 40, StartZ), Obj_Enemy);
			break;

		case 110:
		case 320:
			OBJECT->AddObject<cZako2>("Zako2", Vec3(75, 50, StartZ), Obj_Enemy);
			OBJECT->AddObject<cZako2>("Zako2", Vec3(-75, 50, StartZ), Obj_Enemy);
			break;

		case 120:
		case 150:
		case 180:
		case 210:
			OBJECT->AddObject<cZako1>("Zako1", Vec3(Random(-200,200), 2, StartZ), Obj_Enemy);
			OBJECT->AddObject<cZako1>("Zako1", Vec3(Random(-200,200), 2, StartZ), Obj_Enemy);
			OBJECT->AddObject<cZako1>("Zako1", Vec3(Random(-200,200), 2, StartZ), Obj_Enemy);
			break;

		case 260:
			OBJECT->m_Player->GetComponent<cPlayer>()->m_isAttackAble = true;
			OBJECT->AddObject<cZako3>("Zako3", Vec3(-150, 2, StartZ), Obj_Enemy);
			OBJECT->AddObject<cZako3>("Zako3", Vec3(150, 2, StartZ), Obj_Enemy);
			break;

		case 350:
		case 390:
		case 430:
		case 470:
		{
			int Offset = Random(-3, 3);
			for (int i = -10; i <= 10; i++)
			{
				if (i == Offset) continue;
				OBJECT->AddObject<cTrash>("Trash", Vec3(25 * i, 0, StartZ), Obj_Enemy);
			}
			break;
		}

		case 400:
		case 450:
		case 500:
			OBJECT->AddObject<cZako3>("Zako3", Vec3(Random(-200, 200), 2, StartZ), Obj_Enemy);
			OBJECT->AddObject<cZako3>("Zako3", Vec3(Random(-200, 200), 2, StartZ), Obj_Enemy);
			break;

		case 630  - 80:
			SOUND->PlayBGM("BGM_Stage1_2");
			CAMERA->Fade(0, 255);
			OBJECT->m_Player->GetComponent<cPlayer>()->m_isEnable = false;
			break;

		case 640 - 80:
			CAMERA->Fade(255, -255);
			OBJECT->AddObject<cBoss1>("Boss", Vec3(0, 30, 750), Obj_Enemy);
			CAMERA->m_LookAt = OBJECT->m_Boss;
			CAMERA->m_Pos = Vec3(0, 40, 750);
			break;

		case 670 - 80:
			CAMERA->Fade(0, 510);
			break;

		case 675 - 80:
			CAMERA->Fade(255, -510);
			CAMERA->m_Pos = Vec3(50, 25, 400);
			break;

		case 690 - 80:
			CAMERA->Fade(0, 510);
			break;

		case 695 - 80:
			CAMERA->Fade(255, -510);
			OBJECT->m_Player->GetComponent<cPlayer>()->m_isEnable = true;
			CAMERA->m_Pos = Vec3(0, 30, 300);
			CAMERA->m_TargetPos = Vec3(0, 30, -30);
			CAMERA->m_LookAt = nullptr;
			OBJECT->m_Boss->GetComponent<cBoss1>()->Start();
			break;
		}
	}
}

void cStage1Scene::Render()
{
	//IMAGE->RenderText(to_string(DXUTGetFPS()), Vec2(10, 10), 0, Vec2(1, 1), 0.1, Vec2(0, 0));
}

void cStage1Scene::Release()
{
}
