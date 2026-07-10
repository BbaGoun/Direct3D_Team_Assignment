#include "pch.h"
#include "CPlayer.h"
#include "AbstractFactory.h"
#include "ObjMgr.h"
#include "CBullet1.h"
#include "CSummonee.h"

CPlayer::CPlayer()
{
	m_bIsShootGun = false;
	m_bIsTargeted = false;
	m_bIsBooster = false;
	m_bIsSummoner = true;

	m_bAttacked = false;
	m_iAttackDelay = 0;
}

CPlayer::~CPlayer()
{
}

void CPlayer::Initialize()
{
	m_tINFO.vPos = { WINCX * 0.5f, WINCY * 0.5f, 0 };
	m_tINFO.vLook = { 0, -1, 0 };
	m_fSpeed = 8;
	m_fRadian = 0;

	m_vLocalBodyPoints[0] = { -50.f, -50.f, 0 };
	m_vLocalBodyPoints[1] = { 50.f, -50.f, 0 };
	m_vLocalBodyPoints[2] = { 50.f, 50.f, 0 };
	m_vLocalBodyPoints[3] = { -50.f, 50.f, 0 };

	m_vLocalPosinPoint = { 0, -100.f, 0 };

	m_vLocalShotPosinPoint[0] = {0, -100.f, 0};
	m_vLocalShotPosinPoint[1] = {0, -100.f, 0};

	m_vLocalBackPosinPoint[0] = {0, 100.f, 0};
	m_vLocalBackPosinPoint[1] = {0, 100.f, 0};

	m_vLocalSummonerPosinPoint[0] = { 0, 80.f, 0 };
	m_vLocalSummonerPosinPoint[1] = { 0, 80.f, 0 };
	m_vLocalSummonerPosinPoint[2] = { 0, 80.f, 0 };
	m_vLocalSummonerPosinPoint[3] = { 0, 80.f, 0 };
	m_vLocalSummonerPosinPoint[4] = { 0, 80.f, 0 };
	m_vLocalSummonerPosinPoint[5] = { 0, 80.f, 0 };
	m_vLocalSummonerPosinPoint[6] = { 0, 80.f, 0 };
	m_vLocalSummonerPosinPoint[7] = { 0, 80.f, 0 };

	m_iAttackDelay = 30;
}

