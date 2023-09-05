#include    "framework.h"
#include    "PlayerEffect.h"

PlayerEffect::PlayerEffect()
{
	SetActive(false);
	wstring strImage = L"./Textures/momodora/effect/momo_effect_left.png";
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
		CreateClip(strImage, 100, 100, 10);
	}

	// EFFECT_CHARGE_BODY_GREEN
	{
		wstring strImage = L"./Textures/Megaman/MyResource/BULLET/EFFECT_CHARGE_BODY_GREEN.bmp";
		CreateClip(strImage, 100, 100, 4);
	}

	// EFFECT_DASH_L
	{
		wstring strImage = L"./Textures/Megaman/MyResource/EFFECT/EFFECT_DASH_LEFT.png";
		CreateClip(strImage, 50, 50, 7, AnimationClip::eState::End, 0.06f);
	}

	// EFFECT_DASH_R
	{
		wstring strImage = L"./Textures/Megaman/MyResource/EFFECT/EFFECT_DASH_RIGHT.png";
		CreateClip(strImage, 50, 50, 7, AnimationClip::eState::End, 0.06f);
	}

	// EFFECT_DASH_L
	{
		wstring strImage = L"./Textures/Megaman/resources/Player/Effect/Effect_Back_Left.bmp";
		CreateClip(strImage, 100, 64, 9, AnimationClip::eState::End, 0.03f);
	}

	// EFFECT_DASH_R
	{
		wstring strImage = L"./Textures/Megaman/resources/Player/Effect/Effect_Back_Right.bmp";
		CreateClip(strImage, 100, 64, 9, AnimationClip::eState::End, 0.03f);
	}
}

PlayerEffect::~PlayerEffect()
{
}

void PlayerEffect::Update(Matrix V, Matrix P)
{
	if (!IsActive())
		return;

	SET(m_pAnimation);
	m_pAnimation->Update(V, P);
}

void PlayerEffect::Render()
{
	if (!IsActive())
		return;

	m_pAnimation->Render();
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
