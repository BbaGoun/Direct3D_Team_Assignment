#include "pch.h"
#include "CollisionProcess.h"
#include "ObjMgr.h"
#include <limits>

void CollisionProcess::CollisionBulletToObstacle(CObj* _pBullet, CObj* _pObstacle)
{
	D3DXVECTOR3 point = _pBullet->GetPos();
	vector<D3DXVECTOR3> vertexVec = _pObstacle->GetWorldVertex();

	if (CheckPointInPolygon(point, vertexVec))
		ObjMgr::GetInstance().DeleteSpecificObj(OBJ_BULLET, _pBullet);

	int m = vertexVec.size();
	float minDistance = std::numeric_limits<float>::infinity();

	for (int i = 0; i < m; ++i) {
		D3DXVECTOR3 edge = vertexVec[(i + 1) % m] - vertexVec[i];
		D3DXVECTOR3 toPoint = point - vertexVec[i];

		float t = D3DXVec3Dot(&toPoint, &edge) / D3DXVec3Dot(&edge, &edge);

		t = clampT(t, 0.f, 1.f);

		D3DXVECTOR3 closest = vertexVec[i] + edge * t;

		D3DXVECTOR3 closestToPoint = point - closest;
		float distance = D3DXVec3Length(&closestToPoint);

		if (minDistance > distance)
			minDistance = distance;
	}

	//float radius = static_cast<CBullet*>(_pBullet)->GetRadius();
	float radius = 5;
	if(minDistance < radius)
		ObjMgr::GetInstance().DeleteSpecificObj(OBJ_BULLET, _pBullet);
}

void CollisionProcess::CollisionBulletToObj(CObj* _pBullet, CObj* _pObj)
{
	D3DXVECTOR3 point = _pBullet->GetPos();
	vector<D3DXVECTOR3> vertexVec = pObj->GetWorldVertex();

	if (CheckPointInPolygon(point, vertexVec))
		ObjMgr::GetInstance().DeleteSpecificObj(OBJ_BULLET, _pBullet);

	int m = vertexVec.size();
	float minDistance = std::numeric_limits<float>::infinity();

	for (int i = 0; i < m; ++i) {
		D3DXVECTOR3 edge = vertexVec[(i + 1) % m] - vertexVec[i];
		D3DXVECTOR3 toPoint = point - vertexVec[i];

		float t = D3DXVec3Dot(&toPoint, &edge) / D3DXVec3Dot(&edge, &edge);

		t = clampT(t, 0.f, 1.f);

		D3DXVECTOR3 closest = vertexVec[i] + edge * t;

		D3DXVECTOR3 closestToPoint = point - closest;
		float distance = D3DXVec3Length(&closestToPoint);

		if (minDistance > distance)
			minDistance = distance;
	}

	//float radius = static_cast<CBullet*>(_pBullet)->GetRadius();
	float radius = 5;
	if (minDistance < radius) {
		// 총알에게 오브젝트의 데미지
		_pBullet->SetHP(_pBullet->GetHP() - _pObj->GetDamage());
		// 오브젝트에게 총알의 데미지
		_pObj->SetHP(_pObj->GetHP() - _pBullet->GetDamage());
	}
}

void CollisionProcess::CollisionBulletToBullet(CObj* _pDstObj, CObj* _pSrcObj)
{
	//float radius = static_cast<CBullet*>(_pBullet)->GetRadius();
	float radiusDst = 5;
	//float radius = static_cast<CBullet*>(_pBullet)->GetRadius();
	float radiusSrc = 5;

	D3DXVECTOR3 dstPos = _pDstObj->GetPos();
	D3DXVECTOR3 srcPos = _pSrcObj->GetPos();
	D3DXVECTOR3 srcToDst = dstPos - srcPos;
	float distance = D3DXVec3Length(&srcToDst);

	if (distance < radiusDst + radiusSrc) {
		// dst 총알에게 src 총알의 데미지
		_pDstObj->SetHP(_pDstObj->GetHP() - _pSrcObj->GetDamage());
		// src 총알에게 dst 총알의 데미지
		_pSrcObj->SetHP(_pSrcObj->GetHP() - _pDstObj->GetDamage());
	}
}

void CollisionProcess::CollisionPlayerToObstacle(CObj* _pPlayer, CObj* _pObstacle)
{
	vector<D3DXVECTOR3> playerVertexVec = _pPlayer->GetWorldVertex();
	vector<D3DXVECTOR3> obstacleVertexVec = _pObstacle->GetWorldVertex();
	D3DXVECTOR3 MTV;

	if (CheckSAT(playerVertexVec, obstacleVertexVec, &MTV)) {
		D3DXVECTOR3 playerDir = _pPlayer->GetDir();
		if (D3DXVec3Dot(&playerDir, &MTV) > 0)
			MTV = -MTV;

		D3DXVECTOR3 oldPos = _pPlayer->GetPos();
		D3DXVECTOR3 newPos = oldPos + MTV;
		_pPlayer->SetPos(newPos);
	}
}

