#include "framework.h"
#include "SceneTexture.h"



SceneTexture::SceneTexture()
{
	wstring  img = L"./Textures/CookieRun/Coin.png";
//	m_pTexture = new Texture(img, L"./Shader/Texture.hlsl");
	m_pTexture = new Texture(img, L"./Shader/Texture.hlsl"
	,58*4,0,58*5,59);


	printf("aaa");

	img = L"./Textures/helltaker/idle.png";
	m_pAnimation = new Animation(img, L"./Shader/Texture.hlsl");
	// idle
	{
		shared_ptr<AnimationClip> pClip = make_shared<AnimationClip>(AnimationClip::Loop);
		shared_ptr<Texture> pTexture = m_pAnimation->GetTexture();
		m_pAnimation->AddClip(pClip);
		wstring strImage = L"./Textures/helltaker/idle.png";
		for (int i = 0; i < 12; ++i)
		{
			pClip->AddFrame(pTexture, strImage, (float)i * 100, 30.0f, (float)(i + 1) * 100, 130.0f, 0.05f);
		}
	}

	// run
	{
		AnimationClip* pClip = new AnimationClip(AnimationClip::Loop);
		Texture* pTexture = m_pAnimation->GetTexture();
		m_pAnimation->AddClip(pClip);
		wstring strImage = L"./Textures/helltaker/move.png";
		for (int i = 0; i < 5; ++i)
		{
			pClip->AddFrame(pTexture, strImage, (float)i * 100, 30.0f, (float)(i + 1) * 100, 130.0f, 0.05f);
		}
	}




}

SceneTexture::~SceneTexture()
{
}

void SceneTexture::Update()
{
	// P,V를 만들기 
	Matrix  V, P;

	Vector3 eye = Vector3(0.0f, 0.0f, 0.0f);
	Vector3 up  = Vector3(0.0f, 1.0f, 0.0f);
	Vector3 at  = Vector3(0.0f, 0.0f, 0.1f);
	Vector3 temp = eye + at;
	
	D3DXMatrixLookAtLH(&V, &eye, &temp, &up);

	D3DXMatrixOrthoOffCenterLH(&P, -(float)(MAIN->GetWidth() * 0.5f),
		(float)(MAIN->GetWidth() * 0.5f),
		-(float)(MAIN->GetHeight() * 0.5f),
		(float)(MAIN->GetHeight() * 0.5f), -1.0f,1.0f);

	m_pTexture->Update(V,P);

	m_pAnimation->SetPosition(100.0f, 100.0f);

	if( (KEYBOARD->Press('A')) || (KEYBOARD->Down('D')))
		m_pAnimation->SetPlay(1);
	else
		m_pAnimation->SetPlay(0);

	m_pAnimation->Update(V, P);

}

void SceneTexture::Render()
{
	m_pAnimation->Render();
	m_pTexture->Render();
}


