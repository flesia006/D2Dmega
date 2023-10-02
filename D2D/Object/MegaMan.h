#pragma once
#define    PI             3.141592f
//#define    VERTICAL       PI/2.0f       // sin(90)->1, cos(90)->0
//#define    LEFT_ANGLE     PI/5.0f*3.0f  // 108도
//#define    RIGHT_ANGLE    PI/5.0f*2.0f  // 72도

class PlayerEffect;
class Bullet;
class CopyMegaMan;
class DashMegaMan;
class ChargeEffect;
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
		LEFT_WALLSTICK,
		RIGHT_WALLSTICK,
		LEFT_WALLKICK,
		RIGHT_WALLKICK,
		LEFT_ATTACKED,
		RIGHT_ATTACKED,
		LEFT_HEAVYATTACKED,
		RIGHT_HEAVYATTACKED,
		LEFT_RETURN, 
		RIGHT_RETURN
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

public: 
	// 가상함수
	void	Update() override;  
	void	Render() override;
	void	Reset() override;
	void	PostRender();

	void	SetAttacked(bool atk_l, bool atk_r ) { m_bAttackedLeft = atk_l; m_bAttackedRight = atk_r;	}
	void	SetActive(bool value) { m_bActive = value; if (m_pCollider) m_pCollider->SetActive(value); }
	void	SetTouchBossRoom(bool value) { m_bTouchBossRoom = value; }
	float	GetAttackedTime() { return m_AttackedTime; }
	bool	IsAttacked() { return m_nState >= 20 && m_nState <= 23; }
	bool	IsInvincible() { return m_bInvincible; }
	bool	IsTouchBossRoom() { return m_bTouchBossRoom; }
	void	SetState(eState state, bool Reset = false, bool ignore = false);
	void	SetHp(int hp) { m_nHP = hp; }
	int		GetHp() { return m_nHP; }
	

private:
	// Update
	void	PreUpdate(Vector2& position);
	void	PreUpdateKeyInput();
	void	PreUpdateGravity() {};
	void	PreUpdateState() {};
	void	UpdateBullet();
	void	UpdateEffect();
	void	FireBullet();

	// Init
	void   CreateAnimation();
	void   CreateSound();
	void   CreateClip(wstring strImage, int w, int h, int count, AnimationClip::eState state = AnimationClip::eState::Loop, int start = 0, float speed = 0.1f);
	
	// Collision Check
	void   GroundCheck(Vector2& position);
	void   WallCheck(Vector2& position);
	void   HeadCheck(Vector2& position);
	void   EnemyCheck(Vector2& position);
	
	// State Check
	bool   IsJumping()   { return((m_nState == LEFT_JUMP || m_nState == RIGHT_JUMP)); }
	bool   IsNoJumpFall() { return((m_nState == LEFT_NOJUMPFALL || m_nState == RIGHT_NOJUMPFALL)); }
	bool   IsFalling()   { return((m_nState == LEFT_FALL || m_nState == RIGHT_FALL)); }
	bool   IsMoving() { return((m_nState == LEFT_MOVE || m_nState == RIGHT_MOVE)); }
	bool   IsWallStick() { return m_nState == LEFT_WALLSTICK || m_nState == RIGHT_WALLSTICK; }
	bool   IsWallKick() { return ((m_nState == LEFT_WALLKICK || m_nState == RIGHT_WALLKICK)); }
	bool   IsIdle() { return((m_nState == LEFT_IDLE || m_nState == RIGHT_IDLE)); }
	bool   IsMoveingLeft()  { return((m_nState == LEFT_MOVE || m_nState == LEFT_JUMP || m_nState == LEFT_FALL)); }
	bool   IsMoveingRight() { return((m_nState == RIGHT_MOVE || m_nState == RIGHT_JUMP || m_nState == RIGHT_FALL)); }
	bool   IsDashing() { return((m_nState == LEFT_DASH || m_nState == RIGHT_DASH)); }


	// Setter, Getter
	void   OnEndPlayerEffect();
	void   SetDashSpeed() { m_SpeedX = 660.0f;  }
	void   SetNormalSpeed() { m_SpeedX = 330.0f;  }
	

private:
	shared_ptr<class Animation>   m_pAnimation;
	Collider*	 m_pHead = nullptr;
	Collider*    m_pFeet = nullptr;
	Collider*	 m_pWall = nullptr;
	shared_ptr<class Texture>     m_pHPbar;
	shared_ptr<class Texture>     m_pHPbar2;
	shared_ptr<class Texture>     m_pHPbar3;
	class CopyMegaMan* m_pCopyMegaMan = nullptr;
	vector<class PlayerEffect*> m_cvEffects;
	vector<class ChargeEffect*> m_cvChrEffects;
	vector<class Bullet*>		m_cvBullets;
	vector<class DashMegaMan*>  m_cvDashMegaMan;

private:
	eState m_nState = eState::LEFT_IDLE;
	eState m_BeforeState = eState::LEFT_IDLE;

	// Megaman personal
	float  m_SpeedX = 330.0f;
	float  m_SpeedY = 1080.0f;
	float  m_Gravirty = 0.0f;
	float  m_dashDelay = 0.0f;
	int    m_nHP = 50;
	int    m_OriginHP = 50;
	int    cvPosCount = 0;
	vector<Vector2> m_cvPosition;
	int    m_LastBullet = 0;

	// Timer
	float  m_ChargeCountTime = 0.0f;
	float  m_FallTime = 0.0f;
	float  m_DashTime = 0.0f;
	float  m_FireStateTime = 0.0f;
	float  m_AttackTime = 0.0f;
	float  m_DashMMTime = 0.0f;
	float  m_WallKickTime = 0.0f;
	float  m_AttackedTime = 0.0f;
	float  m_InvincibleTime = 0.0f;
	float  m_BlinkTime = 0.0f;
	float  m_DoorOpen = 0.0f;
	float  m_BossDeathTimer = 0.0f;

	// State support
	bool   m_bGround = false;
	bool   m_bFalling = true;
	bool   m_bCharge = false;
	bool   m_bChargeFull = false;
	bool   m_bDashJump = false;
	bool   m_bFireState = false;
	bool   m_bTouchWall = false;
	bool   m_bAttackedLeft = false;
	bool   m_bAttackedRight = false;	
	bool   m_bInvincible = false;
	bool   m_bHeadTouch = false;
	bool   m_bWalkLine = false;
	bool   m_bTouchBossRoom = false;
	bool   m_bEnterBossRoom = false;
	UINT   lastWallTouch = LEFT;
};

