#include "pch.h"
#include "CBullet2.h"
#include "CPlayer.h"

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

	m_fRadian = D3DX_PI;
}

void CBullet2::Update()
{
	m_tINFO.vPos += m_tINFO.vDir * m_fSpeed;

	D3DXMATRIX matScale, matRotZ, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, 1, 1, 1);
	D3DXMatrixRotationZ(&matRotZ, m_fRadian);
	D3DXMatrixTranslation(&matTrans, m_tINFO.vPos.x, m_tINFO.vPos.y, m_tINFO.vPos.z);

	D3DXMatrixIdentity(&matWorld);
	matWorld = matScale * matRotZ * matTrans;

	for (int i = 0; i < 4; ++i)
	{
		D3DXVec3TransformCoord(&m_vWorldBodyPoint[i], &m_vLocalBodyPoint[i], &matWorld);
	}
}

void CBullet2::LateUpdate()
{

}

void CBullet2::Render(HDC _hDC)
{
	Ellipse(_hDC, m_vWorldBodyPoint[0].x, m_vWorldBodyPoint[1].y, m_vWorldBodyPoint[2].x, m_vWorldBodyPoint[3].y);
}

void CBullet2::Release()
{

}

void CBullet2::TakeDamage(int _iDamage)
{
}
