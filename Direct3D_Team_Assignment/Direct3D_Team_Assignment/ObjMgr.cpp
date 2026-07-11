#include "pch.h"
#include "ObjMgr.h"
#include "CollisionProcess.h"
#include "CBreakableObj.h"
#include "CObj.h"

ObjMgr::ObjMgr()
{
}

ObjMgr::~ObjMgr()
{
	Release();
}

void ObjMgr::Update()
{
	DeleteListPostProcess();

	for (int i = 0; i < OBJ_END; ++i) {
		for (auto& pObj : m_ObjList[i]) {
			pObj->Update();

			if (m_ObjList[i].empty())
				break;
		}
	}

	DeleteListPostProcess();
}

void ObjMgr::LateUpdate()
{
	DeleteListPostProcess();

	for (int i = 0; i < OBJ_END; ++i) {
		for (auto& pObj : m_ObjList[i]) {
			pObj->LateUpdate();

			if (m_ObjList[i].empty())
				break;
		}
	}

	DeleteListPostProcess();

	// 충돌 처리
	// 소환수들의 움직임이 Player의 Update 이후에
	// LateUpdate에서 이뤄지기에 여기서 진행.
	CollisionBulletToObstacle();
	CollisionPlayerToObstacle();

	CollisionBulletToBreakable();
	CollisionPlayerToBreakable();

	CollisionBulletToBullet();
	CollisionBulletToPlayer();

	CollisionPlayerToPlayer();

	DeleteListPostProcess();
}

void ObjMgr::Render(HDC _HDC)
{
	for (int i = 0; i < OBJ_END; ++i) {
		for (auto& pObj : m_ObjList[i]) {
			pObj->Render(_HDC);
		}
	}
}

void ObjMgr::Release()
{
	for (auto& objList : m_ObjList) {
		for (auto& pObj : objList) {
			Safe_Delete(pObj);
		}
		objList.clear();
	}
}

void ObjMgr::AddObject(OBJID eID, CObj* pObj)
{
	pObj->SetObjID(eID);
	m_ObjList[eID].push_back(pObj);
}

void ObjMgr::DeleteObj(OBJID eID)
{
	for (auto& pObj : m_ObjList[eID])
		Safe_Delete(pObj);

	m_ObjList[eID].clear();
}

void ObjMgr::DeleteSpecificObj(OBJID eID, CObj* _pObj)
{
	m_DeleteList[eID].push_back(_pObj);
}

void ObjMgr::DeleteListPostProcess()
{
	bool bFind = false;
	for (int i = 0; i < OBJ_END; ++i) {
		for (auto it = m_ObjList[i].begin(); it != m_ObjList[i].end(); ) {
			auto find_it = find(m_DeleteList[i].begin(), m_DeleteList[i].end(), *it);

			if (find_it != m_DeleteList[i].end()) {
				Safe_Delete(*it);
				it = m_ObjList[i].erase(it);
			}
			else
				++it;
		}
		m_DeleteList[i].clear();
	}
}

void ObjMgr::CollisionBulletToObstacle()
{
	for (auto& pBullet : m_ObjList[OBJ_BULLET]) {
		if (pBullet->GetDead())
			continue;
		for (auto& pObstacle : m_ObjList[OBJ_OBSTACLE]) {
			if (pObstacle->GetDead())
				continue;
			if (CollisionProcess::CollisionBulletToObj(pBullet, pObstacle)) {
				// 벽에 닿으면 삭제
				ObjMgr::GetInstance().DeleteSpecificObj(OBJ_BULLET, pBullet);
			}
		}
	}
}

void ObjMgr::CollisionBulletToBreakable()
{
	for (auto& pBullet : m_ObjList[OBJ_BULLET]) {
		if (pBullet->GetDead())
			continue;
		for (auto& pBreakable : m_ObjList[OBJ_BREAKABLE]) {
			if (pBreakable->GetDead())
				continue;
			if (CollisionProcess::CollisionBulletToObj(pBullet, pBreakable)) {
				// 총알에게 오브젝트의 데미지
				pBullet->TakeDamageByObj(pBreakable);
				// 오브젝트에게 총알의 데미지
				pBreakable->TakeDamageByBullet(pBullet);
			}
		}
	}
}

