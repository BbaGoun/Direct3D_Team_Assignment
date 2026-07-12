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
	//일단 이것 팔로우타겟에서 움직임을 진행하고있으면 필요없긴해요
	//상민이형이 타겟 안없어지게 바꿔서 ㄱㅊ음
	//나중에 플레이어 추적이 아니라 주변 오브젝트 추적으로 바뀌면 그때 고려하면됨.
	//혹시 모르니까 냄겨두고(삭제 해도 바뀐게 없었으니까

	D3DXMATRIX matScale, matRotZ, matTrans, matWorld;

	D3DXMatrixScaling(&matScale, 1, 1, 1);
	D3DXMatrixRotationZ(&matRotZ, m_fRadian);
	D3DXMatrixTranslation(&matTrans, m_tINFO.vPos.x, m_tINFO.vPos.y, m_tINFO.vPos.z);

	D3DXMatrixIdentity(&matWorld);

	matWorld = matScale * matRotZ * matTrans;

	D3DXVec3TransformCoord(&m_vWorldVec, &m_vLocalVec, &matWorld);

	D3DXMATRIX matView = CameraMgr::GetInstance().GetViewMat();
	D3DXMATRIX matProj = CameraMgr::GetInstance().GetProjMat();

	D3DXVec3TransformCoord(&m_vViewVec, &m_vWorldVec, &matView);
	D3DXVec3TransformCoord(&m_vProjVec, &m_vViewVec, &matProj);
	m_vProjVec += {640, 360, 0};
	
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
	//일단 버그중에 하나 고침  아크코사인 다음에 선회력 계산을 해야하는데 순서가 꼬여있었고, 포스문제도 제가 또 확인해볼게요
	if (fAngle > D3DXToRadian(5))
	{
		fAngle = D3DXToRadian(5);
	}
	if (fAngle < D3DXToRadian(-5))
	{
		fAngle = D3DXToRadian(-5);
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

	m_tINFO.vPos += newDir * m_fSpeed;
}