#pragma once
class cStandardShader : public cShader
{
public:
	cStandardShader() { m_Shader = IMAGE->m_Shaders["StandardShader"]; }
	// cShader��(��) ���� ��ӵ�
	virtual void OnBegin() override;
	virtual void OnEnd() override;
	virtual void OnDrawSubset() override;

	float m_White = 0;
	Vec3 m_Color = Vec3(1, 1, 1);
};