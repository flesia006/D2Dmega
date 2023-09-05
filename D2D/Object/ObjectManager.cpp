#include "framework.h"
#include "ObjectManager.h"

ObjectManager::ObjectManager()
{
}

ObjectManager::~ObjectManager()
{
}
void ObjectManager::Delete()
{

}
void ObjectManager::Update(string name)
{
	GameObject* pObject = FindObject(name);
	if (!pObject) return;
	Matrix V = CAMERA->GetView();
	Matrix P = CAMERA->GetProjection();

	pObject->Update(V, P);

}

void ObjectManager::Render(string name)
{
	GameObject* pObject = FindObject(name);
	if (!pObject) return;
	pObject->Render();
}

void ObjectManager::AddObject(string name, GameObject* pObject)
{
	m_cmObjects.insert(make_pair(name, pObject));
}

GameObject* ObjectManager::FindObject(string name)
{
	GameObject* pObject = nullptr;
	if (m_cmObjects.count(name))
		pObject = m_cmObjects[name];

	return pObject;
}


