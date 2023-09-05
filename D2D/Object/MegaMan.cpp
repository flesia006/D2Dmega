#include "framework.h"
#include "MegaMan.h"
#include "./Collider/Collision.h"
#include "./Scene/S01_Field.h"
//#include "./Scene/S02_MoMoDora.h"
#include "./Object/PlayerEffect.h"
#include "Object/Bullet.h"

MegaMan::MegaMan()
{
	CreateAnimation();

	m_pCollider			= new Collider(Vector2(80.0f, 136.0f), GetPosition(), 1);
	m_pFeet				= new Collider(Vector2(80.0f, 30.0f), m_Position, 1);
	m_pWall				= new Collider(Vector2(85.0f, 106.0f), m_Position, 1);
	m_pHPbar			= make_shared<Texture>(L"./Textures/MoModora/Player/heart.png", g_ShaderFile);
//	m_pPlayerEffect1	= make_shared<PlayerEffect>();
//	m_cvEffects[GREEN_CHARGE]	= make_shared<PlayerEffect>();
	

	for (size_t i = 0; i < 5; i++)
		m_cvEffects.push_back(new PlayerEffect());

}

MegaMan::~MegaMan()
{
}



void MegaMan::SetState(eState state)
{
	m_nState = state;
	m_pAnimation->SetPlay((UINT)m_nState);
}

