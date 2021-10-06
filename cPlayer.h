#pragma once
enum Weapon
{
	CANON,
	MINIGUN,
	TORPEDO,
	MISSILE,
	FOCUS
};
class cPlayer : public cComponent
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

	float m_Hp;
	float m_MaxHp;
	int m_Ammo[4];
	float m_Speed;
	float m_Booster;
	float m_Star;
	float m_Slow;
	float m_FloatVel;
	float m_NoAmmoAlpha = 0;
	float m_ReloadingAlpha = 0;
	bool m_isFireAble;
	bool m_isAttackAble;
	Weapon m_CurWeapon;
	cObject* m_Weapons[2];
	float m_Damaged;
	float m_Focus;
	float m_Support;

	bool GetTarget(Vec3 _RayPos, Vec3 _RayDir, bool _isEnemyOnly, Vec3* _HitPos, cObject** _Target);
	void AttackCanon();
	void AttackMinigun();
	void AttackFocus();
	void AttackTorpedo();
	void AttackMissile();
	void DealDamage(float _Damage);
};

