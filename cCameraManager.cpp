#include "DXUT.h"
#include "cCameraManager.h"

void cCameraManager::Init()
{
	D3DXMatrixPerspectiveFovLH(&m_ProjMat, D3DX_PI * 0.5, 16.f / 9.f, 0.1, 10000);
	m_Fade = 0;
	m_FadeSpeed = 0;
}

void cCameraManager::Update()
{
	m_Fade += m_FadeSpeed * NSDT;
	if (m_TargetPos != Vec3(0, 0, 0))
	{
		D3DXVec3Lerp(&m_Pos, &m_Pos, &m_TargetPos, 0.05f * 60 * DT);
		if (D3DXVec3Length(&(m_Pos - m_TargetPos)) <= 1)
		{
			m_Pos = m_TargetPos;
			m_TargetPos = Vec3(0, 0, 0);
		}
	}
	if (m_LookAt == nullptr)
	{
		Matrix R, T;
		Vec3 Pos = m_Pos;
		if (m_ShakeForce != 0)
		{
			m_ShakeForce -= m_ShakeTime * NSDT;
			m_ShakePos = Vec3(Random(-m_ShakeForce, m_ShakeForce), Random(-m_ShakeForce, m_ShakeForce), Random(-m_ShakeForce, m_ShakeForce));
			if (m_ShakeForce <= 0)
			{
				m_ShakeForce = 0;
				m_ShakePos = Vec3(0, 0, 0);
			}
			Pos += m_ShakePos;
		}
		D3DXMatrixRotationQuaternion(&R, &m_Rot);
		D3DXMatrixTranslation(&T, Pos.x, Pos.y, Pos.z);
		m_ViewMat = R * T;
		D3DXMatrixInverse(&m_ViewMat, 0, &m_ViewMat);
	}
	else
	{
		D3DXMatrixLookAtLH(&m_ViewMat, &m_Pos, &m_LookAt->m_Pos, &Vec3(0, 1, 0));
	}

	m_InvRotMat = m_ViewMat;
	memset(&m_InvRotMat._41, 0, sizeof(Vec3));
	D3DXMatrixInverse(&m_InvRotMat, 0, &m_InvRotMat);
}

void cCameraManager::Render()
{
	g_Device->SetTransform(D3DTS_VIEW, &m_ViewMat);
	g_Device->SetTransform(D3DTS_PROJECTION, &m_ProjMat);

	IMAGE->RenderUI(IMAGE->FindTexture("Pixel"), Vec2(0, 0), 0, Vec2(10000, 10000), 0.001, Vec2(0.5, 0.5), D3DCOLOR_ARGB((int)Clamp(m_Fade, 0, 255), 0, 0, 0));
}

void cCameraManager::Release()
{
	m_LookAt = nullptr;
}

void cCameraManager::Shake(float _Force, float _Time)
{
	m_ShakeForce = _Force;
	m_ShakeTime = _Force / _Time;
}

void cCameraManager::Fade(float _Start, float _Speed)
{
	m_Fade = _Start;
	m_FadeSpeed = _Speed;
}

Vec3 cCameraManager::GetRayDir()
{
	g_Device->GetViewport(&m_ViewPort);
	float x = INPUT->m_MouseScreenPos.x * 2.f / m_ViewPort.Width - 1.f;
	float y = INPUT->m_MouseScreenPos.y * -2.f / m_ViewPort.Height + 1.f;
	x /= m_ProjMat(0, 0);
	y /= m_ProjMat(1, 1);

	Vec3 Dir;
	Matrix Mat;
	D3DXVec3Normalize(&Dir, &Vec3(x, y, 1));
	D3DXMatrixInverse(&Mat, 0, &m_ViewMat);
	D3DXVec3TransformNormal(&Dir, &Dir, &Mat);
	return Dir;
}
