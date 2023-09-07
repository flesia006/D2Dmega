#include "framework.h"
#include "CopyMegaMan.h"


CopyMegaMan::CopyMegaMan()
{
	CreateAnimation();
}

CopyMegaMan::~CopyMegaMan()
{
}

void CopyMegaMan::Update(Matrix V, Matrix P)
{
	SET(m_pAnimation);
	m_pAnimation->Update(V, P);
}

void CopyMegaMan::Render()
{

	m_pAnimation->Render();
}

void CopyMegaMan::SetState(eState state)
{
	m_nState = state;
	m_pAnimation->SetPlay((UINT)m_nState);
}

void CopyMegaMan::CreateAnimation()
{
	wstring strImage = L"./Textures/momodora/player/momo_idle.png";

	m_pAnimation = make_shared<Animation>(strImage, g_ShaderFile, GetScale(), GetPosition());


	// LEFT_IDLE
	{
		strImage = L"./Textures/Megaman/Images/Object/x_move.bmp";
		//	CreateClip(strImage, 128, 30+128, 4);

		shared_ptr<AnimationClip>	pClip = make_shared<AnimationClip>();
		shared_ptr<Texture>			pTexture = m_pAnimation->GetTexture();
		m_pAnimation->AddClip(pClip);

		for (int i = 0; i < 4; i++)
		{
			int sx = 128 * i + 40;
			int sy = 30 + 128;
			int ex = sx + 50;
			int ey = sy + 60;
			pClip->AddFrame(pTexture, strImage, sx, sy, ex, ey, 0.3f);
		}
	}

	// RIGHT_IDLE
	{
		strImage = L"./Textures/Megaman/Images/Object/x_move.bmp";
		//	CreateClip(strImage, 128, 30, 4);
		shared_ptr<AnimationClip> pClip = make_shared<AnimationClip>();
		shared_ptr<Texture>       pTexture = m_pAnimation->GetTexture();
		m_pAnimation->AddClip(pClip);

		for (int i = 0; i < 4; i++)
		{
			int sx = 128 * i + 40;
			int sy = 30;
			int ex = sx + 50;
			int ey = sy + 60;
			pClip->AddFrame(pTexture, strImage, sx, sy, ex, ey, 0.3f);
		}
	}

	// LEFT_MOVE
	{
		strImage = L"./Textures/Megaman/MyResource/PLAYER/X_LEFT.png";
		CreateClip(strImage, 100, 520, 14, AnimationClip::Loop, 2, 0.06f);
	}

	// RIGHT_MOVE
	{
		strImage = L"./Textures/Megaman/MyResource/PLAYER/X_RIGHT.png";
		CreateClip(strImage, 100, 520, 14, AnimationClip::Loop, 2, 0.06f);
	}

	// LEFT_JUMP
	{
		strImage = L"./Textures/Megaman/MyResource/PLAYER/X_LEFT.png";
		CreateClip(strImage, 100, 720, 7, AnimationClip::EndStay);
	}

	// RIGHT_JUMP
	{
		strImage = L"./Textures/Megaman/MyResource/PLAYER/X_RIGHT.png";
		CreateClip(strImage, 100, 720, 7, AnimationClip::EndStay);
	}

	// LEFT_FALL
	{
		strImage = L"./Textures/Megaman/MyResource/PLAYER/X_LEFT.png";
		CreateClip(strImage, 100, 820, 3, AnimationClip::EndStay);
	}

	// RIGHT_FALL
	{
		strImage = L"./Textures/Megaman/MyResource/PLAYER/X_RIGHT.png";
		CreateClip(strImage, 100, 820, 3, AnimationClip::EndStay);
	}

	// LEFT_ATTACK
	{
		strImage = L"./Textures/momodora/player/momo_attack.png";
		CreateClip(strImage, 384 / 4, 40, 4);
	}
	// RIGHT_ATTACK
	{
		strImage = L"./Textures/momodora/player/momo_attack.png";
		CreateClip(strImage, 384 / 4, 130, 11);

	}
	// ATTACK_CHARGE_L
	{
		strImage = L"./Textures/momodora/player/momo_idle.png";
		CreateClip(strImage, 480 / 5, 40, 5);
	}

	// ATTACK_CHARGE_R
	{
		strImage = L"./Textures/momodora/player/momo_idle.png";
		CreateClip(strImage, 480 / 5, 130, 5);
	}

	// LEFT_DASH
	{
		strImage = L"./Textures/Megaman/MyResource/PLAYER/X_LEFT.png";
		CreateClip(strImage, 100, 1020, 8, AnimationClip::EndStay);
	}

	// RIGHT_DASH
	{
		strImage = L"./Textures/Megaman/MyResource/PLAYER/X_RIGHT.png";
		CreateClip(strImage, 100, 1020, 8, AnimationClip::EndStay);
	}

	// LEFT_NOJUMPFALL
	{
		strImage = L"./Textures/Megaman/MyResource/PLAYER/X_LEFT.png";
		CreateClip(strImage, 100, 720, 1, AnimationClip::EndStay, 6);
	}

	// RIGHT_NOJUMPFALL
	{
		strImage = L"./Textures/Megaman/MyResource/PLAYER/X_RIGHT.png";
		CreateClip(strImage, 100, 720, 1, AnimationClip::EndStay, 6);
	}

}

void CopyMegaMan::CreateClip(wstring strImage, int w, int h, int count, AnimationClip::eState state, int start, float speed)
{
	shared_ptr<AnimationClip> pClip = make_shared<AnimationClip>(state);
	shared_ptr<Texture>       pTexture = m_pAnimation->GetTexture();
	m_pAnimation->AddClip(pClip);

	for (int i = start; i < start + count; i++)
	{
		int sx = w * i + 20;
		int sy = h;
		int ex = sx + 60;
		int ey = sy + 60;
		pClip->AddFrame(pTexture, strImage, sx, sy, ex, ey, speed);
	}
}
