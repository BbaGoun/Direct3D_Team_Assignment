#include "pch.h"
#include "CBreakableObj.h"
#include "ObjMgr.h"
#include "CameraMgr.h"

CBreakableObj::CBreakableObj()
{
}

CBreakableObj::~CBreakableObj()
{
	Release();
}

void CBreakableObj::Initialize()
{
	if (dis(gen) < 25) { // 정삼각형
		float side = 80.f;
		float halfSide = side * 0.5f;
		float height = side * sqrtf(3) * 0.5f;

		m_vLocalVec.push_back({ 0, -height * 2.f / 3.f, 0 });
		m_vLocalVec.push_back({ halfSide, height / 3.f, 0 });
		m_vLocalVec.push_back({ -halfSide, height / 3.f, 0 });
		m_eType = BREAKABLE_TRIANGLE;
		m_iDropExp = 1;
		m_iHP = 10;
		m_iMaxHP = 10;
	}
	else if (dis(gen) < 50) { // 정사각형
		float halfLength = 75 * 0.5f;
		m_vLocalVec.push_back({ -halfLength, -halfLength, 0 });
		m_vLocalVec.push_back({ halfLength, -halfLength, 0 });
		m_vLocalVec.push_back({ halfLength, halfLength, 0 });
		m_vLocalVec.push_back({ -halfLength, halfLength, 0 });
		m_eType = BREAKABLE_SQUARE;
		m_iDropExp = 2;
		m_iHP = 15;
		m_iMaxHP = 15;
	}
	else if (dis(gen) < 75) { // 정오각형
		for (int i = 0; i < 5; ++i) {
			float x = 45 * sinf(PI * i * 0.4);
			float y = 45 * cosf(PI * i * 0.4);
			m_vLocalVec.push_back({ x, y, 0 });
		}
		m_eType = BREAKABLE_PENTAGON;
		m_iDropExp = 3;
		m_iHP = 20;
		m_iMaxHP = 20;
	}
	else { // 정육각형
		float r = 45;
		float deltaX = r * sqrtf(3) * 0.5f;
		float deltaY = r * 0.5f;
		m_vLocalVec.push_back({0, -r, 0});
		m_vLocalVec.push_back({deltaX, -deltaY, 0});
		m_vLocalVec.push_back({deltaX, deltaY, 0});
		m_vLocalVec.push_back({0, r, 0});
		m_vLocalVec.push_back({-deltaX, deltaY, 0});
		m_vLocalVec.push_back({ -deltaX, -deltaY, 0 });
		m_eType = BREAKABLE_HEXAGON;
		m_iDropExp = 4;
		m_iHP = 25;
		m_iMaxHP = 25;
	}

	m_iDamage = 5;

	m_fRadian = dis(gen) * 3.6f;
	m_vWorldVec.resize(m_vLocalVec.size());
	m_vViewVec.resize(m_vLocalVec.size());
	m_vProjVec.resize(m_vLocalVec.size());
}

void CBreakableObj::Update()
{
	UpdateTimers();

	D3DXMATRIX matScale, matRotZ, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, 1, 1, 1);
	D3DXMatrixRotationZ(&matRotZ, m_fRadian);
	D3DXMatrixTranslation(&matTrans, m_tINFO.vPos.x, m_tINFO.vPos.y, m_tINFO.vPos.z);

	D3DXMatrixIdentity(&matWorld);
	matWorld = matScale * matRotZ * matTrans;

	for (int i = 0; i < m_vLocalVec.size(); ++i) {
		D3DXVec3TransformCoord(&m_vWorldVec[i], &m_vLocalVec[i], &matWorld);
	}

	// 월드 -> 뷰 -> 투영 스페이스 변환
	D3DXMATRIX matView = CameraMgr::GetInstance().GetViewMat();

	D3DXMATRIX matProj = CameraMgr::GetInstance().GetProjMat();

	for (int i = 0; i < m_vWorldVec.size(); ++i) {
		D3DXVec3TransformCoord(&m_vViewVec[i], &m_vWorldVec[i], &matView);
		// Z Division 이 행렬에 포함되어 있음.
		D3DXVec3TransformCoord(&m_vProjVec[i], &m_vViewVec[i], &matProj);
		m_vProjVec[i] += {640, 360, 0};
	}
}

void CBreakableObj::LateUpdate()
{
}

void CBreakableObj::Render(HDC _hDC)
{
	POINT* points = new POINT[m_vProjVec.size()];
	for (int i = 0; i < m_vProjVec.size(); ++i) {
		points[i] = { int(m_vProjVec[i].x), int(m_vProjVec[i].y) };
	}

	HBRUSH hBrush = nullptr;
	switch (m_eType) {
	case BREAKABLE_TRIANGLE:
		hBrush = CreateSolidBrush(RGB(254, 119, 119));
		break;
	case BREAKABLE_SQUARE:
		hBrush = CreateSolidBrush(RGB(254, 232, 103));
		break;
	case BREAKABLE_PENTAGON:
		hBrush = CreateSolidBrush(RGB(137, 255, 104));
		break;
	case BREAKABLE_HEXAGON:
		hBrush = CreateSolidBrush(RGB(191, 129, 245));
		break;
	}

	HBRUSH hOldBrush = (HBRUSH)SelectObject(_hDC, hBrush);
	Polygon(_hDC, points, m_vProjVec.size());

	SelectObject(_hDC, hOldBrush);
	DeleteObject(hBrush);
	delete[] points;
	points = nullptr;
}

void CBreakableObj::Release()
{
}
