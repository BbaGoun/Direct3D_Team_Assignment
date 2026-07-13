#pragma once
#include "CObj.h"

class CBulletDrone :
    public CObj
{
public:
    CBulletDrone();
    virtual ~CBulletDrone();

    void Initialize() override;
    void Update() override;
    void LateUpdate() override;
    void Render(HDC _hDC) override;
    void Release() override;

    void AutoFire();
private:

    float m_fDelayTime;
    float m_fDelayTimer;

    float m_fCurrentSpeed;
    float m_fAccel;
    float m_fDecel;

    bool m_bMove;
};