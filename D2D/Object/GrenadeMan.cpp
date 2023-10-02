#include "framework.h"
#include "GrenadeMan.h"
#include "Scene/S01_Field.h"

#include "Object/Bullet.h"
#include "Object/MegaMan.h"
#include "Object/PlayerEffect.h"

#define  LOOK_L  m_nState %2  == 0 
#define  LOOK_R  m_nState %2  == 1 

#define  ROT_L  GetRotation() == Vector3(0.0f, 0.0f, 0.0f)
#define  ROT_R  GetRotation() == Vector3(0.0f, 180.0f, 0.0f) 

#define  SET_L  SetRotation(Vector3(0.0f, 0.0f, 0.0f))
#define  SET_R  SetRotation(Vector3(0.0f, 180.0f, 0.0f))

GrenadeMan::GrenadeMan()
{
	SetActive(false);
	
	
	m_pCollider = new Collider(Vector2(110.0f, m_tall), m_Position, 1);
	m_pWall = new Collider(Vector2(120.0f, m_tall - 50.0f), m_Position, 1);	

	m_pEffect = new PlayerEffect();
	CreateAnimation();
}


GrenadeMan::~GrenadeMan()
{
}

void GrenadeMan::PreUpdate(Vector2& position)
{
	
	if (IsDeath() && IsActive())
	{
		SetActive(false);
		m_pEffect->Reset();
		m_pEffect->SetActive(true);
		m_pEffect->SetState(PlayerEffect::ENEMYBOMB);
		m_pEffect->SetPosition(GetPosition());
		m_pEffect->SetScale(Vector2(3.5f, 3.5f));
		SOUND->Play("EnemyCut");
	}

	if (!IsActive())
		return;

	Vector2 Mpos = OBJECTMANAGER->FindObject("MEGAMAN")->GetPosition();
	Vector2 Gpos = GetPosition();
	float delta = DELTA;

	if (m_bAttacked)
	{
		m_pAnimation->GetTexture()->UpdateColorBuffer(1, Color(1, 1, 1, 1), 0.3f);
		
	}

	// 1. 벽 붙기 2. 자유 낙하 만들기
	if (!m_bGround)
	{
		if (!IsJumping())
		{
			position.y -= 560 * delta;
		}

		// 2. Juming이거나 낙하인 상태에서 떨어지게 하기
//		else if (IsJumping())
//		{

//		}
	}

	switch (m_Mode)
	{
	case GrenadeMan::ONLY_BOMB:
		// State를 Check
		switch (m_nState)
		{
		case LEFT_IDLE:
		case RIGHT_IDLE:
			if (m_bFound)
			{
				if (Mpos.x > Gpos.x)		SetState(RIGHT_ATTACK);
				else						SetState(LEFT_ATTACK);
			}
			break;

		case LEFT_ATTACK:
		case RIGHT_ATTACK:
			if (m_AttackTime >= 0.3f && m_bFireState == false )
				FireBullet();
			m_AttackTime += delta;
			if (m_AttackTime >= 2.2f)
			{
				if (Mpos.x > Gpos.x)	SetState(RIGHT_IDLE);
				else					SetState(LEFT_IDLE);
				m_AttackTime = 0.0f;
				m_bFireState = false;
			}
			break;
		}
		break;


	case GrenadeMan::WALK_BOMB:
		break;
	case GrenadeMan::ONLY_WALK:


		break;
	case GrenadeMan::WALK_SHOOT:
		switch (m_nState)
		{
		case GrenadeMan::GREEN_IDLE:
			m_IdleTime += delta;
			if (m_IdleTime >= 1.0f)
			{
				SetState(GREEN_MOVE);
				m_IdleTime = 0.0f;
				m_Sequence += 1;
				if (m_Sequence == 4)			m_Sequence = 0;
			}
			break;
		case GrenadeMan::GREEN_FRONT_FIRE:
		case GrenadeMan::GREEN_UP_FIRE:

			if (m_AttackTime >= 0.1f && m_bFireState == false)
				FireBullet();
			m_AttackTime += delta;
			if (m_AttackTime >= 1.0f)
			{
				SetState(GREEN_MOVE);
				m_AttackTime = 0.0f;
				m_bFireState = false;
			}
			m_Sequence = 0;
			break;

		case GrenadeMan::GREEN_MOVE:

			if (ROT_L) 			position.x -= m_SpeedX * delta;
			else				position.x += m_SpeedX * delta;

			m_MoveTime += delta;
			if (m_MoveTime >= 1.0f)
			{
				if (m_Sequence == 0)
				{
					SetState(GREEN_IDLE);
					m_Sequence += 1;
				}
				else if (m_Sequence == 2)
				{
					if (m_bFound)
					{
						if (Mpos.y > Gpos.y && Mpos.y - Gpos.y >= 100.0f)
							SetState(GREEN_UP_FIRE);
						else
							SetState(GREEN_FRONT_FIRE);

						if (Mpos.x > Gpos.x)	SET_R;
						else					SET_L;
					}
					else
						SetState(GREEN_IDLE);
					m_Sequence += 1;
				}
				m_MoveTime = 0.0f;
			}
			break;
		}
		break;

	}


	//Timer

	if (m_bAttacked)
		m_AttackedTime += delta;

	if (m_AttackedTime >= 0.1f)
	{
		m_AttackedTime = 0.0f;
		m_bAttacked = false;
		m_pAnimation->GetTexture()->UpdateColorBuffer(1, Color(1, 1, 1, 1), -0.3f);
	}


	//	case LEFT_JUMP:
	//	case RIGHT_JUMP:
	//		if (m_bGround)
	//		{
	//			if (LOOK_L)
	//				SetState(LEFT_FALL);
	//			else
	//				SetState(RIGHT_FALL);
	//		}
	//		break;
	//	case LEFT_FALL:
	//	case RIGHT_FALL:
	//		m_FallTime += TIME->Delta();
	//		if (m_FallTime >= 1.0f)
	//		{
	//			if (m_nState == LEFT_FALL)
	//				SetState(LEFT_IDLE);
	//			else
	//				SetState(RIGHT_IDLE);
	//			m_FallTime = 0.0f;
	//		}
	//		break;
}

