#include "DXUT.h"
#include "cPlayer.h"

void cPlayer::Init()
{
	OBJECT->m_Player = m_Owner;
	AddComponent<cMeshRenderer>()->m_Mesh = IMAGE->FindMesh("Player");
	GetComponent<cMeshRenderer>()->m_isReflected = true;
	GetComponent<cMeshRenderer>()->AddShader<cStandardShader>();
	AddComponent<cCollider>()->AddCollider(Vec3(0, 0, 0), 0.5);
	GetComponent<cCollider>()->m_CollWith.push_back(Obj_EnemyBullet);

	m_Owner->m_Scale = Vec3(10, 10, 10);
	m_Speed = 75;
	m_Hp = m_MaxHp = 150;
	m_FloatVel = 0;
	m_Owner->SetAlarm("Wave", 0.05);
	m_isFireAble = true;
	m_Damaged = 0;
	m_CurWeapon = CANON;
	m_isAttackAble = true;
	m_Focus = 0;
	m_Support = 0;
	m_Weapons[0] = OBJECT->AddObject<cMeshRenderer>("Weapon", m_Owner->m_Pos, Obj_None);
	m_Weapons[0]->GetComponent<cMeshRenderer>()->m_Mesh = IMAGE->FindMesh("Canon");
	m_Weapons[0]->m_Scale = m_Owner->m_Scale * 1;
	m_Weapons[1] = OBJECT->AddObject<cMeshRenderer>("Weapon", m_Owner->m_Pos, Obj_None);
	m_Weapons[1]->GetComponent<cMeshRenderer>()->m_Mesh = IMAGE->FindMesh("Canon");
	m_Weapons[1]->m_Scale = m_Owner->m_Scale * 1;

	m_Ammo[0] = 30;
	m_Ammo[1] = 50;
	m_Ammo[2] = 5;
	m_Ammo[3] = 15;
	m_Owner->SetAlarm("Turret", 1);
}

