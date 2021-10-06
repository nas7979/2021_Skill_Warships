#include "DXUT.h"
#include "cBoss2_Timeline1.h"

void cBoss2_Timeline1::OnUpdate()
{
	switch (m_Last)
	{
	}

	if (m_Last % 3 == 0)
	{
		for (int i = 0; i < 36; i++)
		{
			for (int j = -2; j <= 2; j++)
			{
				FireBullet("Bullet1_1", m_Owner->m_Pos, 5, MakeQuaternion(Vec3(j * 4, m_Dir + i * 10, 0)), 10, 300, 0.95, 100);
			}
		}
		m_Dir += 2;
	}

	if (m_Last % 8 == 0)
	{
		Quaternion Rot = LookAt(m_Owner->m_Pos, OBJECT->m_Player->m_Pos);
		for (int i = 0; i <= 18; i++)
		{
			FireBullet("Bullet5_3", m_Owner->m_Pos, 10, Rot * MakeQuaternion(Vec3(sin(D3DXToRadian(i * 20)) * 6, cos(D3DXToRadian(i * 20)) * 6, 0)), 15, 800, 0.9, 150);
		}
	}
}
