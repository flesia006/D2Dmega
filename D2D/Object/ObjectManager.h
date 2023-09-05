#pragma once
#define  OBJECTMANAGER ObjectManager::GetInstance()
class ObjectManager
{
private:
	ObjectManager();
	~ObjectManager();
public:
	static ObjectManager* GetInstance()
	{
		static ObjectManager singleTone;
		return &singleTone;
	}

public:
	void			Update(string name);
	void			Render(string name);
	void			AddObject(string name, GameObject* pObject);
	GameObject*		FindObject(string name);


private:
	map<string, GameObject*> m_cmObjects;
	void Delete();

};