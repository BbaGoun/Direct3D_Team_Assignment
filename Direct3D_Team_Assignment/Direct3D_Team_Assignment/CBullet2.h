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

public:
    int GetRadiance() { return (m_vWorldBodyPoint[0].x - m_vWorldBodyPoint[1].x) * 0.5f; }
    void SetSpeed(float speed) { m_fSpeed = speed; }

private:
    D3DXVECTOR3 m_vLocalBodyPoint[4];
    D3DXVECTOR3 m_vWorldBodyPoint[4];

    // CObj을(를) 통해 상속됨
    void TakeDamage(int _iDamage) override;
};