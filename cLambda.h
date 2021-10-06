#pragma once
class cLambda : public cComponent
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

	float m_AlarmTimer = 10000;
	function<void(cObject* _This)> m_OnUpate = nullptr;
	function<void(cObject* _This)> m_OnAlarm = nullptr;
	function<void(cObject* _This)> m_OnRelease = nullptr;
};

