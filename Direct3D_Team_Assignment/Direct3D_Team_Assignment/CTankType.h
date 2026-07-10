#pragma once
#include "CTank.h"

class CTankNomal :
    public CTank
{
public:
    virtual ~CTankNomal() {}

    // CTank을(를) 통해 상속됨
    void Fire(CEnemy* _Enemy) override;
    void RanderPosin(HDC _hdc) override;

    // CTank을(를) 통해 상속됨
    void Initialize() override;
};
class CTankShotGun :
    public CTank
{
public:
    virtual ~CTankShotGun() {}

    // CTank을(를) 통해 상속됨
    void Fire(CEnemy* _Enemy) override;
    void RanderPosin(HDC _hdc) override;

    // CTank을(를) 통해 상속됨
    void Initialize() override;
};

class CTankGuided :
    public CTank
{
public:
};

class CTankBooster :
    public CTank
{
public:

    // CTank을(를) 통해 상속됨
    void Fire(CEnemy* _Enemy) override;
    void RanderPosin(HDC _hdc) override;

    // CTank을(를) 통해 상속됨
    void Initialize() override;
protected:
    vector<D3DXVECTOR3> m_vLBostedPosinPoint;
    vector<D3DXVECTOR3> m_vWBostedPosinPoint;
};

class CTankSommoner :
    public CTank
{
public:

    // CTank을(를) 통해 상속됨
    void Fire(CEnemy* _Enemy) override;
    void RanderPosin(HDC _hdc) override;

    // CTank을(를) 통해 상속됨
    void Initialize() override;
};