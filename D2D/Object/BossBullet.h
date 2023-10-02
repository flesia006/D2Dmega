#pragma once

class PlayerEffect;
class BossBullet : public GameObject
{
public:
	enum eState
	{
		WAVE,
		GROUND_ATK,
		ASCENDING_ATK,
		SPARK,
		BIG_WAVE, 
		THUNDER
	};

public:
	BossBullet();
	BossBullet(eState state);
	~BossBullet();


public: // 가상함수
	virtual void Update() override;    // 순수 가상 함수
	virtual void Render() override;                     // 순수 가상 함수
	virtual void Reset() override;
	virtual void PostRender() {};

public:
	void    SetDirection(int value) { m_Direction = value; }
	bool    IsPlay() { return m_pAnimation->IsPlay(); }
	void    SetState(eState value) { m_nState = value; m_pAnimation->SetPlay((UINT)m_nState); }
	void	SetActive(bool value) { m_bActive = value; if (m_pCollider) m_pCollider->SetActive(value); }
	void    HitCheck(Vector2& position);
	int     GetDirection() { return m_Direction; }

private:
	void   CreateAnimation();
	void   CreateClip(wstring strImage, int w, int h, int count, 
		AnimationClip::eState state = AnimationClip::Loop, float speed = 0.1f);

private:
	eState     m_nState = eState::WAVE;
	Animation* m_pAnimation = nullptr;
	int        m_Direction = 0;  //왼/오른쪽
	float      m_Gravirty = 0.0f;
	const int  BulletDamage = 1;
	const int  ChragedBulletDamage = 4;
	const int  FullChargedBulletDamage = 10;
};