void MegaMan::PreUpdate(Vector2& position)
{
	// VK_LEFT
	{
		if (KEYBOARD->Up(VK_LEFT))
		{
			// LEFT MOVE, LEFT JUMP, LEFT FALL 인경우
			if (m_nState != eState::LEFT_IDLE)
			{
				if (m_bGround) 
					SetState(eState::LEFT_IDLE);
			}
		}
		if (KEYBOARD->Press(VK_LEFT))
		{
			if (m_nState != eState::LEFT_MOVE && !IsJumping() && !IsFalling() && !IsDashing())
				SetState(eState::LEFT_MOVE);

			if (m_nState == eState::LEFT_MOVE || m_nState == LEFT_JUMP 
				|| m_nState == LEFT_FALL || m_nState == LEFT_DASH)
				position.x = position.x - m_SpeedX * TIME->Delta();

			if (IsJumping() || IsFalling())
				m_Angle = LEFT_ANGLE;

			if (m_nState == eState::RIGHT_JUMP)
				SetState(eState::LEFT_JUMP);
		}
	}

	// VK_RIGHT
	{
		if (KEYBOARD->Up(VK_RIGHT))
		{
			// LEFT MOVE, LEFT JUMP, LEFT FALL 인경우
			if (m_nState != eState::RIGHT_IDLE)
			{
				if (m_bGround)
					SetState(eState::RIGHT_IDLE);
			}
		}
		if (KEYBOARD->Press(VK_RIGHT))
		{
			if (m_nState != eState::RIGHT_MOVE && !IsJumping() && !IsFalling() && !IsDashing())
				SetState(eState::RIGHT_MOVE);

			if (m_nState == eState::RIGHT_MOVE || m_nState == RIGHT_JUMP 
				|| m_nState == RIGHT_FALL || m_nState == RIGHT_DASH)
				position.x = position.x + m_SpeedX * TIME->Delta();

			if (IsJumping() || IsFalling())
				m_Angle = RIGHT_ANGLE;

			if (m_nState == eState::LEFT_JUMP)
				SetState(eState::RIGHT_JUMP);
		}
	}

	// X Key : 점프
	{
		if (KEYBOARD->Down('X'))
		{			
			if (!m_bGround)
				return;

			m_BeforeState = m_nState;
			m_bGround = false;
			m_bFalling = false;
			m_Angle = VERTICAL;

			if (m_nState % 2 == 0)
				SetState(eState::LEFT_JUMP);
			else SetState(eState::RIGHT_JUMP);
			m_Gravirty = 0.0f;	

		}
	}

	// ATTACK
	{
		if (KEYBOARD->Press('C'))
		{
			m_ChargeCountTime += TIME->Delta();
			// 1st Charge된 순간
			if ((m_ChargeCountTime >= 1.8f))
			{
				m_bChargeFull = true;
				m_bCharge = false;

				m_cvEffects[BLUE_CHARGE]->SetState(PlayerEffect::EFFECT_CHARGE_BODY_GREEN);
				m_cvEffects[GREEN_CHARGE]->SetState(PlayerEffect::EFFECT_CHARGE_GREEN);

				m_cvEffects[BLUE_CHARGE]->SetPosition(position);
				m_cvEffects[GREEN_CHARGE]->SetPosition(position);

			}
			else if ((m_ChargeCountTime >= 0.5f))
			{
				m_bCharge = true;

				m_cvEffects[BLUE_CHARGE]->SetState(PlayerEffect::EFFECT_CHARGE_BODY);
				m_cvEffects[GREEN_CHARGE]->SetState(PlayerEffect::EFFECT_CHARGE);

				m_cvEffects[BLUE_CHARGE]->SetActive(true);
				m_cvEffects[GREEN_CHARGE]->SetActive(true);

				m_cvEffects[BLUE_CHARGE]->SetPosition(position);
				m_cvEffects[GREEN_CHARGE]->SetPosition(position);
			}
		}
		if (KEYBOARD->Up('C'))
		{
			if (m_bChargeFull || m_bCharge)
			{
				if (m_cvEffects[BLUE_CHARGE]->IsActive())
					m_cvEffects[BLUE_CHARGE]->SetActive(false);
				if (m_cvEffects[GREEN_CHARGE]->IsActive())
					m_cvEffects[GREEN_CHARGE]->SetActive(false);
			}

			FireBullet();

			m_pAnimation->GetTexture()->UpdateColorBuffer(0);
			m_bCharge = m_bChargeFull = false;
			m_ChargeCountTime = 0.0f;
			m_cvEffects[GREEN_CHARGE]->SetActive(false);
		}
	}

	// Dash
	{
		if (KEYBOARD->Press('Z'))
		{			
			if (!m_bGround)
				return;

									
			m_DashTime += TIME->Delta();
			if (m_DashTime >= 0.6f)
			{
				m_cvEffects[DASH_EFCT]->SetActive(false);

				if (m_nState % 2 == 0)
					SetState(eState::LEFT_MOVE);
				else 
					SetState(eState::RIGHT_MOVE);

				SetNormalSpeed();

			}
			else
			{
				m_cvEffects[DASH_EFCT]->SetActive(true);

				if (m_nState % 2 == 0)
				{
					SetState(eState::LEFT_DASH);
					m_cvEffects[DASH_EFCT]->SetState(PlayerEffect::EFFECT_DASH_L);
					m_cvEffects[DASH_EFCT]->SetPosition(Vector2(position.x + 90.0f, position.y - 35.0f));
				}
				else 
				{
					SetState(eState::RIGHT_DASH);
					m_cvEffects[DASH_EFCT]->SetState(PlayerEffect::EFFECT_DASH_R);
					m_cvEffects[DASH_EFCT]->SetPosition(Vector2(position.x - 90.0f, position.y - 35.0f));
				}
				SetDashSpeed();
			}

		}
		if (KEYBOARD->Up('Z'))
		{
			if (m_nState % 2 == 0)
				SetState(eState::LEFT_IDLE);
			else SetState(eState::RIGHT_IDLE);
			SetNormalSpeed();
			m_DashTime = 0.0f;
			m_cvEffects[DASH_EFCT]->SetActive(false);

		}



	}

	// 1. 초기, 2. Jumping, 3. Falling
	if ((m_bGround == false) && (!(IsJumping())))
	{
		position.y = position.y - 3;
	}

	// 2. Juming이거나 낙하인 상태에서 떨어지게 하기
	if ((m_bGround == false) && ((IsJumping())))
	{
		//낙하
		position.y = position.y + sinf(m_Angle) * m_SpeedY - m_Gravirty;

		//낙하 판정
		if (m_Gravirty >= sinf(m_Angle) * m_SpeedY)
			m_bFalling = true;

		//중력		
		if(sinf(m_Angle) * m_SpeedY - m_Gravirty >= -4)
			m_Gravirty = m_Gravirty + 0.15f;
	}

	// State를 Check
	switch (m_nState)
	{
	case LEFT_JUMP:
	case RIGHT_JUMP:
		if (m_bGround)
		{
			if (m_nState == LEFT_JUMP)
				SetState(LEFT_FALL);
			else
				SetState(RIGHT_FALL);
		}
		break;
	case LEFT_FALL:
	case RIGHT_FALL:
		m_FallTime += 0.3f;
		if (m_FallTime >= 1.8f)
		{
			if (m_nState == LEFT_FALL)
				SetState(LEFT_IDLE);
			else
				SetState(RIGHT_IDLE);
			m_FallTime = 0.0f;
		}
		break;
	}
}


