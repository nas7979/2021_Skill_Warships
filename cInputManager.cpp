#include "DXUT.h"
#include "cInputManager.h"

void cInputManager::Init()
{
}

void cInputManager::Update()
{
	memcpy(&m_Old, m_Cur, 128);
	memset(&m_Cur, false, 128);
	for (int i = 0; i < 128; i++)
	{
		if (GetAsyncKeyState(i) & 0x8000)
		{
			m_Cur[i] = true;
		}
	}
	POINT Pt;
	GetCursorPos(&Pt);
	ScreenToClient(DXUTGetHWND(), &Pt);
	m_MouseScreenPos = Vec2(Pt.x, Pt.y);
}

void cInputManager::Render()
{
}

void cInputManager::Release()
{
}

Vec3 cInputManager::GetWorldMousePos(float _Y)
{
	LPDIRECT3DSURFACE9 Original;
	D3DSURFACE_DESC Desc;
	g_Device->GetRenderTarget(0, &Original);
	Original->GetDesc(&Desc);
	float Height = (2 * abs(CAMERA->m_Pos.y - _Y) * tan(D3DX_PI * 0.25));
	float Width = Height * (16.f / 9.f);
	m_MouseWorldPos = Vec2(CAMERA->m_Pos.x + (m_MouseScreenPos.x / (float)Desc.Width - 0.5f) * Width, CAMERA->m_Pos.z - (m_MouseScreenPos.y / (float)Desc.Height - 0.5f) * Height);
	Original->Release();
	return Vec3(m_MouseWorldPos.x, _Y, m_MouseWorldPos.y);
}
