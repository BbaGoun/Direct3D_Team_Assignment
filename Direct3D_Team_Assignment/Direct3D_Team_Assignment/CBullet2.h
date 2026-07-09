#pragma once
#include "CObj.h"
class CBullet2 :
    public CObj
{
public:
    CBullet2();
    ~CBullet2();

public:
    void Initialize() override;
    void Update() override;
    void LateUpdate() override;
    void Render(HDC hDC) override;
    void Release() override;

private:
    D3DXVECTOR3 m_vLocalBodyPoint[4];
    D3DXVECTOR3 m_vWorldBodyPoint[4];
};