#include "framework.h"
#include "MegaMan.h"

#include "Scene/S01_Field.h"

#include "Object/PlayerEffect.h"
#include "Object/Bullet.h"
#include "Object/CopyMegaMan.h"
#include "Object/DashMegaMan.h"
#include "Object/ChargeEffect.h"
#include "Object/GrenadeMan.h"
#include "Object/Spike.h"
#include "Object/Boss.h"

#define  LOOK_L  m_nState %2  == 0 
#define  LOOK_R  m_nState %2  == 1 

MegaMan::MegaMan()
{
	CreateAnimation();
	CreateSound();
	

	// 충돌 판정
	m_pCollider			= new Collider(Vector2(80.0f, 136.0f), GetPosition(), 1);
	m_pFeet				= new Collider(Vector2(60.0f, 60.0f), m_Position, 1);
	m_pHead				= new Collider(Vector2(60.0f, 30.0f), m_Position, 1);
	m_pWall				= new Collider(Vector2(85.0f, 80.0f), m_Position, 1);

	// HUD
	m_pHPbar			= make_shared<Texture>(L"./Textures/Megaman/MyResource/UI/HUD_HPBAR_L.png", g_ShaderFile);
	m_pHPbar2			= make_shared<Texture>(L"./Textures/Megaman/MyResource/UI/HUD_CUR_HP_L.png", g_ShaderFile, 1);
	m_pHPbar3 = make_shared<Texture>(L"./Textures/Megaman/MyResource/UI/HUD_CUR_HP_LESS.png", g_ShaderFile, 1);


	// Effects
	for (size_t i = 0; i < 2; i++)
		m_cvChrEffects.push_back(new ChargeEffect());

	for (size_t i = 0; i < 6; i++)
		m_cvEffects.push_back(new PlayerEffect());

	m_pCopyMegaMan = new CopyMegaMan();

	for (size_t i = 0; i < 3; i++)
		m_cvDashMegaMan.push_back(new DashMegaMan());

	// 포지션 벡터
	m_cvPosition.assign(40, Vector2(0, 0));

	for (size_t i = 0; i < 50; i++)
	{
		m_cvBullets.push_back(new Bullet());
	}
}

MegaMan::~MegaMan()
{

}

void MegaMan::SetState(eState state, bool Reset, bool ignore)
{
	if ((IsAttacked() || IsTouchBossRoom())&& !ignore)
		return;

	m_nState = state;
	m_pAnimation->SetPlay((UINT)m_nState, 0, Reset);
	m_pCopyMegaMan->SetState((CopyMegaMan::eState)state, Reset);

	for(auto& a : m_cvDashMegaMan)
		a->SetState((DashMegaMan::eState)state, Reset);
}

