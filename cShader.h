#pragma once
class cShader
{
public:
	cShader() {};
	virtual ~cShader() {};

	virtual void OnBegin() = 0;
	virtual void OnEnd() = 0;
	virtual void OnDrawSubset() = 0;

	LPD3DXEFFECT m_Shader;
	cObject* m_Owner;
};