void cPlayer::Update()
{
	if(GAME->m_TimerStop == false)
		GAME->m_Time -= NSDT;
	if (INPUT->KeyPress('W'))
	{
		m_Owner->m_Pos.z += m_Speed * DT;
	}
	if (INPUT->KeyPress('S'))
	{
		m_Owner->m_Pos.z -= m_Speed * DT;
	}
	if (INPUT->KeyPress('A'))
	{
		m_Owner->m_Pos.x -= m_Speed * DT;
	}
	if (INPUT->KeyPress('D'))
	{
		m_Owner->m_Pos.x += m_Speed * DT;
	}

	if (INPUT->KeyDown('1') && m_Ammo[0] != 0 && m_CurWeapon != FOCUS)
	{
		m_CurWeapon = CANON;
		m_Weapons[0]->GetComponent<cMeshRenderer>()->m_Mesh = IMAGE->FindMesh("Canon");
		m_Weapons[1]->GetComponent<cMeshRenderer>()->m_Mesh = IMAGE->FindMesh("Canon");
	}
	if (INPUT->KeyDown('2') && m_Ammo[0] != 0 && m_CurWeapon != FOCUS)
	{
		m_CurWeapon = MINIGUN;
		m_Weapons[0]->GetComponent<cMeshRenderer>()->m_Mesh = IMAGE->FindMesh("Minigun");
		m_Weapons[1]->GetComponent<cMeshRenderer>()->m_Mesh = IMAGE->FindMesh("Minigun");
	}
	if (INPUT->KeyDown('3') && m_Ammo[0] != 0 && m_CurWeapon != FOCUS)
	{
		m_CurWeapon = TORPEDO;
		m_Weapons[0]->GetComponent<cMeshRenderer>()->m_Mesh = IMAGE->FindMesh("MissileLauncher");
		m_Weapons[1]->GetComponent<cMeshRenderer>()->m_Mesh = IMAGE->FindMesh("MissileLauncher");
	}
	if (INPUT->KeyDown('4') && m_Ammo[0] != 0 && m_CurWeapon != FOCUS)
	{
		m_CurWeapon = MISSILE;
		m_Weapons[0]->GetComponent<cMeshRenderer>()->m_Mesh = IMAGE->FindMesh("MissileLauncher");
		m_Weapons[1]->GetComponent<cMeshRenderer>()->m_Mesh = IMAGE->FindMesh("MissileLauncher");
	}
	m_Weapons[0]->m_Pos = m_Owner->m_Pos + RotateVec3(Vec3(6, -1, -4), m_Owner->m_Rot);
	m_Weapons[1]->m_Pos = m_Owner->m_Pos + RotateVec3(Vec3(-6, -1, -4), m_Owner->m_Rot);
	m_Weapons[0]->m_Rot = m_Weapons[1]->m_Rot = m_Owner->m_Rot;

	Vec3 HitPos;
	cObject* Target;
	GetTarget(CAMERA->m_Pos, CAMERA->GetRayDir(), false, &HitPos, &Target);
	m_Owner->m_Rot = LookAt(m_Owner->m_Pos, HitPos, Vec3(0, m_Owner->m_Pos.z >= HitPos.z ? 1 : 1, 0));

	if (INPUT->KeyPress(VK_LBUTTON) && m_isFireAble && m_isAttackAble)
	{
		m_isFireAble = false;
		switch (m_CurWeapon)
		{
		case CANON: AttackCanon(); break;
		case MINIGUN: AttackMinigun(); break;
		case FOCUS: AttackFocus(); break;
		case TORPEDO: AttackTorpedo(); break;
		case MISSILE: AttackMissile(); break;
		}
	}

	if (INPUT->KeyPress(VK_SPACE))
	{
		if (m_Owner->m_Pos.y == 3)
			m_FloatVel = 0;
		m_FloatVel += 150 * DT;
	}
	else if(m_Owner->m_Pos.y > 3)
	{
		if (m_Owner->m_Pos.y == 30)
			m_FloatVel = 0;
		m_FloatVel -= 150 * DT;
	}
	m_Owner->m_Pos.y += m_FloatVel * DT;
	if (m_Owner->m_Pos.y < 3)
	{
		cWave* Wave = OBJECT->AddObject<cWave>("Wave", Vec3(m_Owner->m_Pos.x, 0, m_Owner->m_Pos.z), Obj_Particle)->GetComponent<cWave>();
		Wave->Set(abs(m_FloatVel * 0.01), abs(m_FloatVel * 0.005), abs(m_FloatVel * 0.75));
		Wave->m_Owner->m_Scale = Vec3(10, 10, 10);
		m_Owner->m_Pos.y = 3;
		m_FloatVel = 0;
	}

	if (INPUT->KeyPress('R') && m_Ammo[0] != 0 && m_CurWeapon == 0)
	{
		m_Owner->SetAlarm("Reload", 3);
		SOUND->Play("Canon_Reload");
		m_Ammo[0] = 0;
	}
	if (INPUT->KeyDown('Q') && m_Focus <= 0)
	{
		m_Focus = 20;
		m_Owner->SetAlarm("FocusEnd", 5);
		m_CurWeapon = FOCUS;
		m_Weapons[0]->GetComponent<cMeshRenderer>()->m_Mesh = IMAGE->FindMesh("Minigun");
		m_Weapons[1]->GetComponent<cMeshRenderer>()->m_Mesh = IMAGE->FindMesh("Minigun");
	}
	if (INPUT->KeyDown('E') && m_Support <= 0)
	{
		m_Support = 45;
		for (auto& iter : OBJECT->m_Objects[Obj_EnemyBullet])
		{
			iter->m_isDestroyed = true;
		}
		for (int i = 0; i < 15; i++)
		{
			cObject* a = OBJECT->AddObject<cMeshRenderer>("Support", RandomVec3(Vec3(-200, 30, -1000), Vec3(200, 50, -500)), Obj_None);
			a->m_Scale = Vec3(1, 1, 1) * 10;
			a->GetComponent<cMeshRenderer>()->m_Mesh = IMAGE->FindMesh("Player");
			a->GetComponent<cMeshRenderer>()->AddShader<cStandardShader>();
			a->SetAlarm("", a->AddComponent<cLambda>()->m_AlarmTimer = 0.25);
			a->GetComponent<cLambda>()->m_OnUpate = [](cObject* _This)->void {
				_This->m_Pos.z += 300 * DT;
				_This->m_Pos.y += Clamp(_This->m_Pos.z, 0, 1000) * DT;
				if (_This->m_Pos.y >= 300)
				{
					_This->m_isDestroyed = true;
				}
			};
			a->GetComponent<cLambda>()->m_OnAlarm = [&](cObject* _This)->void {
				SOUND->Play("Missile_Fire" + to_string(Random(1, 2)));
				Vec3 HitPos;
				cObject* Target;
				Reset:
				int Pos = Random(0, OBJECT->m_Objects[Obj_Enemy].size() - 1);
				int i = 0;
				for (auto& iter : OBJECT->m_Objects[Obj_Enemy])
				{
					if (i == Pos)
					{
						if (iter->m_Name == "Trash" || iter->m_Name == "Mine")
						{
							Pos = Random(0, OBJECT->m_Objects[Obj_Enemy].size() - 1);
							i = 0;
							goto Reset;
						}
						Target = iter;
						break;
					}
					i++;
				}
				{
					cObject* a;
					for (int i = -1; i <= 1; i += 2)
					{
						a = OBJECT->AddObject<cBullet>("Missile", _This->m_Pos + RotateVec3(Vec3(7 * i, -1, 1), _This->m_Rot), Obj_PlayerBullet);
						a->m_Scale = Vec3(10, 10, 10);
						a->AddComponent<cMeshRenderer>()->m_Mesh = IMAGE->FindMesh("Missile");
						a->GetComponent<cMeshRenderer>()->m_isReflected = true;
						a->GetComponent<cBullet>()->m_Damage = 10;
						a->GetComponent<cBullet>()->m_Speed = 300;
						a->GetComponent<cBullet>()->m_RotSpeed = 2000;
						a->GetComponent<cBullet>()->m_Target = Target;
						a->AddComponent<cCollider>()->AddCollider(Vec3(0, 0, 0), 0.2);
						a->AddComponent<cLambda>()->m_OnRelease = [](cObject* _This)->void {
							cObject* a = OBJECT->AddObject<cParticle>("Particle", _This->m_Pos, Obj_Particle);
							a->AddComponent<cSpriteRenderer>()->m_Sprite = IMAGE->FindTexture("Part_Light");
							a->GetComponent<cSpriteRenderer>()->AddShader<c2DShader>()->SetColor(1, 1, 0.9);
							a->GetComponent<cSpriteRenderer>()->m_Mode = BILL;
							a->GetComponent<cSpriteRenderer>()->m_isReflected = true;
							a->GetComponent<cParticle>()->SetAlpha(1, -0.01, 1.2);
							a->m_Scale = Vec3(1, 1, 1) * Random(50.f, 60.f);
							a = OBJECT->AddObject<cParticle>("Particle", _This->m_Pos, Obj_Particle);
							a->AddComponent<cSpriteRenderer>();
							a->GetComponent<cSpriteRenderer>()->m_Mode = BILL;
							a->GetComponent<cSpriteRenderer>()->m_isReflected = true;
							a->AddComponent<cSpriteAnimation>()->AddAnimation("Effect", "fireboom1_", 60);
							a->m_Scale = Vec3(15, 15, 15) * Random(1.f, 1.25f);
							SOUND->Play("Missile_Hit");
						};
						a->GetComponent<cLambda>()->m_OnAlarm = [](cObject* _This)->void {
							cObject* a = OBJECT->AddObject<cParticle>("Particle", _This->m_Pos, Obj_Particle);
							a->AddComponent<cSpriteRenderer>();
							a->AddComponent<cSpriteAnimation>()->AddAnimation("Effect", "newsmoke" + to_string(Random(1, 2)) + "_", 60);
							a->GetComponent<cSpriteRenderer>()->m_Mode = BILL;
							a->GetComponent<cSpriteRenderer>()->m_isReflected = true;
							a->m_Scale = Vec3(3, 3, 3) * Random(1.f, 1.25f);
							a->m_Rot = MakeQuaternion(Vec3(Random(0, 359), Random(0, 359), Random(0, 359)));
						};
						a->SetAlarm("", a->GetComponent<cLambda>()->m_AlarmTimer = 0.05);
						a->m_Rot = LookAt(_This->m_Pos, Target->m_Pos) * MakeQuaternion(RandomVec3() * 30);
					}
				}
			};
		}
	}

	m_Owner->m_Pos.x = Clamp(m_Owner->m_Pos.x, -50 - m_Owner->m_Pos.z * 1.77, 50 + m_Owner->m_Pos.z * 1.77);
	m_Owner->m_Pos.y = Clamp(m_Owner->m_Pos.y, 3, 30);
	m_Owner->m_Pos.z = Clamp(m_Owner->m_Pos.z, -10, 150);

	m_NoAmmoAlpha -= 255 * DT;
	m_ReloadingAlpha -= 255 * DT;
	m_Damaged -= 255 * DT;

	m_Booster -= DT;
	m_Slow -= DT;
	m_Star -= DT;
	m_Focus -= DT;
	m_Support -= DT;
	if (m_Booster <= 0 && m_Speed == 125)
	{
		m_Speed = 75;
	}
	if (m_Slow <= 0 && m_Speed == 25)
	{
		m_Speed = 75;
	}
	if (m_Star <= 0 && GetComponent<cCollider>()->m_isEnable == false)
	{
		GetComponent<cCollider>()->m_isEnable = true;
		GetComponent<cMeshRenderer>()->GetShader<cStandardShader>()->m_White = 0;
	}
}

