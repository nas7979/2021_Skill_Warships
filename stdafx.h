#pragma once
#include <list>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <queue>
#include <math.h>
#include <time.h>
#include <dsound.h>
#include <functional>
#include "SDKwavefile.h"
#include "SDKsound.h"
#pragma comment (lib, "legacy_stdio_definitions.lib")
#pragma comment (lib, "dsound.lib")

using Vec2 = D3DXVECTOR2;
using Vec3 = D3DXVECTOR3;
using Matrix = D3DXMATRIX;
using Quaternion = D3DXQUATERNION;

#define g_Device DXUTGetD3D9Device()
#define DT GAME->m_DT
#define NSDT GAME->m_NSDT

using namespace std;
#include "Math.h"
using namespace Math;

#include "cSingleton.h"
#include "cImageManager.h"
#include "cSceneManager.h"
#include "cSoundManager.h"
#include "cInputManager.h"
#include "cCameraManager.h"
#include "cObjectManager.h"
#include "cScheduleManager.h"
#include "cGameManager.h"

#include "cShader.h"
#include "cComponent.h"
#include "cMeshRenderer.h"
#include "cSpriteRenderer.h"
#include "cCollider.h"
#include "cSpriteAnimation.h"
#include "cLambda.h"
#include "cParticle.h"
#include "cTimeline.h"
#include "cBullet.h"
#include "cWave.h"
#include "cWeapon.h"
#include "cEnemy.h"
#include "c2DShader.h"
#include "cStandardShader.h"
#include "cScore.h"