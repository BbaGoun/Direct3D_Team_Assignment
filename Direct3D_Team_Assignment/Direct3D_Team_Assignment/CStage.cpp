#include "pch.h"
#include "CStage.h"
#include "AbstractFactory.h"
#include "CPlayer.h"
#include "ObjMgr.h"
#include "CEnemy.h"
#include "CBreakableObj.h"

CStage::CStage()
{
}

CStage::~CStage()
{
	Release();
}

void CStage::Initialize()
{
	CObj* pObj = AbstractFactory<CPlayer>::Create();
	pObj->SetPos({ 300, 360, 0 });
	ObjMgr::GetInstance().AddObject(OBJ_PLAYER, pObj);

	pObj = AbstractFactory<CEnemy>::Create();
	pObj->SetPos({ 980, 360, 0 });
	ObjMgr::GetInstance().AddObject(OBJ_ENEMY, pObj);

	for (int i = 0; i < 20; ++i) {
		pObj = AbstractFactory<CBreakableObj>::Create();
		pObj->SetPos({ dis(gen)/99.f * 1080 + 100, dis(gen)/99.f * 620 + 50, 0});
		ObjMgr::GetInstance().AddObject(OBJ_BREAKABLE, pObj);
	}

	m_bIsCameraRestricted = true;
	m_CameraRestrictZone = { 0, 0, 2560, 1440 };
}

void CStage::Update()
{
	ObjMgr::GetInstance().Update();
}

void CStage::LateUpdate()
{
	ObjMgr::GetInstance().LateUpdate();
}

void CStage::Render(HDC _HDC)
{
	ObjMgr::GetInstance().Render(_HDC);
}

void CStage::Release()
{
	ObjMgr::GetInstance().DeleteObj(OBJ_PLAYER);
	ObjMgr::GetInstance().DeleteObj(OBJ_ENEMY);
}
