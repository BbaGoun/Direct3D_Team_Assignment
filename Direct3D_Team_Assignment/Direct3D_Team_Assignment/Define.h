#pragma once

#define WINCX	1280
#define WINCY	720

#define PI		3.141592f

#define PURE	= 0

extern HWND g_hWnd;

#include <random>

extern std::random_device rd;
extern std::mt19937 gen;
extern std::uniform_int_distribution<int> dis;

typedef struct tagInfo
{
	D3DXVECTOR3		vPos;		// 위치 벡터
	D3DXVECTOR3		vDir;		// 방향 벡터
	D3DXVECTOR3		vLook;		// 방향 벡터

	D3DXVECTOR3		vNormal;	// 방향 벡터(법선 벡터)

	D3DXMATRIX		matWorld;   // 월드 변환 벡터

	bool			bChanged;	// 월드 변환 벡터가 수정되었는지
}INFO;

template<typename T>
void Safe_Delete(T*& p)
{
	if (p)
	{
		delete p;
		p = nullptr;
	}
}

static D3DXVECTOR3 GetMouse()
{
	POINT		pt{};

	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	return D3DXVECTOR3((float)pt.x, (float)pt.y, 0.f);
}
