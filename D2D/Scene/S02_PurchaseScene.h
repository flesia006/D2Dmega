#pragma once
class S02_PurchaseScene : public Scene
{
public:
	S02_PurchaseScene();
	~S02_PurchaseScene();
public:
	virtual void Update() override;    // 순수 가상 함수
	virtual void Render() override;    // 순수 가상 함수
	virtual void PreRender() {};  // 가상 함수
	virtual void PostRender() override;; // 가상 함수
private:
	Texture* m_pBackground[3];
	class Button* m_pButton[16];
	void  OnBulltonClicked(int id);
};

