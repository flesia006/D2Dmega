#include "framework.h"
#include "SceneManager.h"

// Object들
#include  "Object/MegaMan.h"

// 포함되어야할 신들
#include  "./Scene/S01_Field.h"

SceneManager::SceneManager()
{

}
void SceneManager::Init()
{
	OBJECTMANAGER->AddObject("MEGAMAN", new MegaMan());


	m_cvScenes.push_back(make_shared<S01_Field>());

	// 쓰레드를 이용하여 Object,신들
	thread t(bind(&SceneManager::ThreadStart, this));
	t.detach();   // wait 없이 스레드 동작

}
SceneManager::~SceneManager()
{


}

void SceneManager::Update()
{
	CAMERA->Update();

	for (size_t i = 0; i < m_cvScenes.size(); i++)
	{
		if (m_cvScenes[i]->IsActive())
		{
			m_pCurrentScene = m_cvScenes[i];
			m_cvScenes[i]->Update();
		}
		
	}

}
void SceneManager::Render()
{
	for (size_t i = 0; i < m_cvScenes.size(); i++)
	{
		if (m_cvScenes[i]->IsActive())
		{
			m_pCurrentScene = m_cvScenes[i];
			m_cvScenes[i]->Render();
			m_cvScenes[i]->PostRender();
		}
			
	}

}
void SceneManager::ChangeScene(string name)
{
	for (size_t i = 0; i < m_cvScenes.size(); i++)
	{
		if (m_cvScenes[i]->GetSceneName() == name)
			m_cvScenes[i]->SetActive(true);
		else
			m_cvScenes[i]->SetActive(false);
	}
}
////////////////////////////////////////////
// background processing
////////////////////////////////////////////
void SceneManager::ThreadStart()
{

	CreateObjects();



//	m_cvScenes.push_back(make_shared<S02_MoMoDora>());


	printf("thread end\n");


}

void SceneManager::CreateObjects()
{
}
