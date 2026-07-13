#include "pch.h"
#include "CBulletDrone.h"
#include "AbstractFactory.h"
#include "CBullet1.h"
#include "CEnemy.h"
#include "CameraMgr.h"
#include "TimeMgr.h"

CBulletDrone::CBulletDrone()
{
}

CBulletDrone::~CBulletDrone()
{
}

void CBulletDrone::Initialize()
{
	m_fDelayTime = 0;
	m_fDelayTimer = 30.f;

	m_tINFO.vLook = { 0, -1, 0 };
	//m_tINFO.vPos = { 0, -100, 0 };
	m_vdOffestPos = { 0, -100, 0 };

	m_vLocalVec.resize(4);
	m_vWorldVec.resize(4);
	m_vViewVec.resize(4);
	m_vProjVec.resize(4);

	m_vLocalVec[0] = { -15.f, -15.f, 0 };
	m_vLocalVec[1] = { 15.f, -15.f, 0 };
	m_vLocalVec[2] = { 15.f, 15.f, 0 };
	m_vLocalVec[3] = { -15.f, 15.f, 0 };
}

void CBulletDrone::Update()
{
	m_fRadian += 2.0f * TimeMgr::GetInstance().GetDeltaTime();

	D3DXMATRIX matScale, matRotZ, matTrans, matWorld;
	D3DXMATRIX matTransOffset;
	D3DXMATRIX matParWorld;

	D3DXMatrixScaling(&matScale, 1, 1, 1);//크기
	D3DXMatrixRotationZ(&matRotZ, m_fRadian);//회전 행렬(다이렉션을 얼마나 회전할 지)
	D3DXMatrixTranslation(&matTrans, m_tINFO.vPos.x, m_tINFO.vPos.y, m_tINFO.vPos.z);
	D3DXMatrixTranslation(&matTransOffset, m_vdOffestPos.x, m_vdOffestPos.y, m_vdOffestPos.z);
	//이동 행렬에는 월드에 대입할 위치값이 아닌 중점에서의 오프셋을.
	D3DXMatrixIdentity(&matWorld); 

	//결정적으로 크기*이동*행렬 까지의 과정은 사실상 로컬 내에서 중점을 기준으로 돌고있는 상태를 로컬 안에 만들고
	//그 상태에다가 부모의 월드 상태(부모의 중점, 회전각, 크기)를 가져와 대입한거임

	//이같은 경우 당연히 플레이어에게 적용된 월드가 적용되겠지
	
	// 이 부모의 월드 행렬을 대입 하기 전 중점을 기준으로 돌고 있는 공전상태)를 만들어둔것

	matParWorld = dynamic_cast<CEnemy*>(m_pParent)->GetWorld();
	matWorld = matScale * matTransOffset * matRotZ * matParWorld;
	//matWorld = matScale * matRotZ * matTrans * matParWorld;

	//버텍스에 행렬 적용하고
	for (int i = 0; i < 4; ++i) {
		D3DXVec3TransformCoord(&m_vWorldVec[i], &m_vLocalVec[i], &matWorld);
	}
	//D3DXVec3TransformCoord(&m_tINFO.vPos, &m_vdOffestPos, &matWorld);
	D3DXVec3TransformNormal(&m_tINFO.vDir, &m_tINFO.vLook, &matRotZ);

	D3DXMATRIX matView = CameraMgr::GetInstance().GetViewMat();
	D3DXMATRIX matProj = CameraMgr::GetInstance().GetProjMat();

	for (int i = 0; i < m_vWorldVec.size(); ++i) {
		D3DXVec3TransformCoord(&m_vViewVec[i], &m_vWorldVec[i], &matView);
		D3DXVec3TransformCoord(&m_vProjVec[i], &m_vViewVec[i], &matProj);
		m_vProjVec[i] += {640, 360, 0};
	}

	if (m_fDelayTime <= 0)
	{
		AutoFire();
		m_fDelayTime = m_fDelayTimer;
	}
}

void CBulletDrone::LateUpdate()
{
	if (m_fDelayTime >= 0)
	{
		--m_fDelayTime;
		if (m_fDelayTime <= 0)
			m_fDelayTime = 0;
	}
}

void CBulletDrone::Render(HDC _hDC)
{

	HPEN hPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
	HPEN hOldPen = (HPEN)SelectObject(_hDC, hPen);

	MoveToEx(_hDC, m_vProjVec[0].x, m_vProjVec[0].y, nullptr);
	for (int i = 1; i <= 4; ++i) {
		LineTo(_hDC, m_vProjVec[i % 4].x, m_vProjVec[i % 4].y);
	}

	SelectObject(_hDC, hOldPen);
	DeleteObject(hPen);
}

void CBulletDrone::Release()
{
}

void CBulletDrone::AutoFire()
{
	CObj* Temp;
	Temp = AbstractFactory<CBullet1>::Create(m_tINFO.vDir, m_tINFO.vPos, 0.f);
	Temp->SetParent(m_pParent);
	ObjMgr::GetInstance().AddObject(OBJ_BULLET, Temp);
}
