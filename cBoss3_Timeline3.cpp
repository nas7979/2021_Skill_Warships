#include "DXUT.h"
#include "cBoss3_Timeline3.h"

void cBoss3_Timeline3::OnUpdate()
{
	if (m_Last % 20 == 0)
	{
		Vec3 Pos = RandomVec3(Vec3(-200,0,100),Vec3(200,0,300));
		for (int i = 0; i < 50; i++)
		{
			SCHEDULE->AddSchedule(0.1 * i, [=]()->void {
				FireBullet("Bullet5_1", Pos + Vec3(0, 100, 0), 10, MakeQuaternion(Vec3(90, 0, 0)), 10, 300);
			});
		}
		float Dir = 0;
		for (int i = 0; i < 30; i++)
		{
			Dir += 10;
			SCHEDULE->AddSchedule(2 + i * 0.1, [=]()->void {
				for (int i = 0; i < 10; i++)
				{
					FireBullet("Bullet1_1", Pos, 5, MakeQuaternion(Vec3(0, i * 36 + Dir, 0)), 10, 300, 0.95, 75);
				}
			});
		}
	}
}
