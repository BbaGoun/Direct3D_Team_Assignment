#include "pch.h"
#include "CPlayer.h"
#include "AbstractFactory.h"
#include "ObjMgr.h"
#include "CBullet1.h"
#include "CSummonee.h"
#include "CameraMgr.h"
#include "TimeMgr.h"
#include "CTargetBullet2.h"

CPlayer::CPlayer()
{
	m_bIsShootGun = false;
	m_bIsTargeted = false;
	m_bIsBooster = false;
	m_bIsSummoner = false;

	m_bAttacked = false;
	m_iAttackDelay = 0;

	m_iSwitchDelay = 0;

	m_fGoBack = 0;
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

	m_vLocalVec.resize(4);
	m_vWorldVec.resize(4);
	m_vViewVec.resize(4);
	m_vProjVec.resize(4);

	m_vLocalVec[0] = { -50.f, -50.f, 0 };
	m_vLocalVec[1] = { 50.f, -50.f, 0 };
	m_vLocalVec[2] = { 50.f, 50.f, 0 };
	m_vLocalVec[3] = { -50.f, 50.f, 0 };

	m_vLocalPosinVec = { 0, -100.f, 0 };
	m_fRadian = 0;

	m_vLocalShotPosinVec[0] = {0, -100.f, 0};
	m_vLocalShotPosinVec[1] = {0, -100.f, 0};

	m_vLocalBackPosinVec[0] = {0, 100.f, 0};
	m_vLocalBackPosinVec[1] = {0, 100.f, 0};

	m_vLocalSummonerPosinVec[0] = { 0, 80.f, 0 };
	m_vLocalSummonerPosinVec[1] = { 0, 80.f, 0 };
	m_vLocalSummonerPosinVec[2] = { 0, 80.f, 0 };
	m_vLocalSummonerPosinVec[3] = { 0, 80.f, 0 };
	m_vLocalSummonerPosinVec[4] = { 0, 80.f, 0 };
	m_vLocalSummonerPosinVec[5] = { 0, 80.f, 0 };
	m_vLocalSummonerPosinVec[6] = { 0, 80.f, 0 };
	m_vLocalSummonerPosinVec[7] = { 0, 80.f, 0 };

	m_iMaxExp = 100;
	m_iCurrentExp = 0;
	m_iLevel = 1;
	m_iAttackDelay = 30;
	m_iDropExp = 5;
}

