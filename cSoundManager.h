#pragma once
class cSoundManager : public cSingleton<cSoundManager>
{
public:
	void Init();
	void Update();
	void Render();
	void Release();

	CSoundManager m_Manager;
	map<string, CSound*> m_Sounds;
	list<LPDIRECTSOUNDBUFFER> m_Channels;
	LPDIRECTSOUNDBUFFER m_BGM = nullptr;

	void AddSound(string _Key, string _Path);
	LPDIRECTSOUNDBUFFER Play(string _Key, long _Volume = -1500, bool _isLoop = false);
	void StopAll();
	void PlayBGM(string _Key, long _Volume = -1000);
};

#define SOUND cSoundManager::GetInstance()