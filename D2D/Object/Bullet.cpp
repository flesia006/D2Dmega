#include "framework.h"
#include "Bullet.h"
#include "Object/MegaMan.h"
#include "Object/GrenadeMan.h"
#include "Object/Spike.h"
#include "Object/Boss.h"
#include "Object/PlayerEffect.h"
#include "Scene/S01_Field.h"

Bullet::Bullet()
{
	SetActive(false);

	// animation
	wstring strImage = L"./Textures/megaman/myResource/bullet/BULLET_NORMAL_LEFT.png";
	
	m_pAnimation = new Animation(strImage, g_ShaderFile);


	// L_BULLET,
	{
		CreateClip(strImage, 20, 20, 5, 3);
	}
	// L_CHARGE
	{
		strImage = L"./Textures/Megaman/MyResource/BULLET/BULLET_BUSTER_SMALL_LEFT.png";
		CreateClip(strImage, 30, 30, 7, 3);
	}
	// L_FULL_CHARGE
	{
		strImage = L"./Textures/Megaman/MyResource/BULLET/BULLET_BUSTER_LARGE_LEFT.png";
		CreateClip(strImage, 70, 40, 3, 3);
	}
	// GRENADE
	{
		strImage = L"./Textures/Megaman/MyResource/Monster/Generade.bmp";
		CreateClip(strImage, 25, 25, 1, 3);
	}
	// UPSHOT
	{
		strImage = L"./Textures/Megaman/MyResource/BULLET/enemyBullet.png";
		CreateClip(strImage, 16, 11, 4, 0);
	}
	// FRONTSHOT
	{
		strImage = L"./Textures/Megaman/MyResource/BULLET/enemyBullet.png";
		CreateClip(strImage, 16, 11, 4, 0);
	}


	// collider
	m_pCollider = new Collider();
	switch (m_nState)
	{
	case Bullet::L_BULLET:
	case Bullet::L_CHARGE:
	case Bullet::GRENADE:
		m_pCollider->SetScale(m_pAnimation->GetTextureRealSize() - Vector2(40.0f, 40.0f));
		break;
	case Bullet::L_FULL_CHARGE:	
		m_pCollider->SetScale(Vector2(40.0f, 60.0f));
		break;
	case Bullet::UPSHOT:
	case Bullet::FRONTSHOT:
		m_pCollider->SetScale(Vector2(5.0f, 5.0f));
		break;
	}
	m_Gravirty = 0.0f;


	// Effect
	m_pEffect = new PlayerEffect();

}

Bullet::~Bullet()
{
	SAFE_DELETE(m_pAnimation);
	SAFE_DELETE(m_pCollider);
}

void Bullet::Update()
{
	// Effect
	if (m_pEffect->IsActive())
		m_pEffect->Update();
	
	if (!IsActive()) return;

	const  float move_speed = 1500.0f;
	const  float shot_speedX = 400.0f;
	const  float shot_speedY = 400.0f;
	const  float GrenadeSpeedX = 450.0f;
	const  float GrenadeSpeedY = 850.0f;	


	Vector2 position = GetPosition();

	// Animation
	UPDATE(m_pAnimation);

	// Collider
	m_pCollider->SetPosition(position);
	if (m_nState == L_FULL_CHARGE)
		m_pCollider->SetScale(Vector2(140.0f, 60.0f));
	else
		m_pCollider->SetScale(m_pAnimation->GetTextureRealSize() - Vector2(40.0f, 40.0f));
	m_pCollider->Update();

	if (m_nState == GRENADE)
	{
		if (m_Direction == 0)
			position.x -= GrenadeSpeedX * TIME->Delta();
		else
			position.x += GrenadeSpeedX * TIME->Delta();

		//낙하
		position.y += GrenadeSpeedY * DELTA - m_Gravirty;

		//중력		
		m_Gravirty += 48.0f * DELTA;
		if (m_Gravirty > 2.0 * GrenadeSpeedY * DELTA)
			m_Gravirty = 2.0 * GrenadeSpeedY * DELTA;
	}

	else if (m_nState == UPSHOT)
	{
		if (m_Direction == 0)
		{
			position.x -= shot_speedX * TIME->Delta();
			position.y += shot_speedY * TIME->Delta();
		}
		else
		{
			position.x += shot_speedX * TIME->Delta();
			position.y += shot_speedY * TIME->Delta();
		}
	}

	else if (m_nState == FRONTSHOT)
	{
		if (m_Direction == 0)
			position.x -= shot_speedX * TIME->Delta();
		else
			position.x += shot_speedX * TIME->Delta();
	}

	else
	{
		if (m_Direction == 0)
			position.x = position.x - move_speed * TIME->Delta();
		else
			position.x = position.x + move_speed * TIME->Delta();


		// Player의 현재에서 300Pixel이상 되면 없어지게 한다
		Vector2  playerPos = OBJECTMANAGER->FindObject("MEGAMAN")->GetPosition();
		if (abs(position.x - playerPos.x) > 1150.0f)
			SetActive(false);

		if (m_Direction == 0)		SetRotation(Vector3(0.0f, 0.0f, 0.0f));
		else                		SetRotation(Vector3(0.0f, 180.0f, 0.0f));
	}


	HitCheck(position);
	SetPosition(position);
}

