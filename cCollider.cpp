#include "DXUT.h"
#include "cCollider.h"

void cCollider::Init()
{
}

void cCollider::Update()
{
	UpdateCollider();
	cCollider* Other;
	for (auto& Tag : m_CollWith)
	{
		for (auto& iter : OBJECT->m_Objects[Tag])
		{
			Other = iter->GetComponent<cCollider>();
			if (Other->m_isEnable == false || iter->m_isDestroyed)
				continue;
			for (auto& SelfColl : m_Colliders)
			{
				for (auto& OtherColl : Other->m_Colliders)
				{
					if (D3DXVec3Length(&(SelfColl->WorldPos - OtherColl->WorldPos)) <= SelfColl->WorldRadius + OtherColl->WorldRadius)
					{
						for (auto& comp : m_Owner->m_Components)
						{
							comp->OnCollision(iter, SelfColl->WorldPos);
						}
						goto Next;
					}
				}
			}
		Next:
			continue;
		}
	}
}

void cCollider::Render()
{
	for (auto& iter : m_Colliders)
	{
		float Z;
		Matrix ZMat, Mat;
		D3DXQuaternionToAxisAngle(&m_Owner->m_Rot, &Vec3(0, 0, 1), &Z);
		D3DXMatrixRotationX(&Mat, D3DXToRadian(90));
		D3DXMatrixRotationZ(&ZMat, Z);
		Mat *= ZMat;
		Mat *= CAMERA->m_InvRotMat;
		for (int i = 0; i < 4; i++)
		{
			Mat(0, i) *= iter->WorldRadius * 2;
			Mat(1, i) *= iter->WorldRadius * 2;
			Mat(2, i) *= iter->WorldRadius * 2;
		}
		memcpy(&Mat._41, &iter->WorldPos, sizeof(Vec3));
		//IMAGE->RenderSprite(IMAGE->FindTexture("Coll"), Mat, BILL, nullptr, "Default", 99);
	}
}

void cCollider::Release()
{
	for (auto& iter : m_Colliders)
	{
		delete iter;
	}
}

void cCollider::OnCollision(cObject * _Other, Vec3 _CollPos)
{
}

void cCollider::OnAlarm(string _Key)
{
}

void cCollider::OnAnimationEnd(string _Key)
{
}

void cCollider::AddCollider(Vec3 _LocalPos, float _Radius)
{
	Collider* a = new Collider;
	a->LocalPos = _LocalPos;
	a->LocalRadius = _Radius;
	a->WorldPos = m_Owner->m_Pos + RotateVec3(_LocalPos, m_Owner->m_Rot);
	a->WorldRadius = a->LocalRadius * m_Owner->m_Scale.x;
	m_Colliders.push_back(a);
}

void cCollider::UpdateCollider()
{
	for (auto& iter : m_Colliders)
	{
		iter->WorldPos = m_Owner->m_Pos + RotateVec3(iter->LocalPos, m_Owner->m_Rot);
		iter->WorldRadius = iter->LocalRadius * m_Owner->m_Scale.x;
	}
}
