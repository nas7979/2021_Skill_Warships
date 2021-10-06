#include "DXUT.h"
#include "cImageManager.h"

void cImageManager::Init()
{
	D3DXCreateSprite(g_Device, &m_Sprite);
	D3DXCreateFont(g_Device, 64, 0, 1, 1, 0, DEFAULT_CHARSET, 0, 0, 0, L"±Ã¼­", &m_Font);
	ZeroMemory(&m_Material, sizeof(m_Material));
	m_Material.Ambient = D3DXCOLOR(1, 1, 1, 1);
	m_Material.Diffuse = D3DXCOLOR(1, 1, 1, 1);
	m_Material.Emissive = D3DXCOLOR(1, 1, 1, 1);
	m_Material.Specular = D3DXCOLOR(0, 0, 0, 0);
	m_Material.Power = 1;
	D3DXPlaneFromPointNormal(&m_WaterPlane, &Vec3(0, 0, 0), &Vec3(0, -1, 0));
}

void cImageManager::Update()
{
}

void cImageManager::Render()
{
	g_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	g_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	g_Device->SetRenderState(D3DRS_ALPHAREF, 0);
	g_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_NOTEQUAL);
	g_Device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	for (int i = 0; i < 100; i++)
	{
		for (auto& iter : m_SpriteQueue[i])
		{
			iter();
		}
		m_SpriteQueue[i].clear();
	}
	g_Device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	LPDIRECT3DSURFACE9 Original;
	g_Device->GetRenderTarget(0, &Original);

	for (auto& iter : m_Surfaces)
	{
		g_Device->SetRenderTarget(0, iter.second->Surface);
		if (iter.first == "Wave")
		{
			g_Device->SetRenderState(D3DRS_ZENABLE, false);
			g_Device->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00808080, 1, 0);
		}
		else
		{
			g_Device->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0, 1, 0);
		}
		for (int i = 0; i < 100; i++)
		{
			for (auto& func : iter.second->Queue[i])
			{
				func();
			}
			iter.second->Queue[i].clear();
		}
		g_Device->SetRenderState(D3DRS_ZENABLE, true);
	}
	g_Device->SetRenderTarget(0, Original);
	Original->Release();

	Begin();
	for (auto& iter : m_UIQueue)
	{
		iter();
	}
	m_UIQueue.clear();
	End();
}

void cImageManager::Release()
{
	m_Sprite->Release();
	m_Font->Release();
	for (auto& iter : m_Shaders)
	{
		iter.second->Release();
	}
	for (auto& iter : m_Surfaces)
	{
		iter.second->Surface->Release();
		iter.second->Texture->Release();
		delete iter.second;
	}
	for (auto& iter : m_Sprites)
	{
		iter.second->Texture->Release();
		delete iter.second;
	}
	for (auto& iter : m_SpriteAnimations)
	{
		delete iter.second;
	}
	for (auto& iter : m_Meshes)
	{
		for (auto& tex : iter.second->Materials)
		{
			SAFE_RELEASE(tex->pTexture);
			delete tex;
		}
		iter.second->Mesh->Release();
		delete iter.second;
	}
}

void cImageManager::Begin()
{
	m_Sprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_DEPTH_FRONTTOBACK);
	g_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DX_FILTER_NONE);
	g_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DX_FILTER_NONE);
	g_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DX_FILTER_NONE);
}

void cImageManager::End()
{
	m_Sprite->End();
}

void cImageManager::OnLostDevice()
{
	m_Sprite->OnLostDevice();
	m_Font->OnLostDevice();
	for (auto& iter : m_Shaders)
	{
		iter.second->OnLostDevice();
	}
}

void cImageManager::OnResetDevice()
{
	m_Sprite->OnResetDevice();
	m_Font->OnResetDevice();
	for (auto& iter : m_Shaders)
	{
		iter.second->OnResetDevice();
	}
	for (auto& iter : m_SurfaceCreateQueue)
	{
		iter();
	}
	m_SurfaceCreateQueue.clear();
}

void cImageManager::OnModifyDevice()
{
	for (auto& iter : m_Surfaces)
	{
		m_SurfaceCreateQueue.push_back([=]()->void {
			AddSurface(iter.first);
		});
		iter.second->Surface->Release();
		iter.second->Texture->Release();
		delete iter.second;
	}
}

