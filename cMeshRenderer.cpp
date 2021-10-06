#include "DXUT.h"
#include "cMeshRenderer.h"

void cMeshRenderer::Init()
{
}

void cMeshRenderer::Update()
{
}

void cMeshRenderer::Render()
{
	Matrix Mat;
	for (int i = m_isMirrored ? -1 : 1; i <= 1; i += 2)
	{
		Mat = MakeMatrix(m_Owner->m_Pos, m_Owner->m_Rot, Vec3(m_Owner->m_Scale.x * i, m_Owner->m_Scale.y, m_Owner->m_Scale.z));
		IMAGE->RenderMesh(m_Mesh, Mat, m_Shader, m_Surface, m_Order);
		if (m_isReflected)
		{
			Matrix Ref = Mat;
			D3DXMatrixReflect(&Ref, &IMAGE->m_WaterPlane);
			IMAGE->RenderMesh(m_Mesh, Mat * Ref, m_Shader, "Reflect", m_Order);
		}
	}
}

void cMeshRenderer::Release()
{
	if (m_Shader)
	{
		delete m_Shader;
	}
}

void cMeshRenderer::OnCollision(cObject * _Other, Vec3 _CollPos)
{
}

void cMeshRenderer::OnAlarm(string _Key)
{
}

void cMeshRenderer::OnAnimationEnd(string _Key)
{
}
