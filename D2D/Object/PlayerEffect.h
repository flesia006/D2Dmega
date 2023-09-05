#pragma once
class   PlayerEffect : public GameObject
{
public:
	enum eState
	{
		EFFECT_CHARGE,
		EFFECT_CHARGE_BODY,
		EFFECT_CHARGE_GREEN,
		EFFECT_CHARGE_BODY_GREEN,
		EFFECT_DASH_L,
		EFFECT_DASH_R,
		EFFECT_DASHDUST_L,
		EFFECT_DASHDUST_R,
	};

	PlayerEffect();
	~PlayerEffect();

public:
	bool IsPlay()   { return m_pAnimation->IsPlay(); }
	void SetState(eState value) { m_nState = value; m_pAnimation->SetPlay((UINT)m_nState); }
	void SetCallback(function<void()> cbFunction) { m_fpEndCallback = cbFunction; }

public: // 가상함수
	void Update(Matrix V, Matrix P) override;    // 순수 가상 함수
	void Render() override;                     // 순수 가상 함수
	void Reset() {};
	void PostRender() {};

private:
	void   CreateClip(wstring strImage, int w, int h, int count, AnimationClip::eState state = AnimationClip::eState::Loop, float speed = 0.1f);

private:
	shared_ptr<class Animation>	 m_pAnimation;
	eState						m_nState = eState::EFFECT_CHARGE;
	function<void()>			m_fpEndCallback  = nullptr;
};
