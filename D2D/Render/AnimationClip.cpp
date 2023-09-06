#include "framework.h"
#include "AnimationClip.h"


AnimationClip::AnimationClip(eState state)
{
	m_State = state; // Loop, Once, End, EndStay
}

AnimationClip::~AnimationClip()
{
	for (UINT i = 0; i < m_cvFrames.size(); ++i)
	{
		delete m_cvFrames[i];
	}
	m_cvFrames.erase(m_cvFrames.begin(), m_cvFrames.end());
}


void AnimationClip::Update(Matrix V, Matrix P)
{
	// 이상 처리
	if (m_cvFrames.size() == 0)	return;
	if (m_cvFrames.size() <= m_nCurrentFrame)	return;

	Frame* pFrame = m_cvFrames[m_nCurrentFrame];

	// 시간 체크
	if (IsPlay())		// loop(IDLE) : 무한 루프, 그 외에는 attack 
	{
		m_PlayTime += TIME->Delta();
		if (m_PlayTime >= pFrame->m_Time)
		{
			m_nCurrentFrame++;
			// 마지막 프레임까지 왔는가 Check
			if (m_nCurrentFrame == m_cvFrames.size())
			{
				if	(m_State == eState::Once)
				{
					SetStop();
					m_nCurrentFrame = 0;	// 맨 앞 frame 
				}
				else if (m_State == eState::End)
				{
					SetStop();
					m_nCurrentFrame = m_cvFrames.size() - 1;
				}
				else if (m_State == eState::EndStay)
				{
					m_nCurrentFrame = m_cvFrames.size() - 1;
				}
				else m_nCurrentFrame = 0;	// 맨 앞 frame 
			}			
			m_PlayTime = 0.0f;
		}// if end
	}
	//Frame Num 가 바뀌었을 것
	pFrame = m_cvFrames[m_nCurrentFrame];
	m_ptrTexture->SetImageFile(pFrame->m_strImageFile);
	m_ptrTexture->SetOffset(pFrame->m_Offset);
	m_ptrTexture->SetOffsetSize(pFrame->m_OffsetSize);
	m_ptrTexture->Update(V, P);
}

void AnimationClip::Render()
{
	// 이상 처리
	if (m_cvFrames.size() == 0)	return;
	if (m_cvFrames.size() <= m_nCurrentFrame)	return;

	m_ptrTexture->Render();
}

// 1. 1 frame 단위로 Vector 에 저장 
// 2. Animation -> Texture 생성
void AnimationClip::AddFrame(shared_ptr<Texture> pTexture, wstring strImageFile, float startX, float startY, float endX, float endY, float delta)
{
	Frame* pFrame = new Frame();

	pFrame->m_strImageFile	= strImageFile;			
	pFrame->m_Offset		= Vector2(startX, startY);		
	pFrame->m_OffsetSize	= Vector2(endX - startX, endY - startY);	
	pFrame->m_Time			= delta;

	pTexture->CreateShaderResourceView(strImageFile);
	m_cvFrames.push_back(pFrame);

	m_ptrTexture			= pTexture;	// 주소 복사
}

void AnimationClip::AddFrame(wstring strImageFile, float startX, float startY, float endX, float endY, float delta)
{
	Frame* pFrame = new Frame();

	pFrame->m_strImageFile = strImageFile;
	pFrame->m_Offset = Vector2(startX, startY);
	pFrame->m_OffsetSize = Vector2(endX - startX, endY - startY);
	pFrame->m_Time = delta;

	if (m_ptrTexture == nullptr)
	{
		MessageBox(NULL, L"Texture가 null 입니다.", L"Game", NULL);
	}
	else
	{
		m_ptrTexture->CreateShaderResourceView(strImageFile);
		m_cvFrames.push_back(pFrame);		
	}

}

void AnimationClip::SetOffsetSize(float x, float y)
{
	for (UINT i = 0; i < m_cvFrames.size(); i++)
	{
		Frame* pFrame = m_cvFrames[i];
		pFrame->m_OffsetSize = Vector2(x, y);
	}
}