void MegaMan::PreUpdate(Vector2& position)
{
	float delta = DELTA;

	if (m_bAttackedLeft)
	{
		SetState(LEFT_ATTACKED);
		SOUND->Play("Damaged");
	}
	else if (m_bAttackedRight)
	{
		SetState(RIGHT_ATTACKED);
		SOUND->Play("Damaged");
	}
	else if (m_bAttackedLeft && m_bAttackedRight)
	{
		if (LOOK_L)			SetState(LEFT_ATTACKED);
		else     			SetState(RIGHT_ATTACKED);
		SOUND->Play("Damaged");
	}
	
	bool BossDeath = dynamic_pointer_cast<S01_Field>(SCENEMANAGER->GetCurrentScene())->GetBoss()->IsDeath();
	bool BossOpening = dynamic_pointer_cast<S01_Field>(SCENEMANAGER->GetCurrentScene())->GetBoss()->IsOpening();

	if (BossDeath)
	{
		m_BossDeathTimer += delta;
		if (m_BossDeathTimer >= 9.5f)
		{
			SetState(LEFT_RETURN);
			SOUND->Play("M_Return");
			m_BossDeathTimer = -60.0f;
		}
	}

	if (!IsAttacked() && !BossDeath && !BossOpening)
	{
		if (!(KEYBOARD->Press(VK_LEFT) && KEYBOARD->Press(VK_RIGHT)))
		{
			// VK_LEFT
			{
				if (KEYBOARD->Down(VK_LEFT) && IsWallStick() && LOOK_R)
				{
					SetState(RIGHT_NOJUMPFALL);
					m_bDashJump = false;
					SetNormalSpeed();
					m_cvEffects[WALL_EFCT]->SetActive(false);
					m_Gravirty = 0.4f;
				}

				if (KEYBOARD->Press(VK_LEFT))
				{
					if ((IsJumping() || IsNoJumpFall()) && m_bGround)
					{
						SOUND->Play("M_Landing");
					}

					if (!IsDashing() && m_bGround)
					{
						SetState(eState::LEFT_MOVE);
						SetNormalSpeed();
					}

					if (!IsWallKick() && LOOK_L)
						position.x = position.x - m_SpeedX * delta;

					if (m_nState == RIGHT_JUMP)				SetState(LEFT_JUMP);
					if (m_nState == RIGHT_NOJUMPFALL)		SetState(LEFT_NOJUMPFALL);

					if (m_bTouchWall && m_bFalling && (IsJumping() || IsNoJumpFall()))
					{
						SetState(LEFT_WALLSTICK);
						SOUND->Play("M_WallStick");
						m_cvEffects[WALL_EFCT]->SetActive(true);
						m_cvEffects[WALL_EFCT]->SetState(PlayerEffect::EFFECT_WALL_L);
						m_cvEffects[WALL_EFCT]->SetScale(Vector2(3.0f, 3.0f));
					}
				}

				if (KEYBOARD->Up(VK_LEFT))
				{
					// LEFT MOVE, LEFT JUMP, LEFT FALL 인경우
					if (m_bGround)
						SetState(eState::LEFT_IDLE);

					if (m_nState == LEFT_WALLSTICK)
					{
						SetState(LEFT_NOJUMPFALL);
						m_cvEffects[WALL_EFCT]->SetActive(false);
						m_Gravirty = 0.4f;

					}
				}
			}

			// VK_RIGHT
			{
				if (KEYBOARD->Down(VK_RIGHT) && IsWallStick() && LOOK_L)
				{
					SetState(LEFT_NOJUMPFALL);
					m_bDashJump = false;
					SetNormalSpeed();
					m_cvEffects[WALL_EFCT]->SetActive(false);
					m_Gravirty = 0.4f;
				}
				if (KEYBOARD->Press(VK_RIGHT))
				{
					if ((IsJumping() || IsNoJumpFall()) && m_bGround)
					{
						SOUND->Play("M_Landing");
					}
					if (!IsDashing() && m_bGround)
					{
						SetState(eState::RIGHT_MOVE);
						SetNormalSpeed();
					}


					if (!IsWallKick() && LOOK_R)
						position.x = position.x + m_SpeedX * delta;

					if (m_nState == LEFT_JUMP)				SetState(RIGHT_JUMP);
					if (m_nState == LEFT_NOJUMPFALL)		SetState(RIGHT_NOJUMPFALL);

					if (m_bTouchWall && m_bFalling && (IsJumping() || IsNoJumpFall()))
					{
						SetState(RIGHT_WALLSTICK);
						SOUND->Play("M_WallStick");
						m_cvEffects[WALL_EFCT]->SetActive(true);
						m_cvEffects[WALL_EFCT]->SetState(PlayerEffect::EFFECT_WALL_R);
						m_cvEffects[WALL_EFCT]->SetScale(Vector2(3.0f, 3.0f));
					}
				}

				if (KEYBOARD->Up(VK_RIGHT))
				{
					// LEFT MOVE, LEFT JUMP, LEFT FALL 인경우
					if (m_bGround)
						SetState(eState::RIGHT_IDLE);

					if (m_nState == RIGHT_WALLSTICK)
					{
						SetState(RIGHT_NOJUMPFALL);
						m_cvEffects[WALL_EFCT]->SetActive(false);
						m_Gravirty = 0.4f;

					}
				}
			}
		}

		


		// Dash
		{
			if (m_bGround && !IsJumping())
			{
				if (KEYBOARD->Down('Z'))
				{

					m_cvEffects[DASH_EFCT]->SetActive(true);
					m_cvEffects[DASHDUST_EFCT]->SetActive(true);
					m_cvEffects[DASH_EFCT]->SetScale(Vector2(2.0f, 2.5f));

					if (LOOK_L)
					{
						SetState(eState::LEFT_DASH);
						m_cvEffects[DASHDUST_EFCT]->SetState(PlayerEffect::EFFECT_DASHDUST_L);
						m_cvEffects[DASHDUST_EFCT]->SetPosition(Vector2(position.x + 90.0f, position.y - 35.0f));

						m_cvEffects[DASH_EFCT]->SetState(PlayerEffect::EFFECT_DASH_L);

						m_pCollider->SetScale(Vector2(80.0f, 75.0f));

					}
					else
					{
						SetState(eState::RIGHT_DASH);
						m_cvEffects[DASHDUST_EFCT]->SetState(PlayerEffect::EFFECT_DASHDUST_R);
						m_cvEffects[DASHDUST_EFCT]->SetPosition(Vector2(position.x - 90.0f, position.y - 35.0f));

						m_cvEffects[DASH_EFCT]->SetState(PlayerEffect::EFFECT_DASH_R);

						m_pCollider->SetScale(Vector2(80.0f, 75.0f));
					}
					SetDashSpeed();
					SOUND->Play("Dash");
				}

				if (KEYBOARD->Press('Z'))
				{
					SetDashSpeed();
					m_DashTime += delta;
					if (m_DashTime >= 0.6f)
					{
						m_cvEffects[DASH_EFCT]->SetActive(false);
						m_cvEffects[DASHDUST_EFCT]->SetActive(false);

						if (LOOK_L)
							SetState(eState::LEFT_MOVE);
						else
							SetState(eState::RIGHT_MOVE);

						SetNormalSpeed();
					}
					else
					{
						if (LOOK_L)
							m_cvEffects[DASH_EFCT]->SetPosition(Vector2(position.x + 100.0f, position.y - 35.0f));
						else
							m_cvEffects[DASH_EFCT]->SetPosition(Vector2(position.x - 100.0f, position.y - 35.0f));


						if (m_bDashJump == true)
						{
							SetNormalSpeed();
							m_bDashJump = false;
						}
					}
				}

				if (KEYBOARD->Up('Z'))
				{
					if (LOOK_L)
						SetState(eState::LEFT_IDLE);
					else SetState(eState::RIGHT_IDLE);
					SetNormalSpeed();
					m_DashTime = 0.0f;
					m_cvEffects[DASH_EFCT]->SetActive(false);

					if (m_bDashJump == true)
						m_bDashJump = false;
				}
			}

		}

		// X Key : 점프
		{
			if (KEYBOARD->Down('X'))
			{
				if (!IsWallStick() && !m_bGround)
					return;

				m_bGround = false;
				m_bFalling = false;
				m_Gravirty = 0.0f;

				if (!IsWallStick())
				{
					if (LOOK_L)	SetState(eState::LEFT_JUMP);
					else		SetState(eState::RIGHT_JUMP);
				}

				else if (IsWallStick())
				{
					m_cvEffects[WALL_KICK]->SetActive(true);
					m_cvEffects[WALL_KICK]->SetScale(Vector2(2.0f, 2.0f));

					if (LOOK_L && m_bTouchWall && lastWallTouch == LEFT)
					{
						SetState(eState::LEFT_WALLKICK);
						m_cvEffects[WALL_KICK]->SetState(PlayerEffect::EFFECT_WALLKICK_L);
						m_cvEffects[WALL_KICK]->SetPosition(m_Position.x - 30.0f, m_Position.y - 10.0f);
					}
					else if (LOOK_R  && m_bTouchWall && lastWallTouch == RIGHT)
					{
						SetState(eState::RIGHT_WALLKICK);
						m_cvEffects[WALL_KICK]->SetState(PlayerEffect::EFFECT_WALLKICK_L);
						m_cvEffects[WALL_KICK]->SetPosition(m_Position.x + 30.0f, m_Position.y - 10.0f);

					}

					m_cvEffects[WALL_EFCT]->SetActive(false);
				}

				if (KEYBOARD->Press('Z'))
				{
					m_bDashJump = true;
					SetDashSpeed();
				}
				else
				{
					m_bDashJump = false;
					SetNormalSpeed();
				}

				SOUND->Play("JUMP");
				SOUND->Play("JUMP_VOICE");
			}
		}

		// ATTACK
		{
			if (KEYBOARD->Press('C'))
			{
				m_ChargeCountTime += delta;
				// 1st Charge된 순간
				if ((m_ChargeCountTime >= 1.8f))
				{
					m_bChargeFull = true;
					m_bCharge = false;

					m_cvChrEffects[BLUE_CHARGE]->SetState(ChargeEffect::EFFECT_CHARGE_BODY_GREEN);
					m_cvChrEffects[GREEN_CHARGE]->SetState(ChargeEffect::EFFECT_CHARGE_GREEN);

					m_cvChrEffects[BLUE_CHARGE]->SetScale(Vector2(2.0f, 2.0f));

					SOUND->Play("CHARGING");
				}
				else if ((m_ChargeCountTime >= 0.3f))
				{
					m_bCharge = true;

					m_cvChrEffects[BLUE_CHARGE]->SetState(ChargeEffect::EFFECT_CHARGE_BODY);
					m_cvChrEffects[GREEN_CHARGE]->SetState(ChargeEffect::EFFECT_CHARGE);

					m_cvChrEffects[BLUE_CHARGE]->SetActive(true);
					m_cvChrEffects[GREEN_CHARGE]->SetActive(true);

					m_cvChrEffects[BLUE_CHARGE]->SetScale(Vector2(1.0f, 1.0f));
					m_cvChrEffects[GREEN_CHARGE]->SetScale(Vector2(2.0f, 2.0f));

					SOUND->Play("ChargeStart");
				}
			}
			if (KEYBOARD->Up('C'))
			{
				if (m_bChargeFull || m_bCharge)
				{
					if (m_cvChrEffects[BLUE_CHARGE]->IsActive())
						m_cvChrEffects[BLUE_CHARGE]->SetActive(false);
					if (m_cvChrEffects[GREEN_CHARGE]->IsActive())
						m_cvChrEffects[GREEN_CHARGE]->SetActive(false);
				}

				FireBullet();

				m_bCharge = m_bChargeFull = false;
				m_ChargeCountTime = 0.0f;
				m_cvChrEffects[GREEN_CHARGE]->SetActive(false);
			}
		}
	}


	// 1. 벽 붙기 2. 자유 낙하 만들기
	if (!m_bGround )
	{
		if (IsWallStick())
		{
			position.y -= 240.0f * delta;
		}

		else if (!IsJumping() && !IsWallKick())
		{
			if (LOOK_L && !IsNoJumpFall())
			{
				if(!m_bWalkLine)
					SetState(LEFT_NOJUMPFALL);
				m_Gravirty = 48.0f * delta;
			}
			else if (LOOK_R && !IsNoJumpFall())
			{
				if (!m_bWalkLine)
					SetState(RIGHT_NOJUMPFALL);
				m_Gravirty = 48.0f * delta;
			}

			m_bFalling = true;

			m_Gravirty += 48.0f * delta;
			if (m_Gravirty > m_SpeedY * delta)
				m_Gravirty = m_SpeedY * delta;

			if (!m_bWalkLine)
				position.y -= m_Gravirty;
			else 
				position.y -= 480.0f * delta;
		}

		// 2. Juming이거나 낙하인 상태에서 떨어지게 하기
		else if (IsJumping() || IsWallKick())
		{
			//낙하
			position.y += (m_SpeedY - m_Gravirty) * delta;

			//낙하 판정
			if (m_Gravirty >= m_SpeedY)
				m_bFalling = true;

			//중력		
			m_Gravirty += 48.0f; 
			if (m_Gravirty > 2.0 * m_SpeedY)
				m_Gravirty = 2.0 * m_SpeedY;
		}

		else if (IsAttacked())
		{
			m_bFalling = true;
			m_Gravirty = 0.0f;
			m_Gravirty += 48.0f * delta;
			if (m_Gravirty > m_SpeedY * delta)
				m_Gravirty = m_SpeedY * delta;
			position.y -= m_Gravirty;
		}
	}



	// State를 Check
	if (!IsDashing())
	{
		m_pCollider->SetScale(Vector2(80.0f, 136.0f));
		m_pCollider->SetPosition(GetPosition());
	}

	switch (m_nState)
	{
	case LEFT_MOVE:
	case RIGHT_MOVE:
		if (m_bGround)
		{
			int currentCount = cvPosCount;
			int lastCount = currentCount - 1;

			if (lastCount < 0)
				lastCount = 39;

			if (m_cvPosition[currentCount] == m_cvPosition[lastCount])
			{
				if (LOOK_L)
					SetState(LEFT_IDLE);
				else
					SetState(RIGHT_IDLE);
			}
		}
		break;
	case LEFT_JUMP:
	case RIGHT_JUMP:
		if (m_bGround)
		{
			if (LOOK_L)
				SetState(LEFT_FALL);
			else
				SetState(RIGHT_FALL);
			
			m_bDashJump = false;
			SetNormalSpeed();
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
			SOUND->Play("M_Landing");
		}
		break;
	case LEFT_ATTACK:
	case RIGHT_ATTACK:
	case ATTACK_CHARGE_L:
	case ATTACK_CHARGE_R:		
		m_AttackTime += delta;
		if (m_AttackTime >= 0.4f)
		{
			if (LOOK_L)		SetState(LEFT_IDLE);
			else			SetState(RIGHT_IDLE);
			m_AttackTime = 0.0f;
		}
		break;

	case LEFT_NOJUMPFALL:
	case RIGHT_NOJUMPFALL:
		m_bFalling = true;
		if (m_bGround)
		{
			if (LOOK_L)
				SetState(LEFT_FALL);
			else
				SetState(RIGHT_FALL);
			m_bDashJump = false;
			SOUND->Play("M_Landing");
		}
		break;

	case LEFT_WALLSTICK:
	case RIGHT_WALLSTICK:
		if (!m_bTouchWall)
		{
			if (LOOK_L)
				SetState(LEFT_NOJUMPFALL);
			else
				SetState(RIGHT_NOJUMPFALL);
			m_bDashJump = false;
			m_cvEffects[WALL_EFCT]->SetActive(false);
		}
		break;

	case LEFT_WALLKICK:
	case RIGHT_WALLKICK:
		m_WallKickTime += delta;
		if (m_WallKickTime >= 0.15f)
		{
			if (LOOK_L)		SetState(LEFT_JUMP);
			else			SetState(RIGHT_JUMP);
			m_WallKickTime = 0.0f;
			m_bFalling = true;
		}
		else
		{
			if (LOOK_L)
				position.x = position.x + m_SpeedX * delta;
			else
				position.x = position.x - m_SpeedX * delta;
		}
		break;

	case LEFT_ATTACKED:
	case RIGHT_ATTACKED:
		m_AttackedTime += delta;
		if (m_AttackedTime >= 0.4f)
		{
			if (m_bGround)
			{
				if (LOOK_L)		SetState(LEFT_IDLE, false, true);
				else			SetState(RIGHT_IDLE, false, true);
			}
			else
			{

				if (LOOK_L)		SetState(LEFT_NOJUMPFALL, false, true);
				else			SetState(RIGHT_NOJUMPFALL, false, true);
				m_bFalling = true;
			}

			m_bAttackedLeft = false;
			m_bAttackedRight = false;
			m_AttackedTime = 0.0f;
		}
		else
		{
			if (!m_bTouchWall)
			{
				if (LOOK_L)			position.x += m_SpeedX / 4 * delta;
				else				position.x -= m_SpeedX / 4 * delta;
			}
			m_bInvincible = true;
		}
		break;
	}

	// Fire Timer
	if(m_bFireState)
		m_FireStateTime += delta;
	
	if (m_FireStateTime >= 0.25f)
	{
		m_FireStateTime = 0.0f;
		m_bFireState = false;
	}

	if (m_bInvincible)
		m_InvincibleTime += delta;

	if (m_InvincibleTime >= 1.5f)
	{
		m_InvincibleTime = 0.0f;
		m_bInvincible = false;
	}

}