void Bullet::Render()
{
	if (IsActive())
	{
		m_pAnimation->Render();
		if(m_bDebug)
			m_pCollider->Render();
	}

	if (m_pEffect->IsActive())
		m_pEffect->Render();
}

void Bullet::Reset()
{
	m_Direction = 0;
	SetActive(true);
}

void Bullet::CreateClip(wstring strImage, int w, int h, int count, int loop)
{
	AnimationClip::eState state = AnimationClip::eState::Loop;
	if (loop)
		state = (AnimationClip::eState)loop;

	shared_ptr<AnimationClip> pClip = make_shared<AnimationClip>(state);
	shared_ptr<Texture>       pTexture = m_pAnimation->GetTexture();
	m_pAnimation->AddClip(pClip);

	for (int i = 0; i < count; i++)
	{
		int sx = w * i;
		int sy = 0;
		int ex = sx + w;
		int ey = h;
		pClip->AddFrame(pTexture, strImage, sx, sy, ex, ey, 0.1f);
	}
}

void Bullet::HitCheck(Vector2& position)
{
	if (!IsActive())
		return;

	shared_ptr<class S01_Field> pScene = dynamic_pointer_cast<S01_Field>(SCENEMANAGER->GetCurrentScene());

	auto grounds = pScene->GetGround();		

	auto pMM = dynamic_cast<MegaMan*>(OBJECTMANAGER->FindObject("MEGAMAN"));	
	auto MMCollider = pMM->GetCollider();
	auto cvGMans = pScene->GetGrenadeMans();
	auto spikes = pScene->GetSpikes();
	auto pBoss = pScene->GetBoss();

	switch (m_nState)
	{
	case Bullet::L_BULLET:
	case Bullet::L_CHARGE:
	case Bullet::L_FULL_CHARGE:
		for (auto& gm : cvGMans)
		{
			if (!gm->IsActive())
				continue;
			if (Collision::HitTest(gm->GetCollider(), m_pCollider))
			{
				if(m_nState == L_BULLET || m_nState == L_CHARGE )
					SetActive(false);
				m_pEffect->Reset();
				m_pEffect->SetActive(true);
				m_pEffect->SetPosition(GetPosition());
				SOUND->Play("bulletHit");
				if (m_nState == L_FULL_CHARGE)
				{
					if (m_Direction == LEFT)		m_pEffect->SetPosition(GetPosition() - Vector2(80.0f, 0.0f));
					else							m_pEffect->SetPosition(GetPosition() + Vector2(80.0f, 0.0f));
				}
				m_pEffect->SetScale(Vector2(3.0f, 3.0f));
				if (m_nState == L_BULLET || m_nState == L_CHARGE)
					m_pEffect->SetState(PlayerEffect::ENEMYHIT);
				else
					m_pEffect->SetState(PlayerEffect::ENEMYHIT2);
				gm->SetAttacked(true);

				if		(m_nState == L_BULLET)		gm->SetHP(gm->GetHP() - BulletDamage);
				else if (m_nState == L_CHARGE)		gm->SetHP(gm->GetHP() - ChragedBulletDamage);
				else if (m_nState == L_FULL_CHARGE)	gm->SetHP(gm->GetHP() - FullChargedBulletDamage);

				break;
			}
		}
		for (auto& gm : spikes)
		{
			if (!gm->IsActive())
				continue;
			if (Collision::HitTest(gm->GetCollider(), m_pCollider))
			{
				if (!gm->GetCollider()->IsActive())
					continue;

				if (m_nState == L_BULLET || m_nState == L_CHARGE)
					SetActive(false);
				m_pEffect->Reset();
				m_pEffect->SetActive(true);
				SOUND->Play("bulletHit");
				m_pEffect->SetPosition(GetPosition());
				if (m_nState == L_FULL_CHARGE)
				{
					if (m_Direction == LEFT)		m_pEffect->SetPosition(GetPosition() - Vector2(80.0f, 0.0f));
					else							m_pEffect->SetPosition(GetPosition() + Vector2(80.0f, 0.0f));
				}
				m_pEffect->SetScale(Vector2(3.0f, 3.0f));
				m_pEffect->SetState(PlayerEffect::ENEMYHIT);
				gm->SetAttacked(true);

				if (m_nState == L_BULLET)		gm->SetHP(gm->GetHP() - BulletDamage);
				else if (m_nState == L_CHARGE)		gm->SetHP(gm->GetHP() - ChragedBulletDamage);
				else if (m_nState == L_FULL_CHARGE)	gm->SetHP(gm->GetHP() - FullChargedBulletDamage);

				break;
			}
		}
		if (pBoss->IsActive() && pBoss->IsRender() && !pBoss->IsDodge())
		{
			if (Collision::HitTest(pBoss->GetCollider(), m_pCollider))
			{
				SetActive(false);

				m_pEffect->Reset();
				m_pEffect->SetActive(true);
				
				m_pEffect->SetPosition(GetPosition());

				if (m_Direction == LEFT)
					m_pEffect->SetRotation(0, 180, 0);
				else						
					m_pEffect->SetRotation(0, 0, 0);

				if (m_nState == L_FULL_CHARGE)
				{
					if (m_Direction == LEFT)		m_pEffect->SetPosition(GetPosition() - Vector2(80.0f, 0.0f));
					else							m_pEffect->SetPosition(GetPosition() + Vector2(80.0f, 0.0f));
				}
				m_pEffect->SetScale(Vector2(3.0f, 3.0f));

				if (pBoss->IsAttacked() || pBoss->IsBlocking())
				{
					m_pEffect->SetState(PlayerEffect::ENEMYBLOCK);
				}
				else
				{
					if (m_nState == L_BULLET || m_nState == L_CHARGE)
						m_pEffect->SetState(PlayerEffect::ENEMYHIT);
					else
						m_pEffect->SetState(PlayerEffect::ENEMYHIT2);
				}

				if (!pBoss->IsAttacked() && !pBoss->IsBlocking())
				{
					if (m_nState == L_BULLET)		    pBoss->SetHP(pBoss->GetHP() - BulletDamage);
					else if (m_nState == L_CHARGE)		pBoss->SetHP(pBoss->GetHP() - ChragedBulletDamage);
					else if (m_nState == L_FULL_CHARGE)	pBoss->SetHP(pBoss->GetHP() - FullChargedBulletDamage);
					SOUND->Play("bulletHit");
				}

				if(!pBoss->IsBlocking())
					pBoss->SetAttacked(true);

				break;
			}
		}


	case Bullet::GRENADE:
		for (size_t i = 2; i < grounds.size(); i++)
		{
			if (Collision::HitTest(grounds[i], m_pCollider))
			{
				if (m_nState == GRENADE)
				{
					SetActive(false);
					m_pEffect->Reset();
					m_pEffect->SetActive(true);
					m_pEffect->SetPosition(GetPosition());
					m_pEffect->SetScale(Vector2(3.0f, 3.0f));
					m_pEffect->SetState(PlayerEffect::EXPLOSION);
					SOUND->Play("Exlposion");
				}
				break;
			}
		}
		if (Collision::HitTest(MMCollider, m_pCollider) && m_nState == GRENADE)
		{
			if (pMM->IsInvincible())
				return;

			SetActive(false);
			m_pEffect->Reset();
			m_pEffect->SetActive(true);
			m_pEffect->SetPosition(GetPosition());
			m_pEffect->SetScale(Vector2(3.0f, 3.0f));
			m_pEffect->SetState(PlayerEffect::EXPLOSION);
			SOUND->Play("Exlposion");
			if (MMCollider->GetPosition().x > m_pCollider->GetPosition().x)
				pMM->SetAttacked(true, false);
			if (MMCollider->GetPosition().x < m_pCollider->GetPosition().x)
				pMM->SetAttacked(false, true);
			else
				pMM->SetAttacked(true, true);
			pMM->SetHp(pMM->GetHp() - 5);
		}
		break;
	case Bullet::UPSHOT:
	case Bullet::FRONTSHOT:
		if (Collision::HitTest(MMCollider, m_pCollider))
		{
			if (pMM->IsInvincible())
				return;

			SetActive(false);
			if (MMCollider->GetPosition().x > m_pCollider->GetPosition().x)
				pMM->SetAttacked(true, false);
			if (MMCollider->GetPosition().x < m_pCollider->GetPosition().x)
				pMM->SetAttacked(false, true);
			else
				pMM->SetAttacked(true, true);
			pMM->SetHp(pMM->GetHp() - 3);
		}
		break;

	}


}
