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
#include "KeyMgr.h"

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
	pPlayer2->SetParent(pPlayer1);
	ObjMgr::GetInstance().AddObject(OBJ_PLAYER, pPlayer2);
  //플레이어 1
	pPlayer1->SetParent(pPlayer2);

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
	pObj = AbstractFactory<CObstacle>::Create();
	static_cast<CObstacle*>(pObj)->SetObstacle(OBSTACLE_TRIANGLE, { 320, 230, 0 }, 0);
	ObjMgr::GetInstance().AddObject(OBJ_OBSTACLE, pObj);

	pObj = AbstractFactory<CObstacle>::Create();
	static_cast<CObstacle*>(pObj)->SetObstacle(OBSTACLE_SQUARE, { 960, 230, 0 }, 0);
	ObjMgr::GetInstance().AddObject(OBJ_OBSTACLE, pObj);

	pObj = AbstractFactory<CObstacle>::Create();
	static_cast<CObstacle*>(pObj)->SetObstacle(OBSTACLE_PENTAGON, { 320, 590, 0 }, 0);
	ObjMgr::GetInstance().AddObject(OBJ_OBSTACLE, pObj);

	pObj = AbstractFactory<CObstacle>::Create();
	static_cast<CObstacle*>(pObj)->SetObstacle(OBSTACLE_HEXAGON, { 960, 590, 0 }, 0);
	ObjMgr::GetInstance().AddObject(OBJ_OBSTACLE, pObj);

	for (int i = 0; i < 4; ++i) {
		pObj = AbstractFactory<CObstacle>::Create();
		static_cast<CObstacle*>(pObj)->SetObstacle(OBSTACLE_ROTATION_CROSS_PART, { 1280, 720, 0 }, i * D3DXToRadian(90));
		ObjMgr::GetInstance().AddObject(OBJ_OBSTACLE, pObj);
	}

	pObj = AbstractFactory<CObstacle>::Create();
	static_cast<CObstacle*>(pObj)->SetObstacle(OBSTACLE_ORBIT, { 1920, 1030, 0 }, 0);
	ObjMgr::GetInstance().AddObject(OBJ_OBSTACLE, pObj);
	
	pObj = AbstractFactory<CObstacle>::Create();
	static_cast<CObstacle*>(pObj)->SetObstacle(OBSTACLE_ORBIT, { 1920, 1030, 0 }, D3DXToRadian(180));
	ObjMgr::GetInstance().AddObject(OBJ_OBSTACLE, pObj);

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
	KeyInput();
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

void CStage::KeyInput()
{
	if (KeyMgr::GetInstance().KeyDown(VK_LBUTTON)) {
		D3DXVECTOR3 vMouse = GetMouse();
	
		// 뷰포트 -> 투영 차원
		vMouse -= {640, 360, 0};
		// z곱하기, 투영 차원 -> 클립 차원
		float originZ = -CameraMgr::GetInstance().GetCameraCenter().z;
		vMouse *= originZ;

		// 클립 차원 -> 뷰 차원
		D3DXMATRIX matProj = CameraMgr::GetInstance().GetProjMat();
		D3DXMatrixInverse(&matProj, 0, &matProj);
		// proj -> view
		D3DXVec3TransformCoord(&vMouse, &vMouse, &matProj);

		// 뷰 차원 -> 월드 차원
		D3DXMATRIX matView = CameraMgr::GetInstance().GetViewMat();
		D3DXMatrixInverse(&matView, 0, &matView);
		// view -> world
		D3DXVec3TransformCoord(&vMouse, &vMouse, &matView);

		vMouse.z = 0;
		for (int i = 0; i < 12; ++i) {
			D3DXMATRIX matRotZ;
			D3DXVECTOR3 vDir = { 0, 1, 0 };
			D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(i * 30));
			D3DXVec3TransformNormal(&vDir, &vDir, &matRotZ);
			CObj* pObj;
			pObj = AbstractFactory<CBullet1>::Create(vDir, vMouse, 15.f);
			pObj->SetParent(nullptr);
			ObjMgr::GetInstance().AddObject(OBJ_BULLET, pObj);
		}
	}
}
