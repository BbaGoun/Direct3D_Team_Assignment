#include "pch.h"
#include "CBullet1.h"

CBullet1::CBullet1()
{
}

CBullet1::~CBullet1()
{
}

void CBullet1::Initialize()
{
	m_fSize = 30.f;
	m_vLocalPoint = { 0,0,0 };
	m_tINFO.vLook = { 0,-1,0 };
}

void CBullet1::Update()
{
	if (m_bBeTraking)
	{
		//타겟 중점을 가져오고, 외적으로 좌우를 판별했음(현재 총알의 다이렉션 기준 z가 양수면 오른쪽,음수면 왼쪽), 그리고 내적해서 각도를 구한뒤에, 외적 결과값에 따라서 구한 세타를 라디안에 더할지 뺄지 계산.

		D3DXVECTOR3 vTargetPos = m_pEnemy->GetPos();
		D3DXVECTOR3 vTargetDir = vTargetPos - m_tINFO.vPos;//타겟을 향한 방향 벡터 (정규화 안됨)
		D3DXVECTOR3 vCrossVec;
		float fDotVec;

		D3DXVec3Normalize(&vTargetDir, &vTargetDir);//정규화 여기서 했음

		D3DXVec3Cross(&vCrossVec, &m_tINFO.vDir, &vTargetDir);//외적

		fDotVec = D3DXVec3Dot(&m_tINFO.vDir, &vTargetDir);//내적
		if (fDotVec <= -1.f)
			fDotVec = -1.f;
		if (fDotVec >= 1.f)
			fDotVec = 1.f;
		//내적값 소수점 오차방지(원래 식대로면 -1~1 사이가 나오는게 맞는데?!)
		//-1~1로 보정 안하니까 총알이 튀어서 결국 제미나이한테 help 쳐서 추가한건데
		//제미나이 왈: 부동소수점이 어쩌고 해서 소수점 밑으로 완벽한 값이 안나와서 그렇다고 함;

		fDotVec = acosf(fDotVec);
		if (fDotVec > D3DXToRadian(5))
			fDotVec = D3DXToRadian(5);
		//내적값(코사인세타)에 아크코사인 해서 세타 구하고, 이 세타값이 최대 선회력을 넘는 경우 보정함
		if (vCrossVec.z > 0.f)
		{
			m_fRadian += fDotVec;
		}
		else if (vCrossVec.z < 0.f)
		{
			m_fRadian -= fDotVec;
		}
	}

	D3DXMATRIX matScale, matRotZ, matTrans, matWorld;

	D3DXMatrixScaling(&matScale, 1, 1, 1);
	D3DXMatrixRotationZ(&matRotZ, m_fRadian);
	D3DXMatrixTranslation(&matTrans, m_tINFO.vPos.x, m_tINFO.vPos.y, m_tINFO.vPos.z);
	D3DXMatrixIdentity(&matWorld);
	matWorld = matScale * matRotZ * matTrans;

	if (m_bBeTraking)
		D3DXVec3TransformNormal(&m_tINFO.vDir, &m_tINFO.vLook, &matRotZ);

	D3DXVec3TransformCoord(&m_vWorldPoint, &m_vLocalPoint, &matWorld);

	m_tINFO.vPos += m_tINFO.vDir * m_fSpeed;
}

void CBullet1::LateUpdate()
{
}

void CBullet1::Render(HDC _hDC)
{
	Ellipse(_hDC,
		m_vWorldPoint.x - m_fSize / 2,
		m_vWorldPoint.y - m_fSize / 2,
		m_vWorldPoint.x + m_fSize / 2,
		m_vWorldPoint.y + m_fSize / 2);
}

void CBullet1::Release()
{
}
