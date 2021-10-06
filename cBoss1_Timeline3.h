#pragma once
class cBoss1_Timeline3 : public cTimeline
{
public:

	// cTimeline을(를) 통해 상속됨
	virtual void OnUpdate() override;

	float m_Way = 1;
};

