#include "DXUT.h"
#include "cTimeline.h"

void cTimeline::Init()
{
	m_Time = 0;
	m_Last = -1;
}

void cTimeline::Update()
{
	m_Time += DT * 10;
	if ((int)m_Time != m_Last && m_Time > 0)
	{
		m_Last = m_Time;
		OnUpdate();
	}
}

void cTimeline::Render()
{
}

void cTimeline::Release()
{
}

void cTimeline::OnCollision(cObject * _Other, Vec3 _CollPos)
{
}

void cTimeline::OnAlarm(string _Key)
{
}

void cTimeline::OnAnimationEnd(string _Key)
{
}

void cTimeline::Start()
{
	m_isEnable = true;
	m_Time = -30;
	m_Last = -1;
	for (auto& iter : OBJECT->m_Objects[Obj_EnemyBullet])
	{
		iter->m_isDestroyed = true;
	}
}

cObject * cTimeline::FireBullet(string _Sprite, Vec3 _Pos, float _Scale, Quaternion _Rot, float _Damage, float _Speed, float _Friction, float _EndSpeed)
{
	cBullet* a = OBJECT->AddObject<cBullet>("EnemyBullet", _Pos, Obj_EnemyBullet)->GetComponent<cBullet>();
	a->m_Damage = _Damage;
	a->m_Speed = _Speed;
	a->m_Friction = _Friction;
	a->m_EndSpeed = _EndSpeed;
	a->m_Owner->m_Rot = _Rot;
	a->m_Owner->m_Scale = Vec3(1, 1, 1) * _Scale;
	a->AddComponent<cSpriteRenderer>()->m_Sprite = IMAGE->FindTexture(_Sprite);
	a->GetComponent<cSpriteRenderer>()->m_Mode = BILL;
	a->GetComponent<cSpriteRenderer>()->m_isReflected = true;
	a->AddComponent<cCollider>()->AddCollider(Vec3(0, 0, 0), 0.25);
	return a->m_Owner;
}
