#include "pch.h"
#include "CEnemy.h"

CEnemy::CEnemy()
{
}

CEnemy::~CEnemy()
{
}

void CEnemy::Initialize()
{
	//모든 각주는 작업자 본인이 코드의 구조를 까먹지 않을수 있도록 돕는 용도로 작성됨

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

	//오브젝트의 포신(총알이 생성될 부분)의 로컬 포인트.
	m_vLocalPosinPoint = { 0, -100.f, 0 };
	
	//최초 생성시 방향값은 0.
	m_fRadian = 0;
}

void CEnemy::Update()
{
	KeyInput();

	//월드에 적용될 임시 행렬.
	//월드 행렬의 경우 기본 구조체 내에 생성되어있으며, 필요시에만 행렬 갱신을 할수 있도록 제작되고 있음. 아직 미구현(0708)
	//현재 코드에서는 지역 변수를 사용 중.
	D3DXMATRIX matScale, matRotZ, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, 1, 1, 1);
	D3DXMatrixRotationZ(&matRotZ, m_fRadian);
	D3DXMatrixTranslation(&matTrans, m_tINFO.vPos.x, m_tINFO.vPos.y, m_tINFO.vPos.z);

	//월드 행렬 초기화
	D3DXMatrixIdentity(&matWorld);

	//월드 행렬에 구성 요소 적용. 크기/자전/이동/공전/위치(부모)<- 순서 잊지 말것!
	matWorld = matScale * matRotZ * matTrans;


	//사각형의 구성 요소에 월드 행렬 적용
	for (int i = 0; i < 4; ++i) {
		D3DXVec3TransformCoord(&m_vWorldBodyPoints[i], &m_vLocalBodyPoints[i], &matWorld);
	}
	//포신의 구성 요소에 월드 행렬 적용
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

	//총알 발사키
	//if (GetAsyncKeyState('/')) {
	//	m_fRadian += D3DXToRadian(3);
	//}

	//동시 입력시 선입력 처리 할 예외처리 추가 예정
}