void CPlayer::Update()
{
	UpdateTimers();
	if (m_bDead)
		return;

	if (m_iLevel == 2)
	{
		m_fGoBack = 0;
		m_bIsTargeted = true;
	}
	else if (m_bIsTargeted && m_iLevel == 3)
	{
		m_fGoBack = 0;
		m_bIsTargeted = false;
		m_bIsBooster = true;
	}
	else if (m_bIsBooster && m_iLevel == 4)
	{
		m_fGoBack = 0;
		m_bIsBooster = false;
		m_bIsShootGun = true;
	}
	else if (m_bIsShootGun && m_iLevel == 5)
	{
		m_fGoBack = 0;
		//m_bIsShootGun = false;
		//m_bIsSummoner = true;
	}
	/*else if (m_bIsSummoner && m_iLevel == 6)
	{
		m_fGoBack = 0;
	}*/

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

	if (m_fGoBack != 0 && !m_bIsBooster)
	{
		m_fGoBack += 0.5;
	}
	else if (m_fGoBack != 0 && m_bIsBooster)
	{
		m_fGoBack -= 0.5;
	}
	else if (m_fGoBack >= 0 || m_fGoBack <= 0)
	{
		m_fGoBack = 0;
	}

	if (GetAsyncKeyState('S'))
	{
		m_tINFO.vPos -= m_tINFO.vDir * (m_fGoBack * 0.5f);
	}
	else
	{
		m_tINFO.vPos += m_tINFO.vDir * m_fGoBack;
	}

	D3DXMATRIX matScale, matRotZ, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, 1, 1, 1);
	D3DXMatrixRotationZ(&matRotZ, m_fRadian);
	D3DXMatrixTranslation(&matTrans, m_tINFO.vPos.x, m_tINFO.vPos.y, m_tINFO.vPos.z);

	D3DXMatrixIdentity(&matWorld);
	matWorld = matScale * matRotZ * matTrans;

	for (int i = 0; i < 4; ++i) {
		D3DXVec3TransformCoord(&m_vWorldVec[i], &m_vLocalVec[i], &matWorld);
	}

	D3DXVec3TransformCoord(&m_vWorldPosinVec, &m_vLocalPosinVec, &matWorld);

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

		D3DXVec3TransformCoord(&m_vWorldShotPosinVec[i], &m_vLocalShotPosinVec[i], &tempWorld);
		D3DXVec3TransformCoord(&m_vWorldBackPosinVec[i], &m_vLocalBackPosinVec[i], &tempWorld);
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

		D3DXVec3TransformCoord(&m_vWorldSummonerPosinVec[i], &m_vLocalSummonerPosinVec[i], &tempWorld);
	}

	// ���� -> �� -> ���� �����̽� ��ȯ
	D3DXMATRIX matView = CameraMgr::GetInstance().GetViewMat();

	D3DXMATRIX matProj = CameraMgr::GetInstance().GetProjMat();

	for (int i = 0; i < m_vWorldVec.size(); ++i) {
		D3DXVec3TransformCoord(&m_vViewVec[i], &m_vWorldVec[i], &matView);
		// Z Division �� ��Ŀ� ���ԵǾ� ����.
		D3DXVec3TransformCoord(&m_vProjVec[i], &m_vViewVec[i], &matProj);
		m_vProjVec[i] = CameraMgr::GetInstance().ProjToScreen(m_vProjVec[i]);
	}

	D3DXVec3TransformCoord(&m_vViewPosinVec, &m_vWorldPosinVec, &matView);
	D3DXVec3TransformCoord(&m_vProjPosinVec, &m_vViewPosinVec, &matProj);
	m_vProjPosinVec = CameraMgr::GetInstance().ProjToScreen(m_vProjPosinVec);

	for (int i = 0; i < 2; ++i)
	{
		D3DXVec3TransformCoord(&m_vViewShotPosinVec[i], &m_vWorldShotPosinVec[i], &matView);
		D3DXVec3TransformCoord(&m_vProjShotPosinVec[i], &m_vViewShotPosinVec[i], &matProj);
		m_vProjShotPosinVec[i] = CameraMgr::GetInstance().ProjToScreen(m_vProjShotPosinVec[i]);

		D3DXVec3TransformCoord(&m_vViewBackPosinVec[i], &m_vWorldBackPosinVec[i], &matView);
		D3DXVec3TransformCoord(&m_vProjBackPosinVec[i], &m_vViewBackPosinVec[i], &matProj);
		m_vProjBackPosinVec[i] = CameraMgr::GetInstance().ProjToScreen(m_vProjBackPosinVec[i]);
	}

	for (int i = 0; i < 8; ++i)
	{
		D3DXVec3TransformCoord(&m_vViewSummonerPosinVec[i], &m_vWorldSummonerPosinVec[i], &matView);
		D3DXVec3TransformCoord(&m_vProjSummonerPosinVec[i], &m_vViewSummonerPosinVec[i], &matProj);
		m_vProjSummonerPosinVec[i] = CameraMgr::GetInstance().ProjToScreen(m_vProjSummonerPosinVec[i]);
	}
}

void CPlayer::LateUpdate()
{
	if (m_bDead)
		return;
}

