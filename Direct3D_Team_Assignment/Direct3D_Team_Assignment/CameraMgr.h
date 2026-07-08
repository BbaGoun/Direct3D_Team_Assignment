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
	void Set_CameraCenter(D3DXVECTOR3 _CameraCenter) {
		m_CameraWorldCenter = _CameraCenter;
		m_CameraWorldRect = { int(m_CameraWorldCenter.x - 640), int(m_CameraWorldCenter.y - 360), 1280, 720 };
	}

	void Set_Target1(CObj* _pObj) { m_pTarget1 = _pObj; }
	void Set_Target2(CObj* _pObj) { m_pTarget2 = _pObj; }

	void MoveX(float _fX) {
		m_CameraWorldCenter.x += _fX;
		m_CameraWorldRect = { int(m_CameraWorldCenter.x - 640), int(m_CameraWorldCenter.y - 360), 1280, 720 };
	}
	void MoveY(float _fY) {
		m_CameraWorldCenter.y += _fY;
		m_CameraWorldRect = { int(m_CameraWorldCenter.x - 640), int(m_CameraWorldCenter.y - 360), 1280, 720 };
	}

	const RECT& Get_CameraRect() { return m_CameraWorldRect; }

	void Convert_VirtualToWorldPoint(D3DXVECTOR3* _vPoint) {
		(*_vPoint).x += m_CameraWorldRect.left;
		(*_vPoint).y += m_CameraWorldRect.top;
	}

	void Set_Shake(float _fShakeRadian);

private:
	D3DXVECTOR3 m_CameraWorldCenter;
	RECT m_CameraWorldRect;

	D3DXVECTOR3 m_ShakeDir;
	float m_fShakeDistance;
	float m_fMulRadian;

	CObj* m_pTarget1;
	CObj* m_pTarget2;
};