void CollisionProcess::CollisionObjToObj(CObj* _pDstObj, CObj* _pSrcObj)
{
	vector<D3DXVECTOR3> playerVertexVec = _pDstObj->GetWorldVertex();
	vector<D3DXVECTOR3> obstacleVertexVec = _pSrcObj->GetWorldVertex();
	D3DXVECTOR3 MTV;

	if (CheckSAT(playerVertexVec, obstacleVertexVec, &MTV)) {
		// dst에게 src의 데미지
		_pDstObj->SetHP(_pDstObj->GetHP() - _pSrcObj->GetDamage());
		// src에게 dst의 데미지
		_pSrcObj->SetHP(_pSrcObj->GetHP() - _pDstObj->GetDamage());
	}
}

bool CollisionProcess::CheckPointInPolygon(D3DXVECTOR3 _vPoint, vector<D3DXVECTOR3>& _VertexVec)
{
	float ccw;
	bool bFirst = true;
	int m = _VertexVec.size();
	for (int i = 0; i < m; ++i) {
		D3DXVECTOR3 edge = _VertexVec[(i + 1) % m] - _VertexVec[i];
		D3DXVECTOR3 toPoint = _vPoint - _VertexVec[i];

		D3DXVECTOR3 cross;
		D3DXVec3Cross(&cross, &edge, &toPoint);
		if (bFirst) {
			ccw = cross.z;
			bFirst = false;
		}
		else if (ccw * cross.z < 0)
			return false;
	}
	return true;
}

bool CollisionProcess::CheckSAT(vector<D3DXVECTOR3>& _SrcVertexVec, vector<D3DXVECTOR3>& _DstVertexVec, D3DXVECTOR3* pOutMTV)
{
	int n = _SrcVertexVec.size();
	int m = _DstVertexVec.size();

	float overlapMin = std::numeric_limits<float>::infinity();
	D3DXVECTOR3 closestAxis;
	for (int i = 0; i < n; ++i) {
		D3DXVECTOR3 edge = _SrcVertexVec[(i + 1) % n] - _SrcVertexVec[i];
		D3DXVECTOR3 normal = { -edge.y, edge.x, 0 };

		float srcMin = std::numeric_limits<float>::infinity();
		float srcMax = -std::numeric_limits<float>::infinity();

		float dstMin = std::numeric_limits<float>::infinity();
		float dstMax = -std::numeric_limits<float>::infinity();
		for (int j = 0; j < n; ++j) {
			float projection = D3DXVec3Dot(&_SrcVertexVec[j], &normal);
			if (srcMin > projection)
				srcMin = projection;
			if (srcMax < projection)
				srcMax = projection;
		}
		for (int j = 0; j < m; ++j) {
			float projection = D3DXVec3Dot(&_DstVertexVec[j], &normal);
			if (dstMin > projection)
				dstMin = projection;
			if (dstMax < projection)
				dstMax = projection;
		}

		if (srcMax < dstMin || dstMax < srcMin)
			return false;

		float overlap = min(srcMax, dstMax) - max(srcMin, dstMin);

		if ((srcMin <= dstMin && dstMax <= srcMax) || (dstMin <= srcMin && srcMax <= dstMax)) 
		{
			float moveSrc = dstMax - srcMin;
			float moveDst = srcMax - dstMin;

			overlap = min(moveSrc, moveDst);
		}

		if (overlapMin > overlap) {
			overlapMin = overlap;
			closestAxis = normal;
		}
	}
	for (int i = 0; i < m; ++i) {
		D3DXVECTOR3 edge = _DstVertexVec[(i + 1) % n] - _DstVertexVec[i];
		D3DXVECTOR3 normal = { -edge.y, edge.x, 0 };

		float srcMin = std::numeric_limits<float>::infinity();
		float srcMax = -std::numeric_limits<float>::infinity();

		float dstMin = std::numeric_limits<float>::infinity();
		float dstMax = -std::numeric_limits<float>::infinity();
		for (int j = 0; j < n; ++j) {
			float projection = D3DXVec3Dot(&_SrcVertexVec[j], &normal);
			if (srcMin > projection)
				srcMin = projection;
			if (srcMax < projection)
				srcMax = projection;
		}
		for (int j = 0; j < m; ++j) {
			float projection = D3DXVec3Dot(&_DstVertexVec[j], &normal);
			if (dstMin > projection)
				dstMin = projection;
			if (dstMax < projection)
				dstMax = projection;
		}

		if (srcMax < dstMin || dstMax < srcMin)
			return false;

		float overlap = min(srcMax, dstMax) - max(srcMin, dstMin);

		if ((srcMin <= dstMin && dstMax <= srcMax) || (dstMin <= srcMin && srcMax <= dstMax))
		{
			float moveSrc = dstMax - srcMin;
			float moveDst = srcMax - dstMin;

			overlap = min(moveSrc, moveDst);
		}

		if (overlapMin > overlap) {
			overlapMin = overlap;
			closestAxis = normal;
		}
	}

	D3DXVec3Normalize(&closestAxis, &closestAxis);
	closestAxis *= overlapMin;
	*pOutMTV = closestAxis;

	return true;
}
