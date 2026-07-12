#include "pch.h"
#include "CObstacle.h"
#include "CameraMgr.h"

CObstacle::CObstacle()
{
}

CObstacle::~CObstacle()
{
}

void CObstacle::Initialize()
{
}

void CObstacle::Update()
{
	if (m_eType == OBSTACLE_ROTATION_CROSS)
		m_fRadian += D3DXToRadian(1.f);
	D3DXMATRIX matScale, matRotZ, matTrans;
	D3DXMatrixScaling(&matScale, 1, 1, 1);
	D3DXMatrixRotationZ(&matRotZ, m_fRadian);
	D3DXMatrixTranslation(&matTrans, m_tINFO.vPos.x, m_tINFO.vPos.y, m_tINFO.vPos.z);

	D3DXMatrixIdentity(&m_tINFO.matWorld);
	m_tINFO.matWorld = matScale * matRotZ * matTrans;

	for (int i = 0; i < m_vLocalVec.size(); ++i) {
		D3DXVec3TransformCoord(&m_vWorldVec[i], &m_vLocalVec[i], &m_tINFO.matWorld);
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

void CObstacle::LateUpdate()
{
}

void CObstacle::Render(HDC _hDC)
{
	POINT* points = new POINT[m_vProjVec.size()];
	for (int i = 0; i < m_vProjVec.size(); ++i) {
		points[i] = { int(m_vProjVec[i].x), int(m_vProjVec[i].y) };
	}

	HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 0));
	HBRUSH hOldBrush = (HBRUSH)SelectObject(_hDC, hBrush);
	Polygon(_hDC, points, m_vProjVec.size());

	SelectObject(_hDC, hOldBrush);
	DeleteObject(hBrush);
	delete[] points;
	points = nullptr;
}

void CObstacle::Release()
{
}

void CObstacle::SetObstacle(OBSTACLE_ID _eID, D3DXVECTOR3 _vPos, float _fRadian)
{
	switch (_eID) {
	case OBSTACLE_TRIANGLE:
	{
		float side = 160.f;
		float halfSide = side * 0.5f;
		float height = side * sqrtf(3) * 0.5f;

		m_vLocalVec.push_back({ 0, -height * 2.f / 3.f, 0 });
		m_vLocalVec.push_back({ halfSide, height / 3.f, 0 });
		m_vLocalVec.push_back({ -halfSide, height / 3.f, 0 });
		m_eType = OBSTACLE_TRIANGLE;
	}
		break;
	case OBSTACLE_SQUARE:
	{
		float halfLength = 150 * 0.5f;
		m_vLocalVec.push_back({ -halfLength, -halfLength, 0 });
		m_vLocalVec.push_back({ halfLength, -halfLength, 0 });
		m_vLocalVec.push_back({ halfLength, halfLength, 0 });
		m_vLocalVec.push_back({ -halfLength, halfLength, 0 });
		m_eType = OBSTACLE_TRIANGLE;
	}
		break;
	case OBSTACLE_PENTAGON:
	{
		for (int i = 0; i < 5; ++i) {
			float x = 90 * sinf(PI * i * 0.4);
			float y = 90 * cosf(PI * i * 0.4);
			m_vLocalVec.push_back({ x, y, 0 });
		}
		m_eType = OBSTACLE_PENTAGON;
	}
		break;
	case OBSTACLE_HEXAGON:
	{
		float r = 90;
		float deltaX = r * sqrtf(3) * 0.5f;
		float deltaY = r * 0.5f;
		m_vLocalVec.push_back({ 0, -r, 0 });
		m_vLocalVec.push_back({ deltaX, -deltaY, 0 });
		m_vLocalVec.push_back({ deltaX, deltaY, 0 });
		m_vLocalVec.push_back({ 0, r, 0 });
		m_vLocalVec.push_back({ -deltaX, deltaY, 0 });
		m_vLocalVec.push_back({ -deltaX, -deltaY, 0 });
		m_eType = OBSTACLE_HEXAGON;
	}
	break;
	case OBSTACLE_ROTATION_CROSS:
	{
		float r = 200;
		float halfWidth = 20;
		m_vLocalVec.push_back({-halfWidth, -r, 0});
		m_vLocalVec.push_back({halfWidth, -r, 0});
		m_vLocalVec.push_back({halfWidth, -halfWidth, 0});
		m_vLocalVec.push_back({r, -halfWidth, 0});
		m_vLocalVec.push_back({r, halfWidth, 0});
		m_vLocalVec.push_back({halfWidth, halfWidth, 0});
		m_vLocalVec.push_back({halfWidth, r, 0});
		m_vLocalVec.push_back({-halfWidth, r, 0});
		m_vLocalVec.push_back({-halfWidth, halfWidth, 0});
		m_vLocalVec.push_back({-r, halfWidth, 0});
		m_vLocalVec.push_back({-r, -halfWidth, 0});
		m_vLocalVec.push_back({-halfWidth, -halfWidth, 0});
		m_eType = OBSTACLE_ROTATION_CROSS;
	}
	break;
	case OBSTACLE_VERTICAL_WALL:
	{
		m_vLocalVec.push_back({ -100, -800, 0 });
		m_vLocalVec.push_back({ 100, -800, 0 });
		m_vLocalVec.push_back({ 100, 800, 0 });
		m_vLocalVec.push_back({ -100, 800, 0 });
		m_eType = OBSTACLE_VERTICAL_WALL;
	}
	break;
	case OBSTACLE_HORIZONTAL_WALL:
	{
		m_vLocalVec.push_back({ -1500, -100, 0 });
		m_vLocalVec.push_back({ 1500, -100, 0 });
		m_vLocalVec.push_back({ 1500, 100, 0 });
		m_vLocalVec.push_back({ -1500, 100, 0 });
		m_eType = OBSTACLE_HORIZONTAL_WALL;
	}
	break;
	}

	m_tINFO.vPos = _vPos;
	m_fRadian = m_fRadian;
	m_vWorldVec.resize(m_vLocalVec.size());
	m_vViewVec.resize(m_vLocalVec.size());
	m_vProjVec.resize(m_vLocalVec.size());
}
