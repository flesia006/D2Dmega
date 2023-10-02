#include "framework.h"
#include "ChargeEffect.h"

ChargeEffect::ChargeEffect()
{
	SetActive(false);
	wstring strImage = L"./Textures/Megaman/MyResource/BULLET/EFFECT_CHARGE.png";
	wstring  shaderFile = L"./Shader/Texture2.hlsl";
	m_pAnimation = make_shared<Animation>(strImage, shaderFile);

	// EFFECT_CHARGE
	{
		wstring strImage = L"./Textures/Megaman/MyResource/BULLET/EFFECT_CHARGE.png";
		CreateClip(strImage, 100, 100, 10, AnimationClip::Loop, 0.075f);
	}
	// EFFECT_CHARGE_BODY
	{
		wstring strImage = L"./Textures/Megaman/MyResource/BULLET/EFFECT_CHARGE_BODY.png";
		CreateClip(strImage, 100, 100, 4, AnimationClip::Loop, 0.075f);
	}

	// EFFECT_CHARGE_GREEN
	{
		wstring strImage = L"./Textures/Megaman/MyResource/BULLET/EFFECT_CHARGE_GREEN.bmp";
		CreateClip(strImage, 100, 100, 10, AnimationClip::Loop, 0.075f);
	}

	// EFFECT_CHARGE_BODY_GREEN
	{
		wstring strImage = L"./Textures/Megaman/MyResource/BULLET/EFFECT_CHARGE_BODY_GREEN.bmp";
		CreateClip(strImage, 100, 100, 4, AnimationClip::Loop, 0.075f);
	}
}

ChargeEffect::~ChargeEffect()
{
}

void ChargeEffect::Update()
{
	if (!IsActive())
		return;

	UPDATE(m_pAnimation);
}

void ChargeEffect::Render()
{
	if (!IsActive())
		return;

	m_pAnimation->Render();
}

void ChargeEffect::CreateClip(wstring strImage, int w, int h, int count, AnimationClip::eState state, float speed)
{
	shared_ptr<AnimationClip> pClip = make_shared<AnimationClip>(state);
	shared_ptr<Texture> pTexture = m_pAnimation->GetTexture();
	pTexture->UpdateColorBuffer(2);
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
