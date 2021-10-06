#pragma once
class c2DShader : public cShader
{
public:
	c2DShader() { m_Shader = IMAGE->m_Shaders["2DShader"]; }
	// cShader을(를) 통해 상속됨
	virtual void OnBegin() override;
	virtual void OnEnd() override;
	virtual void OnDrawSubset() override;

	float m_R = 1;
	float m_G = 1;
	float m_B = 1;
	float m_A = 1;

	void SetColor(float _R, float _G, float _B)
	{
		m_R = _R;
		m_G = _G;
		m_B = _B;
	}
};

