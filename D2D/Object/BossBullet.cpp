#include "framework.h"
#include "BossBullet.h"
#include "Object/MegaMan.h"
#include "Object/PlayerEffect.h"
#include "Object/Boss.h"
#include "Scene/S01_Field.h"

BossBullet::BossBullet()
{
	SetActive(false);
	CreateAnimation();

	// collider
	m_pCollider = new Collider();
	m_Gravirty = 0.0f;
}

BossBullet::BossBullet(eState state)
{
	SetActive(false);
	CreateAnimation();

	SetState(state);
	// collider
	m_pCollider = new Collider();
	m_Gravirty = 0.0f;
}

BossBullet::~BossBullet()
{
	SAFE_DELETE(m_pAnimation);
	SAFE_DELETE(m_pCollider);
}

void BossBullet::Update()
{
	if (!IsActive())
		return;

	// Animation
	UPDATE(m_pAnimation);

	// Collider
	m_pCollider->SetPosition(GetPosition());
	m_pCollider->SetScale(m_pAnimation->GetTextureRealSize());

	if (m_nState == SPARK)
		m_pCollider->SetScale(Vector2(40, 40));

	m_pCollider->Update();

	const  float move_speed = 1500.0f;
	const  float shot_speedX = 800.0f;
	const  float shot_speedY = 400.0f;
	const  float BigSpeedX = 600.0f;
	const  float GrenadeSpeedY = 850.0f;
	float  Delta = DELTA;

	Vector2 position = GetPosition();

	if (m_nState == WAVE)
	{
		if (GetDirection() == 0)
			position.x -= move_speed * Delta;
		else
			position.x += move_speed * Delta;
	}

	else if (m_nState == GROUND_ATK)
	{
		if (GetDirection() == 0)
			position.x -= shot_speedX * Delta;
		else
			position.x += shot_speedX * Delta;
	}

	else if (m_nState == ASCENDING_ATK)
	{
		position.y += move_speed * Delta;
	}

	else if (m_nState == SPARK || m_nState == THUNDER)
	{
		position = position;
	}

	else if (m_nState == BIG_WAVE)
	{
		if (GetDirection() == 0)
			position.x -= BigSpeedX * Delta;
		else
			position.x += BigSpeedX * Delta;

	}

	HitCheck(position);
	SetPosition(position);

	// Player의 현재에서 1100Pixel이상 되면 없어지게 한다
	Vector2  playerPos = OBJECTMANAGER->FindObject("MEGAMAN")->GetPosition();
	if (abs(position.x - playerPos.x) > 1150.0f || abs(position.y - playerPos.y) > 750.0f)
		SetActive(false);

	if (m_Direction == 0)		SetRotation(Vector3(0.0f, 0.0f, 0.0f));
	else                		SetRotation(Vector3(0.0f, 180.0f, 0.0f));

}

void BossBullet::Render()
{
	if (IsActive())
	{
		m_pAnimation->Render();
		if (m_bDebug)
			m_pCollider->Render();
	}
}

void BossBullet::Reset()
{
	m_Direction = 0;
	SetActive(true);
}