void cPlayer::Render()
{
	IMAGE->RenderUI(IMAGE->FindTexture("UI_Minimap_Front"), Vec2(175, 175), 0, Vec2(0.5, 0.5), 0.09);
	IMAGE->RenderUI(IMAGE->FindTexture("UI_Minimap_Back"), Vec2(175, 175), 0, Vec2(0.5, 0.5), 0.11);
	IMAGE->RenderUI(IMAGE->FindTexture("UI_Minimap_Player"), Vec2(175, 175), 0, Vec2(0.25, 0.25), 0.1);
	Vec2 Pos;
	for (auto& iter : OBJECT->m_Objects[Obj_Enemy])
	{
		Pos = Vec2(-(m_Owner->m_Pos.x - iter->m_Pos.x), m_Owner->m_Pos.z - iter->m_Pos.z);
		if (D3DXVec2Length(&Pos) <= 600)
		{
			IMAGE->RenderUI(IMAGE->FindTexture("UI_Minimap_Enemy"), Vec2(175, 175) + Pos * 0.25, 0, Vec2(0.25, 0.25), 0.1);
		}
	}

	IMAGE->RenderUI(IMAGE->FindTexture("UI_Pannel"), Vec2(275, 20), 0, Vec2(0.5, 0.2), 0.1, Vec2(0, 0));
	RECT Rect = RECT{ 0,0,(long)((m_Hp / m_MaxHp) * 847.f), 167 };
	IMAGE->RenderUI(IMAGE->FindTexture("UI_HpBar_Player"), Vec2(300, 32), 0, Vec2(0.55, 0.2), 0.1, Vec2(0, 0), 0xffffffff, Rect);

	int Offset = m_CurWeapon;
	for (int i = 0; i < 4; i++)
	{
		if (i == 0)
		{
			IMAGE->RenderUI(IMAGE->FindTexture("UI_Weapon" + to_string((m_CurWeapon + i) % 4 + 1)), Vec2(200 + 150 * cos(D3DXToRadian(i * 15)), 200 + 150 * sin(D3DXToRadian(i * 15))), 0, Vec2(0.3, 0.3), 0.1, Vec2(0, 0.5), 0xff303030);
			IMAGE->RenderText(to_string(m_Ammo[(m_CurWeapon + i) % 4]), Vec2(475 + 150 * cos(D3DXToRadian(i * 15)), 200 + 150 * sin(D3DXToRadian(i * 15))), 0, Vec2(0.6, 0.6), 0.1, Vec2(0, 0.5), 0xff303030);
		}
		else
		{
			IMAGE->RenderUI(IMAGE->FindTexture("UI_Weapon" + to_string((m_CurWeapon + i) % 4 + 1)), Vec2(200 + 150 * cos(D3DXToRadian(i * 15)), 200 + 150 * sin(D3DXToRadian(i * 15))), 0, Vec2(0.2 - 0.02 * i, 0.2 - 0.02 * i), 0.1, Vec2(0, 0.5), 0x90303030);
			IMAGE->RenderText(to_string(m_Ammo[(m_CurWeapon + i) % 4]), Vec2(350 + 150 * cos(D3DXToRadian(i * 15)), 200 + 150 * sin(D3DXToRadian(i * 15))), 0, Vec2(0.5 - 0.04 * i, 0.5 - 0.04 * i), 0.1, Vec2(0, 0.5), 0x90303030);
		}
	}

	IMAGE->RenderUI(IMAGE->FindTexture("UI_NoAmmo"), Vec2(960, 540), 0, Vec2(1, 1), 0.01, Vec2(0.5, 0.5), D3DCOLOR_ARGB(max(0, (int)m_NoAmmoAlpha), 255, 255, 255));
	IMAGE->RenderUI(IMAGE->FindTexture("UI_Reloading"), Vec2(960, 540), 0, Vec2(1, 1), 0.01, Vec2(0.5, 0.5), D3DCOLOR_ARGB(max(0, (int)m_ReloadingAlpha), 255, 255, 255));
	IMAGE->RenderUI(IMAGE->FindTexture("UI_Damaged"), Vec2(960, 540), 0, Vec2(1, 1), 0.01, Vec2(0.5, 0.5), D3DCOLOR_ARGB(max(0, (int)m_Damaged), 255, 255, 255));

	IMAGE->RenderUI(IMAGE->FindTexture((m_Ammo[m_CurWeapon] == 0 || !m_isAttackAble) ? "UI_Reticle_Error" : "UI_Reticle"), INPUT->m_MouseScreenPos, 0, Vec2(1, 1), 0.01);

	IMAGE->RenderUI(IMAGE->FindTexture("UI_Focus"), Vec2(1680, 75), 0, Vec2(1.5, 1.5), 0.1);
	if (m_Focus > 0) IMAGE->RenderText(to_string((int)m_Focus + 1), Vec2(1680, 75), 0, Vec2(1.5, 1.5), 0.09);
	IMAGE->RenderUI(IMAGE->FindTexture("UI_Support"), Vec2(1820, 75), 0, Vec2(1.5, 1.5), 0.1);
	if (m_Support > 0) IMAGE->RenderText(to_string((int)m_Support + 1), Vec2(1820, 75), 0, Vec2(1.5, 1.5), 0.09);

	if (m_Slow > 0)
	{
		IMAGE->RenderUI(IMAGE->FindTexture("UI_Status_SpeedUp"), Vec2(400, 125), 0, Vec2(2, 2), 0.1);
		IMAGE->RenderText(to_string((int)m_Slow + 1), Vec2(400 + 64, 125 + 64), 0, Vec2(0.5, 0.5), 0.09, Vec2(1, 1));
	}
	if (m_Booster > 0)
	{
		IMAGE->RenderUI(IMAGE->FindTexture("UI_Status_SpeedDown"), Vec2(550, 125), 0, Vec2(2, 2), 0.1);
		IMAGE->RenderText(to_string((int)m_Booster + 1), Vec2(550 + 64, 125 + 64), 0, Vec2(0.5, 0.5), 0.09, Vec2(1, 1));
	}
	if (m_Focus > 15)
	{
		IMAGE->RenderUI(IMAGE->FindTexture("UI_Focus"), Vec2(700, 125), 0, Vec2(1.5, 1.5), 0.1);
		IMAGE->RenderText(to_string((int)m_Focus - 14), Vec2(700 + 64, 125 + 64), 0, Vec2(0.5, 0.5), 0.09, Vec2(1, 1));
	}
	if (m_Star > 0)
	{
		IMAGE->RenderUI(IMAGE->FindTexture("UI_Status_Star"), Vec2(850, 125), 0, Vec2(2, 2), 0.1);
		IMAGE->RenderText(to_string((int)m_Star + 1), Vec2(850 + 64, 125 + 64), 0, Vec2(0.5, 0.5), 0.09, Vec2(1, 1));
	}

	char Text[16];
	sprintf(Text, "%02d:%02d:%02d", (int)(GAME->m_Time / 60.f), (int)GAME->m_Time % 60, (int)((GAME->m_Time - (int)GAME->m_Time) * 100));
	IMAGE->RenderText(Text, Vec2(960, 50), 0, Vec2(1, 1), 0.1, Vec2(0.5,0.5),0xff000000);
	IMAGE->RenderText("SCORE: " + to_string((int)GAME->m_Score), Vec2(10, 300), 0, Vec2(0.5, 0.5), 0.1, Vec2(0, 0.5), 0xff000000);
}

