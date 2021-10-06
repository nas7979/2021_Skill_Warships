#pragma once
struct SpriteAnimation
{
	vector<Texture*>* Anim;
	float Speed;
	float MaxFrame;
	string Key;
	float isLoop;
};
class cSpriteAnimation : public cComponent
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

	map<string, SpriteAnimation*> m_Animations;
	SpriteAnimation* m_CurAnim = nullptr;
	float m_Frame;
	int m_Last;
	float m_Speed;

	void AddAnimation(string _Key, string _Resource, float _Speed, bool _isLoop = false);
	void SetAnimation(string _Key);
};

