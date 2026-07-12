#pragma once

#include "CObj.h"

class CTargetBullet2 : public CObj
{
public:
	CTargetBullet2();
	~CTargetBullet2();

public:
	void Initialize() override;
	void Update() override;
	void LateUpdate() override;
	void Render(HDC _hDC) override;
	void Release() override;

public:
	void SetTarget(CObj* pTarget) { m_pTarget = pTarget; }

public:
	void FollowTarget(CObj* pTarget);

private:
	D3DXVECTOR3 m_vWorldVec;
	D3DXVECTOR3 m_vLocalVec;
	D3DXVECTOR3 m_vViewVec;
	D3DXVECTOR3 m_vProjVec;

	CObj* m_pTarget;
};