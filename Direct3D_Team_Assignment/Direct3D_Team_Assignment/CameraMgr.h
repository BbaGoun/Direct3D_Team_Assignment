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
	const D3DXVECTOR3& GetCameraCenter() { return m_CameraWorldCenter; }
	const D3DXMATRIX& GetViewMat() { return m_matView; }
	const D3DXMATRIX& GetProjMat() { return m_matProj; }
	const RECT& GetCameraRect() { return m_CameraRect; }
	float GetProjScale() { return 1 / m_fCameraScale; }
	D3DXVECTOR3 ProjToScreen(const D3DXVECTOR3& _vProj) {
		return {
			(_vProj.x + 1.f) * WINCX * 0.5f,
			(_vProj.y + 1.f) * WINCY * 0.5f,
			_vProj.z
		};
	}
	D3DXVECTOR3 ScreenToProj(const D3DXVECTOR3& _vScreen) {
		return {
			_vScreen.x / (WINCX * 0.5f) - 1.f,
			_vScreen.y / (WINCY * 0.5f) - 1.f,
			_vScreen.z
		};
	}
	D3DXVECTOR3 ScreenToWorld(const D3DXVECTOR3& _vScreen) {
		D3DXVECTOR3 vProj = ScreenToProj(_vScreen);
		const float fovY = D3DXToRadian(90.f);
		const float viewZ = -m_CameraWorldCenter.z;
		const float halfViewY = viewZ * tanf(fovY * 0.5f);
		const float halfViewX = halfViewY * float(WINCX) / WINCY;
		D3DXVECTOR3 vView = {
			vProj.x * halfViewX,
			vProj.y * halfViewY,
			viewZ
		};
		D3DXMATRIX matInvView;
		D3DXVECTOR3 vWorld;
		D3DXMatrixInverse(&matInvView, 0, &m_matView);
		D3DXVec3TransformCoord(&vWorld, &vView, &matInvView);
		vWorld.z = 0;
		return vWorld;
	}

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

