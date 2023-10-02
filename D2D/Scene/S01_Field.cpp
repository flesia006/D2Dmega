#include "framework.h"
#include "S01_Field.h"
#include "Collider/Line.h"
#include "Object/GrenadeMan.h"
#include "Object/Spike.h"
#include "Object/Boss.h"


S01_Field::S01_Field()
{
	SetActive(true);  // 1st신일 경우
	SetSceneName("S01_Field");

	wstring strImage = L"./Textures/Megaman/resources/background/Background2.bmp";
	m_pBackground = make_shared<Texture>(strImage, g_ShaderFile);
	Vector2 size = m_pBackground->GetTextureRealSize();
	Vector2 position;
	m_pBackground->SetScale(Vector2(1.0f, 1.0f));
	m_pBackground->SetPosition(Vector2(m_pBackground->GetPosition().x, m_pBackground->GetPosition().y));

	position.x = -(size.x * 0.5f - MAIN->GetWidth() * 0.5f);
	position.y = 0.0f;
	CAMERA->SetPosition(position);
	CAMERA->SetGameObject(OBJECTMANAGER->FindObject("MEGAMAN"));
	CAMERA->SetCornerLeft(Vector2(-size.x*0.5f + MAIN->GetWidth()*0.5f,	0.0f));
	CAMERA->SetCornerRight(Vector2(+size.x*0.5f - MAIN->GetWidth()*0.5f - 1272.0f, 0.0f));

	// Sound
	SOUND->Play("bgm");
//	SOUND->SetMasterVolume(0.7f);

	// 플레이어 위치
	GameObject* pObject = OBJECTMANAGER->FindObject("MEGAMAN");
	pObject->SetPosition(-6200.0f, 0.0f);
	pObject->SetScale(3.1f, 3.1f);

	// 문 위치
	CreateAnimation();
	m_pAnimation->SetPlay(0);

	// 적 위치
	AddGrenadeMan(0, Vector2(-4900.0f-80.0f, 300.0f));
	AddGrenadeMan(3, Vector2(-4900.0f-80.0f, 0.0f));
	AddGrenadeMan(0, Vector2(-4700.0f-80.0f, 300.0f));

	AddGrenadeMan(0, Vector2(-3000.0f-80.0f, 180.0f));
	AddGrenadeMan(3, Vector2(-3000.0f-80.0f, -150.0f));
	AddGrenadeMan(0, Vector2(-2590.0f-80.0f, 315.0f));
	AddGrenadeMan(3, Vector2(-2590.0f-80.0f, -31.0f));
	
	AddSpikes		(Vector2(100.0f  - 80.0f, 100.0f));
	AddSpikes		(Vector2(200.0f  - 80.0f, -100.0f));
	AddSpikes		(Vector2(700.0f  - 80.0f, -140.0f));
	AddSpikes		(Vector2(700.0f  - 80.0f,  150.0f));
	AddSpikes		(Vector2(1600.0f - 80.0f, 100.0f));
	AddSpikes		(Vector2(1650.0f - 80.0f, 50.0f));
	AddSpikes		(Vector2(1700.0f - 80.0f, 0.0f));

	AddGrenadeMan(0, Vector2(2975.0f - 80.0f, 340.0f));
	AddGrenadeMan(3, Vector2(2975.0f - 80.0f, -340.0f));

	m_pBoss = new Boss();
	m_pBoss->SetState(Boss::IDLE);
	m_pBoss->SetPosition(Vector2(6200.0f, 0.0f));
	m_pBoss->SetScale(1.5f, 1.5f);

	// Collider Line
	m_pLine = make_shared<Line>();
	m_pLine->AddLine(-4200.0f - 80.0f, -290.0f, -3600.0f - 80.0f, -130.0f);
	m_pLine->AddLine(-1890.0f - 80.0f, -75.0f, -1285.0f - 80.0f, 85.0f);
	m_pLine->EndLine();

	// Zone
	

	Grounds.push_back(new Collider({ -6600.0f - 80.0f, 600.0f }, { -6585.0f - 80.0f,-400.0f }));
	Grounds.push_back(new Collider({ 6585.0f, 600.0f }, { 6678.0f,-400.0f }));


	Grounds.push_back(new Collider({ -6600.0f - 80.0f,-290.0f }, { -5197.0f - 80.0f,-400.0f }));
	Grounds.push_back(new Collider({ -5800.0f - 80.0f,-231.0f }, { -5400.0f- 80.0f,-340.0f }));
	Grounds.push_back(new Collider({ -5000.0f- 80.0f,-176.0f }, { -4600.0f - 80.0f,-292.0f }));
	Grounds.push_back(new Collider({ -5000.0f- 80.0f, 200.0f }, { -4600.0f - 80.0f, 84.0f }));
											 
	Grounds.push_back(new Collider({ -4300.0f- 80.0f, -290.0f }, { -4200.0f- 80.0f, -400.0f }));	
	Grounds.push_back(new Collider({ -3600.0f- 80.0f, -130.0f }, { -3400.0f- 80.0f, -230.0f }));
											 
	Grounds.push_back(new Collider({ -3090.0f- 80.0f, -290.0f }, { -2890.0f- 80.0f, -390.0f }));
	Grounds.push_back(new Collider({ -3090.0f- 80.0f, 33.0f }, { -2890.0f  - 80.0f, -67.0f }));
	Grounds.push_back(new Collider({ -2690.0f- 80.0f, -130.0f }, { -2490.0f- 80.0f, -230.0f }));
	Grounds.push_back(new Collider({ -2690.0f- 80.0f, 190.0f }, { -2490.0f - 80.0f, 90.0f }));
											 
	Grounds.push_back(new Collider({ -2290.0f- 80.0f, 30.0f }, { -2090.0f - 80.0f, -70.0f }));
	Grounds.push_back(new Collider({ -2090.0f- 80.0f, -75.0f }, { -1890.0f- 80.0f, -175.0f }));
	Grounds.push_back(new Collider({ -1285.0f- 80.0f, 85.0f }, { -1085.0f - 80.0f, -15.0f }));
	Grounds.push_back(new Collider({ -1085.0f- 80.0f, 140.0f }, { -885.0f - 80.0f, 40.0f }));

	//터널 구간
	Grounds.push_back(new Collider({ -985.0f- 80.0f, -300.0f }, { 2175.0f- 80.0f, -400.0f }));
	Grounds.push_back(new Collider({ -185.0f- 80.0f, -190.0f }, { -35.0f - 80.0f, -300.0f }));
	Grounds.push_back(new Collider({ 368.0f - 80.0f, -140.0f }, { 518.0f - 80.0f, -300.0f }));
	Grounds.push_back(new Collider({ 918.0f - 80.0f, -140.0f }, { 1068.0f- 80.0f, -300.0f }));
	Grounds.push_back(new Collider({ 1268.0f- 80.0f, -190.0f }, { 1418.0f- 80.0f, -300.0f }));
	Grounds.push_back(new Collider({ 2170.0f- 80.0f, -140.0f }, { 2570.0f- 80.0f, -400.0f }));

	Grounds.push_back(new Collider({ -580.0f- 80.0f, 400.0f }, { 2570.0f- 80.0f, 245.0f }));
	Grounds.push_back(new Collider({ -580.0f- 80.0f, 245.0f }, { -40.0f - 80.0f, 80.0f }));
	Grounds.push_back(new Collider({ 1620.0f- 80.0f, 245.0f }, { 1770.0f- 80.0f, 80.0f }));
	Grounds.push_back(new Collider({ 2170.0f- 80.0f, 245.0f }, { 2570.0f- 80.0f, 80.0f }));

	Grounds.push_back(new Collider({ 2823.0f- 80.0f, -290.0f },{ 3023.0f- 80.0f, -390.0f }));
	Grounds.push_back(new Collider({ 2823.0f- 80.0f, 190.0f }, { 3023.0f- 80.0f, 90.0f }));
	Grounds.push_back(new Collider({ 3023.0f- 80.0f, 190.0f }, { 3138.0f- 80.0f, -390.0f }));
	Grounds.push_back(new Collider({ 3425.0f- 80.0f, 30.0f },  { 3725.0f- 80.0f, -70.0f }));
	Grounds.push_back(new Collider({ 4030.0f- 80.0f, -290.0f },{ 6675.0f, -400.0f }));
	Grounds.push_back(new Collider({ 4030.0f- 80.0f, -190.0f },{ 4475.0f- 80.0f, -290.0f }));
	Grounds.push_back(new Collider({ 4030.0f- 80.0f, 400.0f }, { 4475.0f- 80.0f, 30.0f }));
	Grounds.push_back(new Collider({ 4475.0f- 80.0f, 400.0f }, { 5240.0f- 80.0f, 350.0f }));
	Grounds.push_back(new Collider({ 5240.0f- 80.0f, 400.0f }, { 5395.0f- 80.0f, 140.0f }));
	Grounds.push_back(new Collider({ 5395.0f- 80.0f, 400.0f }, { 5495.0f- 80.0f, -75.0f }));


	//천장
	Grounds.push_back(new Collider({ 5323.5f, 500.0f }, { 6680.5f, 400.0f }));
	Grounds.push_back(new Collider({ -6600.0f - 80.0f , 600.0f }, { 6680.0f, 500.0f }));

	// 무조건 마지막
	Grounds.push_back(new Collider({ 5323.5f, -75.0f }, { 5415.5f, -290.0f }));

	

	//바닥 임시
//	Grounds.push_back(new Collider({ -6600.0f, -390.0f }, { 6600.0f, -400.0f }));

}

