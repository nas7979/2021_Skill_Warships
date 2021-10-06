#pragma once
namespace Math
{
	template <typename T>
	T Lerp(T _S, T _E, float _T)
	{
		return _S + (_E - _S) * _T;
	}
	int Random(int _Min, int _Max);
	float Random(float _Min, float _Max);
	Vec3 RandomVec3(Vec3 _Min = Vec3(-1, -1, -1), Vec3 _Max = Vec3(1, 1, 1));
	float Sign(float _Val);
	float Clamp(float _Val, float _Min, float _Max);
	Matrix MakeMatrix(Vec3 _Pos, Quaternion _Rot, Vec3 _Scale);
	Quaternion MakeQuaternion(Vec3 _Rot);
	Quaternion MakeLocalQuaternion(Vec3 _Rot, Quaternion _Local);
	Vec3 RotateVec3(Vec3 _Vec, Quaternion _Rot);
	Vec2 RotateVec2(Vec2 _Vec, float _Rot);
	Vec3 PointDirection(Vec3 _Vec);
	bool RayCast(Vec3 _RayPos, Vec3 _RayDir, LPD3DXMESH _Mesh, Vec3* _Pos, Quaternion* _Rot, Vec3* _Scale, Vec3* _HitPos);
	Quaternion LookAt(Vec3 _From, Vec3 _At, Vec3 _Up = Vec3(0, 1, 0));
}