#pragma once
#include "CObj.h"

enum BREAKABLE_ID {
    BREAKABLE_TRIANGLE,
    BREAKABLE_SQUARE,
    BREAKABLE_PENTAGON,
    BREAKABLE_HEXAGON
};

class CBreakableObj :
    public CObj
{
public:
    CBreakableObj();
    ~CBreakableObj() override;

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
    vector<D3DXVECTOR3> m_vLocalPointVec;
    vector<D3DXVECTOR3> m_vWorldPointVec;

    bool m_bDamaged;
    float m_fInvincibleTimer;
    float m_fInvincibleTime;
    
    BREAKABLE_ID m_eType;
};

