#include "DXUT.h"
#include "cBoss3_Timeline1.h"

void cBoss3_Timeline1::OnUpdate()
{
	if (m_Last % 1 == 0)
	{
		for (int i = -10; i <= 10; i++)
		{
			FireBullet("Bullet2_7", Vec3(i * 35, 2, 500), 3, MakeQuaternion(Vec3(0, 180, 0)), 10, 300);
		}
	}

	if (m_Last % 5 == 0)
	{
		Vec3 Pos = Vec3(m_Owner->m_Pos.x, OBJECT->m_Player->m_Pos.y, m_Owner->m_Pos.z);
		Quaternion Rot = LookAt(Pos, OBJECT->m_Player->m_Pos);
		for (int i = -8; i <= 8; i++)
		{
			FireBullet("Bullet2_2", Pos, 5, Rot * MakeQuaternion(Vec3(Random(-1.f, 1.f), i * 6, 0)), 10, 750, 0.95, 150);
		}
	}
}
