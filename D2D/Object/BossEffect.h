#pragma once
class BossEffect : public GameObject
{
public:
	enum eState
	{
		IDLE = 0,
		GUARD,
		DOWN_SLASH,
		UP_SLASH,
		THUNDER,
		DISAPPEAR,
		APPEAR,
		DASH,
		JUMPATTACK,
		OPENING,
		DEFEAT,
		CHARGING
	};

public:
	BossEffect();
	~BossEffect();

public: // 가상함수
	void Update() override;
	void Render() override;
	void Reset() {};
	void PostRender() {};
	void SetState(eState state, bool reset = false);
	shared_ptr<class Animation> GetAnimation() { return m_pAnimation; }

private:
	bool IsPlay() { return m_pAnimation->IsPlay(); }
	void CreateAnimation();
	void   CreateClip(wstring strImage, int w, int h, int count,
		AnimationClip::eState state = AnimationClip::eState::Loop, float speed = 0.1f, int term = 100);


private:
	shared_ptr<class Animation>   m_pAnimation;
	eState m_nState = eState::IDLE;

};

