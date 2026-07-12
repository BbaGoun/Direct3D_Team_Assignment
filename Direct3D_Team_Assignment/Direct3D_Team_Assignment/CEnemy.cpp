#include "pch.h"
#include "CEnemy.h"
#include "CameraMgr.h"
#include "TimeMgr.h"

CEnemy::CEnemy()
{
	//모든 각주는 작업자 본인이 코드의 구조를 까먹지 않을수 있도록 돕는 용도로 작성됨
}

CEnemy::~CEnemy()
{
	Release();
}

void CEnemy::Initialize()
{
	m_eCurTankID = TANK_NOMAL;
	m_eNextTankID = TANK_END;


	m_fDelayTime = 0;

	m_tINFO.vPos = { WINCX * 0.5f, WINCY * 0.5f, 0 };
	m_tINFO.vLook = { 0, -1, 0 };

	m_vLocalVec.resize(4);
	m_vWorldVec.resize(4);
	m_vViewVec.resize(4);
	m_vProjVec.resize(4);

	m_vLocalVec[0] = { -50.f, -50.f, 0 };
	m_vLocalVec[1] = { 50.f, -50.f, 0 };
	m_vLocalVec[2] = { 50.f, 50.f, 0 };
	m_vLocalVec[3] = { -50.f, 50.f, 0 };

	m_vLocalPosinPoint = { 0.f, -100.f, 0 };

	m_fRadian = 0;

	//가속도 세팅
	m_fSpeed = 8;

	m_fCurrentSpeed = 0.f;
	m_fAccel = 0.4f;
	m_fDecel = 0.2f;

	m_bMove = false;

	m_iDropExp = 5;
	m_pTankStat = new CTankNomal;
	m_pTankStat->Initialize(this);
}

void CEnemy::Update()
{
	UpdateTimers();
	if (m_bDead)
		return;

	KeyInput();

	//월드 행렬에 구성 요소 적용. 크기/자전/이동/공전/위치(부모)<- 순서 잊지 말것!
	if (m_bMove == true)
		m_bMove = false;

	D3DXMATRIX matScale, matRotZ, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, 1, 1, 1);
	D3DXMatrixRotationZ(&matRotZ, m_fRadian);
	D3DXMatrixTranslation(&matTrans, m_tINFO.vPos.x, m_tINFO.vPos.y, m_tINFO.vPos.z);

	D3DXMatrixIdentity(&matWorld);
	matWorld = matScale * matRotZ * matTrans;//공전
	matWorld = matScale * matRotZ * matTrans;//공전
	for (int i = 0; i < 4; ++i) {
		D3DXVec3TransformCoord(&m_vWorldVec[i], &m_vLocalVec[i], &matWorld);
	}
	D3DXVec3TransformCoord(&m_vWorldPosinPoint, &m_vLocalPosinPoint, &matWorld);

	D3DXVec3TransformNormal(&m_tINFO.vDir, &m_tINFO.vLook, &matRotZ);
	m_tINFO.vPos += m_tINFO.vDir * m_fCurrentSpeed;

	// 월드 -> 뷰 -> 투영 스페이스 변환
	D3DXMATRIX matView = CameraMgr::GetInstance().GetViewMat();
	D3DXMATRIX matProj = CameraMgr::GetInstance().GetProjMat();

	for (int i = 0; i < m_vWorldVec.size(); ++i) {
		D3DXVec3TransformCoord(&m_vViewVec[i], &m_vWorldVec[i], &matView);
		// Z Division 이 행렬에 포함되어 있음.
		D3DXVec3TransformCoord(&m_vProjVec[i], &m_vViewVec[i], &matProj);
		m_vProjVec[i] += {640, 360, 0};
	}

	if (m_pTankStat != nullptr)
		m_pTankStat->PosUpdate(this);

}

void CEnemy::LateUpdate()
{
	if (m_bDead)
		return;

	if (m_fDelayTime >=0)
	{
		--m_fDelayTime;
		if (m_fDelayTime <=0)
			m_fDelayTime == 0;
	}
	//getdelta타임 
	DecelerationCurrentSpeed();
}

void CEnemy::Render(HDC _hDC)
{
	if (m_bDead)
		return;

	// 정점이 없는 원의 경우 크기 조절을 위한 값
	float projScale = CameraMgr::GetInstance().GetProjScale();

	//색상 지정
	HPEN hPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
	HPEN hOldPen = (HPEN)SelectObject(_hDC, hPen);

	//몸통 렌더.
	MoveToEx(_hDC, m_vProjVec[0].x, m_vProjVec[0].y, nullptr);
	for (int i = 1; i <= 4; ++i) {
		LineTo(_hDC, m_vProjVec[i % 4].x, m_vProjVec[i % 4].y);
	}

	////정면 구분을 위한 원형 도형 렌더.
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

	////포신 렌더
	//MoveToEx(_hDC, m_tINFO.vPos.x, m_tINFO.vPos.y, nullptr);
	//LineTo(_hDC, m_vWorldPosinPoint.x, m_vWorldPosinPoint.y);

	//펜 할당 해제
	SelectObject(_hDC, hOldPen);
	DeleteObject(hPen);

}

