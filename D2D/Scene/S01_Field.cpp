#include "framework.h"
#include "S01_Field.h"
#include "Collider/Line.h"

S01_Field::S01_Field()
{
	SetActive(true);  // 1st신일 경우
	SetSceneName("S01_Field");

	wstring strImage = L"./Textures/Megaman/resources/background/Background.bmp";
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
	CAMERA->SetCornerRight(Vector2(+size.x*0.5f - MAIN->GetWidth()*0.5f, 0.0f));

	// 플레이어 위치
	GameObject* pObject = OBJECTMANAGER->FindObject("MEGAMAN");
	pObject->SetPosition(-6000.0f, 0.0f);
	pObject->SetScale(3.0f, 3.0f);

	// Collider Line
	m_pLine = make_shared<Line>();
	m_pLine->AddLine(-size.x, -346.0f, +size.x, -346.0f);
	m_pLine->EndLine();

	// Zone

	Grounds.push_back(new Collider({ -6600.0f, 400.0f }, { -6585.0f,-400.0f }));
	Grounds.push_back(new Collider({ 6585.0f, 400.0f }, { 6600.0f,-400.0f }));	


	Grounds.push_back(new Collider({ -6600.0f,-284.0f }, { -5197.0f,-400.0f }));
	Grounds.push_back(new Collider({ -5800.0f,-231.0f }, { -5400.0f,-340.0f }));
	Grounds.push_back(new Collider({ -5000.0f,-176.0f }, { -4600.0f,-292.0f }));

	Grounds.push_back(new Collider({ -5000.0f, 200.0f }, { -4600.0f, 84.0f }));



}

S01_Field::~S01_Field()
{
}

void S01_Field::Update()
{
	Matrix V = CAMERA->GetView();
	Matrix P = CAMERA->GetProjection();
	m_pBackground->Update(V, P);
	
	OBJECTMANAGER->Update("MEGAMAN");
	m_pLine->Update(V, P);
	for (auto& c : Grounds)		c->Update(V, P);

}

void S01_Field::Render()
{
	m_pBackground->Render();
	OBJECTMANAGER->Render("MEGAMAN");
//	for (auto& c : Grounds)		c->Render();
}

void S01_Field::PreRender()
{
}

void S01_Field::PostRender()
{
	OBJECTMANAGER->FindObject("MEGAMAN")->PostRender();
}

shared_ptr<Line> S01_Field::GetLines()
{
	return m_pLine;
}
