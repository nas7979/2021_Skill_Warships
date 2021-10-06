#pragma once
class cSpriteRenderer : public cComponent
{
public:

	// cComponent을(를) 통해 상속됨
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void Release() override;
	virtual void OnCollision(cObject * _Other, Vec3 _CollPos) override;
	virtual void OnAlarm(string _Key) override;
	virtual void OnAnimationEnd(string _Key) override;

	Texture* m_Sprite;
	D3DCOLOR m_Color = 0xffffffff;
	cShader* m_Shader = nullptr;
	string m_Surface = "Default";
	int m_Order = 0;
	RenderMode m_Mode = WORLD;
	bool m_isReflected = false;

	template <typename T>
	T* AddShader()
	{
		m_Shader = new T;
		m_Shader->m_Owner = m_Owner;
		return static_cast<T*>(m_Shader);
	}

	template <typename T>
	T* GetShader()
	{
		return static_cast<T*>(m_Shader);
	}
};

