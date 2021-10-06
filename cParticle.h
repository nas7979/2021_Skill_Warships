#pragma once
class cParticle : public cComponent
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

	Vec3 m_PosVel = Vec3(0, 0, 0);
	Vec3 m_RotVel = Vec3(0, 0, 0);
	Vec3 m_ScaVel = Vec3(0, 0, 0);
	float m_AlpVel = 0;
	
	float m_PosFri = 1;
	float m_RotFri = 1;
	float m_ScaFri = 1;
	float m_AlpFri = 1;
	float m_Gravity = 0;

	void SetPos(Vec3 _Start, Vec3 _Vel, float _Fri);
	void SetRot(Quaternion _Start, Vec3 _Vel, float _Fri);
	void SetScale(Vec3 _Start, Vec3 _Vel, float _Fri);
	void SetAlpha(float _Start, float _Vel, float _Fri);
};