void cImageManager::AddTexture(string _Key, string _Path)
{
	Texture* Tex = new Texture();
	if (D3DXCreateTextureFromFileExA(g_Device, _Path.c_str(), D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, 1, 0,
		D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_FILTER_POINT, D3DX_FILTER_POINT, 0, &Tex->Info, 0, &Tex->Texture) == S_OK)
	{
		m_Sprites[_Key] = Tex;
		return;
	}
	delete Tex;
}

void cImageManager::AddMesh(string _Key, string _Path)
{
	Mesh* a = new Mesh();
	m_MeshLoader.Create(g_Device, wstring(_Path.begin(), _Path.end()).c_str());
	a->Mesh = m_MeshLoader.GetMesh();
	for(int i = 0; i < m_MeshLoader.GetNumMaterials(); i++)
		a->Materials.push_back(m_MeshLoader.GetMaterial(i));
	m_Meshes[_Key] = a;
}

void cImageManager::AddSpriteAnimation(string _Key)
{
	char Key[64];
	vector<Texture*>* Vec = new vector<Texture*>;
	for (int i = 1; true; i++)
	{
		sprintf(Key, "%s%d", _Key.c_str(), i);
		auto Find = m_Sprites.find(Key);
		if (Find != m_Sprites.end())
		{
			Vec->push_back((*Find).second);
		}
		else
			break;
	}
	m_SpriteAnimations[_Key] = Vec;
}

void cImageManager::AddSurface(string _Key)
{
	D3DSURFACE_DESC Desc;
	LPDIRECT3DSURFACE9 Original;
	g_Device->GetRenderTarget(0, &Original);
	Original->GetDesc(&Desc);
	Surface* a = new Surface;
	g_Device->CreateTexture(Desc.Width, Desc.Height, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &a->Texture, 0);
	a->Texture->GetSurfaceLevel(0, &a->Surface);
	m_Surfaces[_Key] = a;
	Original->Release();
}

void cImageManager::AddShader(string _Key, string _Path)
{
	LPD3DXEFFECT a;
	LPD3DXBUFFER Error;
	D3DXCreateEffectFromFileA(g_Device, _Path.c_str(), 0, 0, D3DXSHADER_DEBUG, 0, &a, &Error);
	if (Error)
	{
		MessageBoxA(DXUTGetHWND(), (LPCSTR)Error->GetBufferPointer(), "ERROR", 0);
		Error->Release();
	}
	m_Shaders[_Key] = a;
}

Texture * cImageManager::FindTexture(string _Key)
{
	return m_Sprites[_Key];
}

Mesh * cImageManager::FindMesh(string _Key)
{
	return m_Meshes[_Key];
}

vector<Texture*>* cImageManager::FindSpriteAnimation(string _Key)
{
	return m_SpriteAnimations[_Key];
}

Surface * cImageManager::FindSurface(string _Key)
{
	return m_Surfaces[_Key];
}

void cImageManager::RenderUI(Texture * _Texture, Vec2 _Pos, float _Rot, Vec2 _Scale, float _Depth, Vec2 _Pivot, D3DCOLOR _Color, RECT _Rect)
{
	m_UIQueue.push_back([=]()->void {
		Vec2 Center = Vec2((float)_Texture->Info.Width * _Pivot.x, (float)_Texture->Info.Height * _Pivot.y);
		Matrix Mat;
		D3DXMatrixTransformation2D(&Mat, &Center, 0, &_Scale, &Center, _Rot, &(_Pos - Center));
		Mat._43 = _Depth;
		m_Sprite->SetTransform(&Mat);
		m_Sprite->Draw(_Texture->Texture, _Rect.bottom == 0 ? nullptr : &_Rect, nullptr, nullptr, _Color);
	});
}

void cImageManager::RenderText(string _Text, Vec2 _Pos, float _Rot, Vec2 _Scale, float _Depth, Vec2 _Pivot, D3DCOLOR _Color)
{
	m_UIQueue.push_back([=]()->void{
		RECT Rect;
		m_Font->DrawTextA(m_Sprite, _Text.c_str(), -1, &Rect, DT_NOCLIP | DT_CENTER | DT_VCENTER | DT_CALCRECT, 0xffffffff);
		Vec2 Center = Vec2((float)(Rect.right - Rect.left) * _Pivot.x, (float)(Rect.bottom - Rect.top) * _Pivot.y);
		Matrix Mat;
		D3DXMatrixTransformation2D(&Mat, &Center, 0, &_Scale, &Center, _Rot, &(_Pos - Center));
		Mat._43 = _Depth;
		m_Sprite->SetTransform(&Mat);
		m_Font->DrawTextA(m_Sprite, _Text.c_str(), -1, 0, DT_NOCLIP | DT_CENTER | DT_VCENTER, _Color);
	});
}

