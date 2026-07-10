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
	//임시
	m_fSpeed = 8.f;
	m_fSize = 30.f;
	//m_tINFO.vPos = { 300.f, 400.f, 0.f };
	m_vLocalPoint = { 0,0,0 };
}

void CBullet1::Update()
{
	D3DXMATRIX matScale, matRotZ, matTrans, matWorld;


	D3DXMatrixScaling(&matScale, 1, 1, 1);
	D3DXMatrixRotationZ(&matRotZ, m_fRadian);
	D3DXMatrixTranslation(&matTrans, m_tINFO.vPos.x, m_tINFO.vPos.y, m_tINFO.vPos.z);

	D3DXMatrixIdentity(&matWorld);
	matWorld = matScale * matRotZ * matTrans;

	D3DXVec3TransformCoord(&m_vWorldPoint, &m_vLocalPoint, &matWorld);
}

void CBullet1::LateUpdate()
{
	m_tINFO.vPos += m_tINFO.vDir * m_fSpeed;
}

void CBullet1::Render(HDC _hDC)
{
	Ellipse(_hDC,
		m_vWorldPoint.x - m_fSize/2,
		m_vWorldPoint.y - m_fSize/2,
		m_vWorldPoint.x + m_fSize/2,
		m_vWorldPoint.y + m_fSize/2);
	//총알 생성 확인용
	//Ellipse(_hDC,
	//	300, 400, 50, 50);
}

void CBullet1::Release()
{
}

void CBullet1::TakeDamage(int _iDamage)
{
}
