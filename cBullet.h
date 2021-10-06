#pragma once
class cBullet : public cComponent
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

	float m_Speed;
	float m_Damage;
	float m_Friction = 1;
	float m_EndSpeed = 1000;
	cObject* m_Target = nullptr;
	float m_RotSpeed = 90;
	Vec3 m_Dir = Vec3(0, 0, 0);
};

