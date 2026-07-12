#pragma once
#include "CTank.h"
#include "CEnemy.h"
#include "CBullet1.h"
#include "CBulletTrakin1.h"

class CTankNomal :
    public CTank
{
public:
    virtual ~CTankNomal() {}        

    void Fire(CEnemy* _Enemy) override;
    void RanderPosin(HDC _hdc) override;
    void Initialize(CEnemy* _Enemy) override;
};

class CTankShotGun :
    public CTank
{
public:
    virtual ~CTankShotGun() {}

    void Fire(CEnemy* _Enemy) override;
    void RanderPosin(HDC _hdc) override;
    void Initialize(CEnemy* _Enemy) override;
};

class CTankGuided :
    public CTank
{
public:
    void Initialize(CEnemy* _Enemy) override;
    void Fire(CEnemy* _Enemy) override;
    void RanderPosin(HDC _hdc) override;
};

class CTankBooster :
    public CTank
{
public:
    void Fire(CEnemy* _Enemy) override;
    void RanderPosin(HDC _hdc) override;
    void Initialize(CEnemy* _Enemy) override;
protected:
};

class CTankSommoner :
    public CTank
{
public:
    void Fire(CEnemy* _Enemy) override;
    void RanderPosin(HDC _hdc) override;
    void Initialize(CEnemy* _Enemy) override;

private:
    list<CObj*> m_lDroneList;
};