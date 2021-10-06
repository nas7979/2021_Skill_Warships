#include "DXUT.h"
#include "cBoss1_Timeline1.h"
#include "cBoss1.h"

void cBoss1_Timeline1::OnUpdate()
{
	switch (m_Last)
	{
	case 10:
		m_Dir = 0;
	case 15:
	case 20:
	case 25:
	case 30:
	case 35:
	case 40:
	case 45:
		for (int i = 0; i < 45; i++)
		{
			for (int j = -4; j <= 4; j++)
			{
				FireBullet("Bullet1_3", m_Owner->m_Pos, 5, MakeQuaternion(Vec3(j * 4, m_Dir + i * 8, 0)), 10, 300, 0.95, 100);
			}
		}
		m_Dir += 4;
		break;

	case 50:
		GetComponent<cBoss1>()->m_TargetPos = RandomVec3(Vec3(-100, 10, 100), Vec3(100, 50, 200));
		GetComponent<cBoss1>()->m_TargetSpeed = 200;
		GetComponent<cBoss1>()->m_TargetFriction = 0.98;
		for (int i = 0; i < 200; i++)
		{
			FireBullet("Bullet2_5", m_Owner->m_Pos, 5, MakeQuaternion(Vec3(Random(-20.f, 20.f), Random(0,359), 0)), 10, 200);
		}
		break;

	case 80:
		m_Time = 0;
		break;
	}
}
