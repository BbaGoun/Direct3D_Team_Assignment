#include "pch.h"
#include "CBullet2.h"

CBullet2::CBullet2()
{

}

CBullet2::~CBullet2()
{
	Release();
}

void CBullet2::Initialize()
{
	m_tINFO.vPos = { 0, 0, 0 };
	m_tINFO.vLook = { 0, -1, 0 };
	m_fSpeed = 8.f;

	m_vLocalBodyPoint[0] = { -5.f, -5.f, 0.f };
	m_vLocalBodyPoint[1] = { 5.f, -5.f, 0.f };
	m_vLocalBodyPoint[2] = { 5.f, 5.f, 0.f };
	m_vLocalBodyPoint[3] = { -5.f, 5.f, 0.f };
}

void CBullet2::Update()
{

}

void CBullet2::LateUpdate()
{

}

void CBullet2::Render(HDC hDC)
{

}

void CBullet2::Release()
{

}