#pragma once
///////////////////////////////////////
/// Animation
//    AnimationClip
///     Texture
///////////////////////////////////////
class AnimationClip
{
public:
	enum eState
	{
		Loop = 0,   // IDLE
		Once,       // Attack후 1st  image wait
		End,        // Attack후 last image wait
		EndStay
	};
	void  Update();
	void  Render();

public:
	bool IsPlay() { return m_bPlay; }

public:				// setter
	void AddFrame(shared_ptr<Texture> pTexture, wstring strImageFile, float startX, float startY, float endX, float endY, float delta);
	void AddFrame(wstring strImageFile, float startX, float startY, float endX, float endY, float delta);
	void SetPosition(float x, float y)			{ m_ptrTexture->SetPosition(x, y); }
	void SetPosition(Vector2 pos)				{ m_ptrTexture->SetPosition(pos); }
	void SetScale(float x, float y)				{ m_ptrTexture->SetScale(x, y); }
	void SetScale(Vector2 scale)				{ m_ptrTexture->SetScale(scale); }	
	void SetRotation(Vector3 rotation)			{ m_ptrTexture->SetRotation(rotation); }
	void SetRotation(float x, float y, float z) { m_ptrTexture->SetRotation(x, y, z); }
	void SetOffsetSize(float x, float y); 

	void SetStop() { m_bPlay = false; }
	void SetPlay(UINT currentFrameNo = 0) { m_nCurrentFrame = currentFrameNo;  m_bPlay = true; }
	void SetTexture(shared_ptr<Texture> pTexture) { m_ptrTexture = pTexture; }

	UINT GetCurrentFrameNo() { return m_nCurrentFrame; }

private:
	struct Frame
	{
		wstring m_strImageFile;							//Texture SetImageFile 에 보내는 문자열
		Vector2 m_Offset = Vector2(0.0f, 0.0f);			// Texture UpdateBlock
		Vector2 m_OffsetSize = Vector2(0.0f, 0.0f);

		float	m_Time = 0.0f;							// 동적시간
	};

	vector<Frame*>		m_cvFrames;							//IDLE 10 frame 이면 여기에 잡힘
	eState				m_State = eState::Loop;
	UINT				m_nCurrentFrame = 0;
	shared_ptr<class Texture>	m_ptrTexture;				// 주소만 가짐
	bool				m_bPlay = true;						// Animation 실행중이면 true
	float				m_PlayTime = 0.0f;

public:
	AnimationClip(eState state = eState::Loop);
	~AnimationClip();
};