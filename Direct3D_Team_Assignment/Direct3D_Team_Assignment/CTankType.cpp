#include "pch.h"
#include "CTankType.h"
#include "Define.h"

#include "CBullet1.h"

#include "ObjMgr.h"
#include "AbstractFactory.h"

CTankNomal::CTankNomal()
{
}

CTankNomal::~CTankNomal()
{
}

void CTankNomal::Fire(D3DXVECTOR3 _vDir, D3DXVECTOR3 _vPos, float _fSpeed)
{
	ObjMgr::GetInstance().AddObject(OBJ_BULLET, AbstractFactory<CBullet1>::Create(_vDir, _vPos, _fSpeed));
}

CTankShotGun::CTankShotGun()
{
}
CTankShotGun::~CTankShotGun()
{
}

void CTankShotGun::Fire(D3DXVECTOR3 _vDir, D3DXVECTOR3 _vPos, float _fSpeed)
{
	D3DXMATRIX matRotZ;
	for (int i(0); i < 5; ++i)
	{
		D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(rand() % (15-(-15)+1)+(-15)));
		
		D3DXVec3TransformNormal(&_vDir, &_vDir, &matRotZ);
		ObjMgr::GetInstance().AddObject(OBJ_BULLET, AbstractFactory<CBullet1>::Create(_vDir, _vPos, _fSpeed));
	}
}
