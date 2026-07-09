#pragma once
#include "Define.h"
#include "CObj.h"

class CTank
{
public:
    CTank();
    ~CTank();

public:
    //virtual void Rebound() PURE;
    virtual void Fire(D3DXVECTOR3 _vDir, D3DXVECTOR3 _vPos, float _fSpeed) PURE;
};

//할일: 반동 구현 완성. 가속도 넣을지 고민...우선순위에서는 밀림