void MegaMan::PreUpdateKeyInput()
{
}

void MegaMan::Update()
{
	Vector2 position = GetPosition();

	// KeyBoard
	if (!IsTouchBossRoom())
	{
		PreUpdate(position);
		HeadCheck(position);
		GroundCheck(position);
		EnemyCheck(position);
	}

	WallCheck(position);
	SetPosition(position);


	// Animation Update
	m_pAnimation->SetPosition(GetPosition());
	m_pAnimation->SetScale(GetScale());
	m_pAnimation->Update();

	// Collider  Update  
	{
		if (IsDashing())
			m_pCollider->SetPosition(position - Vector2(0.0f, 30.0f));
		else
			m_pCollider->SetPosition(GetPosition());

		m_pCollider->Update();

		m_pHead->SetPosition(GetPosition() + Vector2(0.0f, 55.0f));
		m_pHead->Update();

		m_pFeet->SetPosition(GetPosition() - Vector2(0.0f, 40.0f));
		m_pFeet->Update();

		m_pWall->SetPosition(GetPosition());
		m_pWall->Update();
	}


	UpdateBullet();
	UpdateEffect();

}

void MegaMan::Render()
{

	if ( (IsDashing() || m_bDashJump) && !m_bTouchWall )
	{
		for (auto& a : m_cvDashMegaMan)
			a->Render();
	}
	
	// MM
	if(m_bFireState)
		m_pCopyMegaMan->Render();
	else
	{
		// 무적 일 때 깜빡임 
		if (m_bInvincible)
		{
			m_BlinkTime += DELTA;
			if (m_BlinkTime <= 0.1f && m_BlinkTime >= 0.05f)
				m_pAnimation->Render();
			else if (m_BlinkTime > 0.1f)
				m_BlinkTime = 0.0f;
		}
		else
			m_pAnimation->Render();
	}


	// Collider Render
	if(m_bDebug)
	{
		for (auto& a : m_cvBullets)
			a->SetDebug(true);		
		m_pCollider->UpdateColor(Color(1.0f, 0.0f, 0.0f, 1.0f));
		m_pCollider->Render();
		m_pHead->Render();
		m_pFeet->Render();
		m_pWall->Render();
	}
	else
	{
		for (auto& a : m_cvBullets)
			a->SetDebug(false);
	}



	// Effects
	for (auto& p : m_cvEffects)
	{
		if (p->IsActive())
			p->Render();
	}
	for (auto& p : m_cvChrEffects)
	{
		if (p->IsActive())
			p->Render();
	}

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
	strStatus.push_back(L"LEFT_NOJUMPFALL");
	strStatus.push_back(L"RIGHT_NOJUMPFALL");
	strStatus.push_back(L"LEFT_WALLSTICK");
	strStatus.push_back(L"RIGHT_WALLSTICK");
	strStatus.push_back(L"LEFT_WALLKICK");
	strStatus.push_back(L"RIGHT_WALLKICK");
	strStatus.push_back(L"LEFT_ATTACKED");
	strStatus.push_back(L"RIGHT_ATTACKED");
	strStatus.push_back(L"LEFT_HEAVYATTACKED");
	strStatus.push_back(L"RIGHT_HEAVYATTACKED");
	strStatus.push_back(L"LEFT_RETURN");
	strStatus.push_back(L"RIGHT_RETURN");

	if (m_bDebug)
	{
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



	// Bullet
	for (auto& p : m_cvBullets)
		p->Render();


	Vector2 position = { 0, 0 };

	position = Vector2(85.0f, 300.0f);
	CAMERA->WCtoVC(position);
	m_pHPbar->SetPosition(position);
	m_pHPbar->SetScale(Vector2(3.0f, 3.0f));
	m_pHPbar->Update();
	m_pHPbar->Render();


	float Y = (m_nHP * 4.45f) / m_OriginHP;
	position = Vector2(67.72f, 343.5f);
	CAMERA->WCtoVC(position);
	if ((float)m_nHP / m_OriginHP <= 0.4f)
	{
		m_pHPbar3->SetPosition(position);
		m_pHPbar3->SetScale(Vector2(3.7f, Y));
		m_pHPbar3->Update();
		m_pHPbar3->Render();
	}
	else
	{
		m_pHPbar2->SetPosition(position);
		m_pHPbar2->SetScale(Vector2(3.7f, Y));
		m_pHPbar2->Update();
		m_pHPbar2->Render();
	}
}

void MegaMan::FireBullet()
{
	auto pBullet = m_cvBullets[m_LastBullet];

	if (!m_bCharge && !m_bChargeFull)
	{
		SOUND->Play("bullet");
		pBullet->SetState(Bullet::L_BULLET);
	}
	else if (m_bCharge)
	{
		SOUND->Stop("ChargeStart");
		SOUND->Stop("CHARGING");
		SOUND->Play("SEMIBUSTER");
		pBullet->SetState(Bullet::L_CHARGE);		
	}
	else if (m_bChargeFull)
	{
		SOUND->Stop("ChargeStart");
		SOUND->Stop("CHARGING");
		SOUND->Play("BUSTER");
		pBullet->SetState(Bullet::L_FULL_CHARGE);
	}

	pBullet->SetActive(true);
	pBullet->Reset();

	if(m_nState % 2 ==0)
		pBullet->SetPosition(Vector2(GetPosition().x - 40.0f, GetPosition().y + 25.0f));
	else
		pBullet->SetPosition(Vector2(GetPosition().x + 40.0f, GetPosition().y + 25.0f));

	pBullet->SetDirection((UINT)(m_nState) % 2);

	// 벽 붙는 중 예외 조치
	if (m_nState == LEFT_WALLSTICK)
	{
		pBullet->SetPosition(Vector2(GetPosition().x + 40.0f, GetPosition().y + 25.0f));
		pBullet->SetDirection(RIGHT);
	}
	else if (m_nState == RIGHT_WALLSTICK)
	{
		pBullet->SetPosition(Vector2(GetPosition().x - 40.0f, GetPosition().y + 25.0f));
		pBullet->SetDirection(LEFT);
	}

	pBullet->SetScale(GetScale() * 1.1f);
	pBullet->Update();
//	m_cvBullets.push_back(pBullet);

	m_bFireState = true;
	m_FireStateTime = 0.0f;
	m_AttackTime = 0.0f;

	if (IsIdle() || m_nState == LEFT_ATTACK || m_nState == RIGHT_ATTACK 
		|| m_nState == ATTACK_CHARGE_L || m_nState == ATTACK_CHARGE_R)
	{
		if (m_bChargeFull)
		{
			if (LOOK_L)		SetState(ATTACK_CHARGE_L);
			else			SetState(ATTACK_CHARGE_R);
		}
		else
		{
			if (LOOK_L)		SetState(LEFT_ATTACK, true);
			else			SetState(RIGHT_ATTACK, true);
		}
	}

	m_LastBullet += 1;
	if (m_LastBullet > 49)
		m_LastBullet = 0;

}

void MegaMan::Reset()
{
}




void MegaMan::CreateAnimation()
{
	wstring strImage = L"./Textures/Megaman/Images/Object/x_move.bmp";

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
		strImage = L"./Textures/Megaman/MyResource/PLAYER/X_LEFT.png";
		CreateClip(strImage, 100, 400, 14, AnimationClip::Loop, 2, 0.04f);
	}

	// RIGHT_MOVE
	{
		strImage = L"./Textures/Megaman/MyResource/PLAYER/X_RIGHT.png";
		CreateClip(strImage, 100, 400, 14, AnimationClip::Loop, 2, 0.04f);
	}

	// LEFT_JUMP
	{
		strImage = L"./Textures/Megaman/MyResource/PLAYER/X_LEFT.png";
		CreateClip(strImage, 100, 600, 8, AnimationClip::EndStay);
	}

	// RIGHT_JUMP
	{
		strImage = L"./Textures/Megaman/MyResource/PLAYER/X_RIGHT.png";
		CreateClip(strImage, 100, 600, 8, AnimationClip::EndStay);
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
		CreateClip(strImage, 100, 895, 8, AnimationClip::EndStay);
	}

	// RIGHT_DASH
	{
		strImage = L"./Textures/Megaman/MyResource/PLAYER/X_RIGHT.png";
		CreateClip(strImage, 100, 895, 8, AnimationClip::EndStay);
	}

	// LEFT_NOJUMPFALL
	{
		strImage = L"./Textures/Megaman/MyResource/PLAYER/X_LEFT.png";
		CreateClip(strImage, 100, 600, 1, AnimationClip::EndStay, 7);
	}

	// RIGHT_NOJUMPFALL
	{
		strImage = L"./Textures/Megaman/MyResource/PLAYER/X_RIGHT.png";
		CreateClip(strImage, 100, 600, 1, AnimationClip::EndStay, 7);
	}

	// LEFT_WALLSTICK
	{
		strImage = L"./Textures/Megaman/MyResource/PLAYER/X_LEFT.png";
		CreateClip(strImage, 100, 1100, 4, AnimationClip::EndStay, 0, 0.05f);
	}

	// RIGHT_WALLSTICK
	{
		strImage = L"./Textures/Megaman/MyResource/PLAYER/X_RIGHT.png";
		CreateClip(strImage, 100, 1100, 4, AnimationClip::EndStay, 0, 0.05f);
	}

	// LEFT_WALLKICK
	{
		strImage = L"./Textures/Megaman/MyResource/PLAYER/X_LEFT.png";
		CreateClip(strImage, 100, 1300, 2, AnimationClip::EndStay);
	}

	// RIGHT_WALLKICK
	{
		strImage = L"./Textures/Megaman/MyResource/PLAYER/X_RIGHT.png";
		CreateClip(strImage, 100, 1300, 2, AnimationClip::EndStay);
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
		CreateClip(strImage, 100, 1800, 6, AnimationClip::EndStay, 4);
	}

	// RIGHT_RETURN
	{
		strImage = L"./Textures/Megaman/MyResource/PLAYER/X_RIGHT.png";
		CreateClip(strImage, 100, 1800, 10, AnimationClip::EndStay, 0.05f);
	}

}

