#include "pch.h"
#include "TimeMgr.h"

TimeMgr::TimeMgr() : m_UnscaledTime(0.f), m_ScaledTime(0.f)
, m_LastFixedTime(0.f), m_MaxDeltaTime(0.333f)
, m_UnscaledDeltaTime(0.f), m_DeltaTime(0.f)
, m_FixedDeltaTime(0.02f), m_TimeScale(1.f)
{
}

TimeMgr::~TimeMgr()
{
}
