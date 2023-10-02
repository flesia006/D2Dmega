#pragma once
class GrenadeMan;
class Spike;
class Boss;
class S01_Field  : public Scene
{
public:
	S01_Field();
	~S01_Field();
public:
	void Update() override;
	void Render() override;
	void PreRender() override;
	void PostRender() override;

private:
	void AddGrenadeMan(UINT mode, Vector2 position);
	void AddSpikes(Vector2 position);
	void SpawnEnemy();
	void CreateAnimation();


public:
	shared_ptr<Line>	 GetLines() { return m_pLine; };
	vector<Collider*>	 GetGround()	{ return Grounds; }
	vector<GrenadeMan*>  GetGrenadeMans() { return m_cvGrenadeMan; }
	vector<Spike*>		 GetSpikes() { return m_cvSpikes; }
	shared_ptr<class Animation> GetDoorPtr() { return m_pAnimation; }
	class Boss* GetBoss() { return m_pBoss; }

private:
	shared_ptr<class Texture>  m_pBackground;
	shared_ptr<class Line>     m_pLine;

	vector<Collider*> Grounds;
	vector<class GrenadeMan*> m_cvGrenadeMan;
	vector<class Spike*> m_cvSpikes;
	class Boss* m_pBoss;
	shared_ptr<class Animation> m_pAnimation;

	const int GM_Hp = 5;
	const int SP_Hp = 2;
	bool  m_bDebug = false;
};
