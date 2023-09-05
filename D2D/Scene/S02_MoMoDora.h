#pragma once
class S02_MoMoDora : public Scene
{
public:
	S02_MoMoDora();
	~S02_MoMoDora();
public:
	virtual void Update() override ;    // 순수 가상 함수
	virtual void Render() override ;    // 순수 가상 함수
	virtual void PreRender() {};  // 가상 함수
	virtual void PostRender() {}; // 가상 함수
public:
	shared_ptr<Line> GetLines();
	shared_ptr<Collider> GetLeftZone() { return ZoneLeft; }
	shared_ptr<Collider> GetRightZone() { return ZoneRight; }
private:
	shared_ptr<class Texture>  m_pBackground;
	shared_ptr<class Line>     m_pLine;
	shared_ptr<class Collider> ZoneRight;
	shared_ptr<class Collider> ZoneLeft;
};