void CPlayer::Update()
{
	KeyInput();

	if (m_iAttackDelay >= 30)
	{
		if (!m_bAttacked)
		{
			AttackKeyInput();

			m_bAttacked = true;
			m_iAttackDelay = 0;
		}
	}
	else
	{
		m_bAttacked = false;
		++m_iAttackDelay;
	}

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

	int tempDegree = -60;
	for (int i = 0; i < 2; ++i)
	{
		D3DXMATRIX tempRotZ, tempWorld;

		if (i % 2 == 1)
		{
			tempDegree += 60;
		}
		if (i % 2 == 0)
		{
			tempDegree += 30;
		}

		D3DXMatrixRotationZ(&tempRotZ, D3DXToRadian(tempDegree));

		tempWorld = matScale * matRotZ * tempRotZ * matTrans;

		D3DXVec3TransformCoord(&m_vWorldShotPosinPoint[i], &m_vLocalShotPosinPoint[i], &tempWorld);
		D3DXVec3TransformCoord(&m_vWorldBackPosinPoint[i], &m_vLocalBackPosinPoint[i], &tempWorld);
	}

	tempDegree = -60;
	for (int i = 0; i < 8; ++i)
	{
		D3DXMATRIX tempRotZ, tempWorld;

		if (i % 2 == 1)
		{
			tempDegree += 60;
		}
		if (i % 2 == 0)
		{
			tempDegree += 30;
		}

		D3DXMatrixRotationZ(&tempRotZ, D3DXToRadian(tempDegree));

		tempWorld = matScale * matRotZ * tempRotZ * matTrans;

		D3DXVec3TransformCoord(&m_vWorldSummonerPosinPoint[i], &m_vLocalSummonerPosinPoint[i], &tempWorld);
	}
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

	if (m_bIsShootGun)
	{
		MoveToEx(_hDC, m_tINFO.vPos.x, m_tINFO.vPos.y, nullptr);
		LineTo(_hDC, m_vWorldShotPosinPoint[0].x, m_vWorldShotPosinPoint[0].y);

		MoveToEx(_hDC, m_tINFO.vPos.x, m_tINFO.vPos.y, nullptr);
		LineTo(_hDC, m_vWorldShotPosinPoint[1].x, m_vWorldShotPosinPoint[1].y);
	}
	else if (m_bIsBooster)
	{
		MoveToEx(_hDC, m_tINFO.vPos.x, m_tINFO.vPos.y, nullptr);
		LineTo(_hDC, m_vWorldPosinPoint.x, m_vWorldPosinPoint.y);

		MoveToEx(_hDC, m_tINFO.vPos.x, m_tINFO.vPos.y, nullptr);
		LineTo(_hDC, m_vWorldBackPosinPoint[0].x, m_vWorldBackPosinPoint[0].y);

		MoveToEx(_hDC, m_tINFO.vPos.x, m_tINFO.vPos.y, nullptr);
		LineTo(_hDC, m_vWorldBackPosinPoint[1].x, m_vWorldBackPosinPoint[1].y);
	}
	else if (m_bIsSummoner)
	{
		MoveToEx(_hDC, m_tINFO.vPos.x, m_tINFO.vPos.y, nullptr);
		LineTo(_hDC, m_vWorldSummonerPosinPoint[0].x, m_vWorldSummonerPosinPoint[0].y);

		MoveToEx(_hDC, m_tINFO.vPos.x, m_tINFO.vPos.y, nullptr);
		LineTo(_hDC, m_vWorldSummonerPosinPoint[1].x, m_vWorldSummonerPosinPoint[1].y);

		MoveToEx(_hDC, m_tINFO.vPos.x, m_tINFO.vPos.y, nullptr);
		LineTo(_hDC, m_vWorldSummonerPosinPoint[2].x, m_vWorldSummonerPosinPoint[2].y);

		MoveToEx(_hDC, m_tINFO.vPos.x, m_tINFO.vPos.y, nullptr);
		LineTo(_hDC, m_vWorldSummonerPosinPoint[3].x, m_vWorldSummonerPosinPoint[3].y);

		MoveToEx(_hDC, m_tINFO.vPos.x, m_tINFO.vPos.y, nullptr);
		LineTo(_hDC, m_vWorldSummonerPosinPoint[4].x, m_vWorldSummonerPosinPoint[4].y);

		MoveToEx(_hDC, m_tINFO.vPos.x, m_tINFO.vPos.y, nullptr);
		LineTo(_hDC, m_vWorldSummonerPosinPoint[5].x, m_vWorldSummonerPosinPoint[5].y);

		MoveToEx(_hDC, m_tINFO.vPos.x, m_tINFO.vPos.y, nullptr);
		LineTo(_hDC, m_vWorldSummonerPosinPoint[6].x, m_vWorldSummonerPosinPoint[6].y);

		MoveToEx(_hDC, m_tINFO.vPos.x, m_tINFO.vPos.y, nullptr);
		LineTo(_hDC, m_vWorldSummonerPosinPoint[7].x, m_vWorldSummonerPosinPoint[7].y);
	}
	else
	{
		MoveToEx(_hDC, m_tINFO.vPos.x, m_tINFO.vPos.y, nullptr);
		LineTo(_hDC, m_vWorldPosinPoint.x, m_vWorldPosinPoint.y);
	}

	SelectObject(_hDC, hOldPen);
	DeleteObject(hPen);
}

void CPlayer::Release()
{
}

void CPlayer::TakeDamage(int _iDamage)
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
}

