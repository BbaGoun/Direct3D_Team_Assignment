#pragma once
#include "CObj.h"

class CollisionProcess
{
public:
	static bool CollisionBulletToObstacle(CObj* _pBullet, CObj* _pObstacle);
	static bool CollisionBulletToObj(CObj* _pBullet, CObj* _pObj);
	static bool CollisionBulletToBullet(CObj* _pDstObj, CObj* _pSrcObj);
	static bool CollisionPlayerToObstacle(CObj* _pPlayer, CObj* _pObstacle);
	static bool CollisionObjToObj(CObj* _pDstObj, CObj* _pSrcObj);

private:
	static bool CheckPointInPolygon(D3DXVECTOR3 _vPoint, vector<D3DXVECTOR3>& _VertexVec);
	static bool CheckSAT(vector<D3DXVECTOR3>& _SrcVertexVec, vector<D3DXVECTOR3>& _DstVertexVec, D3DXVECTOR3* pOutMTV);
};

