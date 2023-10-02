#pragma once

class   PlayerEffect;
class   Spike : public GameObject
{
public:
	Spike();
	~Spike();

public:
	bool IsPlay() { return m_pAnimation->IsPlay(); }

	// Setter
	void SetHP(int hp) { m_Hp = hp; }
	void SetCallback(function<void()> cbFunction) { m_fpEndCallback = cbFunction; }
	void SetAttacked(bool atk) { m_bAttacked = atk; }
	void SetOriginPos(Vector2 pos) { m_OriginPosition = pos; }
	void SetActive(bool value) { m_bActive = value; if (m_pCollider) m_pCollider->SetActive(value); }

	// Getter
	int GetHP() { return m_Hp; }
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
	void   UpdateEffect();
	void   PreUpdate(Vector2& position);
	void   GroundCheck(Vector2& position);
	void   WallCheck(Vector2& position);
	void   HeadCheck(Vector2& position);
	bool   IsDeath() { return m_Hp <= 0; }
	

private:
	shared_ptr<class Animation>	 m_pAnimation;
	function<void()>			m_fpEndCallback = nullptr;
	vector<class Bullet*>		m_cvBullets;
	class PlayerEffect* m_pEffect = nullptr;
	class Collider* m_pWall = nullptr;
	class Collider* m_pFeet = nullptr;
	class Collider* m_pHead = nullptr;

	// Private
	int     m_Hp = 0;
	Vector2 m_Position = { 0, 0 };
	Vector2 m_OriginPosition = { 0, 0 };
	Vector2 m_Scale = { 0, 0 };
	float   m_SpeedX = 330.0f;
	float   m_SpeedSpike = 230.0f;
	float   m_SpeedY = 1380.0f;
	float   m_Gravirty = 1380.0f;
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
	bool   m_bRoof = false;
	bool   m_bFalling = true;
	bool   m_bJumping = false;
	bool   m_bFireState = false;
	bool   m_bTouchWall = false;
	bool   m_bFound = false;
	bool   m_bAttacked = false;
};