S01_Field::~S01_Field()
{
}

void S01_Field::Update()
{
	if (KEYBOARD->Down(VK_F1))
	{
		if (!m_bDebug)
			m_bDebug = true;
		else
			m_bDebug = false;
	}


	m_pBackground->Update();
	
	OBJECTMANAGER->Update("MEGAMAN");
	SpawnEnemy();

	for (auto& a : m_cvGrenadeMan)
		a->Update();

	for (auto& a : m_cvSpikes)
		a->Update();

	m_pBoss->Update();
	m_pAnimation->Update();
	m_pLine->Update();

	for (auto& c : Grounds)		
		c->Update();


	GameObject* pObject = OBJECTMANAGER->FindObject("MEGAMAN");

	if(pObject->GetPosition().y < - 600.0f)
		pObject->SetPosition(-6000.0f, 0.0f);


	SOUND->Update();
}

void S01_Field::Render()
{
	m_pBackground->Render();
	m_pAnimation->Render();
	OBJECTMANAGER->Render("MEGAMAN");
	OBJECTMANAGER->FindObject("MEGAMAN")->SetDebug(m_bDebug);

	for (auto& a : m_cvGrenadeMan)
	{
		a->Render();
		a->SetDebug(m_bDebug);
	}

	for (auto& a : m_cvSpikes)
	{
		a->Render(); 
		a->SetDebug(m_bDebug);
	}		

	m_pBoss->Render();
	m_pBoss->SetDebug(m_bDebug);

	if (m_bDebug)
	{
		m_pLine->Render();
		for (auto& c : Grounds)
			c->Render();
	}

}

