#pragma once

#include "CObj.h"

class CSummonee : public CObj
{
public:
	CSummonee();
	~CSummonee();

public:
	void Initialize() override;
	void Update() override;
	void LateUpdate() override;
	void Render(HDC _hDC) override;
	void Release() override;

public:
	void TakeDamage(int _iDamage) override;

private:
	D3DXVECTOR3 m_vLocalBodyPoints[4];
	D3DXVECTOR3 m_vWorldBodyPoints[4];

	D3DXVECTOR3 m_vLocalPosinPoint;
	D3DXVECTOR3 m_vWorldPosinPoint;
};