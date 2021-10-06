#pragma once
class cEnemy : public cComponent
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

	float m_MaxHp;
	float m_Hp;
	float m_Speed;
	float m_Friction = 1;
	list<cWeapon*> m_Weapons;

	float TryRayCast(Vec3* _RayPos, Vec3* _RayDir, bool _isEnemyOnly, Vec3* _HitPos, cObject** _Target);
	virtual void Death() = 0;
};