void MegaMan::Update(Matrix V, Matrix P)
{
	Vector2 position = GetPosition();


	// KeyBoard
	PreUpdate(position);




	WallCheck(position);
	GroundCheck(position);
	SetPosition(position);


	// Animation Update
	m_pAnimation->SetPosition(GetPosition());
	m_pAnimation->SetScale(GetScale());
	m_pAnimation->Update(V, P);

	// Collider  Update  
	m_pCollider->SetPosition(GetPosition());
	m_pCollider->Update(V, P);

	m_pFeet->SetPosition(GetPosition() - Vector2(0.0f, 55.0f));
	m_pFeet->Update(V, P);

	m_pWall->SetPosition(GetPosition() + Vector2(0.0f, 15.0f));
	m_pWall->Update(V, P);


	// bullet
	for (UINT i = 0; i < m_cvBullets.size(); i++)
	{
		if (!m_cvBullets[i]->IsActive())
		{
			delete m_cvBullets[i];
			m_cvBullets.erase(m_cvBullets.begin() + i, m_cvBullets.begin() + i + 1);
			m_cvBullets.shrink_to_fit();
		}
	}

	// effect
	for (auto& p : m_cvEffects)
	{
		if(p->IsActive())
			p->Update(V, P);
	}

}

void MegaMan::Render()
{
	m_pAnimation->Render();
	m_pCollider->UpdateColor(Color(1.0f, 0.0f, 0.0f, 1.0f));
	m_pCollider->Render();
	m_pFeet->Render();
	m_pWall->Render();
	
	// 윈도우의 좌표 
	Vector2 position = Vector2(20.0f, 70.0f);
	CAMERA->WCtoVC(position);

	for (int i = 0; i < m_nHP; i++)
	{
		m_pHPbar->SetPosition(position);
		m_pHPbar->Update(CAMERA->GetView(), CAMERA->GetProjection());
		m_pHPbar->Render();
		position.x = position.x + 30.0f;
	}

	for (auto& p : m_cvBullets)
	{
		if (p->IsActive())
			p->Render();
	}

	for (auto& p : m_cvEffects)
	{
		if (p->IsActive())
			p->Render();
	}
}


void MegaMan::FireBullet()
{
	Bullet* pBullet = new Bullet();
	if (!pBullet)
		return;

	if (m_bCharge)
		pBullet->SetState(Bullet::L_CHARGE);
	else if (m_bChargeFull)
		pBullet->SetState(Bullet::L_FULL_CHARGE);

	pBullet->SetActive(true);
	pBullet->SetPosition(GetPosition());
	pBullet->Reset();
	pBullet->SetPosition(GetPosition());
	pBullet->SetDirection((UINT)(m_nState) % 2);
	pBullet->SetScale(GetScale() * 1.1f);
	m_cvBullets.push_back(pBullet);
}

void MegaMan::Reset()
{
}

void MegaMan::PostRender()
{
	vector<wstring> strStatus;

	strStatus.push_back(L"LEFT_IDLE");
	strStatus.push_back(L"RIGHT_IDLE");
	strStatus.push_back(L"LEFT_MOVE");
	strStatus.push_back(L"RIGHT_MOVE");
	strStatus.push_back(L"LEFT_JUMP");
	strStatus.push_back(L"RIGHT_JUMP");
	strStatus.push_back(L"LEFT_FALL");
	strStatus.push_back(L"RIGHT_FALL");
	strStatus.push_back(L"LEFT_ATTACK");
	strStatus.push_back(L"RIGHT_ATTACK");
	strStatus.push_back(L"ATTACK_CHARGE_L");
	strStatus.push_back(L"ATTACK_CHARGE_R");
	strStatus.push_back(L"LEFT_DASH");
	strStatus.push_back(L"RIGHT_DASH");
	strStatus.push_back(L"RIGHT_DASH");
	strStatus.push_back(L"RIGHT_DASH");
	strStatus.push_back(L"RIGHT_DASH");


	DirectWrite::BeginDraw();
	{
		RECT rect = { 0,0,500,200 };
		wstring str = L"FPS : " + to_wstring(TIME->GetFrame());
		DirectWrite::RenderText(str, rect);

		Vector2 position = Mouse->GetPosition();
		CAMERA->WCtoVC(position);
		str = L"X= " + to_wstring(position.x);
		str = str + L", Y= " + to_wstring(position.y);

		rect.top = rect.top + 20;
		rect.bottom = rect.bottom + 20;
		DirectWrite::RenderText(str, rect);

		str = L"Status : " + strStatus.at((UINT)m_nState);

		rect.top = rect.top + 20;
		rect.bottom = rect.bottom + 20;
		DirectWrite::RenderText(str, rect);
	}
	DirectWrite::EndDraw();
}



