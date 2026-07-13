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

	float vDirOffset;
	for (int i(0); i < m_vWorldPosinPoint.size(); i++)
	{
		vDirOffset = GetPosinRadian(m_vWorldPosinPoint[i], _Enemy->GetPos());

		D3DXMatrixRotationZ(&matRotZ, vDirOffset + D3DXToRadian(-5 + dis(gen) * (10.f / 99.f)));
		D3DXVec3TransformNormal(&vTempDir, &vTempLook, &matRotZ);
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
	D3DXVECTOR3 vTempDir;
	D3DXVECTOR3 vTempLook = _Enemy->GetLook();
	CObj* Temp;

	float fDirOffset;
	float fSpeedOffset;
	for (int i(0); i < m_vWorldPosinPoint.size(); i++)
	{
		for (int j(0); j < 10; j++)
		{
			fDirOffset = GetPosinRadian(m_vWorldPosinPoint[i], _Enemy->GetPos());
			fSpeedOffset = 3.f+(D3DXToRadian(dis(gen) * (17.f/99.f)));
			D3DXMatrixRotationZ(&matRotZ, fDirOffset + D3DXToRadian(-30.f + dis(gen) * (60.f/99.f)));
			D3DXVec3TransformNormal(&vTempDir, &vTempLook, &matRotZ);

			Temp = AbstractFactory<CBullet1>::Create(vTempDir, m_vWorldPosinPoint[i], fSpeedOffset);
			Temp->SetParent(_Enemy);
			ObjMgr::GetInstance().AddObject(OBJ_BULLET, Temp);
		}
	}

	SetDelayAndRebound(_Enemy);
}
//소환사
void CTankSommoner::Fire(CEnemy* _Enemy)
{
	D3DXMATRIX matRotZ;
	D3DXVECTOR3 vTempDir;
	D3DXVECTOR3 vTempLook = _Enemy->GetLook();
	CObj* Temp;

	float vDirOffset;
	for (int i(0); i < m_vWorldPosinPoint.size(); i++)
	{
		vDirOffset = GetPosinRadian(m_vWorldPosinPoint[i], _Enemy->GetPos());
		D3DXMatrixRotationZ(&matRotZ, vDirOffset);
		D3DXVec3TransformNormal(&vTempDir, &vTempLook, &matRotZ);
		Temp = AbstractFactory<CBulletDrone>::Create(vTempDir, _Enemy->GetPos(), 8.f);
		//Temp = AbstractFactory<CBulletDrone>::Create(vTempDir);
		//Temp = AbstractFactory<CBulletDrone>::Create();
		Temp->SetParent(_Enemy);
		ObjMgr::GetInstance().AddObject(OBJ_BULLET, Temp);
	}

	SetDelayAndRebound(_Enemy);
}
//부스터
void CTankBooster::Fire(CEnemy* _Enemy)
{
	D3DXMATRIX matRotZ;
	D3DXVECTOR3 vTempDir;
	D3DXVECTOR3 vTempLook = _Enemy->GetLook();
	CObj* Temp;

	float vDirOffset;
	for (int i(0); i < m_vWorldPosinPoint.size(); i++)
	{
		vDirOffset = GetPosinRadian(m_vWorldPosinPoint[i], _Enemy->GetPos());

		D3DXMatrixRotationZ(&matRotZ, vDirOffset);
		D3DXVec3TransformNormal(&vTempDir, &vTempLook, &matRotZ);
		Temp = AbstractFactory<CBullet1>::Create(vTempDir, m_vWorldPosinPoint[i], 8.f);
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

	float vDirOffset;
	for (int i(0); i < m_vWorldPosinPoint.size(); i++)
	{
		vDirOffset = GetPosinRadian(m_vWorldPosinPoint[i], _Enemy->GetPos());
		D3DXMatrixRotationZ(&matRotZ, vDirOffset);
		D3DXVec3TransformNormal(&vTempDir, &vTempLook, &matRotZ);
		Temp = AbstractFactory<CBulletTrakin1>::Create(vTempDir, m_vWorldPosinPoint[i], 8.f);
		Temp->SetParent(_Enemy);
		static_cast<CBulletTrakin1*>(Temp)->SetTarget(_Enemy->GetParent());
		ObjMgr::GetInstance().AddObject(OBJ_BULLET, Temp);
	}

	SetDelayAndRebound(_Enemy);
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
	m_vLocalPosinPoint.resize(1);
	m_vWorldPosinPoint.resize(1);
	m_vLocalPosinPoint[0] = { 0,-100,0 };

	m_fDelayTime = 50.f;
	m_fRebound = -30.f;
	_Enemy->SetDelayTimer(m_fDelayTime);
}

void CTankBooster::Initialize(CEnemy* _Enemy)
{
	m_vLocalPosinPoint.resize(6);
	m_vWorldPosinPoint.resize(6);
	m_vLocalPosinPoint[0] = { 0,-100,0 };

	m_vLocalPosinPoint[1] = { 0,100,0 };
	m_vLocalPosinPoint[2] = { -50,100,0 };
	m_vLocalPosinPoint[3] = { 50,100,0 };
	m_vLocalPosinPoint[4] = { -30,100,0 };
	m_vLocalPosinPoint[5] = { 30,100,0 };

	m_fDelayTime = 25.f;
	m_fRebound = 30.f;
	_Enemy->SetDelayTimer(m_fDelayTime);
}

void CTankSommoner::Initialize(CEnemy* _Enemy)
{
	m_vLocalPosinPoint.resize(1);
	m_vWorldPosinPoint.resize(1);
	m_vLocalPosinPoint[0] = { 0,-100,0 };

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

