#include "pch.h"
#include "CBulletTrakin1.h"

#include "CameraMgr.h"
CBulletTrakin1::CBulletTrakin1()
{
}

CBulletTrakin1::~CBulletTrakin1()
{
	Release();
}

void CBulletTrakin1::Initialize()
{
	m_fSize = 30.f;

	m_vLocalVec.resize(1);
	m_vWorldVec.resize(1);
	m_vViewVec.resize(1);
	m_vProjVec.resize(1);

	m_vLocalVec[0] = { 0,0,0 };

	m_tINFO.vLook = { 0,-1,0 };

	m_iHP = 5;
	m_iDamage = 5;

	m_bBeTraking = true;
}

void CBulletTrakin1::Update()
{
	if (m_bBeTraking)
	{
		D3DXVECTOR3 vTargetPos = m_pEnemy->GetPos();

		D3DXVECTOR3 vTargetDir = vTargetPos - m_tINFO.vPos;
		D3DXVec3Normalize(&vTargetDir, &vTargetDir);

		D3DXVECTOR3 vCrossVec;
		float fDotVec;

		D3DXVec3Cross(&vCrossVec, &m_tINFO.vDir, &vTargetDir);

		fDotVec = D3DXVec3Dot(&m_tINFO.vDir, &vTargetDir);

		if (fDotVec <= -1.f)
			fDotVec = -1.f;
		if (fDotVec >= 1.f)
			fDotVec = 1.f;

		fDotVec = acosf(fDotVec);
		if (fDotVec > D3DXToRadian(5))
			fDotVec = D3DXToRadian(5);
		if (vCrossVec.z > 0.f)
		{
			m_fRadian += fDotVec;
		}
		else if (vCrossVec.z < 0.f)
		{
			m_fRadian -= fDotVec;
		}
	}
	//����Ȯ���غ����� ���� ��� �Ѻ�����
	UpdateTimers();

	D3DXMATRIX matScale, matRotZ, matTrans, matWorld;
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixRotationZ(&matRotZ, m_fRadian);
	D3DXMatrixScaling(&matScale, 1, 1, 1);
	D3DXMatrixTranslation(&matTrans, m_tINFO.vPos.x, m_tINFO.vPos.y, m_tINFO.vPos.z);

	matWorld = matScale * matRotZ * matTrans;

	for (int i = 0; i < m_vLocalVec.size(); ++i) {
		D3DXVec3TransformCoord(&m_vWorldVec[i], &m_vLocalVec[i], &matWorld);
	}

	if (m_bBeTraking) D3DXVec3TransformNormal(&m_tINFO.vDir, &m_tINFO.vLook, &matRotZ);

	D3DXMATRIX matView = CameraMgr::GetInstance().GetViewMat();
	D3DXMATRIX matProj = CameraMgr::GetInstance().GetProjMat();

	for (int i = 0; i < m_vWorldVec.size(); ++i) {
		D3DXVec3TransformCoord(&m_vViewVec[i], &m_vWorldVec[i], &matView);
		D3DXVec3TransformCoord(&m_vProjVec[i], &m_vViewVec[i], &matProj);
		m_vProjVec[i] = CameraMgr::GetInstance().ProjToScreen(m_vProjVec[i]);
	}

	m_tINFO.vPos += m_tINFO.vDir * m_fSpeed;
}
void CBulletTrakin1::LateUpdate()
{
}

void CBulletTrakin1::Render(HDC _hDC)
{
	float projScale = CameraMgr::GetInstance().GetProjScale();

	Ellipse(_hDC,
		m_vProjVec[0].x - m_fSize / 2 * projScale,
		m_vProjVec[0].y - m_fSize / 2 * projScale,
		m_vProjVec[0].x + m_fSize / 2 * projScale,
		m_vProjVec[0].y + m_fSize / 2 * projScale);
}

void CBulletTrakin1::Release()
{
}