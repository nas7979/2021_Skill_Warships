#pragma once
class cWeapon : public cComponent
{
public:

	// cComponent을(를) 통해 상속됨
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void Release() override;
	virtual void OnCollision(cObject * _Other, Vec3 _CollPos) override;
	virtual void OnAlarm(string _Key) override;
	virtual void OnAnimationEnd(string _Key) override;

	Vec3 m_LocalPos;
	cObject* m_Enemy = nullptr;
	float m_MaxHp;
	float m_Hp;
	bool m_isFireAble = true;
	function<void(cWeapon* _This)> m_OnFire = nullptr;
	list<Schedule*> m_Schedules;
};

