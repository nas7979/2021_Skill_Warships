#pragma once
#include "MeshLoader.h"
struct Texture
{
	LPDIRECT3DTEXTURE9 Texture;
	D3DXIMAGE_INFO Info;
};
struct Surface
{
	LPDIRECT3DSURFACE9 Surface;
	LPDIRECT3DTEXTURE9 Texture;
	vector<function<void()>> Queue[100];
};
struct Mesh
{
	LPD3DXMESH Mesh;
	vector<Material*> Materials;
};
enum RenderMode
{
	WORLD,
	BILL
};
class cShader;
class cImageManager : public cSingleton<cImageManager>
{
public:
	void Init();
	void Update();
	void Render();
	void Release();
	void Begin();
	void End();
	void OnLostDevice();
	void OnResetDevice();
	void OnModifyDevice();

	map<string, Mesh*> m_Meshes;
	map<string, Texture*> m_Sprites;
	map<string, vector<Texture*>*> m_SpriteAnimations;
	map<string, Surface*> m_Surfaces;
	map<string, LPD3DXEFFECT> m_Shaders;
	CMeshLoader m_MeshLoader;
	LPD3DXSPRITE m_Sprite;
	LPD3DXFONT m_Font;
	vector<function<void()>> m_SurfaceCreateQueue;
	vector<function<void()>> m_SpriteQueue[100];
	vector<function<void()>> m_UIQueue;
	D3DMATERIAL9 m_Material;
	D3DXPLANE m_WaterPlane = D3DXPLANE{ 0,0,0,-1 };

	void AddTexture(string _Key, string _Path);
	void AddMesh(string _Key, string _Path);
	void AddSpriteAnimation(string _Key);
	void AddSurface(string _Key);
	void AddShader(string _Key, string _Path);

	Texture* FindTexture(string _Key);
	Mesh* FindMesh(string _Key);
	vector<Texture*>* FindSpriteAnimation(string _Key);
	Surface* FindSurface(string _Key);

	void RenderUI(Texture* _Texture, Vec2 _Pos, float _Rot, Vec2 _Scale, float _Depth, Vec2 _Pivot = Vec2(0.5, 0.5), D3DCOLOR _Color = 0xffffffff, RECT _Rect = RECT{ 0,0,0,0 });
	void RenderText(string _Text, Vec2 _Pos, float _Rot, Vec2 _Scale, float _Depth, Vec2 _Pivot = Vec2(0.5, 0.5), D3DCOLOR _Color = 0xffffffff);
	void RenderMesh(Mesh* _Mesh, Matrix _Mat, cShader* _Shader = nullptr, string _Surface = "Default", int _Order = 0);
	void RenderSprite(Texture* _Texture, Matrix _Mat, RenderMode _Mode, cShader* _Shader = nullptr, string _Surface = "Default", int _Order = 0);
};

#define IMAGE cImageManager::GetInstance()