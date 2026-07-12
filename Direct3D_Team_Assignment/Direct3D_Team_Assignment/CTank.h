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

    void PosUpdate(CEnemy* _Enemy);
    
    float GetPosinRadian(D3DXVECTOR3 _posinPoint, D3DXVECTOR3 _PlayerPoint);

protected:

    float m_fDelayTime;
    float m_fRebound;

    vector<D3DXVECTOR3> m_vLocalPosinPoint;
    vector<D3DXVECTOR3> m_vWorldPosinPoint;

};