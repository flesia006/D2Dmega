#pragma once
class S01_TitleScene : public Scene
{
public:
	S01_TitleScene();
	~S01_TitleScene();
public:
	virtual void Update() override; // 순수 가상 함수
	virtual void Render() override; // 순수 가상 함수
	void    OnBulltonClicked(int id);
private:
	Texture* m_pBackground1 = nullptr;
	Texture* m_pBackground2 = nullptr;
	Texture* m_pBackground3 = nullptr;
	class   Button*   m_pButtonStart =  nullptr;
	class   Button*   m_pButtonExit  = nullptr;
};

