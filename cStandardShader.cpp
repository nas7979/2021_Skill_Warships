#include "DXUT.h"
#include "cStandardShader.h"

void cStandardShader::OnBegin()
{
	m_Shader->SetFloat((D3DXHANDLE)"gWhite", m_White);
	m_Shader->SetValue((D3DXHANDLE)"gColor", &m_Color, sizeof(Vec3));
	m_Shader->SetValue((D3DXHANDLE)"gCameraPos", &CAMERA->m_Pos, sizeof(Vec3));
	m_Shader->SetMatrix((D3DXHANDLE)"gW", &m_Owner->m_Mat);
	m_Shader->SetMatrix((D3DXHANDLE)"gVP", &(CAMERA->m_ViewMat * CAMERA->m_ProjMat));

	UINT Num;
	m_Shader->Begin(&Num, 0);
	m_Shader->BeginPass(0);
}

void cStandardShader::OnEnd()
{
	m_Shader->EndPass();
	m_Shader->End();
}

void cStandardShader::OnDrawSubset()
{
}