void MegaMan::CreateSound()
{

	
}

void MegaMan::UpdateBullet()
{
	// bullet
	for (UINT i = 0; i < m_cvBullets.size(); i++)
	{
		if (m_cvBullets.size() >= 100)
		{
			delete m_cvBullets[0];
			m_cvBullets.erase(m_cvBullets.begin());
			m_cvBullets.shrink_to_fit();
		}
		else
		{
			m_cvBullets[i]->Update();
		}
	}
}

void MegaMan::UpdateEffect()
{
	Vector2 position = GetPosition();

	// effect
	m_cvChrEffects[BLUE_CHARGE]->SetPosition(position);
	m_cvChrEffects[GREEN_CHARGE]->SetPosition(position);
	
	if (!m_cvEffects[WALL_KICK]->IsPlay())
		m_cvEffects[WALL_KICK]->SetActive(false);

	if(LOOK_L)
		m_cvEffects[WALL_EFCT]->SetPosition(position.x - 35.0f, position.y - 45.0f);
	else
		m_cvEffects[WALL_EFCT]->SetPosition(position.x + 35.0f, position.y - 45.0f);
	for (auto& p : m_cvEffects)
	{
		if (p->IsActive())
			p->Update();
	}
	for (auto& p : m_cvChrEffects)
	{
		if (p->IsActive())
			p->Update();
	}

	// Fire Megaman
	m_pCopyMegaMan->SetPosition(GetPosition());
	m_pCopyMegaMan->SetScale(GetScale());
	m_pCopyMegaMan->Update();


	// Dash Megaman

	cvPosCount += 1;
	if (cvPosCount >= 40)		cvPosCount = 0;
	m_cvPosition[cvPosCount] = m_Position;

	Vector2 dashPos = { 0, 0 };

	const int dash_1 = 2;
	const int dash_2 = 6;
	const int dash_3 = 10;

	if (cvPosCount - dash_1 < 0)		dashPos = m_cvPosition[(long long)40 + cvPosCount - dash_1];
	else						dashPos = m_cvPosition[(long long)cvPosCount - dash_1];
	m_cvDashMegaMan[2]->SetPosition(dashPos);
	m_cvDashMegaMan[2]->GetAnimation()->GetTexture()->UpdateColorBuffer(2);

	if (cvPosCount - dash_2 < 0)	dashPos = m_cvPosition[(long long)40 + cvPosCount - dash_2];
	else						dashPos = m_cvPosition[(long long)cvPosCount - dash_2];
	m_cvDashMegaMan[1]->SetPosition(dashPos);
	m_cvDashMegaMan[1]->GetAnimation()->GetTexture()->UpdateColorBuffer(3);

	if (cvPosCount - dash_3 < 0)	dashPos = m_cvPosition[(long long)40 + cvPosCount - dash_3];
	else						dashPos = m_cvPosition[(long long)cvPosCount - dash_3];
	m_cvDashMegaMan[0]->SetPosition(dashPos);
	m_cvDashMegaMan[0]->GetAnimation()->GetTexture()->UpdateColorBuffer(1);

	for (auto& a : m_cvDashMegaMan)
	{
		a->SetScale(GetScale());
		a->Update();
	}
}

