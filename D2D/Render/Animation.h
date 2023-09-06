#pragma once
class Animation
{

public:
	Animation(wstring imgFile, wstring shaderFile);
	Animation(wstring imgFile, wstring shaderFile, Vector2 scale, Vector2 position, Vector3 Rotation = {0, 0, 0});
	~Animation();

public:
	void Update(Matrix V, Matrix P);
	void Render();

public:				
	// setter
	void AddClip(shared_ptr<AnimationClip> pClip);

	void SetPosition(float x, float y)			{ m_pTexture->SetPosition(x, y); }
	void SetPosition(Vector2 pos)				{ m_pTexture->SetPosition(pos); }
	void SetScale(float x, float y)				{ m_pTexture->SetScale(x, y); }
	void SetScale(Vector2 scale)				{ m_pTexture->SetScale(scale); }
	void SetRotation(Vector3 rotation)			{ m_pTexture->SetRotation(rotation); }
	void SetRotation(float x, float y, float z) { m_pTexture->SetRotation(x, y, z); }
	void SetOffsetSize(float x, float y);

	void SetPlay(UINT PlayNo);
	void SetStop();

public:				// getter
	bool IsPlay();
	shared_ptr<Texture> GetTexture()		{ return m_pTexture; }
	Vector2  GetPosition()					{ return m_pTexture->GetPosition(); }
	Vector2  GetScale()						{ return m_pTexture->GetScale(); }
	Vector3  GetRotation()					{ return m_pTexture->GetRotation(); }
	Vector2  GetTextureRealSize()			{ return m_pTexture->GetTextureRealSize(); }
	UINT	 GetClipNo()					{ return m_nAnimeState; }


private:
	shared_ptr<class Texture> m_pTexture;	
	vector<shared_ptr<class AnimationClip>> m_cvAnimationClips;
	UINT m_nAnimeState = 0;



};

