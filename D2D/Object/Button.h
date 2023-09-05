#pragma once
class Button : public GameObject
{
public:
	virtual void Update(Matrix V, Matrix P) override;    // 순수 가상 함수
	virtual void Render() override;                     // 순수 가상 함수
	virtual void Reset() override {};

public:
	void     SetButtonID(int id) { m_ID = id; }
	void     SetCallback(function<void(int)> cbFunction) { m_fpClickCallback = cbFunction; }
private:
	void     MouseOnCheck();
private:
	Texture* m_pTexture[2];
	bool     m_bHover = false;   // Mouse가 in되면 out되면 false
	int      m_ID;
	function<void(int)> m_fpClickCallback = nullptr;

public:
	Button(wstring strImage1, wstring strImage2, wstring strShader );
	~Button();

};