void S01_Field::PreRender()
{
}

void S01_Field::PostRender()
{
	OBJECTMANAGER->FindObject("MEGAMAN")->PostRender();
	m_pBoss->PostRender();
}

void S01_Field::AddGrenadeMan(UINT mode, Vector2 position)
{
	m_cvGrenadeMan.push_back(new GrenadeMan());
	int last = m_cvGrenadeMan.size() - 1;

	switch (mode)
	{
	case 0:
		m_cvGrenadeMan[last]->SetMode(GrenadeMan::ONLY_BOMB);
		m_cvGrenadeMan[last]->SetState(GrenadeMan::LEFT_IDLE);
		m_cvGrenadeMan[last]->SetOriginState(GrenadeMan::LEFT_IDLE);
		m_cvGrenadeMan[last]->SetPosition(position);
		m_cvGrenadeMan[last]->SetOriginPos(position);
		m_cvGrenadeMan[last]->SetHP(GM_Hp);
		m_cvGrenadeMan[last]->SetScale(3.1f, 3.1f);
		break;
	case 1:
		break;
	case 2:
		break;
	case 3:
		m_cvGrenadeMan[last]->SetMode(GrenadeMan::WALK_SHOOT);
		m_cvGrenadeMan[last]->SetState(GrenadeMan::GREEN_MOVE);
		m_cvGrenadeMan[last]->SetOriginState(GrenadeMan::GREEN_MOVE);
		m_cvGrenadeMan[last]->SetPosition(position);
		m_cvGrenadeMan[last]->SetOriginPos(position);
		m_cvGrenadeMan[last]->SetHP(GM_Hp);
		m_cvGrenadeMan[last]->SetScale(3.1f, 3.1f);
		break;



	}

}

void S01_Field::AddSpikes(Vector2 position)
{
	m_cvSpikes.push_back(new Spike());
	int last = m_cvSpikes.size() - 1;

	m_cvSpikes[last]->SetPosition(position);
	m_cvSpikes[last]->SetOriginPos(position);
	m_cvSpikes[last]->SetHP(SP_Hp);
	m_cvSpikes[last]->SetScale(1.5f, 1.5f);
}

