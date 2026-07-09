#include "pch.h"
#include "CBullet2.h"

CBullet::CBullet()
{

}

CBullet::~CBullet()
{
	Release();
}

void CBullet::Initialize()
{
	m_tINFO.vPos = { 0, 0, 0 };
	m_tINFO.vLook = { 0, -1, 0 };
	m_fSpeed = 8.f;

	m_vLocalBodyPoint[0] = { -5.f, -5.f, 0.f };
	m_vLocalBodyPoint[1] = { 5.f, -5.f, 0.f };
	m_vLocalBodyPoint[2] = { 5.f, 5.f, 0.f };
	m_vLocalBodyPoint[3] = { -5.f, 5.f, 0.f };
}

void CBullet::Update()
{

}

void CBullet::LateUpdate()
{

}

void CBullet::Render(HDC hDC)
{

}

void CBullet::Release()
{

}