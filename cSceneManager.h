#pragma once
#include "cScene.h"
class cSceneManager : public cSingleton<cSceneManager>
{
public:
	void Init();
	void Update();
	void Render();
	void Release();

	map<string, cScene*> m_Scenes;
	cScene* m_Cur = nullptr;
	cScene* m_New = nullptr;
	string m_NextScene = "";

	void ChangeScene(string _Key);
};

#define SCENE cSceneManager::GetInstance()