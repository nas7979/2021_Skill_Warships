#pragma once
#include "cObject.h"
class cObjectManager : public cSingleton<cObjectManager>
{
public:
	void Init();
	void Update();
	void Render();
	void Release();

	list<cObject*> m_Objects[Obj_End];
	cObject* m_Player = nullptr;
	cObject* m_Boss = nullptr;

	template<typename T>
	cObject* AddObject(string _Name, Vec3 _Pos, ObjectTag _Tag)
	{
		cObject* a = new cObject;
		a->m_Name = _Name;
		a->m_Pos = _Pos;
		a->m_Tag = _Tag;
		a->AddComponent<T>();
		m_Objects[_Tag].push_back(a);
		return a;
	}
};

#define OBJECT cObjectManager::GetInstance()