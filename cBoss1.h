#pragma once
class cBoss1 : public cEnemy
{
public:
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void Release() override;
	virtual void OnCollision(cObject * _Other, Vec3 _CollPos) override;
	virtual void OnAlarm(string _Key) override;
	virtual void OnAnimationEnd(string _Key) override;

	Vec3 m_TargetPos = Vec3(0, 0, 0);
	bool m_isStarted = false;
	float m_TargetSpeed;
	float m_TargetFriction;
	int m_Phase;
	
	void Start();

	// cEnemy을(를) 통해 상속됨
	virtual void Death() override;
};

