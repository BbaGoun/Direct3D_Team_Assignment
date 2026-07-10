#pragma once
#include "Define.h"
#include "CObj.h"

class CUI
{
public:
	CUI();
	virtual ~CUI() PURE;

public:
	virtual void Initialize() PURE;
	virtual void Update() PURE;
	virtual void LateUpdate() PURE;
	virtual void Render(HDC _hDC) PURE;
	virtual void Release() PURE;

public:
	CObj* GetTarget() { return m_pTarget; }

	void SetTarget(CObj* _pObj) { m_pTarget = _pObj; }

private:

protected:
	INFO m_tINFO;

	CObj* m_pTarget;

	vector<D3DXVECTOR3> m_vLocalVec;
	vector<D3DXVECTOR3> m_vWorldVec;
};

