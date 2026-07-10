#include "pch.h"
#include "CTankType.h"
#include "Define.h"

#include "ObjMgr.h"
#include "AbstractFactory.h"

void CTankNomal::Fire(CEnemy* _Enemy)
{
	D3DXMATRIX matRotZ;
	D3DXVECTOR3 vTempDir = _Enemy->GetDir();
	D3DXVECTOR3 vTempLook = _Enemy->GetLook();

	D3DXMatrixRotationZ(&matRotZ, _Enemy->GetRadian());
	D3DXVec3TransformNormal(&vTempDir, &vTempLook, &matRotZ);
	ObjMgr::GetInstance().AddObject(OBJ_BULLET, AbstractFactory<CBullet1>::Create(vTempDir, _Enemy->GetPos(), 5.f));
	_Enemy->Accelerate(-5.f);
	_Enemy->SetDelay(15.f);

}

void CTankShotGun::Fire(CEnemy* _Enemy)
{
	D3DXMATRIX matRotZ;
	D3DXVECTOR3 vTempDir= _Enemy->GetDir();
	D3DXVECTOR3 vTempLook= _Enemy->GetLook();
	float fTempSpeed;

	D3DXMatrixRotationZ(&matRotZ, _Enemy->GetRadian());
	for (int i(0); i < 10; ++i)
	{
		fTempSpeed = 5.f + (dis(gen) % 9 + 5);
		D3DXMatrixRotationZ(&matRotZ, _Enemy->GetRadian()+D3DXToRadian(-15 + dis(gen) * 0.3));

		D3DXVec3TransformNormal(&vTempDir, &vTempLook, &matRotZ);
		ObjMgr::GetInstance().AddObject(OBJ_BULLET, AbstractFactory<CBullet1>::Create(vTempDir, _Enemy->GetPos(), fTempSpeed));
	}
	_Enemy->Accelerate(-20.f);
	_Enemy->SetDelay(30.f);
}

void CTankSommoner::Fire(CEnemy* _Enemy)
{
	//if (list . . .) 푸쉬백 드론ObjMgr::GetInstance().AddObject(OBJ_BULLET, AbstractFactory<CDrone>::Create());
}

void CTankBooster::Fire(CEnemy* _Enemy)
{
	D3DXMATRIX matRotZ;
	D3DXVECTOR3 vTempDir = _Enemy->GetDir();
	D3DXVECTOR3 vTempLook = _Enemy->GetLook();
	float fTempSpeed;

	D3DXMatrixRotationZ(&matRotZ, _Enemy->GetRadian());
	D3DXVec3TransformNormal(&vTempDir, &vTempLook, &matRotZ);
	ObjMgr::GetInstance().AddObject(OBJ_BULLET, AbstractFactory<CBullet1>::Create(vTempDir, _Enemy->GetPos(), 10.f));
	for (int i(0); i < 5; ++i)
	{
		fTempSpeed = 5.f + (dis(gen) % 9 + 5);
		D3DXMatrixRotationZ(&matRotZ, _Enemy->GetRadian() + D3DXToRadian(180+(-15 + dis(gen) * 0.3)));
		
		D3DXVec3TransformNormal(&vTempDir, &vTempLook, &matRotZ);
		ObjMgr::GetInstance().AddObject(OBJ_BULLET, AbstractFactory<CBullet1>::Create(vTempDir, _Enemy->GetPos(), fTempSpeed));
	}
	_Enemy->Accelerate(+15.f);
	_Enemy->SetDelay(25.f);
}

void CTankGuided::Fire(CEnemy* _Enemy)
{
	//D3DXMATRIX matRotZ;
	//D3DXVECTOR3 vTempDir = _Enemy->GetDir();
	//D3DXVECTOR3 vTempLook = _Enemy->GetLook();

	//D3DXMatrixRotationZ(&matRotZ, _Enemy->GetRadian());
	//D3DXVec3TransformNormal(&vTempDir, &vTempLook, &matRotZ);
	//ObjMgr::GetInstance().AddObject(OBJ_BULLET, AbstractFactory<유도탄>::Create(vTempDir, _Enemy->GetPos(), 5.f));
	//_Enemy->Accelerate(-5.f);
	//_Enemy->SetDelay(80.f);
}

void CTankNomal::RanderPosin(HDC _hdc)
{
}

void CTankShotGun::RanderPosin(HDC _hdc)
{
}

void CTankSommoner::RanderPosin(HDC _hdc)
{
}

void CTankBooster::RanderPosin(HDC _hdc)
{
}

void CTankGuided::RanderPosin(HDC _hdc)
{
}

//포신위치 이니셜라이저하고, 서모너의 경우 리스트 관리..
void CTankNomal::Initialize()
{
}

void CTankShotGun::Initialize()
{
}

void CTankSommoner::Initialize()
{
	
}

void CTankBooster::Initialize()
{
}

void CTankGuided::Initialize()
{
}

