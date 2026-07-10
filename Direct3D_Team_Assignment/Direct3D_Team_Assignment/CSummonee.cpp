#include "pch.h"
#include "CSummonee.h"

CSummonee::CSummonee()
{

}

CSummonee::~CSummonee()
{
	Release();
}

void CSummonee::Initialize()
{
	m_fSpeed = 4.f;

	m_vLocalBodyPoints[0] = { -20, -20, 0 };
	m_vLocalBodyPoints[1] = { 20, -20, 0 };
	m_vLocalBodyPoints[2] = { 20, 20, 0 };
	m_vLocalBodyPoints[3] = { -20, 20, 0 };

	m_vWorldPosinPoint = { 0, -40, 0 };

	m_fRadian = 0;
}

void CSummonee::Update()
{
	D3DXMATRIX matScale, matTrans, matRev, matWorld;
	D3DXMatrixScaling(&matScale, 1, 1, 1);
	D3DXMatrixTranslation(&matTrans, m_tINFO.vPos.x, m_tINFO.vPos.y, m_tINFO.vPos.z);
	D3DXMatrixRotationZ(&matRev, D3DXToRadian(20));

	D3DXMatrixIdentity(&matWorld);
	matWorld = matScale * matTrans* matRev;

	for (int i = 0; i < 4; ++i)
	{
		D3DXVec3TransformCoord(&m_vWorldBodyPoints[i], &m_vLocalBodyPoints[i], &matWorld);
	}

	D3DXVec3TransformCoord(&m_vWorldPosinPoint, &m_vLocalPosinPoint, &matWorld);
}

void CSummonee::LateUpdate()
{

}

void CSummonee::Render(HDC _hDC)
{
	MoveToEx(_hDC, m_vWorldBodyPoints[0].x, m_vWorldBodyPoints[0].y, nullptr);
	for (int i = 1; i <= 4; ++i)
	{
		LineTo(_hDC, m_vWorldBodyPoints[i % 4].x, m_vWorldBodyPoints[i % 4].y);
	}

	Ellipse(_hDC, 
		m_vWorldBodyPoints[0].x - 5, 
		m_vWorldBodyPoints[0].y - 5, 
		m_vWorldBodyPoints[0].x + 5, 
		m_vWorldBodyPoints[0].y + 5);

	Ellipse(_hDC, 
		m_vWorldBodyPoints[1].x - 5, 
		m_vWorldBodyPoints[1].y - 5, 
		m_vWorldBodyPoints[1].x + 5, 
		m_vWorldBodyPoints[1].y + 5);

	MoveToEx(_hDC, m_tINFO.vPos.x, m_tINFO.vPos.y, nullptr);
	LineTo(_hDC, m_vWorldPosinPoint.x, m_vWorldPosinPoint.y);
}

void CSummonee::Release()
{

}