void cPlayer::Release()
{
	OBJECT->m_Player = nullptr;
}

void cPlayer::OnCollision(cObject * _Other, Vec3 _CollPos)
{
	switch (_Other->m_Tag)
	{
	case Obj_EnemyBullet:
	{
		_Other->m_isDestroyed = true;
		DealDamage(_Other->GetComponent<cBullet>()->m_Damage);
		break;
	}
;	}
}

void cPlayer::OnAlarm(string _Key)
{
	if (_Key == "Wave")
	{
		cWave* Wave = OBJECT->AddObject<cWave>("Wave", Vec3(m_Owner->m_Pos.x, 0, m_Owner->m_Pos.z), Obj_Particle)->GetComponent<cWave>();
		Wave->Set(0.5, 0.5, 40);
		m_Owner->SetAlarm("Wave", 0.05);
		if (m_Slow > 0)
		{
			cObject* a;
			Vec3 Dir;
			Dir = RandomVec3();
			a = OBJECT->AddObject<cParticle>("Particle", m_Owner->m_Pos, Obj_Particle);
			a->AddComponent<cSpriteRenderer>()->m_Sprite = IMAGE->FindTexture("Trash" + to_string(Random(1, 4)));
			a->GetComponent<cSpriteRenderer>()->m_Mode = BILL;
			a->GetComponent<cSpriteRenderer>()->m_isReflected = true;
			a->GetComponent<cSpriteRenderer>()->AddShader<c2DShader>();
			a->GetComponent<cParticle>()->SetPos(m_Owner->m_Pos + Dir * 7.5, Dir * Random(30, 60), 1);
			a->GetComponent<cParticle>()->SetScale(Vec3(1, 1, 1) * Random(4.f, 7.f), Vec3(1, 1, 1) * Random(20.f, 30.f), 1);
			a->GetComponent<cParticle>()->SetRot(MakeQuaternion(Vec3(0, 0, Random(0, 359))), Vec3(0, 0, Random(-90, 90)), 1);
			a->GetComponent<cParticle>()->SetAlpha(1, Random(-1.f, -1.5f), 1.5);
			a->GetComponent<cParticle>()->m_Gravity = a->m_Scale.x * 10;
		}
		return;
	}
	if (_Key == "FireAble")
	{
		m_isFireAble = true;
		return;
	}
	if (_Key == "Reload")
	{
		m_Ammo[0] = 30;
		return;
	}
	if (_Key == "FocusEnd")
	{
		m_CurWeapon = CANON;
		m_Weapons[0]->GetComponent<cMeshRenderer>()->m_Mesh = IMAGE->FindMesh("Canon");
		m_Weapons[1]->GetComponent<cMeshRenderer>()->m_Mesh = IMAGE->FindMesh("Canon");
		return;
	}
	if (_Key == "Turret")
	{
		m_Owner->SetAlarm("Turret", 1.5);
		if (m_isEnable == false || m_isAttackAble == false) return;
		cObject* a = OBJECT->AddObject<cBullet>("Bullet", m_Owner->m_Pos, Obj_PlayerBullet);
		a->AddComponent<cSpriteRenderer>()->m_Sprite = IMAGE->FindTexture("Turret_Bullet");
		a->GetComponent<cSpriteRenderer>()->m_Mode = BILL;
		a->GetComponent<cSpriteRenderer>()->m_isReflected = true;
		a->AddComponent<cCollider>()->AddCollider(Vec3(0, 0, 0), 0.3);
		a->GetComponent<cBullet>()->m_Speed = 400;
		a->GetComponent<cBullet>()->m_Damage = 25;
		a->AddComponent<cLambda>()->m_OnRelease = [](cObject* _This)->void {
			cObject* a = OBJECT->AddObject<cParticle>("Particle", _This->m_Pos, Obj_Particle);
			a->AddComponent<cSpriteRenderer>()->m_Sprite = IMAGE->FindTexture("Part_Light");
			a->GetComponent<cSpriteRenderer>()->AddShader<c2DShader>()->SetColor(1, 1, 0.9);
			a->GetComponent<cSpriteRenderer>()->m_Mode = BILL;
			a->GetComponent<cSpriteRenderer>()->m_isReflected = true;
			a->GetComponent<cParticle>()->SetAlpha(1, -0.01, 1.2);
			a->m_Scale = Vec3(1, 1, 1) * Random(80.f, 100.f);
			a = OBJECT->AddObject<cParticle>("Particle", _This->m_Pos, Obj_Particle);
			a->AddComponent<cSpriteRenderer>();
			a->GetComponent<cSpriteRenderer>()->m_Mode = BILL;
			a->GetComponent<cSpriteRenderer>()->m_isReflected = true;
			a->AddComponent<cSpriteAnimation>()->AddAnimation("Effect", "explosiona1_orange_", 60);
			a->m_Scale = Vec3(20, 20, 20) * Random(1.f, 1.25f);
			SOUND->Play("Missile_Hit");
		};
		a->m_Scale = Vec3(5, 5, 5);
		cObject* Target = nullptr;
		float Dist = 100000;
		float DistTemp;
		for (auto& iter : OBJECT->m_Objects[Obj_Enemy])
		{
			DistTemp = D3DXVec3Length(&(iter->m_Pos - m_Owner->m_Pos));
			if (DistTemp < Dist)
			{
				Dist = DistTemp;
				Target = iter;
			}
		}
		if (Target)
			a->m_Rot = LookAt(m_Owner->m_Pos, Target->m_Pos);
		else
			a->m_Rot = m_Owner->m_Rot;
		SOUND->Play("Turret_Fire" + to_string(Random(1, 3)), -2500);
		return;
	}
}

