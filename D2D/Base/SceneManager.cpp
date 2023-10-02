#include "framework.h"
#include "SceneManager.h"

// Object들
#include  "Object/MegaMan.h"
#include  "Object/GrenadeMan.h"

// 포함되어야할 신들
#include  "./Scene/S01_Field.h"

SceneManager::SceneManager()
{
	
}
void SceneManager::Init()
{
	CreateSound();
	OBJECTMANAGER->AddObject("MEGAMAN", new MegaMan());

	m_cvScenes.push_back(make_shared<S01_Field>());

	// 쓰레드를 이용하여 Object,신들
//	thread t(bind(&SceneManager::ThreadStart, this));
//	t.detach();   // wait 없이 스레드 동작

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

//	CreateObjects();



//	m_cvScenes.push_back(make_shared<S02_MoMoDora>());


//	printf("thread end\n");


}

void SceneManager::CreateObjects()
{
}

void SceneManager::CreateSound()
{
	SOUND->AddSound("bgm", L"Sound/SLUS-00561_31 Final Weapon Stage 1.mp3", true);
	SOUND->AddSound("boss", L"Sound/SLUS-00561_32 Colonel & General.mp3", true);
	SOUND->AddSound("bullet", L"Sound/bullet.wav", false, true);
	SOUND->AddSound("bulletHit", L"Sound/bulletHit.wav", false, true);
	SOUND->AddSound("BUSTER", L"Sound/BUSTER.wav");
	SOUND->AddSound("SEMIBUSTER", L"Sound/SEMIBUSTER.wav");
	SOUND->AddSound("ChargeStart", L"Sound/ChargeStart.wav");
	SOUND->AddSound("CHARGING", L"Sound/CHARGING.wav");
	SOUND->AddSound("Damaged", L"Sound/Damage.wav");
	SOUND->AddSound("Dash", L"Sound/Dash.wav", false, true);
	SOUND->AddSound("DASH_STOP", L"Sound/DASH_STOP.wav", false, true);
	SOUND->AddSound("Exlposion", L"Sound/Exlposion.wav", false, true);
	
	SOUND->AddSound("JUMP", L"Sound/JUMP.wav", false, true);
	SOUND->AddSound("JUMP_VOICE", L"Sound/JUMP_VOICE.wav", false, true);
	SOUND->AddSound("Damaged", L"Sound/Damaged.wav");
	SOUND->AddSound("WALL_JUMP_VOICE", L"Sound/WALL_JUMP_VOICE.wav", false, true);
	SOUND->AddSound("DoorOpen", L"Sound/DoorOpen.mp3");
	SOUND->AddSound("BossMeet", L"Sound/BossMeet.mp3", true);
	SOUND->AddSound("Stage_Clear", L"Sound/27_Stage Clear X.mp3");
	SOUND->AddSound("M_Landing", L"Sound/PlayStation - Mega Man X4 - Sound Effects/M_Landing.wav");
	SOUND->AddSound("M_WallStick", L"Sound/PlayStation - Mega Man X4 - Sound Effects/M_WallStick.wav");
	SOUND->AddSound("M_Return", L"Sound/PlayStation - Mega Man X4 - Sound Effects/M_Return.wav");

	SOUND->AddSound("EnemyCut", L"Sound/PlayStation - Mega Man X4 - Sound Effects/EnemyCut.wav");
	SOUND->AddSound("DoorClose", L"Sound/PlayStation - Mega Man X4 - Sound Effects/DoorClose.wav");
	SOUND->AddSound("Stage_Clear", L"Sound/27_Stage Clear X.mp3");


	//보스
	SOUND->AddSound("Boss_BackAtk",L"Sound/PlayStation - Mega Man X4 - Sound Effects/Boss_BackAtk.wav");
	SOUND->AddSound("Boss_bigwave", L"Sound/PlayStation - Mega Man X4 - Sound Effects/Boss_bigwave.wav");
	SOUND->AddSound("Boss_dash", L"Sound/PlayStation - Mega Man X4 - Sound Effects/Boss_dash.wav");
	SOUND->AddSound("Boss_Disappear", L"Sound/PlayStation - Mega Man X4 - Sound Effects/Boss_Disappear.wav");
	SOUND->AddSound("Boss_Lightning", L"Sound/PlayStation - Mega Man X4 - Sound Effects/Boss_Lightning.wav");
	SOUND->AddSound("Boss_spark", L"Sound/PlayStation - Mega Man X4 - Sound Effects/Boss_spark.wav", true);
	SOUND->AddSound("Boss_Thunder_atk", L"Sound/PlayStation - Mega Man X4 - Sound Effects/Boss_Thunder_atk.wav");
	SOUND->AddSound("Boss_wave", L"Sound/PlayStation - Mega Man X4 - Sound Effects/Boss_wave.wav");
	SOUND->AddSound("Boss_BigWave2", L"Sound/PlayStation - Mega Man X4 - Sound Effects/Boss_BigWave2.wav", true);
	SOUND->AddSound("BossExplosion", L"Sound/Exlposion.wav");




}
