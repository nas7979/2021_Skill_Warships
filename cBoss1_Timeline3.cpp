#include "DXUT.h"
#include "cBoss1_Timeline3.h"
#include "cZako4.h"

void cBoss1_Timeline3::OnUpdate()
{
	switch (m_Last)
	{
	case 60:
		m_Time = 0;
		OBJECT->AddObject<cZako4>("Zako4", RandomVec3(Vec3(-200, 30, 750), Vec3(200, 60, 750)), Obj_Enemy)->GetComponent<cEnemy>()->m_Hp = 10;
		break;
	}

	for (int i = 0; i < 25; i++)
	{
		FireBullet("Bullet2_5", Vec3(m_Owner->m_Pos.x, 3, m_Owner->m_Pos.z), 5, MakeQuaternion(Vec3(0, m_Dir + i * 14.4, 0)), 10, 200);
	}
	m_Dir += 2 * m_Way;
	if (round(abs(m_Dir)) == 10)
	{
		m_Way *= -1;
	}
}
