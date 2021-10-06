#include "DXUT.h"
#include "cLoadingScene.h"
#include <filesystem>

void cLoadingScene::Init()
{
	IMAGE->AddShader("Water", "./Resources/Shader/Water.fx");
	IMAGE->AddShader("2DShader", "./Resources/Shader/2DShader.fx");
	IMAGE->AddShader("StandardShader", "./Resources/Shader/StandardShader.fx");
	LoadImage();
	LoadMesh();
	LoadSound();

	m_Wait = 0;
}

void cLoadingScene::Update()
{
	m_Wait += 1;
	if(m_Wait == 3)
		SCENE->ChangeScene("STAGE1");
}

void cLoadingScene::Render()
{
}

void cLoadingScene::Release()
{
}

void cLoadingScene::LoadImage()
{
	string Key;
	vector<string> Anim;
	for (auto& iter : experimental::filesystem::recursive_directory_iterator("./Resources/Image/"))
	{
		if (iter.status().type() == experimental::filesystem::file_type::directory)
			continue;
		Key = iter.path().filename().string();
		Key.pop_back();
		Key.pop_back();
		Key.pop_back();
		Key.pop_back();
		IMAGE->AddTexture(Key, iter.path().string());
		if (Key.back() == '1')
		{
			Key.pop_back();
			if (!(Key.back() == '1' || Key.back() == '2' || Key.back() == '3' || Key.back() == '4' || Key.back() == '5' || Key.back() == '6' || Key.back() == '7' || Key.back() == '8' || Key.back() == '9' || Key.back() == '0'))
				Anim.push_back(Key);
		}
	}
	for (auto& iter : Anim)
	{
		IMAGE->AddSpriteAnimation(iter);
	}
}

void cLoadingScene::LoadMesh()
{
	string Key;
	for (auto& iter : experimental::filesystem::recursive_directory_iterator("./Resources/Mesh/"))
	{
		Key = iter.path().filename().string();
		if (Key.find(".obj", 0) != string::npos)
		{
			Key.pop_back();
			Key.pop_back();
			Key.pop_back();
			Key.pop_back();
			IMAGE->AddMesh(Key, iter.path().string());
		}
	}
}

void cLoadingScene::LoadSound()
{
	string Key;
	for (auto& iter : experimental::filesystem::recursive_directory_iterator("./Resources/Sound/"))
	{
		if (iter.status().type() == experimental::filesystem::file_type::directory)
			continue;
		Key = iter.path().filename().string();
		Key.pop_back();
		Key.pop_back();
		Key.pop_back();
		Key.pop_back();
		SOUND->AddSound(Key, iter.path().string());
	}
}
