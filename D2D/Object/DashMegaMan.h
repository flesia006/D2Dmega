#pragma once
class DashMegaMan : public GameObject
{
public:
	enum eState
	{
		LEFT_IDLE = 0,
		RIGHT_IDLE,
		LEFT_MOVE,
		RIGHT_MOVE,
		LEFT_JUMP,
		RIGHT_JUMP,
		LEFT_FALL,
		RIGHT_FALL,
		LEFT_ATTACK,
		RIGHT_ATTACK,
		ATTACK_CHARGE_L,
		ATTACK_CHARGE_R,
		LEFT_DASH,
		RIGHT_DASH,
		LEFT_NOJUMPFALL,
		RIGHT_NOJUMPFALL,

		LEFT_ATTACKED,
		RIGHT_ATTACKED,
		LEFT_HEAVYATTACKED,
		RIGHT_HEAVYATTACKED,
		LEFT_RETURN, 
		RIGHT_RETURN
	};

public:
	DashMegaMan();
	~DashMegaMan();

public: // 가상함수
	void Update() override;
	void Render() override;
	void Reset() {};
	void PostRender() {};
	void SetState(eState state, bool reset = false);
	shared_ptr<class Animation> GetAnimation() {return m_pAnimation	;}

private:
	bool IsPlay() { return m_pAnimation->IsPlay(); }
	void CreateAnimation();
	void CreateClip(wstring strImage, int w, int h, int count, AnimationClip::eState state = AnimationClip::eState::Loop, int start = 0, float speed = 0.1f);


private:
	shared_ptr<class Animation>   m_pAnimation;
	eState m_nState = eState::LEFT_IDLE;

};

