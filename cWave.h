#pragma once
class cWave : public cComponent
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

	Vec3 m_Velocity = Vec3(0,0,0);
	float m_SizeSpeed = 10;
	float m_AlphaSpeed = 1;

	void Set(float _StartAlpha, float _AlphaSpeed, float _SizeSpeed, bool _WithEffect = true);
};

