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

void CEnemy::Update()
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

void CEnemy::LateUpdate()
{
}

void CEnemy::Render(HDC _hDC)
{
	HPEN hPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
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
}