void S01_Field::SpawnEnemy()
{
	Vector2 Xpos =  OBJECTMANAGER->FindObject("MEGAMAN")->GetPosition();
	for (auto& a : m_cvGrenadeMan)
	{
		// 죽었을 때 리스폰
		if (!a->IsActive() && abs(a->GetOriginPos().x - Xpos.x) > 900.0f && a->GetHP() <= 0)
		{
		//	if (a->GetMode() == GrenadeMan::SPIKEMODE)
		//		break;

			a->Reset();
			a->SetActive(true);
			a->SetRotation(0, 0, 0);
			a->SetHP(GM_Hp);
		}
		// 근처에 가면 스폰
		if (!a->IsActive() && abs(a->GetPosition().x - Xpos.x) <= 900.0f && a->GetHP() > 0)
		{
			a->SetActive(true);
		}
		// 멀리 있는 적은 비활성화
		if (a->IsActive() && abs(a->GetPosition().x - Xpos.x) > 1000.0f)
		{
			a->SetActive(false);
		}
	}

	for (auto& a : m_cvSpikes)
	{
		// 죽었을 때 리스폰
		if (!a->IsActive() && abs(a->GetOriginPos().x - Xpos.x) > 900.0f && a->GetHP() <= 0)
		{
			a->Reset();
			a->SetActive(true);
			a->SetRotation(0, 0, 0);
			a->SetHP(SP_Hp);
		}
		// 근처에 가면 스폰
		if (!a->IsActive() && abs(a->GetPosition().x - Xpos.x) <= 900.0f && a->GetHP() > 0)
		{
			a->SetActive(true);
		}
		// 멀리 있는 적은 비활성화
		if (a->IsActive() && abs(a->GetPosition().x - Xpos.x) > 1000.0f)
		{
			a->SetActive(false);
		}
	}

	// 근처에 가면 스폰
	if (!m_pBoss->IsActive() && abs(m_pBoss->GetPosition().x - Xpos.x) <= 900.0f && m_pBoss->GetHP() > 0)
	{
		m_pBoss->SetActive(true);
	}
	// 멀리 있는 적은 비활성화
	if (m_pBoss->IsActive() && abs(m_pBoss->GetPosition().x - Xpos.x) > 1000.0f)
	{
		m_pBoss->SetActive(false);
	}
}

void S01_Field::CreateAnimation()
{
	wstring strImage = L"./Textures/Megaman/resources/Object/Door.bmp";

	m_pAnimation = make_shared<Animation>(strImage, g_ShaderFile, Vector2(1.7f, 1.7f), Vector2(5368.5f, -186));
	// idle
	{
		shared_ptr<AnimationClip>	pClip = make_shared<AnimationClip>(AnimationClip::EndStay);
		shared_ptr<Texture>			pTexture = m_pAnimation->GetTexture();
		m_pAnimation->AddClip(pClip);

		for (int i = 0; i < 1; i++)
		{
			int sx = 64 * i;
			int sy = 0;
			int ex = sx + 64;
			int ey = 132;
			pClip->AddFrame(pTexture, strImage, sx, sy, ex, ey, 0.1f);
		}
	}


	// open
	{
		shared_ptr<AnimationClip>	pClip = make_shared<AnimationClip>(AnimationClip::EndStay);
		shared_ptr<Texture>			pTexture = m_pAnimation->GetTexture();
		m_pAnimation->AddClip(pClip);

		for (int i = 0; i < 13; i++)
		{
			int sx = 64 * i;
			int sy = 0;
			int ex = sx + 64;
			int ey = 132;
			pClip->AddFrame(pTexture, strImage, sx, sy, ex, ey, 0.05f);
		}
	}

	// close
	{
		shared_ptr<AnimationClip> pClip = make_shared<AnimationClip>(AnimationClip::EndStay);
		shared_ptr<Texture>       pTexture = m_pAnimation->GetTexture();
		m_pAnimation->AddClip(pClip);

		for (int i = 12; i >= 0; i--)
		{
			int sx = 64 * i;
			int sy = 0;
			int ex = sx + 64;
			int ey = 132;
			pClip->AddFrame(pTexture, strImage, sx, sy, ex, ey, 0.05f);
		}
	}
}