void MegaMan::CreateClip(wstring strImage, int w, int h, int count, AnimationClip::eState state, int start, float speed)
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

void MegaMan::GroundCheck(Vector2& position)
{
	shared_ptr<class S01_Field> pScene = dynamic_pointer_cast<S01_Field>(SCENEMANAGER->GetCurrentScene());

	auto grounds = pScene->GetGround();
	auto pLine = pScene->GetLines()->GetLine();

	bool groundCheck = false;
	bool lineCheck = false;

	Vector2 size = m_pCollider->GetScale();
	Vector2 AP2 = Vector2(position.x, position.y - size.y * 0.5f);
	Vector2 AP1 = position;

	if (!IsJumping() || m_bFalling == true || IsAttacked() || (m_nState >= 8 && m_nState <= 11))
	{
		for (size_t i = 2; i < grounds.size(); i++)
		{
			if (Collision::HitTest(grounds[i], m_pFeet))
			{
				groundCheck = true;
				m_cvEffects[WALL_EFCT]->SetActive(false);
				position.y = grounds[i]->GetPosition().y + grounds[i]->GetScale().y / 2 + 136.0f / 2;
				break;
			}
		}
		// Line
		for (int i = 0; i < 2; i++)
		{
			Vector2 BP1 = pLine[i].first;
			Vector2 BP2 = pLine[i].second;
			Vector2 result;

			if (Collision::LineInterscetion(AP1, AP2, BP1, BP2, result))
			{
				lineCheck = true;
				m_bWalkLine = true;
				position.y = result.y + size.y * 0.49f;
				break;
			}
		}
	}
	if (groundCheck)
		m_bWalkLine = false;

	if (groundCheck || lineCheck)
		m_bGround = true;
	else 
		m_bGround = false;

}

