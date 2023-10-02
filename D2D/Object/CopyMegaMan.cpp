#include "framework.h"
#include "CopyMegaMan.h"


CopyMegaMan::CopyMegaMan()
{
	CreateAnimation();
}

CopyMegaMan::~CopyMegaMan()
{
}

void CopyMegaMan::Update()
{
	UPDATE(m_pAnimation);
}

void CopyMegaMan::Render()
{
	m_pAnimation->Render();
}

void CopyMegaMan::SetState(eState state, bool reset)
{
	m_nState = state;
	m_pAnimation->SetPlay((UINT)m_nState, 0, reset);
}

void CopyMegaMan::CreateAnimation()
{
	wstring strImage = L"./Textures/Megaman/Images/Object/x_move.bmp";

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
		CreateClip(strImage, 100, 500, 14, AnimationClip::Loop, 2, 0.04f);
	}

	// RIGHT_MOVE
	{
		strImage = L"./Textures/Megaman/MyResource/PLAYER/X_RIGHT.png";
		CreateClip(strImage, 100, 500, 14, AnimationClip::Loop, 2, 0.04f);
	}

	// LEFT_JUMP
	{
		strImage = L"./Textures/Megaman/MyResource/PLAYER/X_LEFT.png";
		CreateClip(strImage, 100, 700, 7, AnimationClip::EndStay);
	}

	// RIGHT_JUMP
	{
		strImage = L"./Textures/Megaman/MyResource/PLAYER/X_RIGHT.png";
		CreateClip(strImage, 100, 700, 7, AnimationClip::EndStay);
	}

	// LEFT_FALL
	{
		strImage = L"./Textures/Megaman/MyResource/PLAYER/X_LEFT.png";
		CreateClip(strImage, 100, 800, 3, AnimationClip::EndStay);
	}

	// RIGHT_FALL
	{
		strImage = L"./Textures/Megaman/MyResource/PLAYER/X_RIGHT.png";
		CreateClip(strImage, 100, 800, 3, AnimationClip::EndStay);
	}

	// LEFT_ATTACK
	{
		strImage = L"./Textures/Megaman/MyResource/PLAYER/X_LEFT.png";
		CreateClip(strImage, 100, 200, 8, AnimationClip::EndStay, 0, 0.05f);
	}

	// RIGHT_ATTACK
	{
		strImage = L"./Textures/Megaman/MyResource/PLAYER/X_RIGHT.png";
		CreateClip(strImage, 100, 200, 8, AnimationClip::EndStay, 0, 0.05f);

	}
	// ATTACK_CHARGE_L
	{
		strImage = L"./Textures/Megaman/MyResource/PLAYER/X_LEFT.png";
		CreateClip(strImage, 100, 300, 8, AnimationClip::EndStay, 0, 0.05f);
	}

	// ATTACK_CHARGE_R
	{
		strImage = L"./Textures/Megaman/MyResource/PLAYER/X_RIGHT.png";
		CreateClip(strImage, 100, 300, 8, AnimationClip::EndStay, 0, 0.05f);
	}

	// LEFT_DASH
	{
		strImage = L"./Textures/Megaman/MyResource/PLAYER/X_LEFT.png";
		CreateClip(strImage, 100, 995, 8, AnimationClip::EndStay);
	}

	// RIGHT_DASH
	{
		strImage = L"./Textures/Megaman/MyResource/PLAYER/X_RIGHT.png";
		CreateClip(strImage, 100, 995, 8, AnimationClip::EndStay);
	}

	// LEFT_NOJUMPFALL
	{
		strImage = L"./Textures/Megaman/MyResource/PLAYER/X_LEFT.png";
		CreateClip(strImage, 100, 700, 1, AnimationClip::EndStay, 6);
	}

	// RIGHT_NOJUMPFALL
	{
		strImage = L"./Textures/Megaman/MyResource/PLAYER/X_RIGHT.png";
		CreateClip(strImage, 100, 700, 1, AnimationClip::EndStay, 6);
	}

	// LEFT_WALLSTICK
	{
		strImage = L"./Textures/Megaman/MyResource/PLAYER/X_LEFT.png";
		CreateClip(strImage, 100, 1200, 4, AnimationClip::EndStay);
	}

	// RIGHT_WALLSTICK
	{
		strImage = L"./Textures/Megaman/MyResource/PLAYER/X_RIGHT.png";
		CreateClip(strImage, 100, 1200, 4, AnimationClip::EndStay);
	}

	// LEFT_WALLKICK
	{
		strImage = L"./Textures/Megaman/MyResource/PLAYER/X_LEFT.png";
		CreateClip(strImage, 100, 1400, 2, AnimationClip::EndStay);
	}

	// RIGHT_WALLKICK
	{
		strImage = L"./Textures/Megaman/MyResource/PLAYER/X_RIGHT.png";
		CreateClip(strImage, 100, 1400, 2, AnimationClip::EndStay);
	}

	// LEFT_ATTACKED
	{
		strImage = L"./Textures/Megaman/MyResource/PLAYER/X_LEFT.png";
		CreateClip(strImage, 100, 1500, 4, AnimationClip::EndStay);
	}

	// RIGHT_ATTACKED
	{
		strImage = L"./Textures/Megaman/MyResource/PLAYER/X_RIGHT.png";
		CreateClip(strImage, 100, 1500, 4, AnimationClip::EndStay);
	}

	// LEFT_HEAVYATTACKED
	{
		strImage = L"./Textures/Megaman/MyResource/PLAYER/X_LEFT.png";
		CreateClip(strImage, 100, 1600, 5, AnimationClip::EndStay);
	}

	// RIGHT_HEAVYATTACKED
	{
		strImage = L"./Textures/Megaman/MyResource/PLAYER/X_RIGHT.png";
		CreateClip(strImage, 100, 1600, 5, AnimationClip::EndStay);
	}

	// LEFT_RETURN
	{
		strImage = L"./Textures/Megaman/MyResource/PLAYER/X_LEFT.png";
		CreateClip(strImage, 100, 1800, 10, AnimationClip::EndStay);
	}

	// RIGHT_RETURN
	{
		strImage = L"./Textures/Megaman/MyResource/PLAYER/X_RIGHT.png";
		CreateClip(strImage, 100, 1800, 10, AnimationClip::EndStay);
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
		int ey = sy + 100;
		pClip->AddFrame(pTexture, strImage, sx, sy, ex, ey, speed);
	}
}
