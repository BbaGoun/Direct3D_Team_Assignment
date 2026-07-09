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

    D3DXVECTOR3 m_vLocalBodyPoints[4];
    D3DXVECTOR3 m_vWorldBodyPoints[4];

    D3DXVECTOR3 m_vLocalPosinPoint;
    D3DXVECTOR3 m_vWorldPosinPoint;
};

