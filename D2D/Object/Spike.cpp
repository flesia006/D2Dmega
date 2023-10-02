#include "framework.h"
#include "Spike.h"
#include "Scene/S01_Field.h"

#include "Object/MegaMan.h"
#include "Object/PlayerEffect.h"

#define  ROT_L  GetRotation() == Vector3(0.0f, 0.0f, 0.0f)
#define  ROT_R  GetRotation() == Vector3(0.0f, 180.0f, 0.0f) 

#define  SET_L  SetRotation(Vector3(0.0f, 0.0f, 0.0f))
#define  SET_R  SetRotation(Vector3(0.0f, 180.0f, 0.0f))

Spike::Spike()
{
	SetActive(false);
	m_pHead = new Collider(Vector2(80.0f, 20.0f), m_Position, 1);
	m_pCollider = new Collider(Vector2(100.0f, 90.0f), m_Position, 1);
	m_pFeet = new Collider(Vector2(80.0f, 20.0f), m_Position, 1);
	
	m_pEffect = new PlayerEffect();
	CreateAnimation();
}


Spike::~Spike()
{
}

void Spike::PreUpdate(Vector2& position)
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

			
	//³«ÇÏ
	position.y += (m_SpeedY - m_Gravirty) * delta;

	if (ROT_L) 			position.x -= m_SpeedSpike * delta;
	else				position.x += m_SpeedSpike * delta;

	//³«ÇÏ ÆÇÁ¤
	if (m_Gravirty >= m_SpeedY)
		m_bFalling = true;

	//Áß·Â		
	m_Gravirty += 60.0f;
	if (m_Gravirty > 2.0 * m_SpeedY)
		m_Gravirty = 2.0 * m_SpeedY;



	if (m_bRoof)
	{
		m_Gravirty = m_SpeedY;
		m_bFalling = true;
		m_bRoof = false;
	}	

	if (m_bAttacked)
	{
		m_pAnimation->GetTexture()->UpdateColorBuffer(1, Color(1, 1, 1, 1), 0.3f);

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
}

void Spike::Update()
{
	Vector2 position = GetPosition();

	PreUpdate(position);
	UpdateEffect();

	if (!IsActive())
		return;

	if (GetPosition().y < -600.0f)
	{
		SetActive(false);
		SetHP(0);
	}

	// KeyBoard
	WallCheck(position);
	GroundCheck(position);
	HeadCheck(position);
	SetPosition(position);

	// Animation Update
	UPDATE(m_pAnimation);

	// Collider  Update  
	m_pCollider->SetPosition(GetPosition());
	m_pCollider->Update();

	m_pHead->SetPosition(GetPosition() + Vector2(0.0f, 55.0f));
	m_pHead->Update();

	m_pFeet->SetPosition(GetPosition() - Vector2(0.0f, 55.0f));
	m_pFeet->Update();

}


void Spike::Render()
{
	m_pEffect->Render();

	if (!IsActive())
		return;

	m_pAnimation->Render();	
	if (m_bDebug)
	{
		m_pCollider->Render();
		m_pHead->Render();
		m_pFeet->Render();
	}


}

void Spike::Reset()
{
	SetActive(false);
	SetPosition(GetOriginPos());
}

void Spike::CreateAnimation()
{
	// SPIKE
	wstring strImage = L"./Textures/Megaman/resources/Enemy/Spiky_mk/Spiky_mk_Move.bmp";
	wstring  shaderFile = L"./Shader/TextureShine.hlsl";
	m_pAnimation = make_shared<Animation>(strImage, shaderFile);
	CreateClip(strImage, 100, 0, 6, AnimationClip::eState::Loop, 0.005f, 98);

}

void Spike::CreateClip(wstring strImage, int w, int h, int count, AnimationClip::eState state, float speed, int term)
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


void Spike::UpdateEffect()
{
	m_pEffect->Update();
}



void Spike::GroundCheck(Vector2& position)
{
	shared_ptr<class S01_Field> pScene = dynamic_pointer_cast<S01_Field>(SCENEMANAGER->GetCurrentScene());

	auto grounds = pScene->GetGround();
	if (m_bFalling == true)
	{
		for (size_t i = 2; i < grounds.size(); i++)
		{
			if (Collision::HitTest(grounds[i], m_pFeet))
			{
				m_bGround = true;
				position.y = grounds[i]->GetPosition().y + grounds[i]->GetScale().y / 2 + m_tall / 2;

				break;
			}
			else m_bGround = false;
		}
	}

	if (m_bGround && m_bFalling)
	{
		m_Gravirty = 0.0f;
		m_bFalling = false;
		m_bGround = false;
	}
}

void Spike::WallCheck(Vector2& position)
{
	shared_ptr<class S01_Field> pScene = dynamic_pointer_cast<S01_Field>(SCENEMANAGER->GetCurrentScene());

	auto grounds = pScene->GetGround();

	bool noTouch = false;

	for (auto& g : grounds)
	{
		if (Collision::HitTest(g, m_pCollider))
		{

			if ((ROT_L) && (m_Position.x > g->GetPosition().x))
			{
				position.x = g->GetPosition().x + g->GetScale().x / 2 + m_pCollider->GetScale().x / 2;
				SET_R;
			}
			if ((ROT_R) && (m_Position.x < g->GetPosition().x))
			{
				position.x = g->GetPosition().x - g->GetScale().x / 2 - m_pCollider->GetScale().x / 2;
				SET_L;
			}
			m_bTouchWall = true;
			noTouch = true;
		}
	}
	if (!noTouch)		m_bTouchWall = false;
}

void Spike::HeadCheck(Vector2& position)
{
	shared_ptr<class S01_Field> pScene = dynamic_pointer_cast<S01_Field>(SCENEMANAGER->GetCurrentScene());

	auto grounds = pScene->GetGround();
	if (!m_bFalling)
	{
		for (size_t i = 2; i < grounds.size(); i++)
		{
			if (Collision::HitTest(grounds[i], m_pHead))
			{
				m_bFalling = true;
				m_Gravirty = m_SpeedY;
				position.y = grounds[i]->GetPosition().y - grounds[i]->GetScale().y / 2 - 130.0f / 2;
				break;
			}
		}
	}

}
