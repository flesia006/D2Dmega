#pragma once
#define    PI             3.141592f
#define    VERTICAL       PI/2.0f       // sin(90)->1, cos(90)->0
#define    LEFT_ANGLE     PI/5.0f*3.0f  // 108도
#define    RIGHT_ANGLE    PI/5.0f*2.0f  // 72도

class PlayerEffect;
class Bullet;
class MegaMan : public GameObject
{
public:
	MegaMan();
	~MegaMan();
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
		LEFT_FIRE_MOVE,
		RIGHT_FIRE_MOVE,
	};

	enum effects
	{
		BLUE_CHARGE,
		GREEN_CHARGE,
		DASH_EFCT,
		DASHDUST_EFCT,
		WALL_EFCT,
		WALL_KICK
	};

public: // 가상함수
	void Update(Matrix V, Matrix P) override;  
	void Render() override;
	void Reset() override;
	void PostRender();

private:
	void   FireBullet();
	void   SetState(eState state);
	void   SetFireState(eState state);
	void   SetNormalState(eState state);
	void   PreUpdate(Vector2& position);
	void   CreateAnimation();
	void   CreateClip(wstring strImage, int w, int h, int count, AnimationClip::eState state = AnimationClip::eState::Loop, int start = 0, float speed = 0.1f);
	void   GroundCheck(Vector2& position);
	void   WallCheck(Vector2& position);
	bool   IsJumping()   { return((m_nState == LEFT_JUMP || m_nState == RIGHT_JUMP)); }
	bool   IsFalling()   { return((m_nState == LEFT_FALL || m_nState == RIGHT_FALL)); }
	bool   IsMoving() { return((m_nState == LEFT_MOVE || m_nState == RIGHT_MOVE)); }
	bool   IsIdle() { return((m_nState == LEFT_IDLE || m_nState == RIGHT_IDLE)); }
	bool   IsMoveingLeft()  { return((m_nState == LEFT_MOVE || m_nState == LEFT_JUMP || m_nState == LEFT_FALL)); }
	bool   IsMoveingRight() { return((m_nState == RIGHT_MOVE || m_nState == RIGHT_JUMP || m_nState == RIGHT_FALL)); }
	bool   IsDashing() { return((m_nState == LEFT_DASH || m_nState == RIGHT_DASH)); }
	void   OnEndPlayerEffect();
	void   SetDashSpeed() { m_SpeedX = 660.0f;  }
	void   SetNormalSpeed() { m_SpeedX = 330.0f;  }

	shared_ptr<class Animation>   m_pAnimation;
	Collider*    m_pFeet = nullptr;
	Collider*	 m_pWall = nullptr;
	shared_ptr<class Texture>     m_pHPbar;
	vector<PlayerEffect*> m_cvEffects;
	vector<Bullet*> m_cvBullets;

private:
	eState m_nState = eState::LEFT_IDLE;
	eState m_BeforeState = eState::LEFT_IDLE;
	bool   m_bGround = false;
	bool   m_bFalling = true;
	float  m_SpeedX = 330.0f;
	float  m_SpeedY = 8.0f;
	float  m_Gravirty = 0.0f;
	float  m_Angle = 0.0f;
	float  m_dashDelay = 0.0f;
	int    m_nHP   = 4;
	float  m_ChargeCountTime = 0.0f;
	float  m_FallTime = 0.0f;
	float  m_DashTime = 0.0f;
	float  m_FireStateTime = 0.0f;
	bool   m_bCharge = false;
	bool   m_bChargeFull = false;
	bool   m_bDashJump = false;
};

