#pragma once
#include <winnt.h>

class CMainGame
{
public:
	CMainGame();
	~CMainGame();

public:
	void Initialize();
	void RunGame();

private:
	void Update();
	void LateUpdate();
	void Render();
	void Release();

private:
	static constexpr float TARGET_FRAME_TIME = 1.f / 60.f;
	static constexpr int   MAX_FIXED_STEPS_PER_FRAME = 2;

	LARGE_INTEGER	m_LastFrameTime;
	LARGE_INTEGER	m_EndTime;
	float	m_Frequency;
	float	m_FixedAccumulator;

	HDC		m_hDC;
	HDC		m_hMemDC;
	HBITMAP	m_hBitmap;
	HBITMAP	m_hOldBitmap;
};

