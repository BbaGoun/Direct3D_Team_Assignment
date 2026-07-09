#pragma once
#include "CObj.h"

class CollisionProcess
{
public:
	static void CollisionBulletToObstacle(CObj* _pBullet, CObj* _pObstacle);
	static void CollisionBulletToObj(CObj* _pBullet, CObj* pObj);
	static void CollisionBulletToBullet(CObj* _pDstObj, CObj* _pSrcObj);
	static void CollisionPlayerToObstacle(CObj* _pPlayer, CObj* _pObstacle);
	static void CollisionObjToObj(CObj* _pDstObj, CObj* _pSrcObj);

private:
	static bool CheckPointInPolygon(D3DXVECTOR3 _vPoint, vector<D3DXVECTOR3>& _VertexVec);
	static bool CheckSAT(vector<D3DXVECTOR3>& _SrcVertexVec, vector<D3DXVECTOR3>& _DstVertexVec, D3DXVECTOR3* pOutMTV);
};

