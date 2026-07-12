#pragma once
#include "CScene.h"

class SceneMgr
{
private:
	SceneMgr();
	SceneMgr(const SceneMgr& rhs) = delete;
	SceneMgr& operator=(const SceneMgr& rhs) = delete;
	~SceneMgr();

public:
	void	SceneChange(SCENEID sceneId);
	void	SceneChange();

public:
	void	Update();
	void	LateUpdate();
	void	Render(HDC _HDC);
	void	Release();

public:
	static SceneMgr& GetInstance() {
		static SceneMgr Instance;
		return Instance;
	}

	static void DestroyInstance() {
		GetInstance().Release();
	}

public:
	bool Get_SceneRestrictZone(RECT* _pRECT) {
		if (!m_pScene)
			return false;

		*_pRECT = m_pScene->Get_CameraRestrictZone();

		return m_pScene->Get_CameraRestricted();
	}

private:
	CScene* m_pScene;

	SCENEID	m_eNextScene;
	SCENEID m_eCurScene;
};

