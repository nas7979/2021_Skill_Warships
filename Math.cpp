#include "DXUT.h"
#include "Math.h"
#include "Math.h"

int Math::Random(int _Min, int _Max)
{
	return _Min + ((rand() | (rand() << 16)) % (_Max - _Min + 1));
}

float Math::Random(float _Min, float _Max)
{
	return Random((int)(_Min * 1000), (int)(_Max * 1000)) * 0.001;
}

Vec3 Math::RandomVec3(Vec3 _Min, Vec3 _Max)
{
	return Vec3(Random(_Min.x, _Max.x), Random(_Min.y, _Max.y), Random(_Min.z, _Max.z));
}

float Math::Sign(float _Val)
{
	if (_Val < 0)
		return -1;
	if (_Val > 0)
		return 1;
	return 0;
}

float Math::Clamp(float _Val, float _Min, float _Max)
{
	return max(_Min, min(_Max, _Val));
}

Matrix Math::MakeMatrix(Vec3 _Pos, Quaternion _Rot, Vec3 _Scale)
{
	Matrix R, S, T;
	D3DXMatrixTranslation(&T, _Pos.x, _Pos.y, _Pos.z);
	D3DXMatrixRotationQuaternion(&R, &_Rot);
	D3DXMatrixScaling(&S, _Scale.x, _Scale.y, _Scale.z);
	return S * R * T;
}

Quaternion Math::MakeQuaternion(Vec3 _Rot)
{
	Quaternion X, Y, Z;
	D3DXQuaternionRotationAxis(&X, &Vec3(1, 0, 0), D3DXToRadian(_Rot.x));
	D3DXQuaternionRotationAxis(&Y, &Vec3(0, 1, 0), D3DXToRadian(_Rot.y));
	D3DXQuaternionRotationAxis(&Z, &Vec3(0, 0, 1), D3DXToRadian(_Rot.z));
	return X * Y * Z;
}

Quaternion Math::MakeLocalQuaternion(Vec3 _Rot, Quaternion _Local)
{
	Quaternion X, Y, Z;
	D3DXQuaternionRotationAxis(&X, &RotateVec3(Vec3(1, 0, 0), _Local), D3DXToRadian(_Rot.x));
	D3DXQuaternionRotationAxis(&Y, &RotateVec3(Vec3(0, 1, 0), _Local), D3DXToRadian(_Rot.y));
	D3DXQuaternionRotationAxis(&Z, &RotateVec3(Vec3(0, 0, 1), _Local), D3DXToRadian(_Rot.z));
	return X * Y * Z;
}

Vec3 Math::RotateVec3(Vec3 _Vec, Quaternion _Rot)
{
	Matrix Mat;
	D3DXMatrixRotationQuaternion(&Mat, &_Rot);
	D3DXVec3TransformNormal(&_Vec, &_Vec, &Mat);
	return _Vec;
}

Vec2 Math::RotateVec2(Vec2 _Vec, float _Rot)
{
	float Rot = D3DXToRadian(_Rot);
	return Vec2(_Vec.x * cos(Rot) + _Vec.y * -sin(Rot), _Vec.x * sin(Rot) + _Vec.y * cos(Rot));
}

Vec3 Math::PointDirection(Vec3 _Vec)
{
	Vec3 Rot;
	D3DXVec3Normalize(&_Vec, &_Vec);
	Rot.x = D3DXToDegree(atan2(_Vec.y, _Vec.x));
	Rot.y = D3DXToDegree(atan2(_Vec.z, sqrt(_Vec.x * _Vec.x + _Vec.y * _Vec.y)));
	Rot.z = 0;
	return Rot;
}

bool Math::RayCast(Vec3 _RayPos, Vec3 _RayDir, LPD3DXMESH _Mesh, Vec3* _Pos, Quaternion* _Rot, Vec3* _Scale, Vec3* _HitPos)
{
	Quaternion InvRot;
	D3DXQuaternionInverse(&InvRot, _Rot);
	Vec3 LocalPos = RotateVec3(_RayPos - *_Pos, InvRot);
	LocalPos.x /= _Scale->x;
	LocalPos.y /= _Scale->y;
	LocalPos.z /= _Scale->z;
	Vec3 Dir = _RayDir;
	Vec3 LocalDir = RotateVec3(Dir, InvRot);
	LocalDir.x /= _Scale->x;
	LocalDir.y /= _Scale->y;
	LocalDir.z /= _Scale->z;
	int isHit = false;
	float Dist;
	D3DXIntersect(_Mesh, &LocalPos, &LocalDir, &isHit, 0, 0, 0, &Dist, 0, 0);
	*_HitPos = _RayPos + Dir * Dist;
	return isHit;
}

Quaternion Math::LookAt(Vec3 _From, Vec3 _At, Vec3 _Up)
{
	Matrix Mat;
	Quaternion Rot;
	D3DXMatrixLookAtLH(&Mat, &_From, &_At, &_Up);
	D3DXMatrixDecompose(&Vec3(), &Rot, &Vec3(), &Mat);
	D3DXQuaternionInverse(&Rot, &Rot);
	return Rot;
}
