#include "framework.h"
#include "Bullet.h"

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

	auto pTexture = m_pAnimation->GetTexture();
	pTexture->UpdateColorBuffer(3);
	// collider
	m_pCollider = new Collider();
}

Bullet::~Bullet()
{
	SAFE_DELETE(m_pAnimation);
	SAFE_DELETE(m_pCollider);
}

void Bullet::Update(Matrix V, Matrix P)
{
	const  float move_speed = 1500.0f;

	if (!IsActive()) return;

	Vector2 position = GetPosition();


	if (m_Direction == 0)
			position.x = position.x - move_speed * TIME->Delta();
	else
			position.x = position.x + move_speed * TIME->Delta();


	// Player의 현재에서 300Pixel이상 되면 없어지게 한다
	Vector2  playerPos = OBJECTMANAGER->FindObject("MEGAMAN")->GetPosition();

	double   dx = position.x - playerPos.x;
	double   dy = position.y - playerPos.y;
	double   dist = sqrt(dx * dx + dy * dy);

	if (dist > 1000.0f)
		SetActive(false);

	SetPosition(position);

	if (m_Direction == 0)
		SetRotation(Vector3(0.0f, 0.0f, 0.0f));
	else
		SetRotation(Vector3(0.0f, 180.0f, 0.0f));

	SET(m_pAnimation);
	m_pAnimation->Update(V, P);

	SET(m_pCollider);
	m_pCollider->SetScale(m_pAnimation->GetTextureRealSize() - Vector2(0, 40.0f));
	m_pCollider->Update(V, P);

}

void Bullet::Render()
{
	if (!IsActive()) return;

	m_pAnimation->Render();
	//m_pCollider->Render();
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
