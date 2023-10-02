#pragma once
#define  TIME  GameTime::GetInstance()
#define  DELTA GameTime::GetInstance()->Delta()

class GameTime
{
public:
	static GameTime* GetInstance()
	{
		static GameTime singleTone;
		return &singleTone;
	}
public:
	void   Update();
	void   Update(float lockFPS);   // 일정시간을 대기
	float  Delta() { return m_TimeElepased; }
	float  GetRunning() { return m_RunningTime; }
	float  GetFPS() { return m_FramePerSecond; }
	int    GetFrame() { return m_FrameCountPerSecond; }
private:
	std::chrono::system_clock::time_point old_time; // start시간
	float m_TimeElepased = 0.0f;      // Update당 경과 시간
	float m_FPStimeElepased = 0.0f;   // Frame
	float m_RunningTime = 0.0f;       // 총 Running 시간
	float m_FramePerSecond = 0.0f;    // 
	int   m_FrameCount = 0;
	int   m_FrameCountPerSecond = 0;   // 초당 frame Count

private:
	GameTime();
public:
	~GameTime();

};

