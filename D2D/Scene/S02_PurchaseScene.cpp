#include "framework.h"
#include "S02_PurchaseScene.h"
#include  "Object/Button.h"
#include  "Object/Player.h"


Player* PLAYER=nullptr;


S02_PurchaseScene::S02_PurchaseScene()
{
	PLAYER = new Player();


	m_strSceneName = "S02_PurchaseScene";

	wstring  stImage = L"./Textures/CookieRun/Image/PrepareSceneBackground.png";
	wstring  strShader = L"./Shader/Texture.hlsl";
	m_pBackground[0] = new Texture(stImage, strShader);

	stImage = L"./Textures/CookieRun/Image/PrepareScenePetChoice.png";
	m_pBackground[1] = new Texture(stImage, strShader);

	stImage = L"./Textures/CookieRun/Image/PrepareScenePetChoice2.png";
	m_pBackground[2] = new Texture(stImage, strShader);

	for (int i = 0; i < 3; i++)
	{
	//	m_pBackground[i]->SetPosition(0.0f, -24.0f);
	}


	stImage = L"./Textures/CookieRun/Image/";
	wstring  stImage1 = stImage + L"PurchaseButtonIdle.png";
	wstring  stImage2 = stImage + L"PurchaseButtonMouseOver.png";

	function<void(int)> fp;   // void 함수명(전달인자)
	fp = bind(&S02_PurchaseScene::OnBulltonClicked, this, placeholders::_1);

	int id = 1;

	float y = 350.0f;
	for(int i=0; i<3; i++)
	{
		m_pButton[i] = new Button(stImage1, stImage2, strShader);
		m_pButton[i]->SetScale(0.7f, 0.7f);
		m_pButton[i]->SetPosition(410.0f, y);
		m_pButton[i]->SetCallback(fp);
		m_pButton[i]->SetButtonID(id);
		y -= 350.0f;

		id++;
	}
	 y = 300.0f;
	for (int i = 3; i < 6; i++)
	{
		m_pButton[i] = new Button(stImage1, stImage2, strShader);
		m_pButton[i]->SetPosition(410.0, y);
		m_pButton[i]->SetScale(0.7f, 0.7f);
		m_pButton[i]->SetCallback(fp);
		m_pButton[i]->SetButtonID(id);
		y -= 350.0f;
		id++;
	}


	for (int i = 6; i < 16; i++)
	{
		m_pButton[i] = new Button(stImage1, stImage2, strShader);
		m_pButton[i]->SetPosition(800.0, 0.7f);
		m_pButton[i]->SetScale(0.3f, 0.3f);
		m_pButton[i]->SetCallback(fp);
		m_pButton[i]->SetButtonID(id);
		id++;
	}


}

S02_PurchaseScene::~S02_PurchaseScene()
{
}

void S02_PurchaseScene::Update()
{
	Matrix V, P;
	V = CAMERA->GetView();
	P = CAMERA->GetProjection();
	for(int i =0; i<3; i++)
	  m_pBackground[i]->Update(V, P);
	for (int i = 0; i < 16; i++)
		m_pButton[i]->Update(V, P);

}

void S02_PurchaseScene::Render()
{
	for (int i = 0; i < 3; i++)
		m_pBackground[i]->Render();
	for (int i = 0; i < 16; i++)
		m_pButton[i]->Render();
}
void S02_PurchaseScene::PostRender()
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

		rect.top    = rect.top + 20;
		rect.bottom = rect.bottom + 20;
		DirectWrite::RenderText(str, rect);



	}
	DirectWrite::EndDraw();
}
void S02_PurchaseScene::OnBulltonClicked(int id)
{
	switch (id)
	{
	case 1:
		if(PLAYER->GetPet(Player::EPet::PET2))
		    MessageBox(NULL,L"Pet2", L"AAA", MB_OK);
		if (PLAYER->GetPet(Player::EPet::PET1))
			MessageBox(NULL, L"Pet1", L"AAA", MB_OK);
		break;
	}

}