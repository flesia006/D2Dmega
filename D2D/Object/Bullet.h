#pragma once
class PlayerEffect;
class Bullet : public GameObject
{
public:
	Bullet();
	~Bullet();

	enum eState
	{
		L_BULLET,
		L_CHARGE,
		L_FULL_CHARGE,
		GRENADE,
		UPSHOT,
		FRONTSHOT,
	};

public: // 가상함수
	virtual void Update() override;    // 순수 가상 함수
	virtual void Render() override;                     // 순수 가상 함수
	virtual void Reset() override;
	virtual void PostRender() {};

public:
	void    SetDirection(int value) { m_Direction = value; }
	bool    IsPlay() { return m_pAnimation->IsPlay(); }
	void    SetState(eState value) { m_nState = value; m_pAnimation->SetPlay((UINT)m_nState); }
	void    SetActive(bool value) { m_bActive = value; if (m_pCollider) m_pCollider->SetActive(value); }

private:
	void   CreateClip(wstring strImage, int w, int h, int count, int loop = 0);
	void   HitCheck(Vector2& position);

private:
	eState     m_nState = eState::L_BULLET;
	Animation*	   m_pAnimation = nullptr;
	PlayerEffect* m_pEffect = nullptr;
	int        m_Direction = 0;  //왼/오른쪽
	float      m_Gravirty = 0.0f;
	const int  BulletDamage = 1;
	const int  ChragedBulletDamage = 4;
	const int  FullChargedBulletDamage = 15;
};

