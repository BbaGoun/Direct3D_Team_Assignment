#pragma once
#include "Define.h"
class TimeMgr
{
private:
	TimeMgr();
	TimeMgr(const TimeMgr& rhs) = delete;
	TimeMgr& operator=(const TimeMgr& rhs) = delete;
	~TimeMgr();

public:
	static TimeMgr& GetInstance() {
		static TimeMgr Instance;
		return Instance;
	}

public:
	float GetUnscaledTime() { return m_UnscaledTime; }
	float GetScaledTime() { return m_ScaledTime; }
	float GetLastFixedTime() { return m_LastFixedTime; }
	float GetMaxDeltaTime() { return m_MaxDeltaTime; }
	float GetUnscaledDeltaTime() { return m_UnscaledDeltaTime; }
	float GetDeltaTime() { return m_DeltaTime; }
	float GetFixedDeltaTime() { return m_FixedDeltaTime; }
	float GetTimeScale() { return m_TimeScale; }

	void SetUnscaledTime(float _UnscaledDeltaTime) { m_UnscaledTime = _UnscaledDeltaTime; }
	void PlusUnscaledTime() { m_UnscaledTime += m_UnscaledDeltaTime; }

	void SetScaledTime(float _ScaledDeltaTime) { m_ScaledTime = _ScaledDeltaTime; }
	void PlusScaledTime() { m_ScaledTime += m_DeltaTime; }

	void SetLastFixedTime(float _Time) { m_LastFixedTime = _Time; }
	void PlusLastFixedTime() { m_LastFixedTime += m_FixedDeltaTime; }

	void SetMaxDeltaTime(float _MaxDeltaTime) { m_MaxDeltaTime = _MaxDeltaTime; }
	void SetUnscaledDeltaTime(float _RealDeltaTime) { m_UnscaledDeltaTime = _RealDeltaTime; }
	void SetDeltaTime(float _RealDeltaTime) { m_DeltaTime = min(_RealDeltaTime * m_TimeScale, m_MaxDeltaTime); }
	void SetFixedDeltaTime(float _FixedDeltaTime) { m_FixedDeltaTime = _FixedDeltaTime; }
	void SetTimeScale(float _TimeScale) { m_TimeScale = max(0.f, _TimeScale); }

private:
	float m_UnscaledTime;
	float m_ScaledTime;
	float m_LastFixedTime;
	float m_MaxDeltaTime;
	float m_UnscaledDeltaTime;
	float m_DeltaTime;
	float m_FixedDeltaTime;
	float m_TimeScale;
};

