#include "DXUT.h"
#include "cBoss3_Timeline2.h"

void cBoss3_Timeline2::OnUpdate()
{
	if (m_Last % 5 == 0)
	{
		Vec3 Pos = Vec3(Random(-300, 300), 0, 500);
		Quaternion Dir = LookAt(Pos, Vec3(Random(-200, 200), 0, 0));
		for (int i = 0; i < 30; i++)
		{
			SCHEDULE->AddSchedule(0.05 * i, [=]()->void {
				for (int i = 0; i < 10; i++)
				{
					FireBullet("Bullet1_3", Pos + Vec3(0, 10 * i, 0), 5, Dir, 10, 150);
				}
			});
		}
	}

	if (m_Last % 20 == 0)
	{
		int Offset = Random(-5, 5);
		Quaternion Rot = MakeQuaternion(Vec3(0, 180, 0));
		for (int j = 0; j < 5; j++)
		{
			for (int i = -30; i <= 30; i++)
			{
				if (abs(i - Offset) <= 1)
					continue;
				FireBullet("Bullet5_1", Vec3(i * 10, 5 + j * 12, 500), 10, Rot, 10, 200);
			}
		}
	}
}