void CPlayer::AttackKeyInput()
{
	if (GetAsyncKeyState(VK_LSHIFT))
	{
		if (m_bIsShootGun)
		{
			D3DXMATRIX matRotZ, matRot;
			D3DXMatrixRotationZ(&matRotZ, m_fRadian);
			D3DXVec3TransformNormal(&m_tINFO.vDir, &m_tINFO.vLook, &matRotZ);

			int tempAng = 0;
			tempAng = dis(gen) % 41 - 20;
			int tempAng1 = 0;
			tempAng1 = dis(gen) % 41 - 20;
			int tempAng2 = 0;
			tempAng2 = dis(gen) % 41 - 20;
			int tempAng3 = 0;
			tempAng3 = dis(gen) % 41 - 20;
			int tempAng4 = 0;
			tempAng4 = dis(gen) % 41 - 20;
			int tempAng5 = 0;
			tempAng5 = dis(gen) % 41 - 20;
			int tempAng6 = 0;
			tempAng6 = dis(gen) % 41 - 20;
			int tempAng7 = 0;
			tempAng7 = dis(gen) % 41 - 20;
			int tempAng8 = 0;
			tempAng8 = dis(gen) % 41 - 20;
			int tempAng9 = 0;
			tempAng9 = dis(gen) % 41 - 20;

			int tempSpeed = 0;
			tempSpeed = dis(gen) % 6 + 6;
			int tempSpeed1 = 0;
			tempSpeed1 = dis(gen) % 6 + 6;
			int tempSpeed2 = 0;
			tempSpeed2 = dis(gen) % 6 + 6;
			int tempSpeed3 = 0;
			tempSpeed3 = dis(gen) % 6 + 6;
			int tempSpeed4 = 0;
			tempSpeed4 = dis(gen) % 6 + 6;
			int tempSpeed5 = 0;
			tempSpeed5 = dis(gen) % 6 + 6;
			int tempSpeed6 = 0;
			tempSpeed6 = dis(gen) % 6 + 6;
			int tempSpeed7 = 0;
			tempSpeed7 = dis(gen) % 6 + 6;
			int tempSpeed8 = 0;
			tempSpeed8 = dis(gen) % 6 + 6;
			int tempSpeed9 = 0;
			tempSpeed9 = dis(gen) % 6 + 6;

			D3DXVECTOR3 m_vNewDir;
			CObj* pObj = AbstractFactory<CBullet1>::Create();
			pObj->SetPos(m_vWorldPosinPoint);
			D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(tempAng));
			D3DXVec3TransformNormal(&m_vNewDir, &m_tINFO.vDir, &matRotZ);
			pObj->SetDir(m_tINFO.vDir);
			pObj->SetSpeed(tempSpeed);
			ObjMgr::GetInstance().AddObject(OBJ_BULLET, pObj);

			D3DXVECTOR3 m_vNewDir1;
			CObj* pObj1 = AbstractFactory<CBullet1>::Create();
			pObj1->SetPos(m_vWorldPosinPoint);
			D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(tempAng1));
			D3DXVec3TransformNormal(&m_vNewDir1, &m_tINFO.vDir, &matRotZ);
			pObj1->SetDir(m_vNewDir1);
			pObj1->SetSpeed(tempSpeed1);
			ObjMgr::GetInstance().AddObject(OBJ_BULLET, pObj1);

			D3DXVECTOR3 m_vNewDir2;
			CObj* pObj2 = AbstractFactory<CBullet1>::Create();
			pObj2->SetPos(m_vWorldPosinPoint);
			D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(tempAng2));
			D3DXVec3TransformNormal(&m_vNewDir2, &m_tINFO.vDir, &matRotZ);
			pObj2->SetDir(m_vNewDir2);
			pObj2->SetSpeed(tempSpeed2);
			ObjMgr::GetInstance().AddObject(OBJ_BULLET, pObj2);

			D3DXVECTOR3 m_vNewDir3;
			CObj* pObj3 = AbstractFactory<CBullet1>::Create();
			pObj3->SetPos(m_vWorldPosinPoint);
			D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(tempAng3));
			D3DXVec3TransformNormal(&m_vNewDir3, &m_tINFO.vDir, &matRotZ);
			pObj3->SetDir(m_vNewDir3);
			pObj3->SetSpeed(tempSpeed3);
			ObjMgr::GetInstance().AddObject(OBJ_BULLET, pObj3);

			D3DXVECTOR3 m_vNewDir4;
			CObj* pObj4 = AbstractFactory<CBullet1>::Create();
			pObj4->SetPos(m_vWorldPosinPoint);
			D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(tempAng4));
			D3DXVec3TransformNormal(&m_vNewDir4, &m_tINFO.vDir, &matRotZ);
			pObj4->SetDir(m_vNewDir4);
			pObj4->SetSpeed(tempSpeed4);
			ObjMgr::GetInstance().AddObject(OBJ_BULLET, pObj4);

			D3DXVECTOR3 m_vNewDir5;
			CObj* pObj5 = AbstractFactory<CBullet1>::Create();
			pObj5->SetPos(m_vWorldPosinPoint);
			D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(tempAng5));
			D3DXVec3TransformNormal(&m_vNewDir5, &m_tINFO.vDir, &matRotZ);
			pObj5->SetDir(m_vNewDir5);
			pObj5->SetSpeed(tempSpeed5);
			ObjMgr::GetInstance().AddObject(OBJ_BULLET, pObj5);

			D3DXVECTOR3 m_vNewDir6;
			CObj* pObj6 = AbstractFactory<CBullet1>::Create();
			pObj6->SetPos(m_vWorldPosinPoint);
			D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(tempAng6));
			D3DXVec3TransformNormal(&m_vNewDir6, &m_tINFO.vDir, &matRotZ);
			pObj6->SetDir(m_vNewDir6);
			pObj6->SetSpeed(tempSpeed6);
			ObjMgr::GetInstance().AddObject(OBJ_BULLET, pObj6);

			D3DXVECTOR3 m_vNewDir7;
			CObj* pObj7 = AbstractFactory<CBullet1>::Create();
			pObj7->SetPos(m_vWorldPosinPoint);
			D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(tempAng7));
			D3DXVec3TransformNormal(&m_vNewDir7, &m_tINFO.vDir, &matRotZ);
			pObj4->SetDir(m_vNewDir7);
			pObj4->SetSpeed(tempSpeed7);
			ObjMgr::GetInstance().AddObject(OBJ_BULLET, pObj7);

			D3DXVECTOR3 m_vNewDir8;
			CObj* pObj8 = AbstractFactory<CBullet1>::Create();
			pObj8->SetPos(m_vWorldPosinPoint);
			D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(tempAng8));
			D3DXVec3TransformNormal(&m_vNewDir8, &m_tINFO.vDir, &matRotZ);
			pObj8->SetDir(m_vNewDir8);
			pObj8->SetSpeed(tempSpeed8);
			ObjMgr::GetInstance().AddObject(OBJ_BULLET, pObj8);

			D3DXVECTOR3 m_vNewDir9;
			CObj* pObj9 = AbstractFactory<CBullet1>::Create();
			pObj9->SetPos(m_vWorldPosinPoint);
			D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(tempAng9));
			D3DXVec3TransformNormal(&m_vNewDir9, &m_tINFO.vDir, &matRotZ);
			pObj9->SetDir(m_vNewDir9);
			pObj9->SetSpeed(tempSpeed9);
			ObjMgr::GetInstance().AddObject(OBJ_BULLET, pObj9);
		}
		else if (m_bIsTargeted)
		{
			
		}
		else if (m_bIsBooster)
		{
			D3DXMATRIX matRotZ;
			D3DXMatrixRotationZ(&matRotZ, m_fRadian);
			D3DXVec3TransformNormal(&m_tINFO.vDir, &m_tINFO.vLook, &matRotZ);

			CObj* pObj = AbstractFactory<CBullet1>::Create();
			pObj->SetPos(m_vWorldPosinPoint);
			pObj->SetDir(m_tINFO.vDir);
			ObjMgr::GetInstance().AddObject(OBJ_BULLET, pObj);

			D3DXVECTOR3 m_vNewDir1;
			CObj* pObj1 = AbstractFactory<CBullet1>::Create();
			pObj1->SetPos(m_vWorldBackPosinPoint[0]);
			D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(150));
			D3DXVec3TransformNormal(&m_vNewDir1, &m_tINFO.vDir, &matRotZ);
			pObj1->SetDir(m_vNewDir1);
			ObjMgr::GetInstance().AddObject(OBJ_BULLET, pObj1);

			D3DXVECTOR3 m_vNewDir2;
			CObj* pObj2 = AbstractFactory<CBullet1>::Create();
			pObj2->SetPos(m_vWorldBackPosinPoint[1]);
			D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(210));
			D3DXVec3TransformNormal(&m_vNewDir2, &m_tINFO.vDir, &matRotZ);
			pObj2->SetDir(m_vNewDir2);
			ObjMgr::GetInstance().AddObject(OBJ_BULLET, pObj2);

			m_tINFO.vPos += m_tINFO.vDir * 4.f;
		}
		else if (m_bIsSummoner)
		{
			D3DXMATRIX matRotZ;
			D3DXMatrixRotationZ(&matRotZ, m_fRadian);
			D3DXVec3TransformNormal(&m_tINFO.vDir, &m_tINFO.vLook, &matRotZ);

			CObj* pObj = AbstractFactory<CSummonee>::Create();
			pObj->SetPos(m_tINFO.vPos);
			pObj->SetDir(m_tINFO.vDir);
			ObjMgr::GetInstance().AddObject(OBJ_BULLET, pObj);
		}
		else
		{
			D3DXMATRIX matRotZ;
			D3DXMatrixRotationZ(&matRotZ, m_fRadian);
			D3DXVec3TransformNormal(&m_tINFO.vDir, &m_tINFO.vLook, &matRotZ);

			CObj* pObj = AbstractFactory<CBullet1>::Create();
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
