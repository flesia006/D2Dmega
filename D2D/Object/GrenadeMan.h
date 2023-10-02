#pragma once

class   PlayerEffect;
class   GrenadeMan : public GameObject
{
public:
	enum eState
	{
		LEFT_IDLE = 0,
		RIGHT_IDLE,
		LEFT_MOVE,
		RIGHT_MOVE,
		LEFT_ATTACK,
		RIGHT_ATTACK,
		LEFT_JUMP,
		RIGHT_JUMP,
		LEFT_FALL,
		RIGHT_FALL,
		GREEN_IDLE,
		GREEN_FRONT_FIRE,
		GREEN_UP_FIRE,
		GREEN_MOVE,
		GREEN_JUMP,
		GREEN_FALL,

	};

	enum eMode
	{
		ONLY_BOMB = 0, 
		WALK_BOMB,
		ONLY_WALK,
		WALK_SHOOT, 

	};

	GrenadeMan();
//	GrenadeMan(Vector2 position, Vector2 Scale, eMode mode);
	~GrenadeMan();

public:
	bool IsPlay() { return m_pAnimation->IsPlay(); }

	// Setter
	void SetState(eState value) { m_nState = value; m_pAnimation->SetPlay((UINT)m_nState); }
	void SetMode(eMode value) { m_Mode = value; }
	void SetHP(int hp) { m_Hp = hp; }
	void SetCallback(function<void()> cbFunction) { m_fpEndCallback = cbFunction; }
	void SetAttacked(bool atk) { m_bAttacked = atk; }
	void SetOriginState(eState state) { m_OriginState = state; }
	void SetOriginPos(Vector2 pos) { m_OriginPosition = pos; }
	void SetActive(bool value) { m_bActive = value;if(m_pCollider) m_pCollider->SetActive(value); }

	// Getter
	int GetHP() { return m_Hp; }
	eMode GetMode() { return m_Mode; }
	eState GetOriginState() { return m_OriginState; }
	Vector2 GetOriginPos() { return m_OriginPosition; }


public: // 가상함수
	void Update() override;    // 순수 가상 함수
	void Render() override;                     // 순수 가상 함수
	void Reset() override;
	void PostRender() {};

private:
	void   CreateAnimation();
	void   CreateClip(wstring strImage, int w, int h, int count, 
		AnimationClip::eState state = AnimationClip::eState::Loop, float speed = 0.1f, int term = 100);
	void   FireBullet();
	void   UpdateBullet();
	void   UpdateEffect();
	void   PreUpdate(Vector2& position);
	void   GroundCheck(Vector2& position);
	void   WallCheck(Vector2& position);

	bool   IsJumping() { return((m_nState == LEFT_JUMP || m_nState == RIGHT_JUMP)); }
	bool   IsFalling() { return((m_nState == LEFT_FALL || m_nState == RIGHT_FALL)); }
	bool   IsMoving() { return((m_nState == LEFT_MOVE || m_nState == RIGHT_MOVE)); }
	bool   IsIdle() { return((m_nState == LEFT_IDLE || m_nState == RIGHT_IDLE)); }
	bool   IsFoundMM(eMode m_Mode);
	bool   IsDeath() { return m_Hp <= 0; }

private:
	shared_ptr<class Animation>	 m_pAnimation;
	eState						m_nState = eState::LEFT_IDLE;
	eState						m_OriginState = eState::LEFT_IDLE;
	function<void()>			m_fpEndCallback = nullptr;
	vector<class Bullet*>		m_cvBullets;
	class PlayerEffect* m_pEffect = nullptr;
	class Collider* m_pWall = nullptr;
	class Collider* m_pFeet = nullptr;
	class Collider* m_pHead = nullptr;

	// Private
	int    m_Hp = 0;
	eMode   m_Mode = eMode::ONLY_BOMB;
	Vector2 m_Position = { 0, 0 };
	Vector2 m_OriginPosition = { 0, 0 };
	Vector2 m_Scale = { 0, 0 };
	float   m_SpeedX = 330.0f;
	float   m_SpeedSpike = 230.0f;
	float   m_SpeedY = 1880.0f;
	float   m_Gravirty = 0.0f;
	const float  m_tall = 150.0f;
	float  m_AttackedTime = 0.0f;
	UINT   m_Sequence = 0;

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
};
