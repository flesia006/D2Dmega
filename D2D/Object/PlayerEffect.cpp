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
		CreateClip(strImage, 100, 100, 10, 0);
	}
	// EFFECT_CHARGE_BODY
	{
		wstring strImage = L"./Textures/Megaman/MyResource/BULLET/EFFECT_CHARGE_BODY.png";
		CreateClip(strImage, 100, 100, 4, 0);
	}

	// EFFECT_CHARGE_GREEN
	{
		wstring strImage = L"./Textures/Megaman/MyResource/BULLET/EFFECT_CHARGE_GREEN.bmp";
		CreateClip(strImage, 100, 100, 10, 0);
	}

	// EFFECT_CHARGE_BODY_GREEN
	{
		wstring strImage = L"./Textures/Megaman/MyResource/BULLET/EFFECT_CHARGE_BODY_GREEN.bmp";
		CreateClip(strImage, 100, 100, 4, 0);
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

void PlayerEffect::CreateClip(wstring strImage, int w, int h, int count, int loop)
{
	AnimationClip::eState state = AnimationClip::eState::Loop;
	if (loop == 0)
		state = AnimationClip::eState::Once;

	shared_ptr<AnimationClip> pClip = make_shared<AnimationClip>(state);
	shared_ptr<Texture> pTexture = m_pAnimation->GetTexture();
	m_pAnimation->AddClip(pClip);

	for (int i = 0; i < count; i++)
	{
		int sx = w * i;
		int sy = 0;
		int ex = sx + w;
		int ey = h;
		pClip->AddFrame(pTexture, strImage, sx, sy, ex, ey, 0.1f);
	}
}
