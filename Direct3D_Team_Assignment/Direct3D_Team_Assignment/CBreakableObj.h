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
    void TakeDamage(int _iDamage) override;
    void TakeDamageByBullet(int _iDamage, CObj* pBullet);
    void TakeDamageByPlayer(int _iDamage, CObj* pPlayer);

private:
    BREAKABLE_ID m_eType;
};

