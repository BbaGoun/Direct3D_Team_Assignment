#include "pch.h"
#include "CBulletDrone.h"
#include "AbstractFactory.h"
#include "CBullet1.h"
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
	m_tINFO.vPos = { 0, -200, 0 };

	m_vLocalVec.resize(4);
	m_vWorldVec.resize(4);
	m_vViewVec.resize(4);
	m_vProjVec.resize(4);

	m_vLocalVec[0] = { -15.f, -15.f, 0 };
	m_vLocalVec[1] = { 15.f, -15.f, 0 };
	m_vLocalVec[2] = { 15.f, 15.f, 0 };
	m_vLocalVec[3] = { -15.f, 15.f, 0 };
}
/*임시로 로컬로 땡겨와서 공전 후의 위치를 찍은다음에 다시 월드로 돌아가는 느낌인가?

그렇다는건 이미 계산된 부모 포스를 굳이 로컬로 땡겨올 필요도 없이
로컬 상태에서 로컬 중점을 두고 공전 포스를 찍은다음에 그걸 플레이어의 월드 포스에 대입하면 되지 않나..?*/
void CBulletDrone::Update()
{
	m_fRadian += 2.0f * TimeMgr::GetInstance().GetDeltaTime();

	D3DXMATRIX matScale, matRotZ, matTrans, matWorld;
	D3DXMATRIX matParTrans;
	D3DXMatrixScaling(&matScale, 1, 1, 1);
	D3DXMatrixRotationZ(&matRotZ, m_fRadian);
	D3DXMatrixTranslation(&matTrans, m_tINFO.vPos.x, m_tINFO.vPos.y, m_tINFO.vPos.z);

	D3DXMatrixTranslation(&matParTrans, m_pParent->GetPos().x, m_pParent->GetPos().y, m_pParent->GetPos().z);

	D3DXMatrixIdentity(&matWorld); 
	matWorld = matScale * matTrans * matRotZ * matParTrans;
	for (int i = 0; i < 4; ++i) {
		D3DXVec3TransformCoord(&m_vWorldVec[i], &m_vLocalVec[i], &matWorld);
		//D3DXVec3TransformCoord(&m_vWorldVec[i], &m_vLocalVec[i], &matParTrans);
	}

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
	Temp = AbstractFactory<CBullet1>::Create(m_tINFO.vDir, m_tINFO.vPos , 5.f);
	Temp->SetParent(m_pParent);
	ObjMgr::GetInstance().AddObject(OBJ_BULLET, Temp);
}