void ObjMgr::CollisionBulletToBullet()
{
	int i = 0;
	int j = 0;
	for (auto& pBullet1 : m_ObjList[OBJ_BULLET]) {
		if (pBullet1->GetDead()) {
			++i;
			continue;
		}
		j = 0;
		for (auto& pBullet2 : m_ObjList[OBJ_BULLET]) {
			if (j < i)
			{
				++j;
				continue;
			}
			if (pBullet1->GetDead())
				continue;
			if (pBullet1 == pBullet2)
				continue;
			if (pBullet1->GetParent() == pBullet2->GetParent())
				continue;
			if (CollisionProcess::CollisionBulletToBullet(pBullet1, pBullet2)) {
				// pBullet1 총알에게 pBullet2 총알의 데미지
				pBullet1->TakeDamageByBullet(pBullet2);
				// pBullet2 총알에게 pBullet1 총알의 데미지
				pBullet2->TakeDamageByBullet(pBullet1);
			}
		}
		++i;
	}
}

void ObjMgr::CollisionBulletToPlayer()
{
	for (auto& pBullet : m_ObjList[OBJ_BULLET]) {
		if (pBullet->GetDead())
			continue;
		for (auto& pPlayer : m_ObjList[OBJ_PLAYER]) {
			if (pPlayer->GetDead())
				continue;
			if (pBullet->GetParent() == pPlayer)
				continue;

			if (CollisionProcess::CollisionBulletToObj(pBullet, pPlayer)) {
				// 총알에게 플레이어의 데미지
				pBullet->TakeDamageByObj(pPlayer);
				// 플레이어에게 총알의 데미지
				pPlayer->TakeDamageByBullet(pBullet);
			}
		}
	}
}

void ObjMgr::CollisionPlayerToObstacle()
{
	for (auto& pPlayer : m_ObjList[OBJ_PLAYER]) {
		if (pPlayer->GetDead())
			continue;
		for (auto& pObstacle : m_ObjList[OBJ_OBSTACLE]) {
			if (pObstacle->GetDead())
				continue;
			D3DXVECTOR3 MTV;
			if (CollisionProcess::CollisionPlayerToObstacle(pPlayer, pObstacle, &MTV)) {
				// 사물과 겹치면 최소 거리로 탈출
				D3DXVECTOR3 oldPos = pPlayer->GetPos();
				D3DXVECTOR3 newPos = oldPos + MTV;
				pPlayer->SetPos(newPos);
				pPlayer->ReUpdateWorldVertex();
			}
		}
	}
}

void ObjMgr::CollisionPlayerToBreakable()
{
	for (auto& pPlayer : m_ObjList[OBJ_PLAYER]) {
		if (pPlayer->GetDead())
			continue;
		for (auto& pBreakable : m_ObjList[OBJ_BREAKABLE]) {
			if (pBreakable->GetDead())
				continue;
			if (CollisionProcess::CollisionObjToObj(pPlayer, pBreakable)) {
				// 플레이어에게 오브젝트의 데미지
				pPlayer->TakeDamageByObj(pBreakable);
				// 오브젝트에게 플레이어의 데미지
				pBreakable->TakeDamageByObj(pPlayer);
			}
		}
	}
}

void ObjMgr::CollisionPlayerToPlayer()
{
	int i = 0;
	int j = 0;
	for (auto& pPlayer1 : m_ObjList[OBJ_PLAYER]) {
		if (pPlayer1->GetDead()) {
			++i;
			continue;
		}
		j = 0;
		for (auto& pPlayer2 : m_ObjList[OBJ_PLAYER]) {
			if (j < i)
			{
				++j;
				continue;
			}
			if (pPlayer2->GetDead())
				continue;
			if (pPlayer1 == pPlayer2)
				continue;
			if (CollisionProcess::CollisionObjToObj(pPlayer1, pPlayer2)) {
				// pPlayer1에게 pPlayer2의 데미지
				pPlayer1->TakeDamageByObj(pPlayer2);
				// pPlayer2에게 pPlayer1의 데미지
				pPlayer2->TakeDamageByObj(pPlayer1);
			}
		}
		++i;
	}
}
