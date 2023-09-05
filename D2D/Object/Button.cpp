#include "framework.h"
#include "Button.h"

Button::Button(wstring strImage1, wstring strImage2, wstring strShader)
{
	m_pTexture[0] = nullptr;
	m_pTexture[1] = nullptr;

	if (strImage1 != L"")
		m_pTexture[0] = new Texture(strImage1, strShader);
	if (strImage2 != L"")
		m_pTexture[1] = new Texture(strImage2, strShader);
}
void Button::Update(Matrix V, Matrix P)
{
	for (UINT i = 0; i < 2; i++)
	{
		m_pTexture[i]->SetPosition(GetPosition());
		m_pTexture[i]->SetScale(GetScale());
		m_pTexture[i]->SetRotation(GetRotation());
		m_pTexture[i]->Update(V, P);
	}



	MouseOnCheck();

	if (m_bHover)
	{
		if (Mouse->Down(0))
		{
			if (m_fpClickCallback)
				m_fpClickCallback(m_ID);
		}
	}

	
}

void Button::Render()
{
	
	if(m_bHover)
		m_pTexture[0]->Render();
	else
		m_pTexture[1]->Render();

}
void Button::MouseOnCheck()
{
	Vector2 position = Mouse->GetPosition();

	//ScreenToClient()
	

	Vector2 size = m_pTexture[0]->GetTextureRealSize();
	Vector2 min,max;
	min.x = GetPosition().x - size.x * 0.5f;
	min.y = GetPosition().y + size.y * 0.5f;
	max.x = GetPosition().x + size.x * 0.5f;
	max.y = GetPosition().y - size.y * 0.5f;

	CAMERA->VCtoWC(min);
	CAMERA->VCtoWC(max);

	if (min.x <= position.x && position.x <= max.x &&
		min.y <= position.y && position.y <= max.y)
		this->m_bHover = true;
	else
		m_bHover = false;


}



Button::~Button()
{
}