void BossBullet::CreateAnimation()
{
	// animation
	wstring strImage = L"./Textures/megaman/resources/Enemy/Boss/auraBlade.bmp";
	m_pAnimation = new Animation(strImage, g_ShaderFile);

	// WAVE,
	{
		CreateClip(strImage, 64, 116, 3, AnimationClip::Loop, 0.03f);
	}
	// GROUND_ATK
	{
		strImage = L"./Textures/Megaman/resources/Enemy/Boss/Effect_Colonel_GroundSpark.bmp";
		CreateClip(strImage, 130, 68, 4, AnimationClip::Loop, 0.03f);
	}
	// ASCENDING_ATK
	{
		strImage = L"./Textures/Megaman/resources/Enemy/Boss/Effect_Colonel_ElectricBeam.bmp";
		CreateClip(strImage, 54, 132, 4, AnimationClip::Loop, 0.03f);
	}
	//SPARK
	{
		strImage = L"./Textures/Megaman/resources/Enemy/Boss/Effect_Colonel_Spark.bmp";
		CreateClip(strImage, 68, 68, 8, AnimationClip::Loop, 0.01f);
	}
	// BIG_WAVE
	{
		strImage = L"./Textures/Megaman/resources/Enemy/Boss/BigWave.bmp";

		shared_ptr<AnimationClip> pClip = make_shared<AnimationClip>(AnimationClip::eState::EndStay);
		shared_ptr<Texture>       pTexture = m_pAnimation->GetTexture();
		m_pAnimation->AddClip(pClip);

		pClip->AddFrame(pTexture, strImage, 30, 100, 42, 130,       0.2f);
		pClip->AddFrame(pTexture, strImage, 99, 92, 116, 136,       0.2f);
		pClip->AddFrame(pTexture, strImage, 167, 84, 190, 228-84,   0.2f);
		pClip->AddFrame(pTexture, strImage, 237, 73, 261, 228-73,   0.2f);
		pClip->AddFrame(pTexture, strImage, 306, 53, 337, 228 - 53, 0.2f);
		pClip->AddFrame(pTexture, strImage, 373, 31, 421, 228 - 31, 0.2f);

		for (size_t i = 0; i < 15; i++)
		{
			pClip->AddFrame(pTexture, strImage, 306, 53, 337, 228 - 53, 0.05f);
			pClip->AddFrame(pTexture, strImage, 373, 31, 421, 228 - 31, 0.05f);
			pClip->AddFrame(pTexture, strImage, 442, 19, 495, 228 - 19, 0.05f);
			pClip->AddFrame(pTexture, strImage, 510, 4, 573, 228 - 4,   0.05f);
		}
//		pClip->AddFrame(pTexture, strImage, 590, 130, 591, 136,   0.04f);
	}
	// THUNDER
	{
		strImage = L"./Textures/Megaman/resources/Enemy/Boss/Effect_Colonel_ElectricSword.bmp";
		CreateClip(strImage, 34, 260, 4, AnimationClip::Loop, 0.01f);
	}

}

void BossBullet::CreateClip(wstring strImage, int w, int h, int count, AnimationClip::eState state, float speed)
{
	shared_ptr<AnimationClip> pClip = make_shared<AnimationClip>(state);
	shared_ptr<Texture>       pTexture = m_pAnimation->GetTexture();
	m_pAnimation->AddClip(pClip);

	for (int i = 0; i < count; i++)
	{
		int sx = w * i;
		int sy = 0;
		int ex = sx + w;
		int ey = h;
		pClip->AddFrame(pTexture, strImage, sx, sy, ex, ey, speed);
	}
}

void BossBullet::HitCheck(Vector2& position)
{
	if (!IsActive())
		return;

	auto pMM = dynamic_cast<MegaMan*>(OBJECTMANAGER->FindObject("MEGAMAN"));
	auto MMCollider = pMM->GetCollider();

	if (Collision::HitTest(MMCollider, m_pCollider))
	{
		if (pMM->IsInvincible())
			return;

		if (MMCollider->GetPosition().x > m_pCollider->GetPosition().x)
			pMM->SetAttacked(true, false);
		if (MMCollider->GetPosition().x < m_pCollider->GetPosition().x)
			pMM->SetAttacked(false, true);
		else
			pMM->SetAttacked(true, true);

		int curHp = pMM->GetHp();
		switch (m_nState)
		{
		case BossBullet::WAVE:
			pMM->SetHp(curHp - 8);
			break;
		case BossBullet::GROUND_ATK:
			pMM->SetHp(curHp - 5);
			break;
		case BossBullet::ASCENDING_ATK:
			pMM->SetHp(curHp - 5);
			break;
		case BossBullet::SPARK:
			pMM->SetHp(curHp - 5);
			break;
		case BossBullet::BIG_WAVE:
			pMM->SetHp(curHp - 10);
			break;
		case BossBullet::THUNDER:
			pMM->SetHp(curHp - 5);
			break;
		}
	}
}
