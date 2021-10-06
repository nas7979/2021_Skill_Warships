#pragma once
class cObject;
class cCameraManager : public cSingleton<cCameraManager>
{
public:
	void Init();
	void Update();
	void Render();
	void Release();

	Vec3 m_Pos;
	Vec3 m_TargetPos = Vec3(0, 0, 0);
	Quaternion m_Rot;
	float m_ShakeForce = 0;
	float m_ShakeTime = 0;
	Vec3 m_ShakePos;
	Matrix m_ViewMat;
	Matrix m_ProjMat;
	Matrix m_InvRotMat;
	D3DVIEWPORT9 m_ViewPort;
	cObject* m_LookAt = nullptr;
	float m_Fade;
	float m_FadeSpeed;

	void Shake(float _Force, float _Time);
	void Fade(float _Start, float _Speed);
	Vec3 GetRayDir();
};

#define CAMERA cCameraManager::GetInstance()