void GrenadeMan::Update()
{
	Vector2 position = GetPosition();

	PreUpdate(position);
	UpdateEffect();
	UpdateBullet();

	if (!IsActive())
		return;

	if (GetPosition().y < -600.0f)
	{
		SetActive(false);
		SetHP(0);
	}

	// KeyBoard
	IsFoundMM(m_Mode);	
	WallCheck(position);
	GroundCheck(position);
	SetPosition(position);

	// Animation Update
	UPDATE(m_pAnimation);

	// Collider  Update  
	m_pCollider->SetPosition(GetPosition() - Vector2(0.0f, 5.0f)); 
	m_pCollider->Update();	
	m_pWall->SetPosition(GetPosition());
	m_pWall->SetScale(Vector2(120.0f, 50.0f));
	m_pWall->Update();


	
	
}


void GrenadeMan::Render()
{
	m_pEffect->Render();

	// Bullet
	for (auto& p : m_cvBullets)
		p->Render();

	if (!IsActive())
		return;

	m_pAnimation->Render();
	if (m_bDebug)
	{
		m_pWall->Render();
		m_pCollider->Render();
	}

	


}

void GrenadeMan::Reset()
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

void GrenadeMan::CreateAnimation()
{
	
	wstring strImage = L"./Textures/Megaman/MyResource/BULLET/EFFECT_CHARGE.png";
	wstring  shaderFile = L"./Shader/TextureShine.hlsl";
	m_pAnimation = make_shared<Animation>(strImage, shaderFile);

	// LEFT_IDLE
	{
		wstring strImage = L"./Textures/Megaman/MyResource/Monster/KnotBeret_LEFT.bmp";
		CreateClip(strImage, 100, 0, 7);
	}

	// RIGHT_IDLE
	{
		wstring strImage = L"./Textures/Megaman/MyResource/Monster/KnotBeret_RIGHT.bmp";
		CreateClip(strImage, 100, 0, 7);
	}

	// LEFT_MOVE
	{
		wstring strImage = L"./Textures/Megaman/MyResource/Monster/KnotBeret_LEFT.bmp";
		CreateClip(strImage, 100, 100, 7, AnimationClip::eState::Loop, 0.06f);
	}

	// RIGHT_MOVE
	{
		wstring strImage = L"./Textures/Megaman/MyResource/Monster/KnotBeret_RIGHT.bmp";
		CreateClip(strImage, 100, 100, 7, AnimationClip::eState::Loop, 0.06f);
	}

	// LEFT_ATTACK
	{
		wstring strImage = L"./Textures/Megaman/MyResource/Monster/KnotBeret_LEFT.bmp";
		shared_ptr<AnimationClip> pClip = make_shared<AnimationClip>(AnimationClip::eState::End);
		shared_ptr<Texture> pTexture = m_pAnimation->GetTexture();
		m_pAnimation->AddClip(pClip);

		for (int i = 0; i < 8; i++)
		{
			int sx = 100 * i;
			int sy = 200;
			int ex = sx + 100;
			int ey = 200 + 100;
			pClip->AddFrame(pTexture, strImage, sx, sy, ex, ey, 0.1f);
		}
		for (int j = 0; j < 2; j++)
		{
			for (int i = 0; i < 7; i++)
			{
				int sx = 100 * i;
				int sy = 0;
				int ex = sx + 100;
				int ey = 100;
				pClip->AddFrame(pTexture, strImage, sx, sy, ex, ey, 0.1f);
			}
		}
	}

	// RIGHT_ATTACK
	{
		wstring strImage = L"./Textures/Megaman/MyResource/Monster/KnotBeret_RIGHT.bmp";
		shared_ptr<AnimationClip> pClip = make_shared<AnimationClip>(AnimationClip::eState::End);
		shared_ptr<Texture> pTexture = m_pAnimation->GetTexture();
		m_pAnimation->AddClip(pClip);

		for (int i = 0; i < 8; i++)
		{
			int sx = 100 * i;
			int sy = 200;
			int ex = sx + 100;
			int ey = 200 + 100;
			pClip->AddFrame(pTexture, strImage, sx, sy, ex, ey, 0.1f);
		}
		for (int j = 0; j < 2; j++)
		{
			for (int i = 0; i < 7; i++)
			{
				int sx = 100 * i;
				int sy = 0;
				int ex = sx + 100;
				int ey = 100;
				pClip->AddFrame(pTexture, strImage, sx, sy, ex, ey, 0.1f);
			}
		}
	}

	// LEFT_JUMP
	{
		wstring strImage = L"./Textures/Megaman/MyResource/Monster/KnotBeret_LEFT.bmp";
		CreateClip(strImage, 100, 64, 9, AnimationClip::eState::End, 0.03f);
	}

	// RIGHT_JUMP
	{
		wstring strImage = L"./Textures/Megaman/MyResource/Monster/KnotBeret_RIGHT.bmp";
		CreateClip(strImage, 100, 64, 9, AnimationClip::eState::End, 0.03f);
	}

	// LEFT_FALL
	{
		wstring strImage = L"./Textures/Megaman/MyResource/Monster/KnotBeret_LEFT.bmp";
		CreateClip(strImage, 30, 50, 5, AnimationClip::eState::Loop, 0.03f);
	}

	// RIGHT_FALL
	{
		wstring strImage = L"./Textures/Megaman/MyResource/Monster/KnotBeret_RIGHT.bmp";
		CreateClip(strImage, 30, 50, 5, AnimationClip::eState::Loop, 0.03f);
	}

	/////////////////////////////////GREEN///////////////////////////////

	strImage = L"./Textures/Megaman/MyResource/Monster/x4grenademan2.bmp";
	// GREEN_IDLE	
	CreateClip(strImage, 80, 0, 7, AnimationClip::eState::Loop, 0.12f, 80);
	

	// GREEN_FRONT_FIRE	
	CreateClip(strImage, 80, 80, 2, AnimationClip::eState::EndStay, 0.1f, 80);
	

	// GREEN_UP_FIRE	
	CreateClip(strImage, 80, 160, 2, AnimationClip::eState::EndStay, 0.1f, 80);
	

	// GREEN_MOVE
	CreateClip(strImage, 80, 240, 7, AnimationClip::eState::Loop, 0.06f, 80);
	

	// GREEN_JUMP	
	CreateClip(strImage, 80, 320, 2, AnimationClip::eState::EndStay, 0.2f, 80);
	

	// GREEN_FALL	
	CreateClip(strImage, 80, 400, 3, AnimationClip::eState::EndStay, 0.1f, 80);
	

}

