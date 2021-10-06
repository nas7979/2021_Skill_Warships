#include "DXUT.h"
#include "cSpriteRenderer.h"

void cSpriteRenderer::Init()
{
}

void cSpriteRenderer::Update()
{
}

void cSpriteRenderer::Render()
{
	m_Order = Clamp(99 - ((30 + m_Owner->m_Pos.z) * 0.05), 0, 99);
	Matrix Mat;
	if (m_Mode == BILL)
	{
		float Z;
		Matrix ZMat;
		D3DXQuaternionToAxisAngle(&m_Owner->m_Rot, &Vec3(0, 0, 1), &Z);
		D3DXMatrixRotationX(&Mat, D3DXToRadian(90));
		D3DXMatrixRotationZ(&ZMat, Z);
		Mat *= ZMat;
		Mat *= CAMERA->m_InvRotMat;
		for (int i = 0; i < 4; i++)
		{
			Mat(0, i) *= m_Owner->m_Scale.x;
			Mat(1, i) *= m_Owner->m_Scale.y;
			Mat(2, i) *= m_Owner->m_Scale.z;
		}
		memcpy(&Mat._41, &m_Owner->m_Pos, sizeof(Vec3));
	}
	else
		Mat = MakeMatrix(m_Owner->m_Pos, m_Owner->m_Rot, m_Owner->m_Scale);
	IMAGE->RenderSprite(m_Sprite, Mat, m_Mode, m_Shader, m_Surface, max(1, m_Order));
	if (m_isReflected && m_Owner->m_Pos.y > 0)
	{
		Matrix Ref = Mat;
		D3DXMatrixReflect(&Ref, &IMAGE->m_WaterPlane);
		IMAGE->RenderSprite(m_Sprite, Mat * Ref, m_Mode, m_Shader, "Reflect", max(1, m_Order));
	}
}

void cSpriteRenderer::Release()
{
	if (m_Shader)
	{
		delete m_Shader;
	}
}

void cSpriteRenderer::OnCollision(cObject * _Other, Vec3 _CollPos)
{
}

void cSpriteRenderer::OnAlarm(string _Key)
{
}

void cSpriteRenderer::OnAnimationEnd(string _Key)
{
}
