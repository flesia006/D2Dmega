#include  "framework.h"
#include  "Animation.h"


Animation::Animation(wstring imgFile, wstring shaderFile)
{
	m_pTexture = make_shared<Texture>(imgFile, shaderFile);
}

Animation::Animation(wstring imgFile, wstring shaderFile, Vector2 scale, Vector2 position, Vector3 Rotation)
{
	m_pTexture = make_shared<Texture>(imgFile, shaderFile);
	m_pTexture->SetScale(scale);
	m_pTexture->SetPosition(position);
	m_pTexture->SetRotation(Rotation);
}

Animation::~Animation()
{

}

void Animation::Update(Matrix V, Matrix P)
{
	// 捞惑贸府
	if (m_cvAnimationClips.size() == 0) return;
	if (m_nAnimeState >= m_cvAnimationClips.size()) return;

	shared_ptr<AnimationClip> pClip = m_cvAnimationClips[m_nAnimeState];
	pClip->Update(V, P);
}

void Animation::Render()
{
	// 捞惑贸府
	if (m_cvAnimationClips.size() == 0) return;
	if (m_nAnimeState >= m_cvAnimationClips.size()) return;

	shared_ptr<AnimationClip> pClip = m_cvAnimationClips[m_nAnimeState];
	pClip->Render();

}

void Animation::AddClip(shared_ptr<AnimationClip> pClip)
{
	pClip->SetTexture(m_pTexture);
	m_cvAnimationClips.push_back(pClip);
}

void Animation::SetOffsetSize(float x, float y)
{
	for (UINT i = 0; i < m_cvAnimationClips.size(); i++)
	{
		shared_ptr<AnimationClip> pClip = m_cvAnimationClips[i];
		pClip->SetOffsetSize(x, y);
	}
}

void Animation::SetPlay(UINT PlayNo, UINT currentFrame)
{
	// 捞惑贸府
	if (m_cvAnimationClips.size() == 0) return;
	if (m_nAnimeState >= m_cvAnimationClips.size()) return;

	shared_ptr<AnimationClip> pClip = m_cvAnimationClips[m_nAnimeState];

	if (pClip->IsPlay() && PlayNo == m_nAnimeState)
		return;

	m_nAnimeState = PlayNo;
	pClip->SetPlay(currentFrame);

}

void Animation::SetStop()
{
	// 捞惑贸府
	if (m_cvAnimationClips.size() == 0) return;
	if (m_nAnimeState >= m_cvAnimationClips.size()) return;

	shared_ptr<AnimationClip> pClip = m_cvAnimationClips[m_nAnimeState];
	pClip->SetStop();
}

bool Animation::IsPlay()
{
	// 捞惑贸府
	if (m_cvAnimationClips.size() == 0) return false;
	if (m_nAnimeState >= m_cvAnimationClips.size()) return false;

	shared_ptr<AnimationClip> pClip = m_cvAnimationClips[m_nAnimeState];
	return pClip->IsPlay();
}


