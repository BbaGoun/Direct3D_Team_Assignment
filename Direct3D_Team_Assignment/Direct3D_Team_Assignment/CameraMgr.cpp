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
	if (m_pTarget1) {
		D3DXVECTOR3 cameraCenter;
		int width = 1280;
		int height = 720;

		if (m_pTarget2) {
			D3DXVECTOR3 worldPos1 = m_pTarget1->GetPos();

			D3DXVECTOR3 worldPos2 = m_pTarget2->GetPos();

			cameraCenter = {
				(worldPos1.x + worldPos2.x) * 0.5f,
				(worldPos1.y + worldPos2.y) * 0.5f,
				0
			};
		}

		RECT cameraRECT = {
			int(cameraCenter.x - (width >> 1)),
			int(cameraCenter.y - (height >> 1)),
			width,
			height
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
