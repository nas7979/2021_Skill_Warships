#include "DXUT.h"
#include "cWater.h"
#include "cWaterShader.h"

void cWater::Init()
{
	AddComponent<cMeshRenderer>()->m_Mesh = IMAGE->FindMesh("Water");
	GetComponent<cMeshRenderer>()->AddShader<cWaterShader>();
}

void cWater::Update()
{
}

void cWater::Render()
{
}

void cWater::Release()
{
}

void cWater::OnCollision(cObject * _Other, Vec3 _CollPos)
{
}

void cWater::OnAlarm(string _Key)
{
}

void cWater::OnAnimationEnd(string _Key)
{
}
