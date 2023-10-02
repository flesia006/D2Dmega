#include "framework.h"
#include "Boss.h"
#include "Scene/S01_Field.h"

#include "Object/BossBullet.h"
#include "Object/MegaMan.h"
#include "Object/PlayerEffect.h"
#include "Object/BossEffect.h"

#define  LOOK_L  m_nState %2  == 0 
#define  LOOK_R  m_nState %2  == 1 

#define  ROT_L  GetRotation() == Vector3(0.0f, 0.0f, 0.0f)
#define  ROT_R  GetRotation() == Vector3(0.0f, 180.0f, 0.0f) 

#define  SET_L  SetRotation(Vector3(0.0f, 0.0f, 0.0f))
#define  SET_R  SetRotation(Vector3(0.0f, 180.0f, 0.0f))

#define CENTER  6025.5f

Boss::Boss()
{
	SetActive(false);

	m_pCollider = new Collider(Vector2(110.0f, m_tall), m_Position, 1);
//	m_pWall = new Collider(Vector2(120.0f, m_tall - 50.0f), m_Position, 1);

	m_pEffect = new PlayerEffect();
	CreateAnimation();

	pMM = dynamic_cast<MegaMan*>(OBJECTMANAGER->FindObject("MEGAMAN"));

	m_pDisAppearEffect = new BossEffect();
	m_pDisAppearEffect->SetState(BossEffect::eState::DISAPPEAR);

	for (size_t i = 0; i < 5; i++)
		m_cvEffects.push_back(new BossEffect());

	// 포지션 벡터
	m_cvPosition.assign(40, Vector2(0, 0));

	for (size_t i = 0; i < 50; i++)
		m_cvBullets.push_back(new BossBullet());

	m_pHPbar = make_shared<Texture>(L"./Textures/Megaman/resources/Enemy/Boss/HUD_BOSS_HPAR.bmp", g_ShaderFile);
	m_pHPbar2 = make_shared<Texture>(L"./Textures/Megaman/MyResource/UI/HUD_CUR_HP_L.png", g_ShaderFile, 1);
	m_pHPbar3 = make_shared<Texture>(L"./Textures/Megaman/MyResource/UI/HUD_CUR_HP_LESS.png", g_ShaderFile, 1);
}


Boss::~Boss()
{
}

