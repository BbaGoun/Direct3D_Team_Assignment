#pragma once
#include "Define.h"
#include "CObj.h"

class CEnemy;

class CTank
{
public:
    CTank();
    ~CTank();

public:

    virtual void Initialize() PURE;

    virtual void Fire(CEnemy* _Enemy) PURE;
    virtual void RanderPosin(HDC _hdc) PURE;

protected:

    D3DXVECTOR3 m_vLocalPosinPoint;
    D3DXVECTOR3 m_vWorldPosinPoint;
};