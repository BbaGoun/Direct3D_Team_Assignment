#pragma once
#include "CObj.h"
class CameraMgr
{
private:
	CameraMgr();
	~CameraMgr();
	CameraMgr(const CameraMgr& rhs) = delete;
	CameraMgr& operator=(const CameraMgr& rhs) = delete;

public:
	static CameraMgr& GetInstance() {
		static CameraMgr Instance;
		return Instance;
	}

public:
	void LateUpdate();
	void Release();

public:
	const D3DXMATRIX& GetViewMat() { return m_matView; }
	const D3DXMATRIX& GetProjMat() { return m_matProj; }
	const RECT& GetCameraRect() { return m_CameraRect; }
	float GetProjScale() { return 1 / m_fCameraScale; }

	void Set_CameraCenter(D3DXVECTOR3 _CameraCenter) {
		m_CameraWorldCenter = _CameraCenter;
		m_CameraRect = {
			int(m_CameraWorldCenter.x - 1280 * m_fCameraScale * 0.5f),
			int(m_CameraWorldCenter.y - 720 * m_fCameraScale * 0.5f),
			int(m_CameraWorldCenter.x + 1280 * m_fCameraScale * 0.5f),
			int(m_CameraWorldCenter.y + 720 * m_fCameraScale * 0.5f),
		};
	}

	void Set_Target1(CObj* _pObj) { m_pTarget1 = _pObj; }
	void Set_Target2(CObj* _pObj) { m_pTarget2 = _pObj; }

	void Set_Shake(float _fShakeRadian);

private:
	D3DXVECTOR3 m_CameraWorldCenter;
	RECT m_CameraRect;
	float m_fCameraScale;

	D3DXVECTOR3 m_ShakeDir;
	float m_fShakeDistance;
	float m_fMulRadian;

	CObj* m_pTarget1;
	CObj* m_pTarget2;

	D3DXMATRIX m_matView;
	D3DXMATRIX m_matProj;
};

