#include "framework.h"
#include "GameTime.h"

GameTime::GameTime()
{
	old_time = std::chrono::system_clock::now();  //현재시간
}

GameTime::~GameTime()
{
}

void GameTime::Update()
{
	std::chrono::duration<double> sec;

	// 현재시간과 Old시간과의 duration 구하기
	sec = std::chrono::system_clock::now() - old_time;
	// 한 Update당 시간 --> Delta()통해서 각 Object별로 시간 측정
	m_TimeElepased = (float)sec.count();         
	// 현재 시간을 old_time에 저장
	old_time = std::chrono::system_clock::now(); // start
	m_FrameCount++;
	
	// Game에서 동작시간을 저장
	m_RunningTime = m_RunningTime + m_TimeElepased;
	
	m_FPStimeElepased = m_FPStimeElepased + m_TimeElepased;

	if (m_FPStimeElepased > 1.0f)
	{
		m_FramePerSecond = m_FPStimeElepased / m_FrameCount;
		m_FrameCountPerSecond = m_FrameCount;
		m_FPStimeElepased = 0.0f;
		m_FrameCount = 0;
	}
	
//	printf("%f  초당 Frame수:%d\n", m_TimeElepased, m_FrameCountPerSecond);
	

}

void GameTime::Update(float lockFPS)
{

	std::chrono::duration<double> sec;

	// 현재시간과 Old시간과의 duration 구하기
	sec = std::chrono::system_clock::now() - old_time;
	// 한 Update당 시간 --> Delta()통해서 각 Object별로 시간 측정
	m_TimeElepased = (float)sec.count();
	
	while (m_TimeElepased < 1.0f / lockFPS)
	{
		sec = std::chrono::system_clock::now() - old_time;
		m_TimeElepased = (float)sec.count();
	}
	
	
	// 현재 시간을 old_time에 저장

	old_time = std::chrono::system_clock::now(); // start
	m_FrameCount++;

	// Game에서 동작시간을 저장
	m_RunningTime = m_RunningTime + m_TimeElepased;

	m_FPStimeElepased = m_FPStimeElepased + m_TimeElepased;

	if (m_FPStimeElepased > 1.0f)
	{
		m_FramePerSecond = m_FPStimeElepased / m_FrameCount;
		m_FrameCountPerSecond = m_FrameCount;
		m_FPStimeElepased = 0.0f;
		m_FrameCount = 0;
	}
}

