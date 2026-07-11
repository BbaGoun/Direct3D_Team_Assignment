#pragma once
#include "CUI.h"
class CHPBar :
    public CUI
{
public:
    CHPBar();
    ~CHPBar() override;

public:
    // CUI을(를) 통해 상속됨
    void Initialize() override;
    void Update() override;
    void LateUpdate() override;
    void Render(HDC _hDC) override;
    void Release() override;

private:
    D3DXVECTOR3 m_vHPRightTop;
    D3DXVECTOR3 m_vHPRightBottom;
};

