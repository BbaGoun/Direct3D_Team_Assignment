#include "pch.h"
#include "CPlayer.h"
#include "AbstractFactory.h"
#include "ObjMgr.h"
#include "CBullet2.h"

CPlayer::CPlayer()
{
	m_bIsShootGun = true;
}

CPlayer::~CPlayer()
{
}

void CPlayer::Initialize()
{
	m_tINFO.vPos = { WINCX * 0.5f, WINCY * 0.5f, 0 };
	m_tINFO.vLook = { 0, -1, 0 };
	m_fSpeed = 8;

	m_vLocalBodyPoints[0] = { -50.f, -50.f, 0 };
	m_vLocalBodyPoints[1] = { 50.f, -50.f, 0 };
	m_vLocalBodyPoints[2] = { 50.f, 50.f, 0 };
	m_vLocalBodyPoints[3] = { -50.f, 50.f, 0 };

	m_vLocalPosinPoint = { 0, -100.f, 0 };
	m_fRadian = 0;
}

void CPlayer::Update()
{
	KeyInput();

	D3DXMATRIX matScale, matRotZ, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, 1, 1, 1);
	D3DXMatrixRotationZ(&matRotZ, m_fRadian);
	D3DXMatrixTranslation(&matTrans, m_tINFO.vPos.x, m_tINFO.vPos.y, m_tINFO.vPos.z);

	D3DXMatrixIdentity(&matWorld);
	matWorld = matScale * matRotZ * matTrans;

	for (int i = 0; i < 4; ++i) {
		D3DXVec3TransformCoord(&m_vWorldBodyPoints[i], &m_vLocalBodyPoints[i], &matWorld);
	}

	D3DXVec3TransformCoord(&m_vWorldPosinPoint, &m_vLocalPosinPoint, &matWorld);
}

void CPlayer::LateUpdate()
{
}

void CPlayer::Render(HDC _hDC)
{
	HPEN hPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 255));
	HPEN hOldPen = (HPEN)SelectObject(_hDC, hPen);

	MoveToEx(_hDC, m_vWorldBodyPoints[0].x, m_vWorldBodyPoints[0].y, nullptr);
	for (int i = 1; i <= 4; ++i) {
		LineTo(_hDC, m_vWorldBodyPoints[i % 4].x, m_vWorldBodyPoints[i % 4].y);
	}

	Ellipse(_hDC,
		m_vWorldBodyPoints[0].x - 5,
		m_vWorldBodyPoints[0].y - 5,
		m_vWorldBodyPoints[0].x + 5,
		m_vWorldBodyPoints[0].y + 5);

	Ellipse(_hDC,
		m_vWorldBodyPoints[1].x - 5,
		m_vWorldBodyPoints[1].y - 5,
		m_vWorldBodyPoints[1].x + 5,
		m_vWorldBodyPoints[1].y + 5);

	MoveToEx(_hDC, m_tINFO.vPos.x, m_tINFO.vPos.y, nullptr);
	LineTo(_hDC, m_vWorldPosinPoint.x, m_vWorldPosinPoint.y);

	SelectObject(_hDC, hOldPen);
	DeleteObject(hPen);
}

void CPlayer::Release()
{
}

