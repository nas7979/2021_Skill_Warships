#include "DXUT.h"
#include "cTitleScene.h"

void cTitleScene::Init()
{
	m_Text[0] = "START";
	m_Text[1] = "HOW TO PLAY";
	m_Text[2] = "RANKING";
	m_Text[3] = "EXIT";

	memset(m_Color, 0xffffffff, sizeof(m_Color));
	memset(m_Scale, 1, sizeof(m_Scale));
	m_Selected = 0;
	m_isSelected = false;
}

void cTitleScene::Update()
{
	for (int i = 0; i < 4; i++)
	{
		if (i == m_Selected)
		{
			m_Scale[i] = Lerp<float>(m_Scale[i], 1.25, 0.1);
			m_Color[i] = 0xffffffff;
		}
		else
		{
			m_Scale[i] = Lerp<float>(m_Scale[i], 1, 0.1);
			m_Color[i] = 0x90ffffff;
		}
	}
	if (!m_isSelected)
	{

	}
}

void cTitleScene::Render()
{
}

void cTitleScene::Release()
{
}
