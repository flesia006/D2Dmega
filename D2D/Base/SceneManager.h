#pragma once
#define   SCENEMANAGER SceneManager::GetInstance()

class SceneManager
{
public:
	static SceneManager* GetInstance()
	{
		static SceneManager singleTone;
		return &singleTone;
	}
public:
	void Init();
	void Update();
	void Render();
	void ChangeScene(string name);
	shared_ptr<Scene> GetCurrentScene() { return m_pCurrentScene; }
private:
	void  ThreadStart();
	void  CreateObjects();
private:
	vector<shared_ptr<Scene>>   m_cvScenes;
	shared_ptr<Scene>			m_pCurrentScene;


private:
	SceneManager();
public:
	~SceneManager();
};