void MegaMan::CreateAnimation()
{
	wstring strImage = L"./Textures/momodora/player/momo_idle.png";

	m_pAnimation = make_shared<Animation>(strImage, g_ShaderFile, GetScale(), GetPosition());


	// LEFT_IDLE
	{
		strImage = L"./Textures/Megaman/Images/Object/x_move.bmp";
	//	CreateClip(strImage, 128, 30+128, 4);

		shared_ptr<AnimationClip>	pClip	 = make_shared<AnimationClip>();
		shared_ptr<Texture>			pTexture = m_pAnimation->GetTexture();
		m_pAnimation->AddClip(pClip);

		for (int i = 0; i < 4; i++)
		{
			int sx = 128 * i + 40;
			int sy = 30+128;
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
		strImage = L"./Textures/Megaman/Images/Object/x_move.bmp";
	//	CreateClip(strImage, 128, 30+128*3, 14);
		shared_ptr<AnimationClip> pClip = make_shared<AnimationClip>();
		shared_ptr<Texture>       pTexture = m_pAnimation->GetTexture();
		m_pAnimation->AddClip(pClip);

		for (int i = 0; i < 14; i++)
		{
			int sx = 128 * i + 40;
			int sy = 30 + 128*3;
			int ex = sx + 50;
			int ey = sy + 60;
			pClip->AddFrame(pTexture, strImage, sx, sy, ex, ey, 0.06f);
		}
	}

	// RIGHT_MOVE
	{
		strImage = L"./Textures/Megaman/Images/Object/x_move.bmp";
	//	CreateClip(strImage, 128, 30+128*2, 14);
		shared_ptr<AnimationClip> pClip = make_shared<AnimationClip>();
		shared_ptr<Texture>       pTexture = m_pAnimation->GetTexture();
		m_pAnimation->AddClip(pClip);

		for (int i = 0; i < 14; i++)
		{
			int sx = 128 * i + 40;
			int sy = 30 + 128*2;
			int ex = sx + 50;
			int ey = sy + 60;
			pClip->AddFrame(pTexture, strImage, sx, sy, ex, ey, 0.06f);
		}
	}

	// LEFT_JUMP
	{
		strImage = L"./Textures/Megaman/MyResource/PLAYER/X_LEFT.png";
		CreateClip(strImage, 100, 620, 8, AnimationClip::EndStay);
	}

	// RIGHT_JUMP
	{
		strImage = L"./Textures/Megaman/MyResource/PLAYER/X_RIGHT.png";
		CreateClip(strImage, 100, 620, 8, AnimationClip::EndStay);
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
		CreateClip(strImage, 100, 920, 8, AnimationClip::EndStay);
	}

	// RIGHT_DASH
	{
		strImage = L"./Textures/Megaman/MyResource/PLAYER/X_RIGHT.png";
		CreateClip(strImage, 100, 920, 8, AnimationClip::EndStay);
	}
}

void MegaMan::CreateClip(wstring strImage, int w, int h, int count, AnimationClip::eState state)
{
	shared_ptr<AnimationClip> pClip = make_shared<AnimationClip>(state);
	shared_ptr<Texture>       pTexture = m_pAnimation->GetTexture();
	m_pAnimation->AddClip(pClip);

	for (int i = 0; i < count; i++)
	{
		int sx = w * i + 20;
		int sy = h;
		int ex = sx + 60;
		int ey = sy + 60;
		pClip->AddFrame(pTexture, strImage, sx, sy, ex, ey, 0.1f);
	}
}

void MegaMan::GroundCheck(Vector2& position)
{
	shared_ptr<class S01_Field> pScene = dynamic_pointer_cast<S01_Field>(SCENEMANAGER->GetCurrentScene());

	auto grounds = pScene->GetGround();
	if (!IsJumping() || m_bFalling == true)
	{
		for (auto& g : grounds)
		{
			if (Collision::HitTest(g, m_pFeet))
			{
				m_bGround = true;
				position.y = g->GetPosition().y + g->GetScale().y / 2 + m_pCollider->GetScale().y / 2;
				break;
			}
			else m_bGround = false;
		}
	}

}

void MegaMan::WallCheck(Vector2& position)
{
	shared_ptr<class S01_Field> pScene = dynamic_pointer_cast<S01_Field>(SCENEMANAGER->GetCurrentScene());

	auto grounds = pScene->GetGround();

	for (auto& g : grounds)
	{
		if (Collision::HitTest(g, m_pWall))
		{
			if (m_nState == LEFT_MOVE && m_bGround == true && (m_Position.x > g->GetPosition().x))
			{
				position.x = g->GetPosition().x + g->GetScale().x / 2 + m_pWall->GetScale().x / 2;
			}
			if (m_nState == RIGHT_MOVE && m_bGround == true && (m_Position.x < g->GetPosition().x))
			{
				position.x = g->GetPosition().x - g->GetScale().x / 2 - m_pWall->GetScale().x / 2;
			}
		}
	}

}

void MegaMan::OnEndPlayerEffect()
{
	switch (m_nState)
	{
	case eState::LEFT_ATTACK: 
		SetState(eState::LEFT_IDLE);
		break;
	case eState::RIGHT_ATTACK:
		SetState(eState::RIGHT_IDLE);
		break;
	}

	m_bCharge = false;
	m_ChargeCountTime = 0.0f;
}
