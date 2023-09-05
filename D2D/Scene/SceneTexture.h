#pragma once
class SceneTexture final : public Scene
{
public:
	virtual void Update() override;
	virtual void Render() override;


public:
	shared_ptr<Texture>   m_pTexture;
	shared_ptr<Animation> m_pAnimation;

public:
	SceneTexture();
	~SceneTexture();
};

