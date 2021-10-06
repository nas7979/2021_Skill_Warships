#pragma once
class cComponent
{
public:
	cComponent() {};
	virtual ~cComponent() {};

	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual void Release() = 0;
	virtual void OnCollision(cObject* _Other, Vec3 _CollPos) = 0;
	virtual void OnAlarm(string _Key) = 0;
	virtual void OnAnimationEnd(string _Key) = 0;

	cObject* m_Owner;
	bool m_isEnable = true;

	template<typename T>
	T* AddComponent()
	{
		return m_Owner->AddComponent<T>();
	}
	template<typename T>
	T* GetComponent()
	{
		return m_Owner->GetComponent<T>();
	}
};