void MegaMan::WallCheck(Vector2& position)
{
	shared_ptr<class S01_Field> pScene = dynamic_pointer_cast<S01_Field>(SCENEMANAGER->GetCurrentScene());

	auto grounds = pScene->GetGround();

	bool noTouch = false;

	if (IsWallKick())
		return;

	for (size_t i = 0; i < grounds.size(); i++)
	{
		if (Collision::HitTest(grounds[i], m_pWall))
		{
			if (i == grounds.size() - 1)
			{
				if (LOOK_R)
				{
					SetTouchBossRoom(true);
					if (pScene->GetDoorPtr()->GetClipNo() == 0)
					{
						pScene->GetDoorPtr()->SetPlay(1);
						SOUND->Play("DoorOpen");
					}					
					
					m_DoorOpen += DELTA;
					if (m_DoorOpen > 0.65f)
					{
						position.x += 85.0f * DELTA;
					}
				}
				else if ((LOOK_L || m_nState == RIGHT_ATTACKED) && (m_Position.x > grounds[i]->GetPosition().x))
				{
					position.x =
						grounds[i]->GetPosition().x
						+ grounds[i]->GetScale().x / 2
						+ m_pWall->GetScale().x / 2;
					m_bTouchWall = true;
					noTouch = true;
				}
			}
			else
			{
				if ((LOOK_L || m_nState == RIGHT_ATTACKED) && (m_Position.x > grounds[i]->GetPosition().x))
				{
					position.x =
						grounds[i]->GetPosition().x
						+ grounds[i]->GetScale().x / 2
						+ m_pWall->GetScale().x / 2;
					lastWallTouch = LEFT;
				}
				if ((LOOK_R || m_nState == LEFT_ATTACKED) && (m_Position.x < grounds[i]->GetPosition().x))
				{
					position.x =
						grounds[i]->GetPosition().x
						- grounds[i]->GetScale().x / 2
						- m_pWall->GetScale().x / 2;
					lastWallTouch = RIGHT;
				}
				m_bTouchWall = true;				
				noTouch = true;
			}
		}
		if (!Collision::HitTest(grounds[grounds.size() - 1], m_pWall) && IsTouchBossRoom())
		{
			SetTouchBossRoom(false);
			if (pScene->GetDoorPtr()->GetClipNo() == 1)
				pScene->GetDoorPtr()->SetPlay(2);
			if (!m_bEnterBossRoom)
			{
				SOUND->Play("BossMeet");
				SOUND->Play("DoorClose");
				m_bEnterBossRoom = true;
			}
		}
	}

	if(!noTouch)		m_bTouchWall = false;
}

