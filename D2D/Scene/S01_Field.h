#pragma once
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
public:
	shared_ptr<Line>	 GetLines();
	vector<Collider*>	 GetGround()	{ return Grounds; }

private:
	shared_ptr<class Texture>  m_pBackground;
	shared_ptr<class Line>     m_pLine;

	vector<Collider*> Grounds;
};
