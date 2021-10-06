#pragma once
struct Alarm
{
	string Key;
	float Time;
};
enum ObjectTag
{
	Obj_Player,
	Obj_Enemy,
	Obj_PlayerBullet,
	Obj_EnemyBullet,
	Obj_Weapon,
	Obj_Item,
	Obj_Map,
	Obj_Particle,
	Obj_None,
	Obj_End
};
class cComponent;
class cObject
{
public:
	void Init();
	void Update();
	void Render();
	void Release();

	Vec3 m_Pos = Vec3(0, 0, 0);
	Quaternion m_Rot = Quaternion(0, 0, 0, 1);
	Vec3 m_Scale = Vec3(1, 1, 1);
	Matrix m_Mat;
	string m_Name = "";
	ObjectTag m_Tag;
	bool m_isDestroyed = false;
	list<cComponent*> m_Components;
	list<Alarm*> m_Alarms;

	template <typename T>
	T* AddComponent()
	{
		T* a = new T;
		a->m_Owner = this;
		m_Components.push_back(a);
		a->Init();
		return a;
	}
	template <typename T>
	T* GetComponent()
	{
		for (auto& iter : m_Components)
		{
			if (typeid(iter) == typeid(T*))
				return static_cast<T*>(iter);
		}
		T* Cast;
		for (auto& iter : m_Components)
		{
			Cast = dynamic_cast<T*>(iter);
			if (Cast != nullptr)
				return Cast;
		}
		return nullptr;
	}

	void SetAlarm(string _Key, float _Time);

};

