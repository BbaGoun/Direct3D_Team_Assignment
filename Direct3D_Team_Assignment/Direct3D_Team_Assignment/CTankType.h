#pragma once
#include "CTank.h"

#include "CEnemy.h"
#include "CBullet1.h"
#include "CDrone.h"
//#include "유도탄.h"

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

    // CTank을(를) 통해 상속됨
    void Initialize() override;
    void Fire(CEnemy* _Enemy) override;
    void RanderPosin(HDC _hdc) override;
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

private:

    list<CObj*> m_lDroneList;
};