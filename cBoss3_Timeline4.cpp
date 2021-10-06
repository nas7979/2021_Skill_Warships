#include "DXUT.h"
#include "cBoss3_Timeline4.h"

void cBoss3_Timeline4::OnUpdate()
{
	m_Dir -= 3;
	if (m_Last % 4 == 0)
	{
		for (int i = 0; i < 6; i++)
		{
			FireBullet("Bullet2_5", Vec3(100, -0.1, 40), 5, MakeQuaternion(Vec3(0, m_Dir + i * 60, 0)), 10, 50);
			FireBullet("Bullet2_5", Vec3(-100, -0.1, 40), 5, MakeQuaternion(Vec3(0, -m_Dir + i * 60, 0)), 10, 50);
		}
	}

	if (m_Last % 10 == 0)
	{
		Vec3 Pos = RandomVec3(Vec3(-200, 1, 150), Vec3(200, 1, 400));
		float Offset = Random(0.f, 10.f);
		for(int i = 0; i < 36; i++)
		{
			FireBullet("Bullet5_3", Pos, 10, MakeQuaternion(Vec3(0, i * 10 + Offset, 0)), 10, 10, 1.01, 150);
		}
	}

	for (int i = 0; i < 10; i++)
	{
		FireBullet("Bullet1_1", m_Owner->m_Pos, 5, MakeQuaternion(RandomVec3() * 180), 10, Random(100, 200));
	}
}
