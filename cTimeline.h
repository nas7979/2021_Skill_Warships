#pragma once
class cTimeline : public cComponent
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

	float m_Time;
	int m_Last;
	float m_Dir = 0;

	virtual void OnUpdate() = 0;
	void Start();

	cObject* FireBullet(string _Sprite, Vec3 _Pos, float _Scale, Quaternion _Rot, float _Damage, float _Speed, float _Friction = 1, float _EndSpeed = 1000);
};

