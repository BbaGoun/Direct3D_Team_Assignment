#pragma once
#include "CObj.h"
#include "Define.h"

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

private:
    void KeyInput();

private:

    D3DXVECTOR3 m_vLocalBodyPoints[4];
    D3DXVECTOR3 m_vWorldBodyPoints[4];

    D3DXVECTOR3 m_vLocalPosinPoint;
    D3DXVECTOR3 m_vWorldPosinPoint;

    enum TANKID
    {
        TANK_NOMAL,
        TANK_SHOTGUN,
        TANK_GUIDED,
        TANK_BOOSTER,
        TANK_SOMMONER,
        TANK_END
    };

    TANKID m_eTankID;

    //스테이터스 패턴을 통한 현재 탱크 상태 변경
    //스테이터스 패턴을 통해 현재 탱크 상태에 따른 렌더 변경.
    //포신의 위치와 형태를 스테이터스 내에서 변경하도록 하는걸로.
};

