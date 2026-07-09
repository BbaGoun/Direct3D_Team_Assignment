#include "pch.h"
#include "CEnemy.h"

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

	m_pTankStat = new CTankNomal;

	m_iEXP = 0;
	m_iMaxEXP = 0;
	m_iLevel = 1;

	//오브젝트의 월드 스폰 지점을 초기화 (vPos)
	m_tINFO.vPos = { WINCX * 0.5f, WINCY * 0.5f, 0 };
	//오브젝트의 로컬 방향값을 초기화 합니다.
	m_tINFO.vLook = { 0, -1, 0 };
	//각종 움직임을 적용할 스피드값을 초기화
	m_fSpeed = 8;

	//사각형 형태를 렌더하기 위해 각 포인트를 지정했음 (로컬). 현재 오브젝트의 중점은 정 중앙으로 지정되어 있는 상태.
	//로컬 스케일 값을 따로 저장해 변환하는것을 고려중.
	m_vLocalBodyPoints[0] = { -50.f, -50.f, 0 };
	m_vLocalBodyPoints[1] = { 50.f, -50.f, 0 };
	m_vLocalBodyPoints[2] = { 50.f, 50.f, 0 };
	m_vLocalBodyPoints[3] = { -50.f, 50.f, 0 };

	m_vLocalPosinPoint = { 0.f, -100.f, 0 };

	//최초 생성시 방향값은 0.
	m_fRadian = 0;

}

void CEnemy::Update()
{
	KeyInput();

	D3DXMATRIX matScale, matRotZ, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, 1, 1, 1);
	D3DXMatrixRotationZ(&matRotZ, m_fRadian);
	D3DXMatrixTranslation(&matTrans, m_tINFO.vPos.x, m_tINFO.vPos.y, m_tINFO.vPos.z);

	//월드 행렬에 구성 요소 적용. 크기/자전/이동/공전/위치(부모)<- 순서 잊지 말것!
	D3DXMatrixIdentity(&matWorld);
	matWorld = matScale * matRotZ * matTrans;
	for (int i = 0; i < 4; ++i) {
		D3DXVec3TransformCoord(&m_vWorldBodyPoints[i], &m_vLocalBodyPoints[i], &matWorld);
	}
	D3DXVec3TransformCoord(&m_vWorldPosinPoint, &m_vLocalPosinPoint, &matWorld);

	//행렬 적용 파트를 함수로 따로 분리하는것을 고려해보는중. LateUpdate에서 호출해도 문제 없나? 잘 모르겠다.
}

void CEnemy::LateUpdate()
{
}

void CEnemy::Render(HDC _hDC)
{
	//색상 지정
	HPEN hPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
	HPEN hOldPen = (HPEN)SelectObject(_hDC, hPen);

	//몸통 렌더.
	MoveToEx(_hDC, m_vWorldBodyPoints[0].x, m_vWorldBodyPoints[0].y, nullptr);
	for (int i = 1; i <= 4; ++i) {
		LineTo(_hDC, m_vWorldBodyPoints[i % 4].x, m_vWorldBodyPoints[i % 4].y);
	}

	//정면 구분을 위한 원형 도형 렌더.
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

	//포신 렌더
	MoveToEx(_hDC, m_tINFO.vPos.x, m_tINFO.vPos.y, nullptr);
	LineTo(_hDC, m_vWorldPosinPoint.x, m_vWorldPosinPoint.y);

	//펜 할당 해제
	SelectObject(_hDC, hOldPen);
	DeleteObject(hPen);

}

void CEnemy::Release()
{
	Safe_Delete(m_pTankStat);
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
		D3DXMATRIX matRotZ;
		D3DXMatrixRotationZ(&matRotZ, m_fRadian);
		D3DXVec3TransformNormal(&m_tINFO.vDir, &m_tINFO.vLook, &matRotZ);

		m_tINFO.vPos += m_tINFO.vDir * m_fSpeed;
	}
	if (GetAsyncKeyState(VK_DOWN)) {
		D3DXMATRIX matRotZ;
		D3DXMatrixRotationZ(&matRotZ, m_fRadian);
		D3DXVec3TransformNormal(&m_tINFO.vDir, &m_tINFO.vLook, &matRotZ);

		m_tINFO.vPos -= m_tINFO.vDir * m_fSpeed;
	}

	if (GetAsyncKeyState(VK_RSHIFT)) {
		D3DXMATRIX matRotZ, matTrans , matWorld;
		D3DXMatrixRotationZ(&matRotZ, m_fRadian);
		D3DXMatrixTranslation(&matTrans, m_tINFO.vPos.x, m_tINFO.vPos.y, m_tINFO.vPos.z);

		D3DXMatrixIdentity(&matWorld);
		matWorld = matRotZ * matTrans;

		D3DXVec3TransformNormal(&m_tINFO.vDir, &m_tINFO.vLook, &matRotZ);
		D3DXVec3TransformCoord(&m_vWorldPosinPoint, &m_vLocalPosinPoint, &matWorld);

		m_pTankStat->Fire(m_tINFO.vDir, m_vWorldPosinPoint, 3.f);
	}
	if (GetAsyncKeyState('P')) {
		ChaingeTankType(TANK_SHOTGUN);
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
			break;
		case TANK_BOOSTER:
			break;
		case TANK_SOMMONER:
			break;
		case TANK_END:
			break;
		default:
			break;
		}
		m_eNextTankID = m_eCurTankID;
	}
}