void GrenadeMan::CreateClip(wstring strImage, int w, int h, int count, AnimationClip::eState state, float speed, int term)
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

void GrenadeMan::FireBullet()
{
	Bullet* pBullet = new Bullet();
	if (!pBullet)
		return;

	pBullet->SetActive(true);
	pBullet->Reset();

	switch (m_Mode)
	{
		case GrenadeMan::ONLY_BOMB:
		case GrenadeMan::WALK_BOMB:
			{
				pBullet->SetState(Bullet::GRENADE);

				if (LOOK_L)		
				{
					pBullet->SetDirection((UINT)LEFT);
					pBullet->SetPosition(Vector2(GetPosition().x - 40.0f, GetPosition().y + 25.0f));
				}
				else     		
				{
					pBullet->SetDirection((UINT)RIGHT);
					pBullet->SetPosition(Vector2(GetPosition().x + 40.0f, GetPosition().y + 25.0f));
				}
				break;
			}

		case GrenadeMan::WALK_SHOOT:
			{
				if (m_nState == GREEN_UP_FIRE)
					pBullet->SetState(Bullet::UPSHOT);
				else if (m_nState == GREEN_FRONT_FIRE)
					pBullet->SetState(Bullet::FRONTSHOT);
				
				if (ROT_L)		
				{
					pBullet->SetDirection((UINT)LEFT);
					if (m_nState == GREEN_FRONT_FIRE)
						pBullet->SetPosition(Vector2(GetPosition().x - 90.0f, GetPosition().y + 45.0f));
					else if (m_nState == GREEN_UP_FIRE)
						pBullet->SetPosition(Vector2(GetPosition().x - 90.0f, GetPosition().y + 100.0f));

				}
				else     		
				{
					pBullet->SetDirection((UINT)RIGHT);
					if (m_nState == GREEN_FRONT_FIRE)
						pBullet->SetPosition(Vector2(GetPosition().x + 90.0f, GetPosition().y + 45.0f));
					else if (m_nState == GREEN_UP_FIRE)
						pBullet->SetPosition(Vector2(GetPosition().x + 90.0f, GetPosition().y + 100.0f));
				}

				break;
			}
	}
//	Vector2 Mpos = OBJECTMANAGER->FindObject("MEGAMAN")->GetPosition();
//	Vector2 Gpos = GetPosition();

	pBullet->SetScale(GetScale() * 1.1f);
	m_cvBullets.push_back(pBullet);

	m_bFireState = true;

}