void cImageManager::RenderMesh(Mesh * _Mesh, Matrix _Mat, cShader * _Shader, string _Surface, int _Order)
{
	if (_Surface == "Default")
	{
		g_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		if (_Shader)
		{
			_Shader->m_Owner->m_Mat = _Mat;
			_Shader->OnBegin();
			for (int i = 0; i < _Mesh->Materials.size(); i++)
			{
				_Shader->m_Shader->SetTexture((D3DXHANDLE)"gMainTex", _Mesh->Materials[i]->pTexture);
				_Shader->m_Shader->CommitChanges();
				_Shader->OnDrawSubset();
				_Mesh->Mesh->DrawSubset(i);
			}
			_Shader->OnEnd();
		}
		else
		{
			g_Device->SetTransform(D3DTS_WORLD, &_Mat);
			g_Device->SetMaterial(&m_Material);
			for (int i = 0; i < _Mesh->Materials.size(); i++)
			{
				g_Device->SetTexture(0, _Mesh->Materials[i]->pTexture);
				_Mesh->Mesh->DrawSubset(i);
			}
		}
	}
	else
	{
		m_Surfaces[_Surface]->Queue[_Order].push_back([=]()->void {
			g_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
			if (_Shader)
			{
				_Shader->m_Owner->m_Mat = _Mat;
				_Shader->OnBegin();
				for (int i = 0; i < _Mesh->Materials.size(); i++)
				{
					_Shader->m_Shader->SetTexture((D3DXHANDLE)"gMainTex", _Mesh->Materials[i]->pTexture);
					_Shader->m_Shader->CommitChanges();
					_Shader->OnDrawSubset();
					_Mesh->Mesh->DrawSubset(i);
				}
				_Shader->OnEnd();
			}
			else
			{
				g_Device->SetTransform(D3DTS_WORLD, &_Mat);
				g_Device->SetMaterial(&m_Material);
				for (int i = 0; i < _Mesh->Materials.size(); i++)
				{
					g_Device->SetTexture(0, _Mesh->Materials[i]->pTexture);
					_Mesh->Mesh->DrawSubset(i);
				}
			}
		});
	}
}

void cImageManager::RenderSprite(Texture * _Texture, Matrix _Mat, RenderMode _Mode, cShader * _Shader, string _Surface, int _Order)
{
	Mesh* _Mesh = m_Meshes["Plane"];

	if (_Surface == "Default")
	{
		m_SpriteQueue[_Order].push_back([=]()->void {
			g_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
			if (_Shader)
			{
				g_Device->SetTransform(D3DTS_WORLD, &_Mat);
				_Shader->m_Shader->SetTexture((D3DXHANDLE)"gMainTex", _Texture->Texture);
				_Shader->m_Owner->m_Mat = _Mat;
				_Shader->OnBegin();
				for (int i = 0; i < _Mesh->Materials.size(); i++)
				{
					_Shader->OnDrawSubset();
					_Mesh->Mesh->DrawSubset(i);
				}
				_Shader->OnEnd();
			}
			else
			{
				g_Device->SetTransform(D3DTS_WORLD, &_Mat);
				g_Device->SetMaterial(&m_Material);
				for (int i = 0; i < _Mesh->Materials.size(); i++)
				{
					g_Device->SetTexture(0, _Texture->Texture);
					_Mesh->Mesh->DrawSubset(i);
				}
			}
		});
	}
	else
	{
		m_Surfaces[_Surface]->Queue[_Order].push_back([=]()->void {
			g_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
			g_Device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
			if (_Shader)
			{
				_Shader->m_Shader->SetTexture((D3DXHANDLE)"gMainTex", _Texture->Texture);
				_Shader->m_Owner->m_Mat = _Mat;
				_Shader->OnBegin();
				for (int i = 0; i < _Mesh->Materials.size(); i++)
				{
					_Shader->OnDrawSubset();
					_Mesh->Mesh->DrawSubset(i);
				}
				_Shader->OnEnd();
			}
			else
			{
				g_Device->SetTransform(D3DTS_WORLD, &_Mat);
				g_Device->SetMaterial(&m_Material);
				for (int i = 0; i < _Mesh->Materials.size(); i++)
				{
					g_Device->SetTexture(0, _Texture->Texture);
					_Mesh->Mesh->DrawSubset(i);
				}
			}
			g_Device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
		});
	}
}
