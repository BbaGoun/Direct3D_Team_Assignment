#pragma once
#include "CUI.h"
class CPreview :
    public CUI
{
public:
    CPreview();
    ~CPreview() override;

public:
    // CUI을(를) 통해 상속됨
    void Initialize() override;
    void Update() override;
    void LateUpdate() override;
    void Render(HDC _hDC) override;
    void Release() override;
};

