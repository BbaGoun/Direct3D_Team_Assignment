#pragma once
#include "CObj.h"
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

    //플레이어 둘이 나뉘어 있으므로... 
    //스탯 정보를 오브젝트에 멤버로 추가해야 하는지 검토 필요. 기본스탯은 플레이어랑 통일 해야할것 같긴 함
    //대강 이런 느낌의 구조, 아니면 Define에 넣거나. 
    //----------------------
    //typedef struct tagStat
    //{
    //    int t_iHP;
    //    int t_iMaxHP;
    //    int t_iLevel;
    //    int t_iMaxLevel;
    //    int t_iEXP;
    //}STAT;
    //----------------------

    D3DXVECTOR3 m_vLocalBodyPoints[4];
    D3DXVECTOR3 m_vWorldBodyPoints[4];

    D3DXVECTOR3 m_vLocalPosinPoint;
    D3DXVECTOR3 m_vWorldPosinPoint;
};

