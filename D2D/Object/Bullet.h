#pragma once
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
	};

public: // 가상함수
	virtual void Update(Matrix V, Matrix P) override;    // 순수 가상 함수
	virtual void Render() override;                     // 순수 가상 함수
	virtual void Reset() override;
	virtual void PostRender() {};

public:
	void    SetDirection(int value) { m_Direction = value; }
	bool    IsPlay() { return m_pAnimation->IsPlay(); }
	void    SetState(eState value) { m_nState = value; m_pAnimation->SetPlay((UINT)m_nState); }

private:
	void   CreateClip(wstring strImage, int w, int h, int count, int loop = 0);

private:
	eState     m_nState = eState::L_BULLET;
	Animation*	   m_pAnimation = nullptr;
	int        m_Direction = 0;  //왼/오른쪽


};

