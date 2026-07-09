#pragma once
#include "Define.h"

class CObj
{
public:
	CObj();
	virtual ~CObj() PURE;

public:
	virtual void Initialize() PURE;
	virtual void Update() PURE;
	virtual void LateUpdate() PURE;
	virtual void Render(HDC _hDC) PURE;
	virtual void Release() PURE;

public:
	const D3DXVECTOR3& GetPos() { return m_tINFO.vPos; }
	const D3DXVECTOR3& GetDir() { return m_tINFO.vDir; }
	const D3DXVECTOR3& GetLook() { return m_tINFO.vLook; }
	const D3DXVECTOR3& GetNormal() { return m_tINFO.vNormal; }

	void SetPos(D3DXVECTOR3 _vPos) { m_tINFO.vPos = _vPos; }
	void SetDir(D3DXVECTOR3 _vDir) { m_tINFO.vDir = _vDir; }
	void SetLook(D3DXVECTOR3 _vLook) { m_tINFO.vLook = _vLook; }
	void SetNormal(D3DXVECTOR3 _vNormal) { m_tINFO.vNormal = _vNormal; }

protected:

protected:
	INFO m_tINFO;
	float m_fSpeed;
	float m_fRadian;

	int iHP;
	int iMaxHP;

	int iDamage;
};

