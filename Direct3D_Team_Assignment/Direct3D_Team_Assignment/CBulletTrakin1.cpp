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
		//그리고 내적값 한번 -1~1로 보정하는과정을 거친 다음에 아크코사인을 하는게 안정성이 높음
		//지금 아크코사인에다가 내적값 바로 넣고있으니까
		//내적값을 플롯형에 넣고-> -1~1 사이이 값으로 보정하고 그다음에 아크코사인
		//이거 안하니까 저는 총알이 밖으로 튀었음
		//상관없음 어짜피 디삼디엑스투라디안 통해서 저희가 아는 가시적인 형태의 각도로 넘겨주고 있어서
		//애초에 지금 돝백은 라디안을 가진게 아니라, 내적의 결과값을 가지고 있는거고, 이걸 아크코사인으로 넘겨서 실제 내적 끼인각의 라디안을 받아오는 과정인데, 이 과정에서 컴퓨터상의 한계로 부동소수점에서 0.0000000001 같은값이 남아있을까봐 보정을 한번 거쳐주는 과정이 있는거에요
		//일단 문제는 없어보이는데 흠...
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
	//대조확인해보려고 제거 잠깐 켜본거임
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
		m_vProjVec[i] += {640, 360, 0};
	}

	m_tINFO.vPos += m_tINFO.vDir * m_fSpeed;
}
//참조하는 타겟 포스가 몇인지 확인해봐야할듯 중단점 찍어서
//오우 값이 왜 1920 ㅇㅈㄹ 그 혹시 포신 현재 위치도 저 위치인가요 지금 디버그 중이니까 지금 현 상태에서 총알 스폰지점이랑 지금 저 vpos가 같은지 확인해봅시다
//아 코드 밖이라서 그런가
//확실히 포스 지정이 이상한듯 
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