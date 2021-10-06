#pragma once
struct Collider
{
	Vec3 LocalPos;
	Vec3 WorldPos;
	float LocalRadius;
	float WorldRadius;
};
class cCollider : public cComponent
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

	vector<Collider*> m_Colliders;
	vector<ObjectTag> m_CollWith;

	void AddCollider(Vec3 LocalPos, float _Radius);
	void UpdateCollider();
};

