#include "pch.h"
#include "CTankType.h"
#include "Define.h"

#include "ObjMgr.h"
#include "AbstractFactory.h"

//기본
void CTankNomal::Fire(CEnemy* _Enemy)
{
	D3DXMATRIX matRotZ;
	D3DXVECTOR3 vTempDir;
	D3DXVECTOR3 vTempLook = _Enemy->GetLook();
	CObj* Temp;

	for (int i(0); i < m_vWorldPosinPoint.size(); i++)
	{
		vTempDir = m_vWorldPosinPoint[i] - _Enemy->GetPos();
		D3DXVec3Normalize(&vTempDir, &vTempDir);
		Temp = AbstractFactory<CBullet1>::Create(vTempDir, m_vWorldPosinPoint[i], 8.f);
		Temp->SetParent(_Enemy);
		ObjMgr::GetInstance().AddObject(OBJ_BULLET, Temp);
	}

	SetDelayAndRebound(_Enemy);
}
//샷건
void CTankShotGun::Fire(CEnemy* _Enemy)
{
	D3DXMATRIX matRotZ;
	D3DXVECTOR3 vTempDir = _Enemy->GetDir();
	D3DXVECTOR3 vTempLook = _Enemy->GetLook();
	float fTempSpeed;
	CObj* Temp;

	D3DXMatrixRotationZ(&matRotZ, _Enemy->GetRadian());
	for (int i(0); i < 10; ++i)
	{
		fTempSpeed = 5.f + (dis(gen) % 9 + 5);
		D3DXMatrixRotationZ(&matRotZ, _Enemy->GetRadian() + D3DXToRadian(-15 + dis(gen) * 0.3));

		D3DXVec3TransformNormal(&vTempDir, &vTempLook, &matRotZ);
		Temp = AbstractFactory<CBullet1>::Create(vTempDir, _Enemy->GetPos(), fTempSpeed);
		Temp->SetParent(_Enemy);
		ObjMgr::GetInstance().AddObject(OBJ_BULLET, Temp);
	}
	SetDelayAndRebound(_Enemy);
}
//소환사
void CTankSommoner::Fire(CEnemy* _Enemy)
{
	//if (list . . .) 푸쉬백 드론ObjMgr::GetInstance().AddObject(OBJ_BULLET, AbstractFactory<CDrone>::Create());
}
//부스터
void CTankBooster::Fire(CEnemy* _Enemy)
{
	D3DXMATRIX matRotZ;
	D3DXVECTOR3 vTempDir = _Enemy->GetDir();
	D3DXVECTOR3 vTempLook = _Enemy->GetLook();
	float fTempSpeed;
	CObj* Temp;

	D3DXMatrixRotationZ(&matRotZ, _Enemy->GetRadian());
	D3DXVec3TransformNormal(&vTempDir, &vTempLook, &matRotZ);
	Temp = AbstractFactory<CBullet1>::Create(vTempDir, _Enemy->GetPos(), 10.f);

	Temp->SetParent(_Enemy);
	ObjMgr::GetInstance().AddObject(OBJ_BULLET, Temp);
	for (int i(0); i < 5; ++i)
	{
		fTempSpeed = 5.f + (dis(gen) % 9 + 5);
		D3DXMatrixRotationZ(&matRotZ, _Enemy->GetRadian() + D3DXToRadian(180 + (-15 + dis(gen) * 0.3)));

		D3DXVec3TransformNormal(&vTempDir, &vTempLook, &matRotZ);
		Temp = AbstractFactory<CBullet1>::Create(vTempDir, _Enemy->GetPos(), fTempSpeed);
		Temp->SetParent(_Enemy);
		ObjMgr::GetInstance().AddObject(OBJ_BULLET, Temp);
	}
	SetDelayAndRebound(_Enemy);
}
//유도탄
void CTankGuided::Fire(CEnemy* _Enemy)
{
	D3DXMATRIX matRotZ;
	D3DXVECTOR3 vTempDir;
	D3DXVECTOR3 vTempLook = _Enemy->GetLook();
	CObj* Temp;

	for (int i(0); i < m_vWorldPosinPoint.size(); i++)
	{
		vTempDir = m_vWorldPosinPoint[i] - _Enemy->GetPos();
		D3DXVec3Normalize(&vTempDir, &vTempDir);
		Temp = AbstractFactory<CBulletTrakin1>::Create(vTempDir, m_vWorldPosinPoint[i], 5.f);

		Temp->SetParent(_Enemy);
		static_cast<CBulletTrakin1*>(Temp)->SetTarget(_Enemy->GetParent());
		ObjMgr::GetInstance().AddObject(OBJ_BULLET, Temp);
	}

	SetDelayAndRebound(_Enemy);
}

void CTankNomal::RanderPosin(HDC _hdc)
{

}

void CTankShotGun::RanderPosin(HDC _hdc)
{
}

void CTankBooster::RanderPosin(HDC _hdc)
{
}

void CTankGuided::RanderPosin(HDC _hdc)
{
}

void CTankSommoner::RanderPosin(HDC _hdc)
{
}

void CTankNomal::Initialize(CEnemy* _Enemy)
{
	m_vLocalPosinPoint.resize(1);
	m_vWorldPosinPoint.resize(1);
	m_vLocalPosinPoint[0] = { 0,-100,0 };

	m_fDelayTime = 15.f;
	m_fRebound = -5.f;
	_Enemy->SetDelayTimer(m_fDelayTime);
}

void CTankShotGun::Initialize(CEnemy* _Enemy)
{
	m_fDelayTime = 50.f;
	m_fRebound = -30.f;
	_Enemy->SetDelayTimer(m_fDelayTime);
}

void CTankBooster::Initialize(CEnemy* _Enemy)
{
	m_fDelayTime = 25.f;
	m_fRebound = 30.f;
	_Enemy->SetDelayTimer(m_fDelayTime);
}

void CTankSommoner::Initialize(CEnemy* _Enemy)
{
	m_fDelayTime = 20.f;
	m_fRebound = -5.f;
	_Enemy->SetDelayTimer(m_fDelayTime);
}

void CTankGuided::Initialize(CEnemy* _Enemy)
{
	m_vLocalPosinPoint.resize(1);
	m_vWorldPosinPoint.resize(1);
	m_vLocalPosinPoint[0] = { 0,-100,0 };

	m_fDelayTime = 30.f;
	m_fRebound = -10.f;
	_Enemy->SetDelayTimer(m_fDelayTime);
}

