#pragma once
class cWaterShader : public cShader
{
public:
	cWaterShader() {m_Shader = IMAGE->m_Shaders["Water"]; }
	// cShader을(를) 통해 상속됨
	virtual void OnBegin() override;
	virtual void OnEnd() override;
	virtual void OnDrawSubset() override;

	float m_Frame = 0;
	float m_Pos = 0;
};

