#include "pch.h"
#include  "CTargetBullet2.h"

CTargetBullet2::CTargetBullet2()
{
}

CTargetBullet2::~CTargetBullet2()
{
	Release();
}

void CTargetBullet2::Initialize()
{
	m_fSize = 30.f;
	m_vLocalPoint = { 0,0,0 };
}

void CTargetBullet2::Update()
{
	D3DXMATRIX matScale, matRotZ, matTrans, matWorld;

	D3DXMatrixScaling(&matScale, 1, 1, 1);
	D3DXMatrixRotationZ(&matRotZ, m_fRadian);
	D3DXMatrixTranslation(&matTrans, m_tINFO.vPos.x, m_tINFO.vPos.y, m_tINFO.vPos.z);

	D3DXMatrixIdentity(&matWorld);

	matWorld = matScale * matRotZ * matTrans;

	D3DXVec3TransformCoord(&m_vWorldPoint, &m_vLocalPoint, &matWorld);

	if (m_pTarget)
	{
		FollowTarget(m_pTarget);
	}
	else
	{
		m_tINFO.vPos += m_tINFO.vDir * m_fSpeed;
	}
}

void CTargetBullet2::LateUpdate()
{
}

void CTargetBullet2::Render(HDC _hDC)
{
	Ellipse(_hDC,
		m_vWorldPoint.x - m_fSize / 2,
		m_vWorldPoint.y - m_fSize / 2,
		m_vWorldPoint.x + m_fSize / 2,
		m_vWorldPoint.y + m_fSize / 2);
}

void CTargetBullet2::Release()
{
}

void CTargetBullet2::FollowTarget(CObj* pTarget)
{
	D3DXVECTOR3 tempTargetPos = pTarget->GetPos() - m_tINFO.vPos;

	float fLength = sqrtf((tempTargetPos.x * tempTargetPos.x) + (tempTargetPos.y * tempTargetPos.y));

	float fDirX = tempTargetPos.x / fLength;
	float fDirY = tempTargetPos.y / fLength;
	float fDirZ = 0;

	float PosX = 1;
	float PosY = 0;
	float PosZ = 0;

	float fAngle = acosf((fDirX * PosX) + (fDirY * PosY) /
		(sqrtf((fDirX * fDirX) + (fDirY * fDirY)) *
			sqrtf((PosX * PosX) + (PosY * PosY))));

	if (pTarget->GetPos().y > m_tINFO.vPos.y)
	{
		fAngle = 2 * D3DX_PI - fAngle;
	}

	m_tINFO.vPos.x += cos(fAngle) * m_fSpeed;
	m_tINFO.vPos.y -= sin(fAngle) * m_fSpeed;
}