void MegaMan::HeadCheck(Vector2& position)
{
	shared_ptr<class S01_Field> pScene = dynamic_pointer_cast<S01_Field>(SCENEMANAGER->GetCurrentScene());

	auto grounds = pScene->GetGround();
	if ((IsJumping() || IsWallKick()) && m_Gravirty < m_SpeedY)
	{
		for (size_t i = 2; i < grounds.size(); i++)
		{
			if (Collision::HitTest(grounds[i], m_pHead))
			{
				m_bFalling = true;
				m_bHeadTouch = true;
				m_Gravirty = m_SpeedY;
				position.y = grounds[i]->GetPosition().y - grounds[i]->GetScale().y / 2 - 136.0f / 2;
				break;
			}
			else m_bHeadTouch = false;
		}
	}

}

void MegaMan::EnemyCheck(Vector2& position)
{
	if (m_bInvincible)
		return;

	shared_ptr<class S01_Field> pScene = dynamic_pointer_cast<S01_Field>(SCENEMANAGER->GetCurrentScene());

	auto Gmans = pScene->GetGrenadeMans();
	bool noTouch = false;

	for (auto& g : Gmans)
	{
		if (!g->IsActive())
			continue;

		if (Collision::HitTest(g->GetCollider(), m_pCollider))
		{			
			if (g->GetPosition().x > m_pCollider->GetPosition().x)
				m_bAttackedRight = true;
			else if (g->GetPosition().x < m_pCollider->GetPosition().x)
				m_bAttackedLeft = true;
			else
			{
				m_bAttackedRight = true;
				m_bAttackedLeft = true;
			}
			if (!IsInvincible())
				m_nHP -= 5;
		}
	}
	auto spikes = pScene->GetSpikes();

	for (auto& g : spikes)
	{
		if (!g->IsActive())
			continue;

		if (Collision::HitTest(g->GetCollider(), m_pCollider))
		{
			if (g->GetPosition().x > m_pCollider->GetPosition().x)
				m_bAttackedRight = true;
			else if (g->GetPosition().x < m_pCollider->GetPosition().x)
				m_bAttackedLeft = true;
			else
			{
				m_bAttackedRight = true;
				m_bAttackedLeft = true;
			}
			if (!IsInvincible())
				m_nHP -= 5;
		}
	}

	auto Boss = pScene->GetBoss();

	if (Boss->IsActive() && Boss->IsRender() && !Boss->IsDodge() && !Boss->IsDeath())
	{
		if (Collision::HitTest(Boss->GetCollider(), m_pCollider))
		{
			if (Boss->GetPosition().x > m_pCollider->GetPosition().x)
				m_bAttackedRight = true;
			else if (Boss->GetPosition().x < m_pCollider->GetPosition().x)
				m_bAttackedLeft = true;
			else
			{
				m_bAttackedRight = true;
				m_bAttackedLeft = true;
			}
			if (!IsInvincible())
			{
				if (Boss->IsBackAtk())
					m_nHP -= 10;
				else
					m_nHP -= 5;
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
