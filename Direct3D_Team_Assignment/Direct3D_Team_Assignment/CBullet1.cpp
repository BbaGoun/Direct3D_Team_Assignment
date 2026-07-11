#include "pch.h"
#include "CBullet1.h"
#include "CameraMgr.h"

CBullet1::CBullet1()
{
}

CBullet1::~CBullet1()
{
}

void CBullet1::Initialize()
{
	m_fSize = 30.f;
	
	m_vLocalVec.resize(1);
	m_vWorldVec.resize(1);
	m_vViewVec.resize(1);
	m_vProjVec.resize(1);

	m_vLocalVec[0] = { 0,0,0 };

	m_iHP = 5;
	m_iDamage = 5;
}

void CBullet1::Update()
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

void CBullet1::LateUpdate()
{
	m_tINFO.vPos += m_tINFO.vDir * m_fSpeed;
}

void CBullet1::Render(HDC _hDC)
{
	// 정점이 없는 원의 경우 크기 조절을 위한 값
	float projScale = CameraMgr::GetInstance().GetProjScale();

	Ellipse(_hDC,
		m_vProjVec[0].x - m_fSize / 2 * projScale,
		m_vProjVec[0].y - m_fSize / 2 * projScale,
		m_vProjVec[0].x + m_fSize / 2 * projScale,
		m_vProjVec[0].y + m_fSize / 2 * projScale);
}

void CBullet1::Release()
{
}
