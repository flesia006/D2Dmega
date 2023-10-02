#include "framework.h"
#include "DashMegaMan.h"


DashMegaMan::DashMegaMan()
{
	CreateAnimation();
}

DashMegaMan::~DashMegaMan()
{
}

void DashMegaMan::Update()
{
	UPDATE(m_pAnimation);
}

void DashMegaMan::Render()
{

	m_pAnimation->Render();
}

void DashMegaMan::SetState(eState state, bool reset)
{
	m_nState = state;
	m_pAnimation->SetPlay((UINT)m_nState, 0, reset);
}

void DashMegaMan::CreateAnimation()
{
	wstring strImage = L"./Textures/Megaman/Images/Object/x_move.bmp";

	wstring  shaderFile = L"./Shader/Texture2.hlsl";
	m_pAnimation = make_shared<Animation>(strImage, shaderFile, GetScale(), GetPosition());


	// LEFT_IDLE
	{
		strImage = L"./Textures/Megaman/MyResource/PLAYER/EFFECT_X_LEFT_DASH.bmp";
		CreateClip(strImage, 100, 100, 4, AnimationClip::Loop, 0, 0.15f);
	}

	// RIGHT_IDLE
	{
		strImage = L"./Textures/Megaman/MyResource/PLAYER/EFFECT_X_RIGHT_DASH.bmp";
		CreateClip(strImage, 100, 100, 4, AnimationClip::Loop, 0, 0.15f);
	}

	// LEFT_MOVE
	{
		strImage = L"./Textures/Megaman/MyResource/PLAYER/EFFECT_X_LEFT_DASH.bmp";
		CreateClip(strImage, 100, 400, 14, AnimationClip::Loop, 2, 0.04f);
	}

	// RIGHT_MOVE
	{
		strImage = L"./Textures/Megaman/MyResource/PLAYER/EFFECT_X_RIGHT_DASH.bmp";
		CreateClip(strImage, 100, 400, 14, AnimationClip::Loop, 2, 0.04f);
	}

	// LEFT_JUMP
	{
		strImage = L"./Textures/Megaman/MyResource/PLAYER/EFFECT_X_LEFT_DASH.bmp";
		CreateClip(strImage, 100, 600, 7, AnimationClip::EndStay);
	}

	// RIGHT_JUMP
	{
		strImage = L"./Textures/Megaman/MyResource/PLAYER/EFFECT_X_RIGHT_DASH.bmp";
		CreateClip(strImage, 100, 600, 7, AnimationClip::EndStay);
	}

	// LEFT_FALL
	{
		strImage = L"./Textures/Megaman/MyResource/PLAYER/EFFECT_X_LEFT_DASH.bmp";
		CreateClip(strImage, 100, 800, 3, AnimationClip::EndStay);
	}

	// RIGHT_FALL
	{
		strImage = L"./Textures/Megaman/MyResource/PLAYER/EFFECT_X_RIGHT_DASH.bmp";
		CreateClip(strImage, 100, 800, 3, AnimationClip::EndStay);
	}

	// LEFT_ATTACK
	{
		strImage = L"./Textures/Megaman/MyResource/PLAYER/EFFECT_X_LEFT_DASH.bmp";
		CreateClip(strImage, 100, 200, 8, AnimationClip::EndStay, 0, 0.05f);
	}

	// RIGHT_ATTACK
	{
		strImage = L"./Textures/Megaman/MyResource/PLAYER/EFFECT_X_RIGHT_DASH.bmp";
		CreateClip(strImage, 100, 200, 8, AnimationClip::EndStay, 0, 0.05f);

	}
	// ATTACK_CHARGE_L
	{
		strImage = L"./Textures/Megaman/MyResource/PLAYER/EFFECT_X_LEFT_DASH.bmp";
		CreateClip(strImage, 100, 300, 8, AnimationClip::EndStay, 0, 0.05f);
	}

	// ATTACK_CHARGE_R
	{
		strImage = L"./Textures/Megaman/MyResource/PLAYER/EFFECT_X_RIGHT_DASH.bmp";
		CreateClip(strImage, 100, 300, 8, AnimationClip::EndStay, 0, 0.05f);
	}

	// LEFT_DASH
	{
		strImage = L"./Textures/Megaman/MyResource/PLAYER/EFFECT_X_LEFT_DASH.bmp";
		CreateClip(strImage, 100, 895, 8, AnimationClip::EndStay);
	}

	// RIGHT_DASH
	{
		strImage = L"./Textures/Megaman/MyResource/PLAYER/EFFECT_X_RIGHT_DASH.bmp";
		CreateClip(strImage, 100, 895, 8, AnimationClip::EndStay);
	}

	// LEFT_NOJUMPFALL
	{
		strImage = L"./Textures/Megaman/MyResource/PLAYER/EFFECT_X_LEFT_DASH.bmp";
		CreateClip(strImage, 100, 600, 1, AnimationClip::EndStay, 6);
	}

	// RIGHT_NOJUMPFALL
	{
		strImage = L"./Textures/Megaman/MyResource/PLAYER/EFFECT_X_RIGHT_DASH.bmp";
		CreateClip(strImage, 100, 600, 1, AnimationClip::EndStay, 6);
	}

	// LEFT_WALLSTICK
	{
		strImage = L"./Textures/Megaman/MyResource/PLAYER/EFFECT_X_LEFT_DASH.bmp";
		CreateClip(strImage, 100, 1100, 4, AnimationClip::EndStay);
	}

	// RIGHT_WALLSTICK
	{
		strImage = L"./Textures/Megaman/MyResource/PLAYER/EFFECT_X_RIGHT_DASH.bmp";
		CreateClip(strImage, 100, 1100, 4, AnimationClip::EndStay);
	}

	// LEFT_WALLKICK
	{
		strImage = L"./Textures/Megaman/MyResource/PLAYER/EFFECT_X_LEFT_DASH.bmp";
		CreateClip(strImage, 100, 1300, 2, AnimationClip::EndStay);
	}

	// RIGHT_WALLKICK
	{
		strImage = L"./Textures/Megaman/MyResource/PLAYER/EFFECT_X_RIGHT_DASH.bmp";
		CreateClip(strImage, 100, 1300, 2, AnimationClip::EndStay);
	}

	// LEFT_ATTACKED
	{
		strImage = L"./Textures/Megaman/MyResource/PLAYER/EFFECT_X_LEFT_DASH.bmp";
		CreateClip(strImage, 100, 1500, 4, AnimationClip::EndStay);
	}

	// RIGHT_ATTACKED
	{
		strImage = L"./Textures/Megaman/MyResource/PLAYER/EFFECT_X_RIGHT_DASH.bmp";
		CreateClip(strImage, 100, 1500, 4, AnimationClip::EndStay);
	}

	// LEFT_HEAVYATTACKED
	{
		strImage = L"./Textures/Megaman/MyResource/PLAYER/EFFECT_X_LEFT_DASH.bmp";
		CreateClip(strImage, 100, 1600, 5, AnimationClip::EndStay);
	}

	// RIGHT_HEAVYATTACKED
	{
		strImage = L"./Textures/Megaman/MyResource/PLAYER/EFFECT_X_RIGHT_DASH.bmp";
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

void DashMegaMan::CreateClip(wstring strImage, int w, int h, int count, AnimationClip::eState state, int start, float speed)
{
	shared_ptr<AnimationClip> pClip = make_shared<AnimationClip>(state);
	shared_ptr<Texture>       pTexture = m_pAnimation->GetTexture();
	pTexture->UpdateColorBuffer(1);
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