void cPlayer::OnAnimationEnd(string _Key)
{
}

bool cPlayer::GetTarget(Vec3 _RayPos, Vec3 _RayDir, bool _isEnemyOnly, Vec3 * _HitPos, cObject ** _Target)
{
	Vec3 HitPosTemp;
	float Dist = 10000;
	float DistTemp;
	*_Target = nullptr;
	for (auto& iter : OBJECT->m_Objects[Obj_Enemy])
	{
		if (iter->GetComponent<cCollider>()->m_isEnable == false)
			continue;
		DistTemp = iter->GetComponent<cEnemy>()->TryRayCast(&_RayPos, &_RayDir, _isEnemyOnly, &HitPosTemp, _Target);
		if (DistTemp <= Dist)
		{
			Dist = DistTemp;
			*_HitPos = HitPosTemp;
		}
	}
	if (Dist == 10000)
		RayCast(_RayPos, _RayDir, IMAGE->FindMesh("Skybox")->Mesh, &Vec3(0, 0, 0), &MakeQuaternion(Vec3(0, 0, 0)), &Vec3(2000, 3000, 1000), _HitPos);
	return *_Target != nullptr;
}

void cPlayer::AttackCanon()
{
	m_Owner->SetAlarm("FireAble", 0.1);
	if (m_Ammo[0] == 0)
	{
		m_ReloadingAlpha = 255;
		SOUND->Play("OutofAmmo");
		return;
	}
	SOUND->Play("Canon_Fire");
	Vec3 HitPos;
	cObject* Target;
	m_Ammo[0] -= 1;
	if (m_Ammo[0] == 0)
	{
		m_Owner->SetAlarm("Reload", 3);
		SOUND->Play("Canon_Reload");
	}
	for (int i = -1; i <= 1; i += 2)
	{
		Vec3 Dir = RotateVec3(Vec3(0, 0, 1), m_Owner->m_Rot);
		Dir.x += Random(-0.01f, 0.01f);
		Dir.y += Random(-0.01f, 0.01f);
		D3DXVec3Normalize(&Dir, &Dir);
		if (GetTarget(m_Owner->m_Pos, Dir, false, &HitPos, &Target))
		{
			if (Target->m_Tag == Obj_Enemy)
			{
				Target->GetComponent<cEnemy>()->m_Hp -= 3;
				Target->GetComponent<cMeshRenderer>()->GetShader<cStandardShader>()->m_White = 2;
			}
			else
			{
				Target->GetComponent<cWeapon>()->m_Hp -= 3;
				Target->GetComponent<cMeshRenderer>()->GetShader<cStandardShader>()->m_White = 2;
			}

			for (int i = 0; i < 10; i++)
			{
				cObject* a = OBJECT->AddObject<cParticle>("Particle", HitPos, Obj_Particle);
				a->AddComponent<cSpriteRenderer>()->m_Sprite = IMAGE->FindTexture("Part_Star");
				a->GetComponent<cSpriteRenderer>()->AddShader<c2DShader>()->SetColor(1, 1, 0.9);
				a->GetComponent<cSpriteRenderer>()->m_Mode = BILL;
				a->GetComponent<cParticle>()->SetPos(HitPos, RandomVec3() * 150, 1);
				a->GetComponent<cParticle>()->SetScale(Vec3(1, 1, 1) * Random(5.f, 6.f), Vec3(1, 1, 1) * -25, 1);
				a->m_Rot = MakeQuaternion(Vec3(0, 0, Random(0, 359)));
			}
			cObject* a = OBJECT->AddObject<cParticle>("Particle", HitPos, Obj_Particle);
			a->AddComponent<cSpriteRenderer>()->m_Sprite = IMAGE->FindTexture("Part_Light");
			a->GetComponent<cSpriteRenderer>()->AddShader<c2DShader>()->SetColor(1, 1, 0.9);
			a->GetComponent<cSpriteRenderer>()->m_Mode = BILL;
			a->GetComponent<cSpriteRenderer>()->m_isReflected = true;
			a->GetComponent<cParticle>()->SetAlpha(1, -15, 1);
			a->m_Scale = Vec3(1, 1, 1) * Random(50.f, 70.f);
		}

		cObject* a = OBJECT->AddObject<cSpriteRenderer>("Trail", m_Owner->m_Pos + RotateVec3(Vec3(7 * i, -1, 1), m_Owner->m_Rot) + Dir * 50, Obj_Particle);
		a->GetComponent<cSpriteRenderer>()->m_Sprite = IMAGE->FindTexture("Bullet_Tracer");
		a->GetComponent<cSpriteRenderer>()->AddShader<c2DShader>();
		a->AddComponent<cLambda>()->m_OnUpate = [HitPos](cObject* _This)->void {
			float& Alpha = _This->GetComponent<cSpriteRenderer>()->GetShader<c2DShader>()->m_A;
			Alpha -= 1 * DT;
			if (Alpha <= 0 || D3DXVec3Length(&(HitPos - _This->m_Pos)) <= 1500 * DT)
				_This->m_isDestroyed = true;
			_This->m_Pos += RotateVec3(Vec3(0, 0, 3000 * DT), _This->m_Rot);
		};
		a->m_Scale = Vec3(10, 10, 200);
		a->m_Rot = LookAt(m_Owner->m_Pos, m_Owner->m_Pos + Dir, Vec3(0, m_Owner->m_Pos.z >= HitPos.z ? 1 : 1, 0));
	}
}