void GrenadeMan::UpdateBullet()
{
	// bullet
	for (UINT i = 0; i < m_cvBullets.size(); i++)
	{
		if (m_cvBullets.size() >= 10)
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

void GrenadeMan::UpdateEffect()
{
	m_pEffect->Update();
}



void GrenadeMan::GroundCheck(Vector2& position)
{
	shared_ptr<class S01_Field> pScene = dynamic_pointer_cast<S01_Field>(SCENEMANAGER->GetCurrentScene());

	auto grounds = pScene->GetGround();
	if (m_bFalling == true)
	{

		for (size_t i = 2; i < grounds.size(); i++)
		{
			if (Collision::HitTest(grounds[i], m_pCollider))
			{
				m_bGround = true;
				position.y = grounds[i]->GetPosition().y + grounds[i]->GetScale().y / 2 + m_tall / 2;

				if (ROT_L && (m_pCollider->GetPosition().x - m_pCollider->GetScale().x / 2)
					<= (grounds[i]->GetPosition().x - grounds[i]->GetScale().x / 2) 
					&& m_nState != GREEN_FRONT_FIRE && m_nState != GREEN_UP_FIRE)
				{
					SetRotation(0, 180.0f, 0);
				}
				else if (ROT_R && (m_pCollider->GetPosition().x + m_pCollider->GetScale().x / 2)
					>= (grounds[i]->GetPosition().x + grounds[i]->GetScale().x / 2) 
					&& m_nState != GREEN_FRONT_FIRE && m_nState != GREEN_UP_FIRE)
				{
					SetRotation(0, 0, 0);
				}
				break;
			}
			else m_bGround = false;
		}
	}
}

void GrenadeMan::WallCheck(Vector2& position)
{
	if (m_Mode == ONLY_BOMB || m_Mode == WALK_SHOOT)
		return;

	shared_ptr<class S01_Field> pScene = dynamic_pointer_cast<S01_Field>(SCENEMANAGER->GetCurrentScene());

	auto grounds = pScene->GetGround();

	bool noTouch = false;


	for (auto& g : grounds)
	{
		if (Collision::HitTest(g, m_pWall))
		{
			
			if ((ROT_L) && (m_Position.x > g->GetPosition().x))
			{
				SET_R;
			}
			if ((ROT_R) && (m_Position.x < g->GetPosition().x))
			{
				SET_L;
			}
			m_bTouchWall = true;
			noTouch = true;
		}
	}
	if (!noTouch)		m_bTouchWall = false;
}

bool GrenadeMan::IsFoundMM(eMode m_Mode)
{
	bool found = false;
	Vector2 Mpos = OBJECTMANAGER->FindObject("MEGAMAN")->GetPosition();
	Vector2 Gpos = GetPosition();
	eState state = m_nState;

	switch (m_Mode)
	{
	case GrenadeMan::ONLY_BOMB:
		if (abs(Mpos.x - Gpos.x) <= 450.0f && IsIdle())
		{			
			found = true;
		}

		break;
	case GrenadeMan::WALK_BOMB:
		break;
	case GrenadeMan::ONLY_WALK:
		if (abs(Mpos.x - Gpos.x) <= 700.0f)
		{
			found = true;
		}
		break;
	case GrenadeMan::WALK_SHOOT:
		if (abs(Mpos.x - Gpos.x) <= 650.0f)
		{
			found = true;
		}
		break;
	}

	if (found)		m_bFound = true;
	else			m_bFound = false;

	return found;
}
