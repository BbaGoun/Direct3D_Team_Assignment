#pragma once
#include "Define.h"
#include "CObj.h"


class CTank
{
public:
    CTank();
    ~CTank();

public:
    virtual void Fire(D3DXVECTOR3 _vDir, D3DXVECTOR3 _vPos, float _fSpeed) = 0;
};