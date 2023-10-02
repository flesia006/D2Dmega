#pragma once
#pragma once

class   PlayerEffect;
class   BossBullet;
class   Boss : public GameObject
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

	enum Sequence
	{
		p_OPENING = 0,
		P_THREE_WAVE,
		P_BACKATTACK,
		P_THUNDER_ATK,
		P_BIG_WAVE,
		P_DASH,
		P_DISAPPEAR,
	};

	enum eBullet
	{
		WAVE,
		GROUND_ATK,
		ASCENDING_ATK,
		SPARK,
		BIG_WAVE, 
		THUNDER_EFT
	};

	Boss();
	~Boss();

public:
	bool IsPlay() { return m_pAnimation->IsPlay(); }
	void PostRender() override;

	// Setter
	void SetState(eState value);
	void SetHP(int hp) { m_Hp = hp; }
	void SetCallback(function<void()> cbFunction) { m_fpEndCallback = cbFunction; }
	void SetAttacked(bool atk) { m_bAttacked = atk; }
	void SetOriginState(eState state) { m_OriginState = state; }
	void SetOriginPos(Vector2 pos) { m_OriginPosition = pos; }
	void SetActive(bool value) { m_bActive = value; if (m_pCollider) m_pCollider->SetActive(value); }

	// Getter
	int GetHP() { return m_Hp; }
	eState GetOriginState() { return m_OriginState; }
	Vector2 GetOriginPos() { return m_OriginPosition; }
	bool IsRender() { return m_bRender; }
	bool IsAttacked() { return m_bAttacked; }
	bool IsDodge() { return m_nState == DASH || m_Sequence == P_DISAPPEAR; }
	bool IsBlocking() { return m_nState == GUARD; }
	bool IsBackAtk() { return m_Sequence == P_BACKATTACK; }
	bool   IsDeath() { return m_Hp <= 0; }
	bool  IsOpening() { return m_bOpening; }

public: // 가상함수
	void Update() override;    // 순수 가상 함수
	void Render() override;                     // 순수 가상 함수
	void Reset() override;
private:
	void   CreateAnimation();
	void   CreateClip(wstring strImage, int w, int h, int count,
		AnimationClip::eState state = AnimationClip::eState::Loop, float speed = 0.1f, int term = 100);
	void   FireBullet(eBullet state, UINT direction, Vector2 position);
	void   UpdateBullet();
	void   UpdateEffect(Vector2& position);
	void   PreUpdate(Vector2& position);
	void   GroundCheck(Vector2& position);

//	bool   IsJumping() { return((m_nState == LEFT_JUMP || m_nState == RIGHT_JUMP)); }
	bool   IsIdle() { return m_nState == IDLE; }

	void SetLookMM();
	void SetDontLookMM();

	//   패턴
	void Opening	   (Vector2& position);
	void FireThreeWave (Vector2& position);
	void BackAttack    (Vector2& position);
	void ThunderAttack (Vector2& position);
	void BigWaveAttack (Vector2& position);
	void Dash          (Vector2& position);
	void Disappear     (Vector2& position);

private:
	shared_ptr<class Animation>	 m_pAnimation;
	class MegaMan* pMM = nullptr;

	eState						m_nState = eState::IDLE;
	eState						m_OriginState = eState::IDLE;
	function<void()>			m_fpEndCallback = nullptr;
	shared_ptr<class Texture>     m_pHPbar;
	shared_ptr<class Texture>     m_pHPbar2;
	shared_ptr<class Texture>     m_pHPbar3;
	vector<class BossBullet*>		m_cvBullets;
	vector<class BossEffect*>		m_cvEffects;
	class BossEffect*  m_pDisAppearEffect = nullptr;
	class PlayerEffect* m_pEffect = nullptr;
	class Collider* m_pWall = nullptr;
	class Collider* m_pFeet = nullptr;
	class Collider* m_pHead = nullptr;

	// Private
	Vector2 m_Position = { 0, 0 };
	Vector2 m_OriginPosition = { 0, 0 };
	Vector2 m_Scale = { 0, 0 };
	Vector2 m_OriginPos = { 0, 0 };
	vector<Vector2> m_cvPosition;
	const float   m_SpeedX = 700.0f;
	const float  m_tall = 220.0f;
	float   m_BreakX = 0.0f;
	float   m_SpeedY = 1580.0f;
	float   m_Gravirty = 0.0f;
	float  m_AttackedTime = 0.0f;
	float  m_PatternTimer = 0.0f;
	UINT   m_Sequence = p_OPENING;
	UINT   m_LastSequence = 0;	
	int   cvPosCount = 0;

	const int    m_OriginHP = 300;
	int    m_Hp = 300;

	int   lastBullet = 0;
	int   BlickTimes = 0;
	float  m_BlinkTime = 0.75f;

	// Timer
	float  m_AttackTime = 0.0f;
	float  m_FallTime = 0.0f;
	float  m_IdleTime = 0.0f;
	float  m_MoveTime = 0.0f;

	// State support
	bool   m_bGround = false;
	bool   m_bFalling = true;
	bool   m_bJumping = false;
	bool   m_bFireState = false;
	bool   m_bTouchWall = false;
	bool   m_bFound = false;
	bool   m_bAttacked = false;
	bool   m_PlayOnce = false;
	bool   m_bRender = false;
	bool   m_bHPRender = false;
	bool   m_bRenderDash = false;
	bool   m_bDisappear = false;
	bool   m_bDodge = false;
	bool   m_Blinking = false;
	bool   m_bOpening = false;
};
