#include "framework.h"
#include "S01_TitleScene.h"
#include "Object/Button.h"

S01_TitleScene::S01_TitleScene()
{
	SetActive(true);

	SetSceneName("S01_TitleScene");



	wstring  stImage = L"./Textures/CookieRun/Title/TitleCsceneImage.png";
	wstring  strShader = L"./Shader/Texture.hlsl";
	m_pBackground1 = new Texture(stImage, strShader);


	function<void(int)> fp;   // void 함수명(전달인자)
	fp = bind(&S01_TitleScene::OnBulltonClicked, this, placeholders::_1);

	{
		wstring  stImage =  L"./Textures/CookieRun/Image/";
		wstring  stImage1 = stImage + L"GameStartButton.png";
		wstring  stImage2 = stImage + L"GameStartButtonMouseOver.png";

		m_pButtonStart = new Button(stImage1, stImage2, strShader);
		m_pButtonStart->SetPosition(467.0f, -120.0f);
		m_pButtonStart->SetScale(0.7f, 0.7f);
		m_pButtonStart->SetButtonID(1);
		m_pButtonStart->SetCallback(fp);


	}
	{
		wstring  stImage = L"./Textures/CookieRun/Image/";
		wstring  stImage1 = stImage + L"GameStartButton.png";
		wstring  stImage2 = stImage + L"GameStartButtonMouseOver.png";

		m_pButtonExit = new Button(stImage1, stImage2, strShader);
		m_pButtonExit->SetPosition(467.0f, -350.0f);
		m_pButtonExit->SetScale(0.7f, 0.7f);
		m_pButtonExit->SetButtonID(2);
		m_pButtonExit->SetCallback(fp);
	}

}

S01_TitleScene::~S01_TitleScene()
{
	SAFE_DELETE(m_pBackground1);
}

void S01_TitleScene::Update()
{
	Matrix V = CAMERA->GetView();
	Matrix P = CAMERA->GetProjection();

	m_pBackground1->Update(V, P);
	m_pButtonStart->Update(V, P);
	m_pButtonExit->Update(V, P);

}

void S01_TitleScene::Render()
{
	m_pBackground1->Render();
	m_pButtonStart->Render();
	m_pButtonExit->Render();
}

void S01_TitleScene::OnBulltonClicked(int id)
{
	switch (id)
	{
	case 1:
		SCENEMANAGER->ChangeScene("S02_PurchaseScene");
		break;
	case 2:
		PostQuitMessage(0);
		break;
	}
}
