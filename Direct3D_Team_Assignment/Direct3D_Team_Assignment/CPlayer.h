#pragma once
#include "CObj.h"
class CPlayer :
    public CObj
{
public:
    CPlayer();
    ~CPlayer() override;

public:
    // CObj을(를) 통해 상속됨
    void Initialize() override;
    void Update() override;
    void LateUpdate() override;
    void Render(HDC _hDC) override;
    void Release() override;

public:
    //vector GetWorldPoints() { return vector<D3DXVECTOR3> playerBodyWorldPoint = m_vWorldBodyPoints; }
    void ReUpdateWorldVertex() override;

private:
    void KeyInput();
    void AttackKeyInput();
    void UpdateTimers() override;
    void Dead() override;

private:
    D3DXVECTOR3 m_vLocalPosinVec;
    D3DXVECTOR3 m_vWorldPosinVec;
    D3DXVECTOR3 m_vViewPosinVec;
    D3DXVECTOR3 m_vProjPosinVec;

    D3DXVECTOR3 m_vLocalShotPosinVec[2];
    D3DXVECTOR3 m_vWorldShotPosinVec[2];
    D3DXVECTOR3 m_vViewShotPosinVec[2];
    D3DXVECTOR3 m_vProjShotPosinVec[2];

    D3DXVECTOR3 m_vLocalBackPosinVec[2];
    D3DXVECTOR3 m_vWorldBackPosinVec[2];
    D3DXVECTOR3 m_vViewBackPosinVec[2];
    D3DXVECTOR3 m_vProjBackPosinVec[2];

    D3DXVECTOR3 m_vLocalSummonerPosinVec[8];
    D3DXVECTOR3 m_vWorldSummonerPosinVec[8];
    D3DXVECTOR3 m_vViewSummonerPosinVec[8];
    D3DXVECTOR3 m_vProjSummonerPosinVec[8];

    bool m_bIsShootGun;
    bool m_bIsTargeted;
    bool m_bIsBooster;
    bool m_bIsSummoner;

    bool m_bAttacked;
    int m_iAttackDelay;

    bool m_bRespawn;
    float m_fRespawnTimer;
    int m_iSwitchDelay;

    float m_fGoBack;
};