void cPlayer::AttackMinigun()
{
	m_Owner->SetAlarm("FireAble", 0.02);
	if (m_Ammo[1] == 0)
	{
		m_NoAmmoAlpha = 255;
		SOUND->Play("OutofAmmo");
		return;
	}
	SOUND->Play("Minigun_Fire");
	Vec3 HitPos;
	cObject* Target;
	m_Ammo[1] -= 1;
	for (int i = -1; i <= 1; i += 2)
	{
		Vec3 Dir = RotateVec3(Vec3(0, 0, 1), m_Owner->m_Rot);
		Dir.x += Random(-0.025f, 0.025f);
		Dir.y += Random(-0.025f, 0.025f);
		D3DXVec3Normalize(&Dir, &Dir);
		if (GetTarget(m_Owner->m_Pos, Dir, false, &HitPos, &Target))
		{
			if (Target->m_Tag == Obj_Enemy)
			{
				Target->GetComponent<cEnemy>()->m_Hp -= 4;
				Target->GetComponent<cMeshRenderer>()->GetShader<cStandardShader>()->m_White = 2;
			}
			else
			{
				Target->GetComponent<cWeapon>()->m_Hp -= 4;
				Target->GetComponent<cMeshRenderer>()->GetShader<cStandardShader>()->m_White = 2;
			}

			for (int i = 0; i < 5; i++)
			{
				cObject* a = OBJECT->AddObject<cParticle>("Particle", HitPos, Obj_Particle);
				a->AddComponent<cSpriteRenderer>()->m_Sprite = IMAGE->FindTexture("Part_Star");
				a->GetComponent<cSpriteRenderer>()->AddShader<c2DShader>()->SetColor(1, 1, 0.9);
				a->GetComponent<cSpriteRenderer>()->m_Mode = BILL;
				a->GetComponent<cParticle>()->SetPos(HitPos, RandomVec3() * 150, 1);
				a->GetComponent<cParticle>()->SetScale(Vec3(1, 1, 1) * Random(5.f, 6.f), Vec3(1, 1, 1) * -25, 1);
				a->m_Rot = MakeQuaternion(Vec3(0, 0, Random(0, 359)));
			}
			cObject* a = OBJECT->AddObject<cParticle>("Particle", HitPos, Obj_Particle);
			a->AddComponent<cSpriteRenderer>()->m_Sprite = IMAGE->FindTexture("Part_Light");
			a->GetComponent<cSpriteRenderer>()->AddShader<c2DShader>()->SetColor(1, 1, 0.9);
			a->GetComponent<cSpriteRenderer>()->m_Mode = BILL;
			a->GetComponent<cSpriteRenderer>()->m_isReflected = true;
			a->GetComponent<cParticle>()->SetAlpha(1, -15, 1);
			a->m_Scale = Vec3(1, 1, 1) * Random(50.f, 70.f);
		}

		cObject* a = OBJECT->AddObject<cSpriteRenderer>("Trail", m_Owner->m_Pos + RotateVec3(Vec3(7 * i, -1, 1), m_Owner->m_Rot) + Dir * 50, Obj_Particle);
		a->GetComponent<cSpriteRenderer>()->m_Sprite = IMAGE->FindTexture("Bullet_Tracer");
		a->GetComponent<cSpriteRenderer>()->AddShader<c2DShader>();
		a->AddComponent<cLambda>()->m_OnUpate = [HitPos](cObject* _This)->void {
			float& Alpha = _This->GetComponent<cSpriteRenderer>()->GetShader<c2DShader>()->m_A;
			Alpha -= 1 * DT;
			if (Alpha <= 0 || D3DXVec3Length(&(HitPos - _This->m_Pos)) <= 1500 * DT)
				_This->m_isDestroyed = true;
			_This->m_Pos += RotateVec3(Vec3(0, 0, 3000 * DT), _This->m_Rot);
		};
		a->m_Scale = Vec3(10, 10, 200);
		a->m_Rot = LookAt(m_Owner->m_Pos, m_Owner->m_Pos + Dir, Vec3(0, m_Owner->m_Pos.z >= HitPos.z ? 1 : 1, 0));
	}
}

