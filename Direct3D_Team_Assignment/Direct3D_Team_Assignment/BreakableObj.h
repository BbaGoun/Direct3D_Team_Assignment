#pragma once
#include "CObj.h"

enum BREAKABLE_ID {
    BREAKABLE_TRIANGLE,
    BREAKABLE_SQUARE,
    BREAKABLE_PENTAGON,
    BREAKABLE_HEXAGON
};

class BreakableObj :
    public CObj
{
public:
    BreakableObj();
    ~BreakableObj() override;

public:
    // CObj을(를) 통해 상속됨
    void Initialize() override;
    void Update() override;
    void LateUpdate() override;
    void Render(HDC _hDC) override;
    void Release() override;

private:

public:

private:
    list<D3DXVECTOR3> m_vList;
    bool m_bDamaged;
    float m_fInvincibleTimer;
    float m_fInvincibleTime;
};

