#include "framework.h"
#include "AlterMegaMan.h"

#include "./Collider/Collision.h"
#include "./Scene/S01_Field.h"
//#include "./Scene/S02_MoMoDora.h"
#include "./Object/PlayerEffect.h"
#include "Object/Bullet.h"

AlterMegaMan::AlterMegaMan()
{
	CreateAnimation();
}

AlterMegaMan::~AlterMegaMan()
{
}



void AlterMegaMan::SetState(eState state)
{
	m_nState = state;
	m_pAnimation->SetPlay((UINT)m_nState);
}

void AlterMegaMan::PreUpdate(Vector2& position)
{


}


void AlterMegaMan::Update(Matrix V, Matrix P)
{

	// KeyBoard
	auto position = OBJECTMANAGER->FindObject("MEGAMAN")->GetPosition();
	SetPosition(position);
	PreUpdate(m_Position);

	// Animation Update
	m_pAnimation->SetPosition(m_Position);
	m_pAnimation->Update(V, P);
}

void AlterMegaMan::Render()
{
	m_pAnimation->Render();
}


void AlterMegaMan::FireBullet()
{

}

void AlterMegaMan::Reset()
{
}

void AlterMegaMan::PostRender()
{

}



void AlterMegaMan::CreateAnimation()
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

	// LEFT_FIRE_MOVE
	{
		strImage = L"./Textures/Megaman/MyResource/PLAYER/X_LEFT.png";
		CreateClip(strImage, 100, 520, 14, AnimationClip::Loop, 2, 0.06f);
	}

	// RIGHT_FIRE_MOVE
	{
		strImage = L"./Textures/Megaman/MyResource/PLAYER/X_RIGHT.png";
		CreateClip(strImage, 100, 520, 14, AnimationClip::Loop, 2, 0.06f);
	}

	// LEFT_FIRE_JUMP
	{
		strImage = L"./Textures/Megaman/MyResource/PLAYER/X_LEFT.png";
		CreateClip(strImage, 100, 720, 7, AnimationClip::EndStay);
	}

	// RIGHT_FIRE_JUMP
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

	// LEFT_FIRE_DASH
	{
		strImage = L"./Textures/Megaman/MyResource/PLAYER/X_LEFT.png";
		CreateClip(strImage, 100, 1020, 8, AnimationClip::EndStay);
	}

	// RIGHT_FIRE_DASH
	{
		strImage = L"./Textures/Megaman/MyResource/PLAYER/X_RIGHT.png";
		CreateClip(strImage, 100, 1020, 8, AnimationClip::EndStay);
	}

	// LEFT_FIRE_NOJUMPFALL
	{
		strImage = L"./Textures/Megaman/MyResource/PLAYER/X_LEFT.png";
		CreateClip(strImage, 100, 720, 1, AnimationClip::EndStay, 6);
	}

	// RIGHT_FIRE_NOJUMPFALL
	{
		strImage = L"./Textures/Megaman/MyResource/PLAYER/X_RIGHT.png";
		CreateClip(strImage, 100, 720, 1, AnimationClip::EndStay, 6);
	}


}

void AlterMegaMan::CreateClip(wstring strImage, int w, int h, int count, AnimationClip::eState state, int start, float speed)
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

void AlterMegaMan::GroundCheck(Vector2& position)
{

}

void AlterMegaMan::WallCheck(Vector2& position)
{

}

void AlterMegaMan::OnEndPlayerEffect()
{

}
