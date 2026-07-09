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
	const float& GetSpeed() { return m_fSpeed; }
	const float& GetRadian() { return m_fRadian; }
	const int& GetHP() { return m_iHP; }
	const int& GetMaxHP() { return m_iMaxHP; }
	const int& GetDamage() { return m_iDamage; }
	const vector<D3DXVECTOR3>& GetWorldVertex() { return m_vWorldVec; }

	void SetPos(D3DXVECTOR3 _vPos) { m_tINFO.vPos = _vPos; }
	void SetDir(D3DXVECTOR3 _vDir) { m_tINFO.vDir = _vDir; }
	void SetLook(D3DXVECTOR3 _vLook) { m_tINFO.vLook = _vLook; }
	void SetNormal(D3DXVECTOR3 _vNormal) { m_tINFO.vNormal = _vNormal; }
	void SetSpeed(float _fSpeed) { m_fSpeed = _fSpeed; }
	void SetRadian(float _fRadian) { m_fRadian = _fRadian; }
	void SetHP(int _iHP) { m_iHP = _iHP; }
	void SetMaxHP(int _iMaxHP) { m_iMaxHP = _iMaxHP; }
	void SetDamage(int _iDamage) { m_iDamage = _iDamage; }

protected:

protected:
	INFO m_tINFO;
	float m_fSpeed;
	float m_fRadian;

	int m_iHP;
	int m_iMaxHP;

	int m_iDamage;

	vector<D3DXVECTOR3> m_vLocalVec;
	vector<D3DXVECTOR3> m_vWorldVec;
};

