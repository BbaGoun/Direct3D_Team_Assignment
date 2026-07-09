#pragma once
#include "CTank.h"
#include "Define.h"

class CTankNomal :
    public CTank
{
public:
    CTankNomal();
    ~CTankNomal();

    void Fire(D3DXVECTOR3 _vDir, D3DXVECTOR3 _vPos, float _fSpeed) override;

};
class CTankShotGun :
    public CTank
{
public:
    CTankShotGun();
    ~CTankShotGun();

    void Fire(D3DXVECTOR3 _vDir, D3DXVECTOR3 _vPos, float _fSpeed) override;

};