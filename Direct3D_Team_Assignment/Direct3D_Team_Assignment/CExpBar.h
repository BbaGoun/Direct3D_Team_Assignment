#pragma once
#include "CUI.h"
class CExpBar :
    public CUI
{
public:
    CExpBar();
    ~CExpBar() override;

public:
    // CUI을(를) 통해 상속됨
    void Initialize() override;
    void Update() override;
    void LateUpdate() override;
    void Render(HDC _hDC) override;
    void Release() override;

private:
    D3DXVECTOR3 m_vExpRightTop;
    D3DXVECTOR3 m_vExpRightBottom;
};