void Boss::PostRender()
{
	if (m_bHPRender)
	{
		Vector2 position = { 0, 0 };

		position = Vector2(1250.0f, 270.0f);
		CAMERA->WCtoVC(position);
		m_pHPbar->SetPosition(position);
		m_pHPbar->SetScale(Vector2(3.0f, 3.0f));
		m_pHPbar->Update();
		m_pHPbar->Render();

		float Y = (m_Hp * 5.95f) / m_OriginHP;
		position = Vector2(1250.5f, 327.7f);
		CAMERA->WCtoVC(position);
		if ((float)m_Hp / m_OriginHP <= 0.4f)
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

}

void Boss::SetState(eState value)
{
	m_nState = value; 
	m_pAnimation->SetPlay((UINT)m_nState);

	for (auto& a : m_cvEffects)
		a->SetState((BossEffect::eState)value);
}

void Boss::PreUpdate(Vector2& position)
{
	float delta = DELTA;
	random_device rd;
	uniform_int_distribution<int> dist(-250, 250);

	if (IsDeath() && IsActive())
	{		
		SetState(Boss::DEFEAT);		

		for (auto& a : m_cvBullets)
			a->SetActive(false);

		SOUND->Stop("Boss_spark");
		SOUND->Stop("boss");
		SOUND->Stop("Boss_Lightning");
		SOUND->Stop("Boss_Thunder_atk");
		SOUND->Stop("Boss_bigwave");
		SOUND->Stop("Boss_BigWave2");
		

		m_PatternTimer += delta;
		if (m_PatternTimer < 7.0f)
		{
			m_AttackedTime += delta;
			if (m_AttackedTime < 0.1f)
			{
				m_pAnimation->GetTexture()->UpdateColorBuffer(1, Color(1, 1, 1, 1), 0.4f);
				m_pEffect->Reset();

				if (m_Blinking)
					m_Blinking = false;				
			}
			else if (m_AttackedTime >= 0.1f && m_AttackedTime < 0.3f)
			{
				m_pAnimation->GetTexture()->UpdateColorBuffer(1, Color(1, 1, 1, 1), -0.4f);

				if (!m_Blinking)
				{
					m_pEffect->SetActive(true);
					int x = dist(rd);
					int y = dist(rd);
					m_pEffect->SetState(PlayerEffect::ENEMYBOMB);
					m_pEffect->SetPosition(GetPosition() + Vector2(x, y));
					m_pEffect->SetScale(Vector2(3.5f, 3.5f));
					m_Blinking = true;					
				}
			}
			else
			{
				m_AttackedTime = 0.0f;
				SOUND->Play("Exlposion");
			}
		}
		else if(m_PatternTimer >= 7.0f && m_PatternTimer < 10.0f )
		{			
			m_bHPRender = false;		
			m_pEffect->Reset();
			m_pEffect->SetActive(true);
			m_pEffect->SetState(PlayerEffect::EXPLOSION);
			m_pEffect->SetPosition(GetPosition());
			m_pEffect->SetScale(Vector2(4.5f, 4.5f));
			SOUND->Play("Exlposion");
			SOUND->Play("Stage_Clear");
			SetActive(false);			
			m_PatternTimer = 0.0f;
		}
	}

	if (!IsActive() || IsDeath())
		return;


	

	if (m_bAttacked)
	{
		m_pAnimation->GetTexture()->UpdateColorBuffer(1, Color(1, 1, 1, 1), 0.3f);
	}

	// 1. 벽 붙기 2. 자유 낙하 만들기
	if (!m_bGround && m_Sequence != BIG_WAVE)
	{
		position.y -= 560 * delta;
	}


	//Timer

	if (m_bAttacked)
		m_AttackedTime += delta;

	if (m_AttackedTime >= 0.7f)
	{
		m_AttackedTime = 0.0f;
		m_bAttacked = false;
		m_pAnimation->GetTexture()->UpdateColorBuffer(1, Color(1, 1, 1, 1), -0.3f);
	}

	// Sequence Pattern
	switch (m_Sequence)
	{
	case 0:
		Opening(position);
		break;
	case 1:
		FireThreeWave(position);
		break;
	case 2:
		BackAttack(position);
		break;
	case 3:
		ThunderAttack(position);
		break;
	case 4:
		BigWaveAttack(position);
		break;
	case 5:
		Dash(position);
		break;
	case 6:
		Disappear(position);
		break;
	}


}

void Boss::Update()
{
	Vector2 position = GetPosition();
	UpdateEffect(position);

	if (!IsActive())
		return;

	PreUpdate(position);
	UpdateBullet();

	if (GetPosition().y < -600.0f)
	{
		SetActive(false);
		SetHP(0);
	}

	// KeyBoard
	GroundCheck(position);
	SetPosition(position);

	// Animation Update
	UPDATE(m_pAnimation);

	// Collider  Update  
	m_pCollider->SetPosition(GetPosition());
	m_pCollider->Update();

	if (m_bDisappear)
		m_pDisAppearEffect->Update();

}


void Boss::Render()
{
	m_pEffect->Render();

	if (!IsActive())
		return;

	if (m_bRenderDash)
	{
		for (auto& a : m_cvEffects)
			a->Render();
	}

	if (m_bRender)
		m_pAnimation->Render();

//	// Bullet
	for (auto& p : m_cvBullets)
	{
		if(p->IsActive())
			p->Render();
	}



	if (m_bDisappear)
		m_pDisAppearEffect->Render();
	if (m_bDebug)
	{
		m_pCollider->Render(); 
		for (auto a : m_cvBullets)
			a->SetDebug(true);
	}
	else
	{
		for (auto a : m_cvBullets)
			a->SetDebug(false);
	}
}

void Boss::Reset()
{
	SetActive(false);
	m_pAnimation->SetPlay(GetOriginState());
	SetPosition(GetOriginPos());
	m_pAnimation->SetPosition(GetOriginPos());
	m_pCollider->SetPosition(GetOriginPos());
	m_pWall->SetPosition(GetOriginPos());
	for (auto& a : m_cvBullets)
	{
		if (a->IsActive())
			a->SetActive(false);
	}
}

void Boss::CreateAnimation()
{

	wstring strImage = L"./Textures/Megaman/resources/Enemy/Boss/Colonel_Idle.bmp";
	wstring  shaderFile = L"./Shader/TextureShine.hlsl";
	m_pAnimation = make_shared<Animation>(strImage, shaderFile);

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
		strImage = L"./Textures/Megaman/resources/Enemy/Boss/colonelJump.bmp";
		CreateClip(strImage, 94, 0, 3, AnimationClip::eState::Loop, 0.2f, 99);
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

void Boss::CreateClip(wstring strImage, int w, int h, int count, AnimationClip::eState state, float speed, int term)
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

void Boss::FireBullet(eBullet state, UINT direction, Vector2 position)
{
	auto pLastBullet = m_cvBullets[lastBullet];

	pLastBullet->SetActive(true);
	pLastBullet->SetState((BossBullet::eState)state);
	pLastBullet->SetDirection(direction);
	pLastBullet->SetPosition(position);
	pLastBullet->SetScale(Vector2(1.2f, 1.2f));

	if(state == WAVE) 
		pLastBullet->SetScale(Vector2(1.6f, 1.6f));

	if (state == BIG_WAVE)
		pLastBullet->SetScale(Vector2(5.1f, 5.1f));

	pLastBullet->Update();

	m_bFireState = true;

	lastBullet += 1;
	if (lastBullet > 49)
		lastBullet = 0;
}

void Boss::UpdateBullet()
{
	// bullet
	for (UINT i = 0; i < m_cvBullets.size(); i++)
	{
		if (m_cvBullets[i]->IsActive())
		{		
			m_cvBullets[i]->Update();
		}

	}
}

void Boss::UpdateEffect(Vector2& position)
{
	m_pEffect->Update();

	// Dash Megaman

	cvPosCount += 1;

	if (cvPosCount >= 40)		
		cvPosCount = 0;

	m_cvPosition[cvPosCount] = position;

	Vector2 dashPos = { 0, 0 };

	const int dash_1 = 2;
	const int dash_2 = 4;
	const int dash_3 = 7;
	const int dash_4 = 11;
	const int dash_5 = 14;

	if (cvPosCount - dash_1 < 0)		
		dashPos = m_cvPosition[(long long)40 + cvPosCount - dash_1];
	else						
		dashPos = m_cvPosition[(long long)cvPosCount - dash_1];
	m_cvEffects[4]->SetPosition(dashPos);

	if (cvPosCount - dash_2 < 0)	dashPos = m_cvPosition[(long long)40 + cvPosCount - dash_2];
	else						dashPos = m_cvPosition[(long long)cvPosCount - dash_2];
	m_cvEffects[3]->SetPosition(dashPos);

	if (cvPosCount - dash_3 < 0)	dashPos = m_cvPosition[(long long)40 + cvPosCount - dash_3];
	else						dashPos = m_cvPosition[(long long)cvPosCount - dash_3];
	m_cvEffects[2]->SetPosition(dashPos);

	if (cvPosCount - dash_4 < 0)	dashPos = m_cvPosition[(long long)40 + cvPosCount - dash_4];
	else						dashPos = m_cvPosition[(long long)cvPosCount - dash_4];
	m_cvEffects[1]->SetPosition(dashPos);

	if (cvPosCount - dash_5 < 0)	dashPos = m_cvPosition[(long long)40 + cvPosCount - dash_5];
	else						dashPos = m_cvPosition[(long long)cvPosCount - dash_5];
	m_cvEffects[0]->SetPosition(dashPos);

	for (auto& a : m_cvEffects)
	{
		a->SetScale(GetScale());
		a->SetRotation(GetRotation());
		a->Update();
	}
}



void Boss::GroundCheck(Vector2& position)
{
	shared_ptr<class S01_Field> pScene = dynamic_pointer_cast<S01_Field>(SCENEMANAGER->GetCurrentScene());

	auto grounds = pScene->GetGround();
	if (m_bFalling == true)
	{
		if (Collision::HitTest(grounds[grounds.size()-9], m_pCollider))
		{
			m_bGround = true;
			position.y = grounds[grounds.size() - 9]->GetPosition().y 
						 + grounds[grounds.size() - 9]->GetScale().y / 2 
						 + m_tall / 2;
		}
		else m_bGround = false;
	}
}

void Boss::SetLookMM()
{
	auto MMpos = pMM->GetPosition();

	if (MMpos.x > GetPosition().x)
		SetRotation(0, 180.0f, 0);
	else
		SetRotation(0, 0.0f, 0);
}

void Boss::SetDontLookMM()
{
	auto MMpos = pMM->GetPosition();

	if (MMpos.x > GetPosition().x)
		SetRotation(0, 0.0f, 0);
	else
		SetRotation(0, 180.0f, 0);
}

void Boss::FireThreeWave(Vector2& position)
{
	m_PatternTimer += DELTA;

	// 가드 자세
	if (m_PatternTimer < 1.5f)
	{		
		if (!m_PlayOnce)
		{
			SetLookMM();
			SetState(Boss::GUARD);
			m_PlayOnce = true;
		}
	}
	// 아래 베기
	else if (m_PatternTimer >= 1.5f && m_PatternTimer < 2.3f)
	{		
		if (m_PlayOnce)
		{
			SetLookMM();
			SetState(Boss::DOWN_SLASH);
			SOUND->Play("Boss_wave");
			FireBullet(eBullet::WAVE, ROT_L ? LEFT : RIGHT,
				ROT_L ? position - Vector2(50.0f, -80.0f) : position + Vector2(50.0f, 80.0f));
			m_PlayOnce = false;
		}
	}
	// 올려 베기
	else if (m_PatternTimer >= 2.3f && m_PatternTimer < 3.1f)
	{
		if (!m_PlayOnce)
		{
			SetState(Boss::UP_SLASH);
			SOUND->Play("Boss_wave");
			FireBullet(eBullet::WAVE, ROT_L ? LEFT : RIGHT,
				ROT_L ? position - Vector2(50.0f, 80.0f) : position + Vector2(50.0f, -80.0f));
			m_PlayOnce = true;
		}
	}
	// 아래 베기
	else if (m_PatternTimer >= 3.1f && m_PatternTimer < 3.9f)
	{
		if (m_PlayOnce)
		{
			SetState(Boss::DOWN_SLASH);
			SOUND->Play("Boss_wave");
			FireBullet(eBullet::WAVE, ROT_L ? LEFT : RIGHT,
				ROT_L ? position - Vector2(50.0f, -80.0f) : position + Vector2(50.0f, 80.0f));
			m_PlayOnce = false;
		}
	}
	// idle
	else if (m_PatternTimer >= 3.9f && m_PatternTimer < 4.6f)
	{
		if (!m_PlayOnce)
		{
			SetState(Boss::IDLE);
			m_PlayOnce = true;
		}
	}

	// idle
	else if (m_PatternTimer >= 4.6f)
	{
		m_PatternTimer = 0.0f;
		m_Sequence = P_DISAPPEAR;
		m_LastSequence = P_THREE_WAVE;
		m_PlayOnce = false;
	}
}

void Boss::BackAttack(Vector2& position)
{
	m_PatternTimer += DELTA;	
	random_device rd;
	uniform_int_distribution<int> dist(0, 99);

	if (m_PatternTimer < 0.7f)
	{
		if (!m_PlayOnce)
		{
			auto megaManPos = pMM->GetPosition();

			if (megaManPos.x < CENTER)
			{
				position = megaManPos + Vector2(50.0f, 0.0f);
				SET_L;
			}
			else
			{
				position = megaManPos - Vector2(50.0f, 0.0f);
				SET_R;
			}

			m_bRender = true;
			m_PlayOnce = true;
			SetState(Boss::UP_SLASH);
			SOUND->Play("Boss_BackAtk");
		}
	}

	else if (m_PatternTimer >= 0.7f)
	{
		m_PatternTimer = 0.0f;
		m_PlayOnce = false;

		if (dist(rd) % 2 == 0)
			m_Sequence = P_DASH;
		else
			m_Sequence = P_DISAPPEAR;

		m_LastSequence = P_BACKATTACK;
	}

}

void Boss::ThunderAttack(Vector2& position)
{
	m_PatternTimer += DELTA;
	position.x = CENTER;

	// 나타나기
	if (m_PatternTimer < 0.7f)
	{
		if (!m_PlayOnce)
		{
			m_bRender = true;
			SET_L;
			SetState(Boss::APPEAR);
			m_PlayOnce = true;
		}
	}

	// 아주 잠깐 가드
	else if (m_PatternTimer >= 0.7f && m_PatternTimer < 1.0f)
	{
		if (m_PlayOnce)
		{
			SetState(Boss::GUARD);
			m_PlayOnce = false;
		}
	}

	// 번개
	else if (m_PatternTimer >= 1.0f && m_PatternTimer < 2.8f)
	{
		if (!m_PlayOnce)
		{
			FireBullet(THUNDER_EFT, LEFT, Vector2(CENTER - 50, 250.0f));
			FireBullet(SPARK, LEFT, Vector2(CENTER - 47, 100.0f));
			SetState(Boss::THUNDER);
			SOUND->Play("Boss_Lightning");
			SOUND->Play("Boss_spark");
			
			m_PlayOnce = true;
		}
		if (m_PatternTimer > 2.0f)
		{
			for (auto& a : m_cvBullets)
				a->SetActive(false);
			SOUND->Play("Boss_Thunder_atk");
		}
	}

	else if (m_PatternTimer >= 2.8f && m_PatternTimer < 2.9f)
	{
		if (m_PlayOnce)
		{
			
			FireBullet(GROUND_ATK, LEFT, Vector2 (CENTER, -290.0f));
			FireBullet(GROUND_ATK, RIGHT, Vector2(CENTER, -290.0f));
			m_PlayOnce = false;
		}
	}

	else if (m_PatternTimer >= 2.9f && m_PatternTimer < 3.04f)
	{
		if (!m_PlayOnce)
		{
			FireBullet(SPARK, LEFT, Vector2 (CENTER - 100 , -290.0f ));
			FireBullet(SPARK, RIGHT, Vector2(CENTER + 100 , -290.0f ));
			m_PlayOnce = true;
		}
	}

	else if (m_PatternTimer >= 3.04f && m_PatternTimer < 3.18f)
	{
		if (m_PlayOnce)
		{
			FireBullet(SPARK, LEFT, Vector2 (CENTER - 300, - 290.0f ));
			FireBullet(SPARK, RIGHT, Vector2(CENTER + 300, -290.0f));
			m_PlayOnce = false;
		}
	}

	else if (m_PatternTimer >= 3.18f && m_PatternTimer < 3.92f)
	{
		if (!m_PlayOnce)
		{
			FireBullet(SPARK, LEFT,  Vector2(CENTER - 500 , - 290.0f ));
			FireBullet(SPARK, RIGHT, Vector2(CENTER + 500 , -290.0f ));
			m_PlayOnce = true;
		}
	}

	else if (m_PatternTimer >= 3.92f && m_PatternTimer < 4.0f)
	{
		if (m_PlayOnce)
		{
			
			for (auto& a : m_cvBullets)
				a->SetActive(false);
			SOUND->Stop("Boss_spark");
			FireBullet(ASCENDING_ATK, LEFT,  Vector2(CENTER - 100 ,- 290.0f ));
			FireBullet(ASCENDING_ATK, RIGHT, Vector2(CENTER + 100 ,- 290.0f ));
			FireBullet(ASCENDING_ATK, LEFT, Vector2(CENTER - 300 ,- 290.0f ));
			FireBullet(ASCENDING_ATK, RIGHT, Vector2(CENTER + 300 ,- 290.0f ));
			FireBullet(ASCENDING_ATK, LEFT, Vector2(CENTER - 500 ,- 290.0f ));
			FireBullet(ASCENDING_ATK, RIGHT, Vector2(CENTER + 500 ,- 290.0f ));
			m_PlayOnce = false;
		}
	}

	// idle
	else if (m_PatternTimer >= 4.0f && m_PatternTimer < 4.6f)
	{
		if (!m_PlayOnce)
		{
			SetState(Boss::IDLE);
			m_PlayOnce = true;
		}
	}

	// Next Pattern
	else if (m_PatternTimer >= 4.6f)
	{
		m_PatternTimer = 0.0f;
		m_Sequence = P_DASH;
		m_LastSequence = P_THUNDER_ATK;
		m_PlayOnce = false;
	}
}

void Boss::BigWaveAttack(Vector2& position)
{	
	m_PatternTimer += DELTA;

	if (m_PatternTimer < 0.3f)
	{
		// 점프 시키기
		if (!m_PlayOnce)
		{
			SetLookMM();
			SetState(Boss::JUMPATTACK);
			SetScale(Vector2(2.7f, 2.7f));
			m_Gravirty = 0.0f;
			m_bGround = false;
			m_bFalling = false;
			m_PlayOnce = true;
			m_OriginPosition = position;
		}
	}

	//낙하
	position.y += (m_SpeedY - m_Gravirty) * DELTA;

	//낙하 판정
	if (m_Gravirty >= m_SpeedY)
		m_bFalling = true;

	//중력		
	m_Gravirty += 48.0f;
	if (m_Gravirty > 2.0 * m_SpeedY)
		m_Gravirty = 2.0 * m_SpeedY;

	if (m_PatternTimer > 0.6f)
		SOUND->Play("Boss_bigwave");

	if (m_bGround && m_bFalling && m_PatternTimer < 2.3f)
	{
		if (m_PlayOnce)
		{
			SetLookMM();			
			SetState(Boss::DOWN_SLASH);
			SetScale(Vector2(1.5f, 1.5f));
			FireBullet(eBullet::BIG_WAVE, ROT_L ? LEFT : RIGHT,
				ROT_L ? position - Vector2(50.0f, 80.0f) : position + Vector2(50.0f, -80.0f));
			SOUND->Play("Boss_BigWave2");
			m_PlayOnce = false;
		}
	}

	// 아래 베기
	
	{
		// 잠깐 idle
		if (m_PatternTimer >= 2.3f && m_PatternTimer < 2.8f)
		{
			if (!m_PlayOnce)
			{
				SetState(Boss::IDLE);  
				m_PlayOnce = true;
			}
		}
		else if (m_PatternTimer >= 2.8f)
		{
			m_PatternTimer = 0.0f;
			m_Sequence = P_DISAPPEAR;
			m_LastSequence = P_BIG_WAVE;
			SOUND->Stop("Boss_BigWave2");
			m_PlayOnce = false;
		}
	}


}

void Boss::Dash(Vector2& position)
{
	random_device rd;
	uniform_int_distribution<int> dist(0, 99);

	if (!m_PlayOnce)
	{
		m_bRenderDash = true;
		m_PlayOnce = true;
		SetState(Boss::DASH);
		SOUND->Play("Boss_dash");
		if (m_LastSequence == P_THUNDER_ATK)
			SetLookMM();		
		else if (position.x > CENTER)
			SET_L;
		else if (position.x <= CENTER)
			SET_R;
	}


	if (ROT_L)
	{
		position.x -= (m_SpeedX - m_BreakX) * DELTA;
		if (position.x < CENTER - 300.0f)
		{
			m_BreakX += 30.0f;
			if (m_BreakX >= m_SpeedX)
				m_BreakX = m_SpeedX;
			SetState(Boss::IDLE);
		}
	}
	else
	{
		position.x += (m_SpeedX - m_BreakX) * DELTA;
		if (position.x > CENTER + 250.0f)
		{
			m_BreakX += 30.0f;
			if (m_BreakX >= m_SpeedX)
				m_BreakX = m_SpeedX;
			SetState(Boss::IDLE);
		}
	}

	if (m_BreakX == m_SpeedX)
	{
		m_bRenderDash = false;
		m_PatternTimer += DELTA;
		if (m_PatternTimer >= 0.3f)
		{
			m_BreakX = 0.0f;
			m_PlayOnce = false;
			m_PatternTimer = 0.0f;

			int random = dist(rd) % 3;

			if (random == 0)
				m_Sequence = P_THREE_WAVE;
			else if (random == 1)
				m_Sequence = P_DISAPPEAR;
			else
				m_Sequence = P_BIG_WAVE;
			m_LastSequence = P_DASH;
		}
	}
}

void Boss::Disappear(Vector2& position)
{
	m_PatternTimer += DELTA;
	random_device rd;
	uniform_int_distribution<int> dist(0, 99);


	if (m_PatternTimer < 0.3f)
	{
		if (!m_PlayOnce)
		{
			SetLookMM();
			m_PlayOnce = true;
			SetState(Boss::DISAPPEAR);
			SOUND->Play("Boss_Disappear");
			m_OriginPos = position;
			m_pDisAppearEffect->SetPosition(position);
			m_pDisAppearEffect->SetRotation(GetRotation());
			m_pDisAppearEffect->SetState(BossEffect::DISAPPEAR);
			m_pDisAppearEffect->SetScale(1.5f, 1.5f);
			m_bDisappear = true;
		}

		if (ROT_L)
		{
			position.x -= m_SpeedX/2 * DELTA;
			m_OriginPos.x += m_SpeedX / 2 * DELTA;
			m_pDisAppearEffect->SetPosition(m_OriginPos);
		}
		else
		{
			position.x += m_SpeedX/2 * DELTA;
			m_OriginPos.x -= m_SpeedX / 2 * DELTA;
			m_pDisAppearEffect->SetPosition(m_OriginPos);
		}
	}

	else if (m_PatternTimer >= 0.3f && m_PatternTimer < 2.0f)
	{
		m_bRender = false;
		m_bDisappear = false;
	}

	else if (m_PatternTimer >= 2.0f)
	{
		m_PatternTimer = 0.0f;
		m_PlayOnce = false;		

		if (dist(rd) % 2 == 0)
			m_Sequence = P_BACKATTACK;
		else
			m_Sequence = P_THUNDER_ATK;

		m_LastSequence = P_DISAPPEAR;
	}

}

void Boss::Opening(Vector2& position)
{
	Vector2 Mpos = OBJECTMANAGER->FindObject("MEGAMAN")->GetPosition();
	Vector2 Gpos = GetPosition();

	if (Gpos.x - Mpos.x < 400.0f)
	{
		m_bOpening = true;
	}
	if (m_bOpening)
	{
		m_PatternTimer += DELTA;

		if (m_PatternTimer >= 0.1f && m_PatternTimer < 0.5f)
		{
			if (!m_PlayOnce)
			{
				m_PlayOnce = true;
				SOUND->Stop("BossMeet");
				FireBullet(THUNDER_EFT, LEFT, Vector2(position.x - 43, 250.0f));
				SOUND->Play("Boss_spark");
			}
		}
		else if (m_PatternTimer >= 0.5f && m_PatternTimer < 0.6f)
		{
			if (m_PlayOnce)
			{
				m_PlayOnce = false;
				SOUND->Stop("Boss_spark");
				for (auto& a : m_cvBullets)
					a->SetActive(false);
			}
		}
		else if (m_PatternTimer >= 1.0f && m_PatternTimer < 1.5f)
		{
			if (!m_PlayOnce)
			{
				m_PlayOnce = true;
				FireBullet(THUNDER_EFT, LEFT, Vector2(position.x - 43, 250.0f));
				SOUND->Play("Boss_spark");
			}
		}
		else if (m_PatternTimer >= 1.5f && m_PatternTimer < 1.6f)
		{
			if (m_PlayOnce)
			{
				m_PlayOnce = false;
				SOUND->Stop("Boss_spark");
				for (auto& a : m_cvBullets)
					a->SetActive(false);
			}
		}
		else if (m_PatternTimer >= 2.0f && m_PatternTimer < 4.0f)
		{
			if (!m_PlayOnce)
			{
				m_PlayOnce = true;
				FireBullet(THUNDER_EFT, LEFT, Vector2(position.x - 43, 250.0f));
				SOUND->Play("Boss_spark");
				SetState(Boss::CHARGING);
				m_bRender = true;
			}
		}
		else if (m_PatternTimer >= 4.0f && m_PatternTimer < 6.1f)
		{
			SetState(Boss::OPENING);
			if (m_PatternTimer >= 4.6f && m_PatternTimer < 4.7f)
			{
				if (m_PlayOnce)
				{
					m_PlayOnce = false;
					SOUND->Stop("Boss_spark");
					SOUND->Play("Boss_Lightning");
					for (auto& a : m_cvBullets)
						a->SetActive(false);
				}
			}

		}

		else if (m_PatternTimer >= 6.0f)
		{
			m_PatternTimer = 0.0f;
			m_PlayOnce = false;
			m_bOpening = false;
			m_bRender = true;
			m_Sequence = P_DISAPPEAR;
			m_LastSequence = p_OPENING;
			SOUND->Play("boss");
			m_bHPRender = true;
		}
	}
}


