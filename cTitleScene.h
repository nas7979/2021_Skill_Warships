#pragma once
class cTitleScene : public cScene
{
public:

	// cScene을(를) 통해 상속됨
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void Release() override;

	string m_Text[4];
	float m_Scale[4];
	D3DCOLOR m_Color[4];
	int m_Selected;
	bool m_isSelected;
};

