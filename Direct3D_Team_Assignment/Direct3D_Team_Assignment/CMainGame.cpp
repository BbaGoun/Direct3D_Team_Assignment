#include "pch.h"
#include "Define.h"
#include "CMainGame.h"
#include "AbstractFactory.h"
#include <profileapi.h>
#include "TimeMgr.h"
#include "SceneMgr.h"
#include "CameraMgr.h"

CMainGame::CMainGame()
{
}

CMainGame::~CMainGame()
{
	Release();
}

void CMainGame::Initialize()
{
	m_hDC = GetDC(g_hWnd);

	m_hMemDC = CreateCompatibleDC(m_hDC);
	m_hBitmap = CreateCompatibleBitmap(m_hDC, WINCX, WINCY);
	m_hOldBitmap = (HBITMAP)SelectObject(m_hMemDC, m_hBitmap);

	LARGE_INTEGER tmp;
	QueryPerformanceFrequency(&tmp);
	m_Frequency = float(tmp.QuadPart);
	QueryPerformanceCounter(&m_LastFrameTime);
	m_FixedAccumulator = 0.f;

	SceneMgr::GetInstance().SceneChange(SC_STAGE1);
}

void CMainGame::RunGame()
{
	QueryPerformanceCounter(&m_EndTime);

	float elapsed = float(m_EndTime.QuadPart - m_LastFrameTime.QuadPart) / m_Frequency;

	if (elapsed < TARGET_FRAME_TIME) {
		return;
	}

	elapsed = min(elapsed, TimeMgr::GetInstance().GetMaxDeltaTime());
	m_LastFrameTime = m_EndTime;

	TimeMgr::GetInstance().SetUnscaledDeltaTime(elapsed);
	TimeMgr::GetInstance().SetDeltaTime(elapsed);
	TimeMgr::GetInstance().PlusUnscaledTime();
	TimeMgr::GetInstance().PlusScaledTime();

	Update();
	LateUpdate();
	Render();
}

void CMainGame::Update()
{
	SceneMgr::GetInstance().Update();
}

void CMainGame::LateUpdate()
{
	SceneMgr::GetInstance().LateUpdate();
	CameraMgr::GetInstance().LateUpdate();
}

void CMainGame::Render()
{
	Rectangle(m_hMemDC, -1, -1, WINCX + 1, WINCY + 1);

	SceneMgr::GetInstance().Render(m_hMemDC);

	BitBlt(m_hDC, 0, 0, WINCX, WINCY, m_hMemDC, 0, 0, SRCCOPY);
}

void CMainGame::Release()
{
	ReleaseDC(g_hWnd, m_hDC);

	SelectObject(m_hMemDC, m_hOldBitmap);
	DeleteDC(m_hMemDC);
	DeleteObject(m_hBitmap);

	SceneMgr::DestroyInstance();
}
