#include "DXUT.h"
#include "c2DShader.h"

void c2DShader::OnBegin()
{
	UINT Num;
	m_Shader->SetMatrix((D3DXHANDLE)"gWVP", &(m_Owner->m_Mat * CAMERA->m_ViewMat * CAMERA->m_ProjMat));
	m_Shader->SetValue((D3DXHANDLE)"gColor", &m_R, sizeof(float) * 4);
	m_Shader->Begin(&Num, 0);
	m_Shader->BeginPass(0);
}

void c2DShader::OnEnd()
{
	m_Shader->EndPass();
	m_Shader->End();
}

void c2DShader::OnDrawSubset()
{
}
