#include "pch.h"
#include "SceneMgr.h"
#include "Define.h"
#include "CMenu.h"
#include "CStage.h"
#include "ObjMgr.h"

SceneMgr::SceneMgr() : m_pScene(nullptr), m_eNextScene(SC_MENU),
m_eCurScene(SC_END)
{
}

SceneMgr::~SceneMgr()
{
	Release();
}

void SceneMgr::SceneChange(SCENEID sceneId)
{
	m_eNextScene = sceneId;
	m_eCurScene = SC_END;
	SceneChange();
}

void SceneMgr::SceneChange()
{
	if (m_eCurScene != m_eNextScene)
	{
		m_eCurScene = m_eNextScene;

		Safe_Delete(m_pScene);

		switch (m_eNextScene)
		{
		case SC_MENU:
			m_pScene = new CMenu;
			break;

		case SC_STAGE1:
			m_pScene = new CStage;
			break;
		}

		m_pScene->Initialize();
	}
}

void SceneMgr::Update()
{
	if (m_pScene)
		m_pScene->Update();
}

void SceneMgr::LateUpdate()
{
	if (m_pScene)
		m_pScene->LateUpdate();
}

void SceneMgr::Render(HDC _HDC)
{
	if (m_pScene)
		m_pScene->Render(_HDC);
}

void SceneMgr::Release()
{
	Safe_Delete(m_pScene);
}