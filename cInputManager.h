#pragma once
class cInputManager : public cSingleton<cInputManager>
{
public:
	void Init();
	void Update();
	void Render();
	void Release();

	bool m_Cur[128];
	bool m_Old[128];
	Vec2 m_MouseScreenPos;
	Vec2 m_MouseWorldPos;

	bool KeyDown(int _Key) { return m_Cur[_Key] && !m_Old[_Key]; }
	bool KeyPress(int _Key) { return m_Cur[_Key]; }
	bool KeyUp(int _Key) { return !m_Cur[_Key] && m_Old[_Key]; }
	Vec3 GetWorldMousePos(float _Z);
};

#define INPUT cInputManager::GetInstance()