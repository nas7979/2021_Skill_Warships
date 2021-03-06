#include "DXUT.h"
#include "cMainGame.h"
#include "cLoadingScene.h"
#include "cStage1Scene.h"
#include "cStage2Scene.h"
#include "cRankingScene.h"
#include "cTitleScene.h"

void cMainGame::Init()
{
	IMAGE->CreateInstance();
	SOUND->CreateInstance();
	INPUT->CreateInstance();
	OBJECT->CreateInstance();
	CAMERA->CreateInstance();
	SCHEDULE->CreateInstance();
	SCENE->CreateInstance();
	GAME->CreateInstance();

	SCENE->m_Scenes["LOADING"] = new cLoadingScene;
	SCENE->m_Scenes["STAGE1"] = new cStage1Scene;
	SCENE->m_Scenes["STAGE2"] = new cStage2Scene;
	SCENE->m_Scenes["RANKING"] = new cRankingScene;
	SCENE->m_Scenes["TITLE"] = new cTitleScene;

	SCENE->ChangeScene("LOADING");
	IMAGE->AddSurface("Reflect");
	IMAGE->AddSurface("Wave");
}

void cMainGame::Update()
{
	INPUT->Update();
	GAME->Update();
	if (GAME->m_Stop == false)
	{
		SCENE->Update();
		OBJECT->Update();
		SCHEDULE->Update();
		CAMERA->Update();
	}
	SOUND->Update();
}

void cMainGame::Render()
{
	CAMERA->Render();
	SCENE->Render();
	OBJECT->Render();
	GAME->Render();

	IMAGE->Render();
}

void cMainGame::Release()
{
	GAME->m_Ranking.push_back(Ranking{ "SAD", Random(1000000, 10000000) });
	GAME->SortRanking();

	ofstream File("Ranking.txt");
	int i = 0;
	for (auto& iter : GAME->m_Ranking)
	{
		File << iter.Name;
		File << endl;
		File << iter.Score;
		File << endl;
		i++;
		if (i == 10)
			break;
	}
	File.close();

	INPUT->ReleaseInstance();
	OBJECT->ReleaseInstance();
	CAMERA->ReleaseInstance();
	SCHEDULE->ReleaseInstance();
	SCENE->ReleaseInstance();
	GAME->ReleaseInstance();
	SOUND->ReleaseInstance();
	IMAGE->ReleaseInstance();
}
