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

    // CObj을(를) 통해 상속됨
    void Initialize() override;
    void Update() override;
    void LateUpdate() override;
    void Render(HDC _hDC) override;
    void Release() override;

public:
    //vector<D3DXVECTOR3> GetWorldBodyPoints() {
    //    return m_vWorldBodyPoints;}

public:
    void TakeDamage(int _iDamage) override {}

private:

    void KeyInput();
    void ChaingeTankType(TANKID _eID);

private:

    D3DXVECTOR3 m_vLocalBodyPoints[4];
    D3DXVECTOR3 m_vWorldBodyPoints[4];

    //vector<D3DXVECTOR3>m_vLocalBodyPoints;
    //vector<D3DXVECTOR3>m_vWorldBodyPoints;
    
    D3DXVECTOR3 m_vLocalPosinPoint;
    D3DXVECTOR3 m_vWorldPosinPoint;

    TANKID m_eNextTankID;
    TANKID m_eCurTankID;
    CTank* m_pTankStat;

    int m_iEXP;
    int m_iMaxEXP;

    int m_iLevel;

    int m_iDelay;

    //스테이터스 패턴을 통한 현재 탱크 상태 변경
    //스테이터스 패턴을 통해 현재 탱크 상태에 따른 렌더 변경.
    //포신의 위치와 형태를 스테이터스 내에서 변경하도록 하는걸로.
};

