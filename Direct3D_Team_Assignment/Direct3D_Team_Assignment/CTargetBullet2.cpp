#include "pch.h"
#include "CTargetBullet2.h"
#include "CameraMgr.h"

CTargetBullet2::CTargetBullet2()
{
}

CTargetBullet2::~CTargetBullet2()
{
	Release();
}

void CTargetBullet2::Initialize()
{
	m_tINFO.vLook = { 0, -1, 0 };

	m_fRadius = 30.f;

	m_vLocalVec = { 0,0,0 };
}

void CTargetBullet2::Update()
{
	if (m_pTarget)
	{
		FollowTarget(m_pTarget);
	}
	else
		m_tINFO.vPos += m_tINFO.vDir * m_fSpeed;

	D3DXMATRIX matScale, matRotZ, matTrans, matWorld;

	D3DXMatrixScaling(&matScale, 1, 1, 1);
	D3DXMatrixRotationZ(&matRotZ, m_fRadian);
	D3DXMatrixTranslation(&matTrans, m_tINFO.vPos.x, m_tINFO.vPos.y, m_tINFO.vPos.z);

	D3DXMatrixIdentity(&matWorld);

	matWorld = matScale * matRotZ * matTrans;

	D3DXVec3TransformNormal(&m_tINFO.vDir, &m_tINFO.vLook, &matRotZ);
	D3DXVec3TransformCoord(&m_vWorldVec, &m_vLocalVec, &matWorld);

	D3DXMATRIX matView = CameraMgr::GetInstance().GetViewMat();
	D3DXMATRIX matProj = CameraMgr::GetInstance().GetProjMat();

	D3DXVec3TransformCoord(&m_vViewVec, &m_vWorldVec, &matView);
	D3DXVec3TransformCoord(&m_vProjVec, &m_vViewVec, &matProj);
	m_vProjVec = CameraMgr::GetInstance().ProjToScreen(m_vProjVec);
	
}

void CTargetBullet2::LateUpdate()
{
}

void CTargetBullet2::Render(HDC _hDC)
{

	float projScale = CameraMgr::GetInstance().GetProjScale();
	Ellipse(_hDC,
		m_vProjVec.x - m_fRadius / 2 * projScale,
		m_vProjVec.y - m_fRadius / 2 * projScale,
		m_vProjVec.x + m_fRadius / 2 * projScale,
		m_vProjVec.y + m_fRadius / 2 * projScale);
}

void CTargetBullet2::Release()
{
}

void CTargetBullet2::FollowTarget(CObj* pTarget)
{
	D3DXVECTOR3 tempTargetPos = pTarget->GetPos() - m_tINFO.vPos;

	D3DXVec3Normalize(&tempTargetPos, &tempTargetPos);
	
	D3DXVECTOR3 tempCrossResult;
	D3DXVec3Cross(&tempCrossResult, &m_tINFO.vDir, &tempTargetPos);
	
	float fAngle = D3DXVec3Dot(&m_tINFO.vDir, &tempTargetPos);

	if (fAngle <= -1.f)
		fAngle = -1.f;
	if (fAngle >= 1.f)
		fAngle = 1.f;

	fAngle = acosf(fAngle);
	
	if (fAngle > D3DXToRadian(5))
	{
		fAngle = D3DXToRadian(5);
	}

	if (tempCrossResult.z > 0)
	{
		fAngle = -fAngle;
	}

	m_fRadian += fAngle;

	D3DXMATRIX newMatDIR;
	D3DXVECTOR3 newDir;

	D3DXMatrixRotationZ(&newMatDIR, m_fRadian);
	D3DXVec3TransformNormal(&newDir, &m_tINFO.vDir, &newMatDIR);

	m_tINFO.vPos -= newDir * m_fSpeed;
}