void CEnemy::Release()
{
	Safe_Delete(m_pTankStat);
}

void CEnemy::ReUpdateWorldVertex()
{
	D3DXMATRIX matScale, matRotZ, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, 1, 1, 1);
	D3DXMatrixRotationZ(&matRotZ, m_fRadian);
	D3DXMatrixTranslation(&matTrans, m_tINFO.vPos.x, m_tINFO.vPos.y, m_tINFO.vPos.z);

	//월드 행렬에 구성 요소 적용. 크기/자전/이동/공전/위치(부모)<- 순서 잊지 말것!
	D3DXMatrixIdentity(&matWorld);
	matWorld = matScale * matRotZ * matTrans;
	for (int i = 0; i < m_vLocalVec.size(); ++i) {
		D3DXVec3TransformCoord(&m_vWorldVec[i], &m_vLocalVec[i], &matWorld);
	}
	D3DXVec3TransformCoord(&m_vWorldPosinPoint, &m_vLocalPosinPoint, &matWorld);
}

void CEnemy::DecelerationCurrentSpeed()
{
	if (!m_bMove) {
		if (m_fCurrentSpeed > 0.f) {
			m_fCurrentSpeed -= m_fDecel;
			if (m_fCurrentSpeed < 0.f) m_fCurrentSpeed = 0.f;
		}
		else if (m_fCurrentSpeed < 0.f) {
			m_fCurrentSpeed += m_fDecel;
			if (m_fCurrentSpeed > 0.f) m_fCurrentSpeed = 0.f;
		}
	}
}

void CEnemy::Accelerate(float _fFactor)
{
	m_fCurrentSpeed += m_fAccel * _fFactor;
	if (m_fCurrentSpeed > m_fSpeed)  m_fCurrentSpeed = m_fSpeed;
	if (m_fCurrentSpeed < -m_fSpeed) m_fCurrentSpeed = -m_fSpeed;
	m_bMove = true;
}

void CEnemy::KeyInput()
{
	m_tINFO.vDir = { 0, 0, 0 };

	if (GetAsyncKeyState(VK_LEFT)) {
		m_fRadian += D3DXToRadian(-3);
	}
	if (GetAsyncKeyState(VK_RIGHT)) {
		m_fRadian += D3DXToRadian(3);
	}
	if (GetAsyncKeyState(VK_UP)) {
		Accelerate(1.f);
	}
	else if (GetAsyncKeyState(VK_DOWN)) {
		Accelerate(-1.f);
	}

	if (GetAsyncKeyState(VK_RSHIFT)) {
		if (m_fDelayTime <= 0)
		{
			m_pTankStat->Fire(this);
		}
	}
	if (GetAsyncKeyState('P')) {
		TANKID eNextID;
		eNextID = (TANKID)((m_eCurTankID)+1 % TANK_END);
		ChaingeTankType(eNextID);
	}
	if (GetAsyncKeyState('O')) {
		TANKID eBeforID;
		eBeforID = (TANKID)((m_eCurTankID)+TANK_END - 1 % TANK_END);
		ChaingeTankType(eBeforID);
	}
}

void CEnemy::ChaingeTankType(TANKID _eID)
{
	m_eNextTankID = _eID;
	if (m_eNextTankID != m_eCurTankID)
	{
		Safe_Delete(m_pTankStat);
		switch (m_eNextTankID)
		{
		case TANK_NOMAL:
			m_pTankStat = new CTankNomal;
			break;
		case TANK_SHOTGUN:
			m_pTankStat = new CTankShotGun;
			break;
		case TANK_GUIDED:
			m_pTankStat = new CTankGuided;
			break;
		case TANK_BOOSTER:
			m_pTankStat = new CTankBooster;
			break;
		case TANK_SOMMONER:
			m_pTankStat = new CTankSommoner;
			break;
		}
		m_eCurTankID = m_eNextTankID;
		m_pTankStat->Initialize(this);
	}
}

void CEnemy::UpdateTimers()
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
			m_tINFO.vPos = { 1920, 1080, 0 };
		}
	}
}

void CEnemy::Dead()
{
	m_bRespawn = true;
	m_bDead = true;
	m_fRespawnTimer = 3.f;
}