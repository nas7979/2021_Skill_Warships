#include "DXUT.h"
#include "cStage2Scene.h"
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
#include "cBoss2.h"
#include "cBoss3.h"

void cStage2Scene::Init()
{
	CAMERA->m_Pos = Vec3(0, 30, -30);
	CAMERA->m_Rot = MakeQuaternion(Vec3(30, 0, 0));
	GAME->m_Time = 180;
	GAME->m_ScrollSpeed = 0.03;
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
	OBJECT->AddObject<cBoss3>("Boss3", Vec3(0, 10000, 10000), Obj_Enemy);
}

void cStage2Scene::Update()
{
	float StartZ = 750;
	m_Time += DT * 10;
	if ((int)(m_Time) != m_Last)
	{
		m_Last = m_Time;
		switch (m_Last)
		{
		case 30:
			OBJECT->AddObject<cZako2>("Zako2", Vec3(100, 30, StartZ), Obj_Enemy);
			OBJECT->AddObject<cZako2>("Zako2", Vec3(-100, 30, StartZ), Obj_Enemy);
			OBJECT->AddObject<cZako3>("Zako3", Vec3(50, 2, StartZ), Obj_Enemy)->GetComponent<cEnemy>()->m_Speed = 125;
			OBJECT->AddObject<cZako3>("Zako3", Vec3(-50, 2, StartZ), Obj_Enemy)->GetComponent<cEnemy>()->m_Speed = 125;
			break;

		case 50:
			for (int i = -10; i <= 10; i++)
			{
				if (i == 0) continue;
				OBJECT->AddObject<cTrash>("Trash", Vec3(25 * i, 0, StartZ), Obj_Enemy);
			}
			break;

		case 80:
			OBJECT->AddObject<cZako4>("Zako4", Vec3(100, 30, StartZ), Obj_Enemy);
			OBJECT->AddObject<cZako4>("Zako4", Vec3(-100, 30, StartZ), Obj_Enemy);
			OBJECT->AddObject<cZako2>("Zako2", Vec3(0, 30, StartZ), Obj_Enemy);
			OBJECT->AddObject<cZako3>("Zako3", Vec3(50, 2, StartZ), Obj_Enemy)->GetComponent<cEnemy>()->m_Speed = 125;
			OBJECT->AddObject<cZako3>("Zako3", Vec3(-50, 2, StartZ), Obj_Enemy)->GetComponent<cEnemy>()->m_Speed = 125;
			break;

		case 130:
			OBJECT->AddObject<cBoss2>("Boss2", Vec3(0, -30, -99), Obj_Enemy);
			break;

		case 550:
			OBJECT->AddObject<cZako1>("Zako1", Vec3(-120, 2, StartZ), Obj_Enemy);
			OBJECT->AddObject<cZako1>("Zako1", Vec3(-90, 2, StartZ), Obj_Enemy);
			OBJECT->AddObject<cZako1>("Zako1", Vec3(-60, 2, StartZ), Obj_Enemy);
			OBJECT->AddObject<cZako1>("Zako1", Vec3(-30, 2, StartZ), Obj_Enemy);
			OBJECT->AddObject<cZako1>("Zako1", Vec3(0, 2, StartZ), Obj_Enemy);
			OBJECT->AddObject<cZako1>("Zako1", Vec3(30, 2, StartZ), Obj_Enemy);
			OBJECT->AddObject<cZako1>("Zako1", Vec3(60, 2, StartZ), Obj_Enemy);
			OBJECT->AddObject<cZako1>("Zako1", Vec3(90, 2, StartZ), Obj_Enemy);
			OBJECT->AddObject<cZako1>("Zako1", Vec3(120, 2, StartZ), Obj_Enemy);
			break;

		case 600:
		case 630:
		case 660:
		case 690:
		case 720:
		case 750:
		case 780:
		case 810:
		case 840:
		case 870:
		case 900:
			switch (Random(1, 3))
			{
			case 1: OBJECT->AddObject<cZako1>("Zako1", Vec3(Random(-200,200), 2, StartZ), Obj_Enemy); break;
			case 2: OBJECT->AddObject<cZako2>("Zako2", Vec3(Random(-200, 200), Random(20, 40), StartZ), Obj_Enemy); break;
			case 3: OBJECT->AddObject<cZako3>("Zako3", Vec3(Random(-200,200), 2, StartZ), Obj_Enemy); break;
			case 4: OBJECT->AddObject<cZako4>("Zako4", Vec3(Random(-200, 200), Random(20, 40), StartZ), Obj_Enemy); break;
			}
			break;

		case 700:
			OBJECT->AddObject<cZako5>("Zako5", Vec3(-125, 40, StartZ), Obj_Enemy);
			break;
		case 850:
			OBJECT->AddObject<cZako5>("Zako5", Vec3(125, 40, StartZ), Obj_Enemy);
			break;
		}
	}
}

void cStage2Scene::Render()
{
	//IMAGE->RenderText(to_string(DXUTGetFPS()), Vec2(10, 10), 0, Vec2(1, 1), 0.1, Vec2(0, 0));
}

void cStage2Scene::Release()
{
}
