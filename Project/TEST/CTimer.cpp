#include "pch.h"
#include "CTimer.h"

namespace
{
	constexpr float LEAST_FPS = 1.f / 60.f;
}

CTimer::CTimer()
	:
	m_accumulate{0},
	m_frameCount{0},
	m_count{0},
	m_deltaTime{}
{
	m_prevTime = std::chrono::high_resolution_clock::now();
}

CTimer::~CTimer() = default;

void CTimer::Update()
{
	m_currentTime = std::chrono::high_resolution_clock::now();
	m_deltaTime   = m_currentTime - m_prevTime;
	m_prevTime    = m_currentTime;

	++m_count;
	m_accumulate += m_deltaTime.count();
	if (m_accumulate >= 1.f)
	{
		m_accumulate = m_accumulate - 1.f;
		m_frameCount = m_count;
		m_count      = 0;
	}
}

float CTimer::GetDeltaTime() const
{
	if (m_deltaTime.count() > LEAST_FPS)
	{
		return LEAST_FPS;
	}
	return m_deltaTime.count();
}

int CTimer::GetFPS() const
{
	return m_frameCount;
}
