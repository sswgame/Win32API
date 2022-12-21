#pragma once
class CTimer
{
public:
	CTimer();
	~CTimer();
	void  Update();
	float GetDeltaTime() const;
	int   GetFPS() const;

private:
	float m_accumulate;
	int   m_frameCount;
	int   m_count;

	std::chrono::duration<float>                                m_deltaTime;
	std::chrono::time_point<std::chrono::high_resolution_clock> m_currentTime;
	std::chrono::time_point<std::chrono::high_resolution_clock> m_prevTime;
};
