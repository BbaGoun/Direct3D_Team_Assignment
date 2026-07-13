#include "pch.h"
#include "CObstacle.h"
#include "CameraMgr.h"
#include "AbstractFactory.h"

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
	D3DXMatrixIdentity(&m_tINFO.matWorld);

	if (m_eType == OBSTACLE_ROTATION_CROSS_PART) {
		D3DXMATRIX matScale, matRotZ, matTrans;
		m_fRadian += D3DXToRadian(3.f);
		D3DXMatrixScaling(&matScale, 1, 1, 1);
		D3DXMatrixRotationZ(&matRotZ, m_fRadian);
		D3DXMatrixTranslation(&matTrans, m_tINFO.vPos.x, m_tINFO.vPos.y, m_tINFO.vPos.z);
		m_tINFO.matWorld = matScale * matRotZ * matTrans;
	}
	else if (m_eType == OBSTACLE_CIRCLE) {
		D3DXVECTOR3 originPos = m_pParent->GetPos();
		D3DXMATRIX matScale, matRotZ, matTrans, matRevol, matRTrans, matSatelPos;
		D3DXMatrixIdentity(&matRevol);
		m_fRadian += D3DXToRadian(3.f);

		D3DXMatrixScaling(&matScale, 1, 1, 1);
		D3DXMatrixRotationZ(&matRotZ, m_fRadian);
		D3DXMatrixTranslation(&matTrans, originPos.x, originPos.y, originPos.z);
		
		D3DXMatrixTranslation(&matRTrans, -originPos.x, -originPos.y, -originPos.z);
		D3DXMatrixTranslation(&matSatelPos, 0, -320, 0);
		matRevol = matRTrans * matSatelPos * matRotZ * matTrans;

		m_tINFO.matWorld = matScale * matRotZ * matTrans * matRevol;
	}
	else {
		D3DXMATRIX matScale, matRotZ, matTrans;
		D3DXMatrixScaling(&matScale, 1, 1, 1);
		D3DXMatrixRotationZ(&matRotZ, m_fRadian);
		D3DXMatrixTranslation(&matTrans, m_tINFO.vPos.x, m_tINFO.vPos.y, m_tINFO.vPos.z);
		m_tINFO.matWorld = matScale * matRotZ * matTrans;
	}
	
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
	if (m_eType == OBSTACLE_ORBIT)
	{
		delete m_pSatellite;
		m_pSatellite = nullptr;
	}
}

void CObstacle::SetObstacle(OBSTACLE_ID _eID, D3DXVECTOR3 _vPos, float _fRadian)
{
	switch (_eID) {
	case OBSTACLE_CIRCLE:
	{
		float circleR = 60;
		for (int i = 0; i < 12; ++i) {
			m_vLocalVec.push_back({ cosf(D3DXToRadian(-30 * i)) * circleR,
				sinf(D3DXToRadian(-30 * i)) * circleR , 0 });
		}
		m_eType = OBSTACLE_CIRCLE;
	}
	break;
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
	case OBSTACLE_ROTATION_CROSS_PART:
	{
		float r = 200;
		float halfWidth = 20;
		m_vLocalVec.push_back({ 0, 0, 0 });
		m_vLocalVec.push_back({-halfWidth, -halfWidth, 0});
		m_vLocalVec.push_back({-halfWidth, -r, 0});
		m_vLocalVec.push_back({halfWidth, -r, 0});
		m_vLocalVec.push_back({halfWidth, -halfWidth, 0});
		m_eType = OBSTACLE_ROTATION_CROSS_PART;
	}
	break;
	case OBSTACLE_ORBIT:
	{
		m_pSatellite = AbstractFactory<CObstacle>::Create();
		m_pSatellite->SetParent(this);
		static_cast<CObstacle*>(m_pSatellite)->SetObstacle(OBSTACLE_CIRCLE, { 0, 0, 0 }, 0);
		m_pSatellite->SetRadian(_fRadian);
		ObjMgr::GetInstance().AddObject(OBJ_OBSTACLE, m_pSatellite);
		m_eType == OBSTACLE_ORBIT;
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
	m_fRadian = _fRadian;
	m_vWorldVec.resize(m_vLocalVec.size());
	m_vViewVec.resize(m_vLocalVec.size());
	m_vProjVec.resize(m_vLocalVec.size());
}
