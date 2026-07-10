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

public:
    void TakeDamage(int _iDamage) override;

private:
    void KeyInput();
    void AttackKeyInput();

private:
    D3DXVECTOR3 m_vLocalPosinPoint;
    D3DXVECTOR3 m_vWorldPosinPoint;

    D3DXVECTOR3 m_vLocalShotPosinPoint[2];
    D3DXVECTOR3 m_vWorldShotPosinPoint[2];

    D3DXVECTOR3 m_vLocalBackPosinPoint[2];
    D3DXVECTOR3 m_vWorldBackPosinPoint[2];

    D3DXVECTOR3 m_vLocalSummonerPosinPoint[8];
    D3DXVECTOR3 m_vWorldSummonerPosinPoint[8];

    bool m_bIsShootGun;

    // CObj��(��) ���� ��ӵ�
    bool m_bIsTargeted;
    bool m_bIsBooster;
    bool m_bIsSummoner;

    bool m_bAttacked;
    int m_iAttackDelay;

    // CObj을(를) 통해 상속됨
    void TakeDamage(int _iDamage) override;
};