void CPlayer::Render(HDC _hDC)
{
	if (m_bDead)
		return;

	// ������ ���� ���� ��� ũ�� ������ ���� ��
	float projScale = CameraMgr::GetInstance().GetProjScale();

	HPEN hPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 255));
	HPEN hOldPen = (HPEN)SelectObject(_hDC, hPen);

	MoveToEx(_hDC, m_vProjVec[0].x, m_vProjVec[0].y, nullptr);
	for (int i = 1; i <= 4; ++i) {
		LineTo(_hDC, m_vProjVec[i % 4].x, m_vProjVec[i % 4].y);
	}

	Ellipse(_hDC,
		m_vProjVec[0].x - 5 * projScale,
		m_vProjVec[0].y - 5 * projScale,
		m_vProjVec[0].x + 5 * projScale,
		m_vProjVec[0].y + 5 * projScale);

	Ellipse(_hDC,
		m_vProjVec[1].x - 5 * projScale,
		m_vProjVec[1].y - 5 * projScale,
		m_vProjVec[1].x + 5 * projScale,
		m_vProjVec[1].y + 5 * projScale);

	D3DXVECTOR3 projPosinStart = m_vProjVec[0] + ((m_vProjVec[1] - m_vProjVec[0]) * 0.5f) + ((m_vProjVec[2] - m_vProjVec[1]) * 0.5f);

	if (m_bIsShootGun)
	{
		MoveToEx(_hDC, projPosinStart.x, projPosinStart.y, nullptr);
		LineTo(_hDC, m_vProjShotPosinVec[0].x, m_vProjShotPosinVec[0].y);

		MoveToEx(_hDC, projPosinStart.x, projPosinStart.y, nullptr);
		LineTo(_hDC, m_vProjShotPosinVec[1].x, m_vProjShotPosinVec[1].y);
	}
	else if (m_bIsBooster)
	{
		MoveToEx(_hDC, projPosinStart.x, projPosinStart.y, nullptr);
		LineTo(_hDC, m_vProjPosinVec.x, m_vProjPosinVec.y);

		MoveToEx(_hDC, projPosinStart.x, projPosinStart.y, nullptr);
		LineTo(_hDC, m_vProjBackPosinVec[0].x, m_vProjBackPosinVec[0].y);

		MoveToEx(_hDC, projPosinStart.x, projPosinStart.y, nullptr);
		LineTo(_hDC, m_vProjBackPosinVec[1].x, m_vProjBackPosinVec[1].y);
	}
	else if (m_bIsSummoner)
	{
		MoveToEx(_hDC, projPosinStart.x, projPosinStart.y, nullptr);
		LineTo(_hDC, m_vProjSummonerPosinVec[0].x, m_vProjSummonerPosinVec[0].y);

		MoveToEx(_hDC, projPosinStart.x, projPosinStart.y, nullptr);
		LineTo(_hDC, m_vProjSummonerPosinVec[1].x, m_vProjSummonerPosinVec[1].y);

		MoveToEx(_hDC, projPosinStart.x, projPosinStart.y, nullptr);
		LineTo(_hDC, m_vProjSummonerPosinVec[2].x, m_vProjSummonerPosinVec[2].y);

		MoveToEx(_hDC, projPosinStart.x, projPosinStart.y, nullptr);
		LineTo(_hDC, m_vProjSummonerPosinVec[3].x, m_vProjSummonerPosinVec[3].y);

		MoveToEx(_hDC, projPosinStart.x, projPosinStart.y, nullptr);
		LineTo(_hDC, m_vProjSummonerPosinVec[4].x, m_vProjSummonerPosinVec[4].y);

		MoveToEx(_hDC, projPosinStart.x, projPosinStart.y, nullptr);
		LineTo(_hDC, m_vProjSummonerPosinVec[5].x, m_vProjSummonerPosinVec[5].y);

		MoveToEx(_hDC, projPosinStart.x, projPosinStart.y, nullptr);
		LineTo(_hDC, m_vProjSummonerPosinVec[6].x, m_vProjSummonerPosinVec[6].y);

		MoveToEx(_hDC, projPosinStart.x, projPosinStart.y, nullptr);
		LineTo(_hDC, m_vProjSummonerPosinVec[7].x, m_vProjSummonerPosinVec[7].y);
	}
	else
	{
		MoveToEx(_hDC, projPosinStart.x, projPosinStart.y, nullptr);
		LineTo(_hDC, m_vProjPosinVec.x, m_vProjPosinVec.y);
	}

	SelectObject(_hDC, hOldPen);
	DeleteObject(hPen);
}

void CPlayer::Release()
{
}

void CPlayer::ReUpdateWorldVertex()
{
	D3DXMATRIX matScale, matRotZ, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, 1, 1, 1);
	D3DXMatrixRotationZ(&matRotZ, m_fRadian);
	D3DXMatrixTranslation(&matTrans, m_tINFO.vPos.x, m_tINFO.vPos.y, m_tINFO.vPos.z);

	D3DXMatrixIdentity(&matWorld);
	matWorld = matScale * matRotZ * matTrans;

	for (int i = 0; i < 4; ++i) {
		D3DXVec3TransformCoord(&m_vWorldVec[i], &m_vLocalVec[i], &matWorld);
	}

	D3DXVec3TransformCoord(&m_vWorldPosinVec, &m_vLocalPosinVec, &matWorld);
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
		D3DXMatrixRotationZ(&matRotZ, PI+m_fRadian);
		D3DXVec3TransformNormal(&m_tINFO.vDir, &m_tINFO.vLook, &matRotZ);

		m_tINFO.vPos += m_tINFO.vDir * m_fSpeed;
	}
}

