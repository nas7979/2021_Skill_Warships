#pragma once
#include "cTimeline.h"
class cBoss2_Timeline1 :
	public cTimeline
{
public:

	// cTimeline을(를) 통해 상속됨
	virtual void OnUpdate() override;
};

