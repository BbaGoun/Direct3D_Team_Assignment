#pragma once
#include "CObj.h"

enum OBJID {
	OBJ_OBSTACLE,

	OBJ_BREAKABLE,

	OBJ_BULLET,

	OBJ_PLAYER,

	OBJ_UI,

	OBJ_END
};

class ObjMgr
{
private:
	ObjMgr();
	ObjMgr(const ObjMgr& rhs) = delete;
	ObjMgr& operator=(const ObjMgr& rhs) = delete;
	~ObjMgr();

public:
	static ObjMgr& GetInstance() {
		static ObjMgr Instance;
		return Instance;
	}

public:
	void Update();
	void LateUpdate();
	void Render(HDC _HDC);
	void Release();

public:
	void AddObject(OBJID eID, CObj* pObj);
	void DeleteObj(OBJID eID);
	void DeleteSpecificObj(OBJID eID, CObj* _pObj);
	void DeleteListPostProcess();

private:
	void CollisionBulletToObstacle();
	void CollisionBulletToBreakable();
	void CollisionBulletToBullet();
	void CollisionBulletToPlayer();

	void CollisionPlayerToObstacle();
	void CollisionPlayerToBreakable();
	void CollisionPlayerToPlayer();

private:
	list<CObj*>		m_ObjList[OBJ_END];
	list<CObj*>		m_DeleteList[OBJ_END];
};