void CPlayer::AttackKeyInput()
{
	if (GetAsyncKeyState(VK_LSHIFT))
	{
		if (m_bIsShootGun)
		{
			m_fGoBack = 0;
			D3DXMATRIX matRotZ, matRot;
			D3DXMatrixRotationZ(&matRotZ, m_fRadian);
			D3DXVec3TransformNormal(&m_tINFO.vDir, &m_tINFO.vLook, &matRotZ);

			for (int i = 0; i < 10; ++i)
			{
				int tempAng = dis(gen) % 41 - 20;
				int tempSpeed = dis(gen) % 6 + 6;

				D3DXVECTOR3 m_vNewDir;
				CObj* pObj = AbstractFactory<CBullet1>::Create();
				pObj->SetPos(m_vWorldPosinVec);
				D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(tempAng));
				D3DXVec3TransformNormal(&m_vNewDir, &m_tINFO.vDir, &matRotZ);
				pObj->SetDir(m_vNewDir);
				pObj->SetSpeed(tempSpeed);
				pObj->SetParent(this);
				ObjMgr::GetInstance().AddObject(OBJ_BULLET, pObj);
			}

			m_fGoBack = -20.f;
		}
		else if (m_bIsTargeted)
		{
			D3DXMATRIX matRotZ;
			D3DXMatrixRotationZ(&matRotZ, m_fRadian);
			D3DXVec3TransformNormal(&m_tINFO.vDir, &m_tINFO.vLook, &matRotZ);

			CObj* pObj = AbstractFactory<CTargetBullet2>::Create();
			pObj->SetPos(m_vWorldPosinVec);
			pObj->SetDir(m_tINFO.vDir);
			pObj->SetParent(this);
			static_cast<CTargetBullet2*>(pObj)->SetTarget(m_pParent);
			ObjMgr::GetInstance().AddObject(OBJ_BULLET, pObj);

			m_fGoBack = -10.f;
		}
		else if (m_bIsBooster)
		{
			D3DXMATRIX matRotZ;
			D3DXMatrixRotationZ(&matRotZ, m_fRadian);
			D3DXVec3TransformNormal(&m_tINFO.vDir, &m_tINFO.vLook, &matRotZ);

			CObj* pObj = AbstractFactory<CBullet1>::Create();
			pObj->SetPos(m_vWorldPosinVec);
			pObj->SetDir(m_tINFO.vDir);
			pObj->SetParent(this);
			ObjMgr::GetInstance().AddObject(OBJ_BULLET, pObj);

			D3DXVECTOR3 m_vNewDir1;
			CObj* pObj1 = AbstractFactory<CBullet1>::Create();
			pObj1->SetPos(m_vWorldBackPosinVec[0]);
			D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(150));
			D3DXVec3TransformNormal(&m_vNewDir1, &m_tINFO.vDir, &matRotZ);
			pObj1->SetDir(m_vNewDir1);
			pObj1->SetParent(this);
			ObjMgr::GetInstance().AddObject(OBJ_BULLET, pObj1);

			D3DXVECTOR3 m_vNewDir2;
			CObj* pObj2 = AbstractFactory<CBullet1>::Create();
			pObj2->SetPos(m_vWorldBackPosinVec[1]);
			D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(210));
			D3DXVec3TransformNormal(&m_vNewDir2, &m_tINFO.vDir, &matRotZ);
			pObj2->SetDir(m_vNewDir2);
			pObj2->SetParent(this);
			ObjMgr::GetInstance().AddObject(OBJ_BULLET, pObj2);

			m_fGoBack = 10.f;
		}
		else if (m_bIsSummoner)
		{
			D3DXMATRIX matRotZ;
			D3DXMatrixRotationZ(&matRotZ, m_fRadian);
			D3DXVec3TransformNormal(&m_tINFO.vDir, &m_tINFO.vLook, &matRotZ);

			CObj* pObj = AbstractFactory<CSummonee>::Create();
			pObj->SetPos(m_tINFO.vPos);
			pObj->SetDir(m_tINFO.vDir);
			pObj->SetParent(this);
			ObjMgr::GetInstance().AddObject(OBJ_BULLET, pObj);
		}
		else
		{
			D3DXMATRIX matRotZ;
			D3DXMatrixRotationZ(&matRotZ, m_fRadian);
			D3DXVec3TransformNormal(&m_tINFO.vDir, &m_tINFO.vLook, &matRotZ);

			CObj* pObj = AbstractFactory<CBullet1>::Create();
			pObj->SetPos(m_vWorldPosinVec);
			pObj->SetDir(m_tINFO.vDir);
			pObj->SetParent(this);
			ObjMgr::GetInstance().AddObject(OBJ_BULLET, pObj);

			m_fGoBack = -10.f;
		}
	}
}

void CPlayer::UpdateTimers()
{
	m_fInvincibleTimer = max(0.f, m_fInvincibleTimer - TimeMgr::GetInstance().GetDeltaTime());
	if (m_bRespawn) {
		m_fRespawnTimer = max(0.f, m_fRespawnTimer - TimeMgr::GetInstance().GetDeltaTime());
		if (m_fRespawnTimer <= 0) {
			m_bRespawn = false;
			m_bDead = false;
			m_iHP = m_iMaxHP;
			m_iCurrentExp = 0;
			m_iMaxExp = 3;
			m_iLevel = 1;
			m_tINFO.vPos = { 640, 360, 0 };
		}
	}
}

void CPlayer::Dead()
{
	m_bRespawn = true;
	m_bDead = true;
	m_fRespawnTimer = 3.f;
}