void cPlayer::AttackFocus()
{
	m_Owner->SetAlarm("FireAble", 0.01);
	SOUND->Play("Minigun_Fire");
	Vec3 HitPos;
	cObject* Target;
	for (int j = 0; j < 2; j++)
	{
		for (int i = -1; i <= 1; i += 2)
		{
			Vec3 Dir = RotateVec3(Vec3(0, 0, 1), m_Owner->m_Rot);
			Dir.x += Random(-0.01f, 0.01f);
			Dir.y += Random(-0.01f, 0.01f);
			D3DXVec3Normalize(&Dir, &Dir);
			if (GetTarget(m_Owner->m_Pos, Dir, false, &HitPos, &Target))
			{
				if (Target->m_Tag == Obj_Enemy)
				{
					Target->GetComponent<cEnemy>()->m_Hp -= 2;
					Target->GetComponent<cMeshRenderer>()->GetShader<cStandardShader>()->m_White = 2;
				}
				else
				{
					Target->GetComponent<cWeapon>()->m_Hp -= 2;
					Target->GetComponent<cMeshRenderer>()->GetShader<cStandardShader>()->m_White = 2;
				}

				for (int i = 0; i < 5; i++)
				{
					cObject* a = OBJECT->AddObject<cParticle>("Particle", HitPos, Obj_Particle);
					a->AddComponent<cSpriteRenderer>()->m_Sprite = IMAGE->FindTexture("Part_Star");
					a->GetComponent<cSpriteRenderer>()->AddShader<c2DShader>()->SetColor(1, 1, 0.9);
					a->GetComponent<cSpriteRenderer>()->m_Mode = BILL;
					a->GetComponent<cParticle>()->SetPos(HitPos, RandomVec3() * 150, 1);
					a->GetComponent<cParticle>()->SetScale(Vec3(1, 1, 1) * Random(5.f, 6.f), Vec3(1, 1, 1) * -25, 1);
					a->m_Rot = MakeQuaternion(Vec3(0, 0, Random(0, 359)));
				}
				cObject* a = OBJECT->AddObject<cParticle>("Particle", HitPos, Obj_Particle);
				a->AddComponent<cSpriteRenderer>()->m_Sprite = IMAGE->FindTexture("Part_Light");
				a->GetComponent<cSpriteRenderer>()->AddShader<c2DShader>()->SetColor(1, 1, 0.9);
				a->GetComponent<cSpriteRenderer>()->m_Mode = BILL;
				a->GetComponent<cSpriteRenderer>()->m_isReflected = true;
				a->GetComponent<cParticle>()->SetAlpha(1, -15, 1);
				a->m_Scale = Vec3(1, 1, 1) * Random(50.f, 70.f);
			}

			cObject* a = OBJECT->AddObject<cSpriteRenderer>("Trail", m_Owner->m_Pos + RotateVec3(Vec3(7 * i, -1, 1), m_Owner->m_Rot) + Dir * 50, Obj_Particle);
			a->GetComponent<cSpriteRenderer>()->m_Sprite = IMAGE->FindTexture("Bullet_Tracer");
			a->GetComponent<cSpriteRenderer>()->AddShader<c2DShader>();
			a->AddComponent<cLambda>()->m_OnUpate = [HitPos](cObject* _This)->void {
				float& Alpha = _This->GetComponent<cSpriteRenderer>()->GetShader<c2DShader>()->m_A;
				Alpha -= 1 * DT;
				if (Alpha <= 0 || D3DXVec3Length(&(HitPos - _This->m_Pos)) <= 1500 * DT)
					_This->m_isDestroyed = true;
				_This->m_Pos += RotateVec3(Vec3(0, 0, 3000 * DT), _This->m_Rot);
			};
			a->m_Scale = Vec3(10, 10, 200);
			a->m_Rot = LookAt(m_Owner->m_Pos, m_Owner->m_Pos + Dir, Vec3(0, m_Owner->m_Pos.z >= HitPos.z ? 1 : 1, 0));
		}
	}
}

void cPlayer::AttackTorpedo()
{
	m_Owner->SetAlarm("FireAble", 1);
	if (m_Ammo[2] == 0)
	{
		m_NoAmmoAlpha = 255;
		SOUND->Play("OutofAmmo");
		return;
	}
	SOUND->Play("Torpedo_Fire");
	Vec3 HitPos;
	cObject* Target;
	m_Ammo[2] -= 1;
	if (GetTarget(CAMERA->m_Pos, CAMERA->GetRayDir(), true, &HitPos, &Target))
	{
		cObject* a = OBJECT->AddObject<cBullet>("Torpedo", Vec3(m_Owner->m_Pos.x, 0, m_Owner->m_Pos.z), Obj_PlayerBullet);
		a->m_Scale = Vec3(10, 10, 10);
		a->AddComponent<cMeshRenderer>()->m_Mesh = IMAGE->FindMesh("Torpedo");
		a->GetComponent<cMeshRenderer>()->m_Surface = "Reflect";
		a->GetComponent<cBullet>()->m_Damage = 100;
		a->GetComponent<cBullet>()->m_Speed = 100;
		a->GetComponent<cBullet>()->m_Target = Target;
		a->AddComponent<cCollider>()->AddCollider(Vec3(0, 1, 0), 0.2);
		a->AddComponent<cLambda>()->m_OnUpate = [](cObject* _This)->void {
			_This->m_Pos.y = 0;
		};
		a->GetComponent<cLambda>()->m_OnRelease = [](cObject* _This)->void {
			OBJECT->AddObject<cWave>("Wave", Vec3(_This->m_Pos.x, 0, _This->m_Pos.z), Obj_Particle)->GetComponent<cWave>()->Set(1.5, 1.5, 150);
			cObject* a = OBJECT->AddObject<cParticle>("Particle", Vec3(_This->m_Pos.x, -2, _This->m_Pos.z), Obj_Particle);
			a->AddComponent<cSpriteRenderer>()->m_Sprite = IMAGE->FindTexture("Part_Light");
			a->GetComponent<cSpriteRenderer>()->AddShader<c2DShader>()->SetColor(1, 1, 0.9);
			a->GetComponent<cSpriteRenderer>()->m_Mode = BILL;
			a->GetComponent<cSpriteRenderer>()->m_Surface = "Reflect";
			a->GetComponent<cParticle>()->SetAlpha(3, -1, 1.1);
			a->m_Scale = Vec3(1, 1, 1) * Random(70.f, 100.f);
			SOUND->Play("Torpedo_Hit");
		};
		a->SetAlarm("TorpedoWave", 0);
		a->m_Rot = m_Owner->m_Rot;
	}
}

