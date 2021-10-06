#include "DXUT.h"
#include "cWaterShader.h"

void cWaterShader::OnBegin()
{
	UINT Num;
	m_Frame += DT * 20;
	m_Pos += DT * 20 * (GAME->m_ScrollSpeed / m_Owner->m_Scale.z) * 150;
	while(m_Frame >= 120)
		m_Frame -= 120;
	m_Shader->SetTexture((D3DXHANDLE)"gNormalTex", IMAGE->FindTexture("WaterNormal" + to_string((int)m_Frame + 1))->Texture);
	m_Shader->SetTexture((D3DXHANDLE)"gFoamTex", IMAGE->FindTexture("Water_Foam")->Texture);
	m_Shader->SetTexture((D3DXHANDLE)"gReflectedTex", IMAGE->FindSurface("Reflect")->Texture);
	m_Shader->SetTexture((D3DXHANDLE)"gWaveTex", IMAGE->FindSurface("Wave")->Texture);
	m_Shader->SetFloat((D3DXHANDLE)"gTime", m_Pos);
	m_Shader->SetFloat((D3DXHANDLE)"gScrollSpeed", 1);
	m_Shader->SetMatrix((D3DXHANDLE)"gW", &(m_Owner->m_Mat));
	m_Shader->SetMatrix((D3DXHANDLE)"gVP", &(CAMERA->m_ViewMat * CAMERA->m_ProjMat));
	m_Shader->SetMatrix((D3DXHANDLE)"gWV", &(m_Owner->m_Mat * CAMERA->m_ViewMat));
	m_Shader->SetValue((D3DXHANDLE)"gCameraPos",&CAMERA->m_Pos, sizeof(Vec3));
	m_Shader->Begin(&Num, 0);
	m_Shader->BeginPass(0);
}

void cWaterShader::OnEnd()
{
	m_Shader->EndPass();
	m_Shader->End();
}

void cWaterShader::OnDrawSubset()
{
}
