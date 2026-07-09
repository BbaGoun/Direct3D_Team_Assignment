#include "pch.h"
#include "CTankType.h"

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
	ObjMgr::GetInstance().AddObject(OBJ_ENEMY_BULLET, AbstractFactory<CBullet1>::Create(_vDir, _vPos, _fSpeed));
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
	//D3DXMatrixRotationZ(&matRotZ, );

	//D3DXVec3TransformNormal(&m_tINFO.vDir, &m_tINFO.vLook, &matRotZ);
	ObjMgr::GetInstance().AddObject(OBJ_ENEMY_BULLET, AbstractFactory<CBullet1>::Create(_vDir, _vPos, _fSpeed));
}
