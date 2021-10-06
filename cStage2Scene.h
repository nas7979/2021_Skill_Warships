#pragma once
class cStage2Scene : public cScene
{
public:

	// cScene을(를) 통해 상속됨
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void Release() override;

	float m_Time = 0;
	int m_Last = 0;
};

