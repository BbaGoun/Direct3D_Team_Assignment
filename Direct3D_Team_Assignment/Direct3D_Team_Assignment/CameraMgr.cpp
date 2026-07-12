#include "pch.h"
#include "CameraMgr.h"
#include "SceneMgr.h"

CameraMgr::CameraMgr() :m_pTarget1(nullptr), m_pTarget2(nullptr)
{
}

CameraMgr::~CameraMgr()
{
}

void CameraMgr::LateUpdate() {
	if (m_pTarget1 && m_pTarget2) {
		D3DXVECTOR3 cameraCenter;
		
		D3DXVECTOR3 worldPos1 = m_pTarget1->GetPos();

		D3DXVECTOR3 worldPos2 = m_pTarget2->GetPos();

		cameraCenter = {
			(worldPos1.x + worldPos2.x) * 0.5f,
			(worldPos1.y + worldPos2.y) * 0.5f,
			0
		};

		float gapX = fabsf(worldPos1.x - worldPos2.x);
		float gapY = fabsf(worldPos1.y - worldPos2.y);
		float scaleX = min(2.f, max(1.f, gapX / 1024));
		float scaleY = min(2.f, max(1.f, gapY / 576));
		m_fCameraScale = max(scaleX, scaleY);
		cameraCenter.z = -m_fCameraScale;

		RECT cameraRECT = {
			int(cameraCenter.x - 1280 * m_fCameraScale * 0.5f),
			int(cameraCenter.y - 720 * m_fCameraScale * 0.5f),
			int(cameraCenter.x + 1280 * m_fCameraScale * 0.5f),
			int(cameraCenter.y + 720 * m_fCameraScale * 0.5f),
		};

		RECT rc{};
		if (SceneMgr::GetInstance().Get_SceneRestrictZone(&rc)) {
			if (cameraRECT.left < rc.left) {
				int gap = rc.left - cameraRECT.left;
				cameraCenter.x += gap;
			}
			if (rc.right < cameraRECT.right) {
				int gap = cameraRECT.right - rc.right;
				cameraCenter.x -= gap;
			}
			if (cameraRECT.top < rc.top) {
				int gap = rc.top - cameraRECT.top;
				cameraCenter.y += gap;
			}
			if (rc.bottom < cameraRECT.bottom) {
				int gap = cameraRECT.bottom - rc.bottom;
				cameraCenter.y -= gap;
			}
		}

		if (m_fShakeDistance > 0) {
			cameraCenter.x += m_ShakeDir.x * m_fShakeDistance * cosf(m_fMulRadian);
			cameraCenter.y += m_ShakeDir.y * m_fShakeDistance * cosf(m_fMulRadian);
			m_fShakeDistance -= 1;
			m_fMulRadian += 0.7854f;
		}

		D3DXVECTOR3 vAt = cameraCenter;
		vAt.z = 1;
		D3DXVECTOR3 vUp = { 0, 1, 0 };
		D3DXMatrixLookAtLH(&m_matView,
			&cameraCenter,
			&vAt,
			&vUp);
		D3DXMatrixPerspectiveFovLH(&m_matProj,
			D3DXToRadian(90),
			1,
			1,
			100
		);

		Set_CameraCenter(cameraCenter);
	}
}

void CameraMgr::Release() {

}

void CameraMgr::Set_Shake(float _fShakeRadian)
{
	m_ShakeDir = { cosf(_fShakeRadian), -sinf(_fShakeRadian), 0 };
	m_fShakeDistance += 10;
	m_fMulRadian = 0;
}
