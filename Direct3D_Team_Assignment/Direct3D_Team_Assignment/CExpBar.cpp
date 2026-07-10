#include "pch.h"
#include "CExpBar.h"

CExpBar::CExpBar()
{
}

CExpBar::~CExpBar()
{
}

void CExpBar::Initialize()
{
	m_vLocalVec.push_back({ -50, -12.5, 0 });
	m_vLocalVec.push_back({ 50, -12.5, 0 });
	m_vLocalVec.push_back({ 50, 12.5, 0 });
	m_vLocalVec.push_back({ -50, 12.5, 0 });

	m_vWorldVec.resize(m_vLocalVec.size());
}

void CExpBar::Update()
{
	D3DXVECTOR3 targetPos = m_pTarget->GetPos();
	float rate = float(m_pTarget->GetCurrentExp()) / m_pTarget->GetMaxExp();

	D3DXVECTOR3 expRightTop = m_vLocalVec[0] + (m_vLocalVec[1] - m_vLocalVec[0]) * rate;
	D3DXVECTOR3 expRightBottom = m_vLocalVec[3] + (m_vLocalVec[2] - m_vLocalVec[3]) * rate;

	D3DXMATRIX matWorld;
	D3DXMatrixIdentity(&matWorld);

	D3DXMATRIX matTrans;
	D3DXMatrixTranslation(&matTrans, targetPos.x, targetPos.y + 120, 0);

	for (int i = 0; i < m_vLocalVec.size(); ++i) {
		D3DXVec3TransformCoord(&m_vWorldVec[i], &m_vLocalVec[i], &matTrans);
	}

	D3DXVec3TransformCoord(&m_vWorldExpRightTop, &expRightTop, &matTrans);
	D3DXVec3TransformCoord(&m_vWorldExpRightBottom, &expRightBottom, &matTrans);
}

void CExpBar::LateUpdate()
{
}

void CExpBar::Render(HDC _hDC)
{
	POINT pointsBase[4];
	POINT pointsBar[4];
	for (int i = 0; i < m_vWorldVec.size(); ++i) {
		pointsBase[i] = { int(m_vWorldVec[i].x), int(m_vWorldVec[i].y) };
	}
	pointsBar[0] = { int(m_vWorldVec[0].x), int(m_vWorldVec[0].y) };
	pointsBar[1] = { int(m_vWorldExpRightTop.x), int(m_vWorldExpRightTop.y) };
	pointsBar[2] = { int(m_vWorldExpRightBottom.x), int(m_vWorldExpRightBottom.y) };
	pointsBar[3] = { int(m_vWorldVec[3].x), int(m_vWorldVec[3].y) };

	HBRUSH hBrush1 = CreateSolidBrush(RGB(255, 255, 255));
	HBRUSH hOldBrush = (HBRUSH)SelectObject(_hDC, hBrush1);
	Polygon(_hDC, pointsBase, 4);
	SelectObject(_hDC, hOldBrush);

	HBRUSH hBrush2 = CreateSolidBrush(RGB(255, 255, 0));
	hOldBrush = (HBRUSH)SelectObject(_hDC, hBrush2);
	Polygon(_hDC, pointsBar, 4);
	SelectObject(_hDC, hOldBrush);

	DeleteObject(hBrush1);
	DeleteObject(hBrush2);
}

void CExpBar::Release()
{
}
