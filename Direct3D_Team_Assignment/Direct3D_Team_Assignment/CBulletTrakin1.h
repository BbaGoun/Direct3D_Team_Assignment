#pragma once
#include "CBullet1.h"
class CBulletTrakin1 :
    public CObj
{
public:

    CBulletTrakin1();
    virtual ~CBulletTrakin1();
    // CObj을(를) 통해 상속됨
    void Initialize() override;
    void Update() override;
    void LateUpdate() override;
    void Render(HDC _hDC) override;
    void Release() override;

    float GetRadius() { return m_fSize / 2; }

    void SetTarget(CObj* _pTarget) { m_pEnemy = _pTarget; }

private:

    float m_fSize;
    float m_fDuration;
    bool m_bBeTraking;

    CObj* m_pEnemy;
};
