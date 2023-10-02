#include "framework.h"
#include "BossEffect.h"

BossEffect::BossEffect()
{
	SetActive(true);
	CreateAnimation();
}

BossEffect::~BossEffect()
{
}

void BossEffect::Update()
{
	UPDATE(m_pAnimation);
}

void BossEffect::Render()
{

	m_pAnimation->Render();
}

void BossEffect::SetState(eState state, bool reset)
{
	m_nState = state;
	m_pAnimation->SetPlay((UINT)m_nState, 0, reset);
}

void BossEffect::CreateAnimation()
{

	wstring strImage = L"./Textures/Megaman/resources/Enemy/Boss/Colonel_Idle.bmp";
	m_pAnimation = make_shared<Animation>(strImage, g_ShaderFile);

	// IDLE
	{
		strImage = L"./Textures/Megaman/resources/Enemy/Boss/Colonel_Idle.bmp";
		CreateClip(strImage, 267, 0, 3, AnimationClip::Loop, 0.1f, 146);
	}

	// GUARD
	{
		strImage = L"./Textures/Megaman/resources/Enemy/Boss/Colonel_StandBy.bmp";
		CreateClip(strImage, 126, 0, 1, AnimationClip::eState::EndStay, 10.0f, 157);
	}

	// DOWN_SLASH
	{
		strImage = L"./Textures/Megaman/resources/Enemy/Boss/Colonel_Slash.bmp";
		shared_ptr<AnimationClip> pClip = make_shared<AnimationClip>(AnimationClip::eState::EndStay);
		shared_ptr<Texture> pTexture = m_pAnimation->GetTexture();
		m_pAnimation->AddClip(pClip);

		for (int i = 0; i < 5; i++)
		{
			int sx = 278 * i;
			int sy = 0;
			int ex = sx + 278;
			int ey = 208;
			pClip->AddFrame(pTexture, strImage, sx, sy, ex, ey, 0.05f);
		}
	}

	// UP_SLASH
	{
		strImage = L"./Textures/Megaman/resources/Enemy/Boss/Colonel_Slash.bmp";
		shared_ptr<AnimationClip> pClip = make_shared<AnimationClip>(AnimationClip::eState::EndStay);
		shared_ptr<Texture> pTexture = m_pAnimation->GetTexture();
		m_pAnimation->AddClip(pClip);

		for (int i = 5; i < 11; i++)
		{
			int sx = 278 * i;
			int sy = 0;
			int ex = sx + 278;
			int ey = 208;
			pClip->AddFrame(pTexture, strImage, sx, sy, ex, ey, 0.05f);
		}
	}

	// THUNDER
	{
		strImage = L"./Textures/Megaman/resources/Enemy/Boss/Colonel_Thunder.bmp";
		CreateClip(strImage, 124, 0, 18, AnimationClip::eState::EndStay, 0.1f, 427);
	}

	// DISAPPEAR
	{
		strImage = L"./Textures/Megaman/resources/Enemy/Boss/Colonel_FadeOut.bmp";
		CreateClip(strImage, 174, 0, 4, AnimationClip::eState::End, 0.04f, 132);
	}

	// APPEAR
	{
		strImage = L"./Textures/Megaman/resources/Enemy/Boss/Colonel_FadeOut.bmp";
		shared_ptr<AnimationClip> pClip = make_shared<AnimationClip>(AnimationClip::eState::End);
		shared_ptr<Texture> pTexture = m_pAnimation->GetTexture();
		m_pAnimation->AddClip(pClip);

		for (int i = 3; i >= 0; i--)
		{
			int sx = 174 * i;
			int sy = 0;
			int ex = sx + 174;
			int ey = 132;
			pClip->AddFrame(pTexture, strImage, sx, sy, ex, ey, 0.04f);
		}
	}

	// DASH
	{
		strImage = L"./Textures/Megaman/resources/Enemy/Boss/Colonel_FadeOut.bmp";
		CreateClip(strImage, 174, 0, 1, AnimationClip::eState::EndStay, 1.0f, 132);
	}

	// JUMPATTACK
	{
		wstring strImage = L"./Textures/Megaman/MyResource/Monster/KnotBeret_RIGHT.bmp";
		CreateClip(strImage, 30, 50, 5, AnimationClip::eState::Loop, 0.03f);
	}


	// OPENING
	{

		strImage = L"./Textures/Megaman/resources/Enemy/Boss/Colonel_Thunder.bmp";
		shared_ptr<AnimationClip> pClip = make_shared<AnimationClip>(AnimationClip::eState::EndStay);
		shared_ptr<Texture> pTexture = m_pAnimation->GetTexture();
		m_pAnimation->AddClip(pClip);

		for (int i = 0; i < 11; i++)
		{
			int sx = 124 * i;
			int sy = 0;
			int ex = sx + 124;
			int ey = 427;
			pClip->AddFrame(pTexture, strImage, sx, sy, ex, ey, 0.1f);
		}

		strImage = L"./Textures/Megaman/resources/Enemy/Boss/Colonel_Idle.bmp";
		pClip->AddFrame(pTexture, strImage, 0, 0, 267, 146, 1.0f);
	}

	// DEFEAT
	{
		strImage = L"./Textures/Megaman/resources/Enemy/Boss/Colonel_Lose.bmp";
		CreateClip(strImage, 136, 0, 1, AnimationClip::eState::EndStay, 1.0f, 175);
	}

	// CHARGING
	{
		strImage = L"./Textures/Megaman/resources/Enemy/Boss/Colonel_Thunder.bmp";
		CreateClip(strImage, 124, 0, 6, AnimationClip::eState::Loop, 0.1f, 427);
	}
}

void BossEffect::CreateClip(wstring strImage, int w, int h, int count, AnimationClip::eState state, float speed, int term)
{
	shared_ptr<AnimationClip> pClip = make_shared<AnimationClip>(state);
	shared_ptr<Texture> pTexture = m_pAnimation->GetTexture();
	m_pAnimation->AddClip(pClip);

	for (int i = 0; i < count; i++)
	{
		int sx = w * i;
		int sy = h;
		int ex = sx + w;
		int ey = h + term;
		pClip->AddFrame(pTexture, strImage, sx, sy, ex, ey, speed);
	}
}