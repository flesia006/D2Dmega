#pragma once
class Scene
{
public:
	virtual void Update() = 0;    // 순수 가상 함수
	virtual void Render() = 0;    // 순수 가상 함수
	virtual void PreRender() {};  // 가상 함수
	virtual void PostRender() {}; // 가상 함수

public: // setter
	void   SetActive(bool Active) { m_bActive = Active; }
	void   SetSceneName(string name) { m_strSceneName = name; }
public: // getter
	bool   IsActive()             { return m_bActive; }
	string GetSceneName()         { return m_strSceneName; }
protected:
	string  m_strSceneName;         // 현재 신명
	string  m_strNextSceneName;     // 다음 신명
	bool    m_bActive = false;       // 활성화
	bool    m_bReset  = true;       //  1->2->1
	bool    m_bDisplay = false;
	int     m_nSceneID = 0;


};