void cPlayer::AttackMissile()
{
	m_Owner->SetAlarm("FireAble", 0.1);
	if (m_Ammo[3] == 0)
	{
		m_NoAmmoAlpha = 255;
		SOUND->Play("OutofAmmo");
		return;
	}
	SOUND->Play("Missile_Fire" + to_string(Random(1, 2)));
	Vec3 HitPos;
	cObject* Target;
	m_Ammo[3] -= 1;
	if (GetTarget(CAMERA->m_Pos, CAMERA->GetRayDir(), true, &HitPos, &Target))
	{
		cObject* a;
		for (int i = -1; i <= 1; i += 2)
		{
			a = OBJECT->AddObject<cBullet>("Missile", m_Owner->m_Pos + RotateVec3(Vec3(7 * i, -1, 1), m_Owner->m_Rot), Obj_PlayerBullet);
			a->m_Scale = Vec3(10, 10, 10);
			a->AddComponent<cMeshRenderer>()->m_Mesh = IMAGE->FindMesh("Missile");
			a->GetComponent<cMeshRenderer>()->m_isReflected = true;
			a->GetComponent<cBullet>()->m_Damage = 10;
			a->GetComponent<cBullet>()->m_Speed = 200;
			a->GetComponent<cBullet>()->m_RotSpeed = 50;
			a->GetComponent<cBullet>()->m_Target = Target;
			a->AddComponent<cCollider>()->AddCollider(Vec3(0, 0, 0), 0.2);
			a->AddComponent<cLambda>()->m_OnUpate = [](cObject* _This)->void {
				_This->GetComponent<cBullet>()->m_RotSpeed += 100 * DT;
			};
			a->GetComponent<cLambda>()->m_OnRelease = [](cObject* _This)->void {
				cObject* a = OBJECT->AddObject<cParticle>("Particle", _This->m_Pos, Obj_Particle);
				a->AddComponent<cSpriteRenderer>()->m_Sprite = IMAGE->FindTexture("Part_Light");
				a->GetComponent<cSpriteRenderer>()->AddShader<c2DShader>()->SetColor(1, 1, 0.9);
				a->GetComponent<cSpriteRenderer>()->m_Mode = BILL;
				a->GetComponent<cSpriteRenderer>()->m_isReflected = true;
				a->GetComponent<cParticle>()->SetAlpha(1, -0.01, 1.2);
				a->m_Scale = Vec3(1, 1, 1) * Random(50.f, 60.f);
				a = OBJECT->AddObject<cParticle>("Particle", _This->m_Pos, Obj_Particle);
				a->AddComponent<cSpriteRenderer>();
				a->GetComponent<cSpriteRenderer>()->m_Mode = BILL;
				a->GetComponent<cSpriteRenderer>()->m_isReflected = true;
				a->AddComponent<cSpriteAnimation>()->AddAnimation("Effect", "fireboom1_", 60);
				a->m_Scale = Vec3(15, 15, 15) * Random(1.f, 1.25f);
				SOUND->Play("Missile_Hit");
			};
			a->GetComponent<cLambda>()->m_OnAlarm = [](cObject* _This)->void {
				cObject* a = OBJECT->AddObject<cParticle>("Particle", _This->m_Pos, Obj_Particle);
				a->AddComponent<cSpriteRenderer>();
				a->AddComponent<cSpriteAnimation>()->AddAnimation("Effect", "newsmoke" + to_string(Random(1, 2)) + "_", 60);
				a->GetComponent<cSpriteRenderer>()->m_Mode = BILL;
				a->GetComponent<cSpriteRenderer>()->m_isReflected = true;
				a->m_Scale = Vec3(3, 3, 3) * Random(1.f, 1.25f);
				a->m_Rot = MakeQuaternion(Vec3(Random(0, 359), Random(0, 359), Random(0, 359)));
			};
			a->SetAlarm("", a->GetComponent<cLambda>()->m_AlarmTimer = 0.05);
			a->m_Rot = m_Owner->m_Rot * MakeQuaternion(RandomVec3() * 30);
		}
	}
}

void cPlayer::DealDamage(float _Damage)
{
	if (GAME->m_Inv)
		return;
	m_Hp -= _Damage;
	m_Damaged = 255;
	GetComponent<cMeshRenderer>()->GetShader<cStandardShader>()->m_White = 1;
	GetComponent<cCollider>()->m_isEnable = false;
	SOUND->Play("Player_Hit");
	CAMERA->Shake(1, 1);
	m_Star = 2;
	if (m_Hp <= 0)
	{
		GAME->HitStop(3, 0);
		SCHEDULE->AddSchedule(3, [&]()->void {
			for (int i = 0; i < 3; i++)
			{
				GAME->Explosion(m_Owner->m_Pos + RandomVec3() * 10, Random(10.f, 15.f));
				CAMERA->Shake(5, 1);
				SOUND->Play("Explosion3_" + to_string(Random(1, 4)));
			}
		});
		SCHEDULE->AddSchedule(5, []()->void {
			CAMERA->Fade(0, 255);
		});
		SCHEDULE->AddSchedule(6, []()->void {
			SCENE->ChangeScene("RANKING");
		});
	}
}
