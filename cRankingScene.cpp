#include "DXUT.h"
#include "cRankingScene.h"

void cRankingScene::Init()
{
	m_isTyping = true;
	m_Name.clear();
	CAMERA->Fade(255, -255);
	SAFE_RELEASE(SOUND->m_BGM);
}

void cRankingScene::Update()
{
	if (!m_isTyping)
	{
		if (INPUT->KeyDown(VK_RETURN))
		{
			CAMERA->Fade(0, 255);
			SCHEDULE->AddSchedule(1, []()->void {
				SCENE->ChangeScene("TITLE");
			});
		}
	}
	else
	{
		if (m_Name.size() != 3)
		{
			for (int i = 'A'; i <= 'Z'; i++)
			{
				if (INPUT->KeyDown(i))
				{
					m_Name.push_back(i);
					break;
				}
			}
		}
		else
		{
			if (INPUT->KeyDown(VK_RETURN))
			{
				m_isTyping = false;
				GAME->m_Ranking.push_back(Ranking{ m_Name, GAME->m_Score });
				GAME->SortRanking();
			}
		}
		if (m_Name.size() != 0)
		{
			if (INPUT->KeyDown(VK_BACK))
			{
				m_Name.pop_back();
			}
		}
	}
}

void cRankingScene::Render()
{
	IMAGE->RenderText(GAME->m_Win ? "백신을 무사히 일본에 전달하는 데에 성공했습니다!" : "빼앗긴 백신을 되찾는 데에 실패했습니다.", Vec2(960, 50), 0, Vec2(1, 1), 0.1, Vec2(0.5, 0.5), 0xffff0000);
	IMAGE->RenderText("이름을 입력하세요.", Vec2(960, 130), 0, Vec2(0.5, 0.5), 0.1);
	IMAGE->RenderText(m_Name, Vec2(960, 210), 0, Vec2(2, 2), 0.1);
	int i = 0;
	for (auto& iter : GAME->m_Ranking)
	{
		IMAGE->RenderText(iter.Name, Vec2(960 - 500, 320 + i * 70), 0, Vec2(1, 1), 0.1, Vec2(0, 0.5));
		IMAGE->RenderText(to_string(iter.Score), Vec2(960 + 500, 320 + i * 70), 0, Vec2(1, 1), 0.1, Vec2(1, 0.5));
		i++;
		if (i == 10)
			break;
	}
}

void cRankingScene::Release()
{
}
