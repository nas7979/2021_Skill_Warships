#include "DXUT.h"
#include "cBoss1_Timeline2.h"

void cBoss1_Timeline2::OnUpdate()
{
	if (m_Last % 2 == 0)
	{
		Vec3 Pos;
		for (int x = -1; x <= 1; x += 2)
		{
			for (int y = -10; y <= 10; y++)
			{
				Pos = m_Owner->m_Pos + Vec3(0, y * 10, 0);
				FireBullet("Bullet5_1", Pos, 10, LookAt(Vec3(Pos.x, OBJECT->m_Player->m_Pos.y, Pos.z), OBJECT->m_Player->m_Pos + Vec3(25 * x, 0, 0)), 20, 500, 0.95, 100);
			}
		}
	}

	if (m_Last % 5 == 0)
	{
		Vec3 Pos = OBJECT->m_Player->m_Pos + RotateVec3(Vec3(0, 0, 150), MakeQuaternion(Vec3(0, Random(0, 359), 0)));
		Quaternion Dir = LookAt(Pos, OBJECT->m_Player->m_Pos + RandomVec3(Vec3(-10, 0, -10), Vec3(10, 0, 10)));
		for (int i = 0; i < 10; i++)
		{
			SCHEDULE->AddSchedule(0.05 * i, [=]()->void {
				FireBullet("Bullet1_6", Pos, 5, Dir, 10, 100);
			});
		}
	}
}
