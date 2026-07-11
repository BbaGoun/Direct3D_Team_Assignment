#include "pch.h"
#include "CStage.h"
#include "AbstractFactory.h"
#include "CPlayer.h"
#include "ObjMgr.h"
#include "CEnemy.h"
#include "CBreakableObj.h"
#include "UIMgr.h"
#include "CHPBar.h"
#include "UIFactory.h"
#include "CExpBar.h"
#include "CObstacle.h"
#include "CameraMgr.h"
#include "TimeMgr.h"

CStage::CStage()
{
}

CStage::~CStage()
{
	Release();
}

void CStage::Initialize()
{
	// 플레이어 1
	CObj* pPlayer1 = AbstractFactory<CPlayer>::Create();
	pPlayer1->SetPos({ 640, 360, 0 });
	ObjMgr::GetInstance().AddObject(OBJ_PLAYER, pPlayer1);

	CUI* pPlayer1HP = UIFactory<CHPBar>::Create();
	pPlayer1HP->SetTarget(pPlayer1);
	UIMgr::GetInstance().AddUI(UI_NORMAL, pPlayer1HP);

	CUI* pPlayer1Exp = UIFactory<CExpBar>::Create();
	pPlayer1Exp->SetTarget(pPlayer1);
	UIMgr::GetInstance().AddUI(UI_NORMAL, pPlayer1Exp);

	// 플레이어 2
	CObj* pPlayer2 = AbstractFactory<CEnemy>::Create();
	pPlayer2->SetPos({ 1920, 1080, 0 });
	ObjMgr::GetInstance().AddObject(OBJ_PLAYER, pPlayer2);

	CUI* pPlayer2HP = UIFactory<CHPBar>::Create();
	pPlayer2HP->SetTarget(pPlayer2);
	UIMgr::GetInstance().AddUI(UI_NORMAL, pPlayer2HP);

	CUI* pPlayer2Exp = UIFactory<CExpBar>::Create();
	pPlayer2Exp->SetTarget(pPlayer2);
	UIMgr::GetInstance().AddUI(UI_NORMAL, pPlayer2Exp);

	// 카메라 설정
	CameraMgr::GetInstance().Set_Target1(pPlayer1);
	CameraMgr::GetInstance().Set_Target2(pPlayer2);

	CObj* pObj;
	// 중앙 장애물
	for (int i = 0; i < 2; ++i) {
		pObj = AbstractFactory<CObstacle>::Create();
		static_cast<CObstacle*>(pObj)->SetObstacle(OBSTACLE_TRIANGLE, { 320+i*1280.f, 180+i*720.f, 0 }, 0);
		ObjMgr::GetInstance().AddObject(OBJ_OBSTACLE, pObj);

		pObj = AbstractFactory<CObstacle>::Create();
		static_cast<CObstacle*>(pObj)->SetObstacle(OBSTACLE_SQUARE, { 960 + i * 1280.f, 180 + i * 720.f, 0 }, 0);
		ObjMgr::GetInstance().AddObject(OBJ_OBSTACLE, pObj);

		pObj = AbstractFactory<CObstacle>::Create();
		static_cast<CObstacle*>(pObj)->SetObstacle(OBSTACLE_PENTAGON, { 320 + i * 1280.f, 540 + i * 720.f, 0 }, 0);
		ObjMgr::GetInstance().AddObject(OBJ_OBSTACLE, pObj);

		pObj = AbstractFactory<CObstacle>::Create();
		static_cast<CObstacle*>(pObj)->SetObstacle(OBSTACLE_HEXAGON, { 960 + i * 1280.f, 540 + i * 720.f, 0 }, 0);
		ObjMgr::GetInstance().AddObject(OBJ_OBSTACLE, pObj);
	}

	// 상하좌우 벽
	pObj = AbstractFactory<CObstacle>::Create();
	static_cast<CObstacle*>(pObj)->SetObstacle(OBSTACLE_VERTICAL_WALL, { -100, 720, 0 }, 0);
	ObjMgr::GetInstance().AddObject(OBJ_OBSTACLE, pObj);

	pObj = AbstractFactory<CObstacle>::Create();
	static_cast<CObstacle*>(pObj)->SetObstacle(OBSTACLE_VERTICAL_WALL, { 2660, 720, 0 }, 0);
	ObjMgr::GetInstance().AddObject(OBJ_OBSTACLE, pObj);

	pObj = AbstractFactory<CObstacle>::Create();
	static_cast<CObstacle*>(pObj)->SetObstacle(OBSTACLE_HORIZONTAL_WALL, { 1280, -100, 0 }, 0);
	ObjMgr::GetInstance().AddObject(OBJ_OBSTACLE, pObj);

	pObj = AbstractFactory<CObstacle>::Create();
	static_cast<CObstacle*>(pObj)->SetObstacle(OBSTACLE_HORIZONTAL_WALL, { 1280, 1540, 0 }, 0);
	ObjMgr::GetInstance().AddObject(OBJ_OBSTACLE, pObj);

	for (int i = 0; i < 10; ++i) {
		pObj = AbstractFactory<CBreakableObj>::Create();
		pObj->SetPos({ dis(gen) / 99.f * 2360 + 100, dis(gen) / 99.f * 1340 + 50, 0 });
		ObjMgr::GetInstance().AddObject(OBJ_BREAKABLE, pObj);
	}

	m_bIsCameraRestricted = true;
	m_CameraRestrictZone = { 0, 0, 2560, 1440 };

	m_fSpawnTimer = 5.f;
}

void CStage::Update()
{
	SpawnBreakable();
	ObjMgr::GetInstance().Update();
	UIMgr::GetInstance().Update();
}

void CStage::LateUpdate()
{
	ObjMgr::GetInstance().LateUpdate();
	UIMgr::GetInstance().LateUpdate();

}

void CStage::Render(HDC _HDC)
{
	ObjMgr::GetInstance().Render(_HDC);
	UIMgr::GetInstance().Render(_HDC);
}

void CStage::Release()
{
	ObjMgr::GetInstance().DeleteObj(OBJ_PLAYER);
	UIMgr::GetInstance().DeleteUI(UI_NORMAL);
}

void CStage::SpawnBreakable()
{
	m_fSpawnTimer = max(0.f, m_fSpawnTimer - TimeMgr::GetInstance().GetDeltaTime());
	if (m_fSpawnTimer <= 0.f) {
		for (int i = 0; i < 5; ++i) {
			CObj* pObj = AbstractFactory<CBreakableObj>::Create();
			pObj->SetPos({ dis(gen) / 99.f * 2360 + 100, dis(gen) / 99.f * 1340 + 50, 0 });
			ObjMgr::GetInstance().AddObject(OBJ_BREAKABLE, pObj);
		}
		m_fSpawnTimer = 5.f;
	}
}