void CPlayer::KeyInput()
{
	m_tINFO.vDir = { 0, 0, 0 };

	if (GetAsyncKeyState('A')) {
		m_fRadian += D3DXToRadian(-3);
	}
	if (GetAsyncKeyState('D')) {
		m_fRadian += D3DXToRadian(3);
	}

	if (GetAsyncKeyState('W')) {

		D3DXMATRIX matRotZ;
		D3DXMatrixRotationZ(&matRotZ, m_fRadian);
		D3DXVec3TransformNormal(&m_tINFO.vDir, &m_tINFO.vLook, &matRotZ);

		m_tINFO.vPos += m_tINFO.vDir * m_fSpeed;
	}
	if (GetAsyncKeyState('S')) {
		D3DXMATRIX matRotZ;
		D3DXMatrixRotationZ(&matRotZ, m_fRadian);
		D3DXVec3TransformNormal(&m_tINFO.vDir, &m_tINFO.vLook, &matRotZ);

		m_tINFO.vPos -= m_tINFO.vDir * m_fSpeed;
	}

	if (GetAsyncKeyState(VK_LSHIFT))
	{
		if (m_bIsShootGun)
		{
			D3DXMATRIX matRotZ, matRot;
			D3DXMatrixRotationZ(&matRotZ, m_fRadian);
			D3DXVec3TransformNormal(&m_tINFO.vDir, &m_tINFO.vLook, &matRotZ);
			D3DXMatrixRotationZ(&matRot, m_fRadian);
			D3DXVec3TransformNormal(&m_tINFO.vDir, &m_tINFO.vLook, &matRot);

			int tempAng = 0;
			//tempAng = dis(gen) % 41 - 20;
			int tempAng2 = 0;
			//tempAng2 = dis(gen) % 41 - 20;
			int tempAng3 = 0;
			//tempAng3 = dis(gen) % 41 - 20;
			int tempAng4 = 0;
			//tempAng4 = dis(gen) % 41 - 20;

			int tempSpeed = 0;
			//tempSpeed = dis(gen) % 6 + 6;
			int tempSpeed2 = 0;
			//tempSpeed2 = dis(gen) % 6 + 6;
			int tempSpeed3 = 0;
			//tempSpeed3 = dis(gen) % 6 + 6;
			int tempSpeed4 = 0;
			//tempSpeed4 = dis(gen) % 6 + 6;

			CObj* pObj = AbstractFactory<CBullet2>::Create();
			pObj->SetPos(m_vWorldPosinPoint);
			pObj->SetDir(m_tINFO.vDir);
			ObjMgr::GetInstance().AddObject(OBJ_BULLET, pObj);
			D3DXVECTOR3 m_vNewDir;
			CObj* pObj1 = AbstractFactory<CBullet2>::Create();
			pObj1->SetPos(m_vWorldPosinPoint);
			D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(20.f));
			D3DXVec3TransformNormal(&m_vNewDir, &m_tINFO.vDir, &matRotZ);
			pObj1->SetDir(m_vNewDir);
			ObjMgr::GetInstance().AddObject(OBJ_BULLET, pObj1);
			D3DXVECTOR3 m_vNewDir2;
			CObj* pObj2 = AbstractFactory<CBullet2>::Create();
			pObj2->SetPos(m_vWorldPosinPoint);
			D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(-20.f));
			D3DXVec3TransformNormal(&m_vNewDir2, &m_tINFO.vDir, &matRotZ);
			pObj2->SetDir(m_vNewDir2);
			ObjMgr::GetInstance().AddObject(OBJ_BULLET, pObj2);
			D3DXVECTOR3 m_vNewDir3;
			CObj* pObj3 = AbstractFactory<CBullet2>::Create();
			pObj3->SetPos(m_vWorldPosinPoint);
			D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(10.f));
			D3DXVec3TransformNormal(&m_vNewDir3, &m_tINFO.vDir, &matRotZ);
			pObj3->SetDir(m_vNewDir3);
			ObjMgr::GetInstance().AddObject(OBJ_BULLET, pObj3);
			D3DXVECTOR3 m_vNewDir4;
			CObj* pObj4 = AbstractFactory<CBullet2>::Create();
			pObj4->SetPos(m_vWorldPosinPoint);
			D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(-10.f));
			D3DXVec3TransformNormal(&m_vNewDir4, &m_tINFO.vDir, &matRotZ);
			pObj4->SetDir(m_vNewDir4);
			ObjMgr::GetInstance().AddObject(OBJ_BULLET, pObj4);
		}
		else
		{
			D3DXMATRIX matRotZ;
			D3DXMatrixRotationZ(&matRotZ, m_fRadian);
			D3DXVec3TransformNormal(&m_tINFO.vDir, &m_tINFO.vLook, &matRotZ);

			CObj* pObj = AbstractFactory<CBullet2>::Create();
			pObj->SetPos(m_vWorldPosinPoint);
			pObj->SetDir(m_tINFO.vDir);
			ObjMgr::GetInstance().AddObject(OBJ_BULLET, pObj);

			m_tINFO.vPos -= m_tINFO.vDir * 2.f;
		}
	}
}

void CPlayer::TakeDamage(int _iDamage)
{
}
