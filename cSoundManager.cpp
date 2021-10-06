#include "DXUT.h"
#include "cSoundManager.h"

void cSoundManager::Init()
{
	m_Manager.Initialize(DXUTGetHWND(), 1);
}

void cSoundManager::Update()
{
	DWORD Status;
	for (auto& iter = m_Channels.begin(); iter != m_Channels.end();)
	{
		(*iter)->GetStatus(&Status);
		if ((Status & DSBSTATUS_PLAYING) == 0)
		{
			(*iter)->Release();
			iter = m_Channels.erase(iter);
		}
		else
			iter++;
	}
}

void cSoundManager::Render()
{
}

void cSoundManager::Release()
{
	StopAll();
	SAFE_RELEASE(m_BGM);
	for (auto& iter : m_Sounds)
	{
		delete iter.second;
	}
}

void cSoundManager::AddSound(string _Key, string _Path)
{
	CSound* a;
	m_Manager.Create(&a, (LPWSTR)wstring(_Path.begin(), _Path.end()).c_str(), DSBCAPS_CTRLVOLUME);
	m_Sounds[_Key] = a;
}

LPDIRECTSOUNDBUFFER cSoundManager::Play(string _Key, long _Volume, bool _isLoop)
{
	LPDIRECTSOUNDBUFFER a;
	m_Manager.GetDirectSound()->DuplicateSoundBuffer(m_Sounds[_Key]->GetBuffer(0), &a);
	a->SetVolume(_Volume);
	a->Play(0, 0, _isLoop ? DSBPLAY_LOOPING : 0);
	m_Channels.push_back(a);
	return a;
}

void cSoundManager::StopAll()
{
	for (auto& iter : m_Channels)
	{
		iter->Stop();
		iter->Release();
	}
	m_Channels.clear();
}

void cSoundManager::PlayBGM(string _Key, long _Volume)
{
	SAFE_RELEASE(m_BGM);
	LPDIRECTSOUNDBUFFER a;
	m_Manager.GetDirectSound()->DuplicateSoundBuffer(m_Sounds[_Key]->GetBuffer(0), &a);
	a->SetVolume(_Volume);
	a->Play(0, 0, DSBPLAY_LOOPING );
	m_BGM = a;
}
