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

    virtual void Initialize(CEnemy* _Enemy) PURE;

    virtual void Fire(CEnemy* _Enemy) PURE;
    virtual void RanderPosin(HDC _hdc) PURE;

    void SetDelayAndRebound(CEnemy* _Enemy);

protected:

    float m_fDelayTime;
    float m_fRebound;

    D3DXVECTOR3 m_vLocalPosinPoint;
    D3DXVECTOR3 m_vWorldPosinPoint;
};