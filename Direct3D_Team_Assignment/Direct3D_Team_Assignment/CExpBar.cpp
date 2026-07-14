#include "pch.h"
#include "CExpBar.h"
#include "CameraMgr.h"

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
	m_vViewVec.resize(m_vLocalVec.size());
	m_vProjVec.resize(m_vLocalVec.size());
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

	D3DXVec3TransformCoord(&m_vExpRightTop, &expRightTop, &matTrans);
	D3DXVec3TransformCoord(&m_vExpRightBottom, &expRightBottom, &matTrans);

	// ���� -> �� -> ���� �����̽� ��ȯ
	D3DXMATRIX matView = CameraMgr::GetInstance().GetViewMat();

	D3DXMATRIX matProj = CameraMgr::GetInstance().GetProjMat();

	for (int i = 0; i < m_vWorldVec.size(); ++i) {
		D3DXVec3TransformCoord(&m_vViewVec[i], &m_vWorldVec[i], &matView);
		// Z Division �� ��Ŀ� ���ԵǾ� ����.
		D3DXVec3TransformCoord(&m_vProjVec[i], &m_vViewVec[i], &matProj);
		m_vProjVec[i] = CameraMgr::GetInstance().ProjToScreen(m_vProjVec[i]);
	}
	D3DXVec3TransformCoord(&m_vExpRightTop, &m_vExpRightTop, &matView);
	D3DXVec3TransformCoord(&m_vExpRightTop, &m_vExpRightTop, &matProj);
	m_vExpRightTop = CameraMgr::GetInstance().ProjToScreen(m_vExpRightTop);
	D3DXVec3TransformCoord(&m_vExpRightBottom, &m_vExpRightBottom, &matView);
	D3DXVec3TransformCoord(&m_vExpRightBottom, &m_vExpRightBottom, &matProj);
	m_vExpRightBottom = CameraMgr::GetInstance().ProjToScreen(m_vExpRightBottom);
}

void CExpBar::LateUpdate()
{
}

void CExpBar::Render(HDC _hDC)
{
	POINT pointsBase[4];
	POINT pointsBar[4];
	for (int i = 0; i < m_vProjVec.size(); ++i) {
		pointsBase[i] = { int(m_vProjVec[i].x), int(m_vProjVec[i].y) };
	}
	pointsBar[0] = { int(m_vProjVec[0].x), int(m_vProjVec[0].y) };
	pointsBar[1] = { int(m_vExpRightTop.x), int(m_vExpRightTop.y) };
	pointsBar[2] = { int(m_vExpRightBottom.x), int(m_vExpRightBottom.y) };
	pointsBar[3] = { int(m_vProjVec[3].x), int(m_vProjVec[3].y) };

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
