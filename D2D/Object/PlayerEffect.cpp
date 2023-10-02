#include    "framework.h"
#include    "PlayerEffect.h"

PlayerEffect::PlayerEffect()
{
	SetActive(false);
	wstring strImage = L"./Textures/Megaman/MyResource/BULLET/EFFECT_CHARGE.png";
	m_pAnimation = make_shared<Animation>(strImage, g_ShaderFile);
	
	// EFFECT_CHARGE
	{
		wstring strImage = L"./Textures/Megaman/MyResource/BULLET/EFFECT_CHARGE.png";
		CreateClip(strImage, 100, 100, 10);
	}
	// EFFECT_CHARGE_BODY
	{
		wstring strImage = L"./Textures/Megaman/MyResource/BULLET/EFFECT_CHARGE_BODY.png";
		CreateClip(strImage, 100, 100, 4);
	}

	// EFFECT_CHARGE_GREEN
	{
		wstring strImage = L"./Textures/Megaman/MyResource/BULLET/EFFECT_CHARGE_GREEN.bmp";
		CreateClip(strImage, 100, 100, 10, AnimationClip::eState::Loop, 0.6f);
	}

	// EFFECT_CHARGE_BODY_GREEN
	{
		wstring strImage = L"./Textures/Megaman/MyResource/BULLET/EFFECT_CHARGE_BODY_GREEN.bmp";
		CreateClip(strImage, 100, 100, 4, AnimationClip::eState::Loop, 0.6f);
	}

	// EFFECT_DASH_L
	{
		wstring strImage = L"./Textures/Megaman/MyResource/EFFECT/EFFECT_DASH_LEFT.bmp";
		CreateClip(strImage, 50, 50, 8, AnimationClip::eState::End, 0.05f);
	}

	// EFFECT_DASH_R
	{
		wstring strImage = L"./Textures/Megaman/MyResource/EFFECT/EFFECT_DASH_RIGHT.bmp";
		CreateClip(strImage, 50, 50, 8, AnimationClip::eState::End, 0.05f);
	}

	// EFFECT_DASHDUST_L
	{
		wstring strImage = L"./Textures/Megaman/resources/Player/Effect/Effect_Back_Left.bmp";
		CreateClip(strImage, 100, 64, 9, AnimationClip::eState::End, 0.03f);
	}

	// EFFECT_DASHDUST_R
	{
		wstring strImage = L"./Textures/Megaman/resources/Player/Effect/Effect_Back_Right.bmp";
		CreateClip(strImage, 100, 64, 9, AnimationClip::eState::End, 0.03f);
	}

	// EFFECT_WALL_L
	{
		wstring strImage = L"./Textures/Megaman/MyResource/EFFECT/EFFECT_WALL_LEFT.bmp";
		CreateClip(strImage, 30, 50, 5, AnimationClip::eState::Loop, 0.03f);
	}

	// EFFECT_WALL_R
	{
		wstring strImage = L"./Textures/Megaman/MyResource/EFFECT/EFFECT_WALL_RIGHT.bmp";
		CreateClip(strImage, 30, 50, 5, AnimationClip::eState::Loop, 0.03f);
	}

	// EFFECT_WALLKICK_L
	{
		wstring strImage = L"./Textures/Megaman/MyResource/EFFECT/EFFECT_WALLKICK_LEFT.bmp";
		CreateClip(strImage, 50, 50, 5, AnimationClip::eState::End, 0.05f);
	}

	// EFFECT_WALLKICK_R
	{
		wstring strImage = L"./Textures/Megaman/MyResource/EFFECT/EFFECT_WALLKICK_RIGHT.bmp";
		CreateClip(strImage, 50, 50, 5, AnimationClip::eState::End, 0.05f);
	}

	// EXPLOSION
	{
		wstring strImage = L"./Textures/Megaman/MyResource/EFFECT/EFFECT_EXPLOSION.bmp";
		CreateClip(strImage, 100, 100, 19, AnimationClip::eState::End, 0.03f);
	}

	// ENEMYHIT
	{
		wstring strImage = L"./Textures/Megaman/MyResource/EFFECT/enemyHit.bmp";
		CreateClip(strImage, 40, 72, 5, AnimationClip::eState::End, 0.03f);
	}

	// ENEMYBOMB
	{
		wstring strImage = L"./Textures/Megaman/MyResource/EFFECT/EnemyExplosion.png";
		CreateClip(strImage, 60, 60, 10, AnimationClip::eState::End, 0.03f);
	}

	// ENEMYBLOCK
	{
		wstring strImage = L"./Textures/Megaman/MyResource/EFFECT/EFFECT_BULLET_BLOCKED_RIGHT.bmp";
		CreateClip(strImage, 50, 100, 6, AnimationClip::eState::End, 0.03f);
	}

	// ENEMYHIT2
	{
		wstring strImage = L"./Textures/Megaman/MyResource/EFFECT/EFFECT_BUSTER_BULLET_COLLISON_RIGHT.bmp";
		CreateClip(strImage, 100, 100, 5, AnimationClip::eState::End, 0.03f);
	}
}

PlayerEffect::~PlayerEffect()
{
}

void PlayerEffect::Update()
{
	if (!IsActive())
		return;

	UPDATE(m_pAnimation);
}

void PlayerEffect::Render()
{
	if (!IsActive())
		return;

	m_pAnimation->Render();
}

void PlayerEffect::Reset()
{
	SetActive(false);
	m_pAnimation->SetPlay((UINT)m_nState);
}

void PlayerEffect::CreateClip(wstring strImage, int w, int h, int count, AnimationClip::eState state, float speed)
{
	shared_ptr<AnimationClip> pClip = make_shared<AnimationClip>(state);
	shared_ptr<Texture> pTexture = m_pAnimation->GetTexture();
	m_pAnimation->AddClip(pClip);

	for (int i = 0; i < count; i++)
	{
		int sx = w * i;
		int sy = 0;
		int ex = sx + w;
		int ey = h;
		pClip->AddFrame(pTexture, strImage, sx, sy, ex, ey, speed);
	}
}
