#include "pch.h"
#include "ObjMgr.h"

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
