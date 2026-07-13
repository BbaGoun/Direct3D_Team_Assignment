#pragma once
#include "CObj.h"
#include "Define.h"

//#include <vector>

#include "CTank.h"
#include "CTankType.h"

enum TANKID
{
    TANK_NOMAL,
    TANK_SHOTGUN,
    TANK_GUIDED,
    TANK_BOOSTER,
    TANK_SOMMONER,
    TANK_END
};

class CEnemy :
    public CObj
{
public:
    CEnemy();
    ~CEnemy() override;

public:

    void Initialize() override;
    void Update() override;
    void LateUpdate() override;
    void Render(HDC _hDC) override;
    void Release() override;

public:
    void ReUpdateWorldVertex() override;
private:

public:
    void DecelerationCurrentSpeed();
    void Accelerate(float _fFactor);

    void SetDelayTime() { m_fDelayTime = m_fDelayTimer;}
    void SetDelayTimer(float _fDelay) { m_fDelayTimer = _fDelay;}

    void SetBeMove(bool _bMove) { m_bMove = _bMove;}

    bool GetMove() { return m_bMove;}
    float GetDelay() { return m_fDelayTime;}

private:

    void KeyInput();
    void ChaingeTankType(TANKID _eID);
    void UpdateTimers() override;
    void Dead() override;

private:

    TANKID m_eNextTankID;
    TANKID m_eCurTankID;
    CTank* m_pTankStat;

    float m_fDelayTime;
    float m_fDelayTimer;

    float m_fCurrentSpeed;
    float m_fAccel;        
    float m_fDecel;        

    bool m_bMove;

    bool m_bRespawn;
    float m_fRespawnTimer;

    int m_iCurLevel;
};

