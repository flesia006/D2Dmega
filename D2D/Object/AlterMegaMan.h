#pragma once

#pragma once
#define    PI             3.141592f
#define    VERTICAL       PI/2.0f       // sin(90)->1, cos(90)->0
#define    LEFT_ANGLE     PI/5.0f*3.0f  // 108도
#define    RIGHT_ANGLE    PI/5.0f*2.0f  // 72도

class PlayerEffect;
class Bullet;
class AlterMegaMan : public GameObject
{
public:
	AlterMegaMan();
	~AlterMegaMan();
public:
	enum eState
	{
		LEFT_IDLE = 0,
		RIGHT_IDLE,
		LEFT_FIRE_MOVE,
		RIGHT_FIRE_MOVE,
		LEFT_FIRE_JUMP,
		RIGHT_FIRE_JUMP,
		LEFT_FALL,
		RIGHT_FALL,
		LEFT_ATTACK,
		RIGHT_ATTACK,
		ATTACK_CHARGE_L,
		ATTACK_CHARGE_R,
		LEFT_FIRE_DASH,
		RIGHT_FIRE_DASH,
		LEFT_FIRE_NOJUMPFALL,
		RIGHT_FIRE_NOJUMPFALL,
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
	void   Update(Matrix V, Matrix P) override;
	void   Render() override;
	void   Reset() override;
	void   PostRender();
	
	void   SetState(eState state);

private:
	void   FireBullet();
	void   PreUpdate(Vector2& position);
	void   CreateAnimation();
	void   CreateClip(wstring strImage, int w, int h, int count, AnimationClip::eState state = AnimationClip::eState::Loop, int start = 0, float speed = 0.1f);
	void   GroundCheck(Vector2& position);
	void   WallCheck(Vector2& position);
	bool   IsJumping() { return((m_nState == LEFT_FIRE_JUMP || m_nState == RIGHT_FIRE_JUMP)); }
	bool   IsFalling() { return((m_nState == LEFT_FALL || m_nState == RIGHT_FALL)); }
	bool   IsMoving() { return((m_nState == LEFT_FIRE_MOVE || m_nState == RIGHT_FIRE_MOVE)); }
	bool   IsIdle() { return((m_nState == LEFT_IDLE || m_nState == RIGHT_IDLE)); }
	bool   IsMoveingLeft() { return((m_nState == LEFT_FIRE_MOVE || m_nState == LEFT_FIRE_JUMP || m_nState == LEFT_FALL)); }
	bool   IsMoveingRight() { return((m_nState == RIGHT_FIRE_MOVE || m_nState == RIGHT_FIRE_JUMP || m_nState == RIGHT_FALL)); }
	bool   IsDashing() { return((m_nState == LEFT_FIRE_DASH || m_nState == RIGHT_FIRE_DASH)); }
	void   OnEndPlayerEffect();
	void   SetDashSpeed() { m_SpeedX = 660.0f; }
	void   SetNormalSpeed() { m_SpeedX = 330.0f; }

	shared_ptr<class Animation>   m_pAnimation;
	Collider* m_pFeet = nullptr;
	Collider* m_pWall = nullptr;

private:
	eState m_nState = eState::LEFT_IDLE;
	eState m_BeforeState = eState::LEFT_IDLE;

	// Megaman personal
	float  m_SpeedX = 330.0f;
	float  m_SpeedY = 8.0f;
	float  m_Gravirty = 0.0f;
	float  m_Angle = 0.0f;
	float  m_dashDelay = 0.0f;
	int    m_nHP = 4;

	// Timer
	float  m_ChargeCountTime = 0.0f;
	float  m_FallTime = 0.0f;
	float  m_DashTime = 0.0f;
	float  m_FireStateTime = 0.0f;

	// State support
	bool   m_bGround = false;
	bool   m_bFalling = true;
	bool   m_bCharge = false;
	bool   m_bChargeFull = false;
	bool   m_bDashJump = false;
	bool   m_bFireState = false;
};

