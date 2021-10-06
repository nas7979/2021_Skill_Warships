#include "DXUT.h"
#include "cSpriteAnimation.h"

void cSpriteAnimation::Init()
{
	m_Frame = 0;
	m_Last = -1;
	m_Speed = 1;
}

void cSpriteAnimation::Update()
{
	m_Frame += DT * m_CurAnim->Speed * m_Speed;
	if (m_Frame >= m_CurAnim->MaxFrame)
	{
		if (m_CurAnim->isLoop)
		{
			m_Frame = 0;
		}
		else
		{
			m_Frame = m_CurAnim->MaxFrame - 1;
			m_Speed = 0;
			for (auto& iter : m_Owner->m_Components)
			{
				iter->OnAnimationEnd(m_CurAnim->Key);
			}
		}
	}
	if ((int)m_Frame != m_Last)
	{
		m_Last = m_Frame;
		GetComponent<cSpriteRenderer>()->m_Sprite = (*m_CurAnim->Anim)[m_Last];
	}
}

void cSpriteAnimation::Render()
{
}

void cSpriteAnimation::Release()
{
	for (auto& iter : m_Animations)
	{
		delete iter.second;
	}
}

void cSpriteAnimation::OnCollision(cObject * _Other, Vec3 _CollPos)
{
}

void cSpriteAnimation::OnAlarm(string _Key)
{
}

void cSpriteAnimation::OnAnimationEnd(string _Key)
{
}

void cSpriteAnimation::AddAnimation(string _Key, string _Resource, float _Speed, bool _isLoop)
{
	SpriteAnimation* a = new SpriteAnimation;
	a->Key = _Key;
	a->Anim = IMAGE->FindSpriteAnimation(_Resource);
	a->Speed = _Speed;
	a->isLoop = _isLoop;
	a->MaxFrame = a->Anim->size();
	m_Animations[_Key] = a;
	m_CurAnim = a;
	GetComponent<cSpriteRenderer>()->m_Sprite = (*m_CurAnim->Anim)[0];
}

void cSpriteAnimation::SetAnimation(string _Key)
{
	m_CurAnim = m_Animations[_Key];
	m_Frame = 0;
	m_Last = -1;
	m_Speed = 1;
}
