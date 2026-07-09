#pragma once
#include "CObj.h"
#include "Define.h"
class CTank :
    public CObj
{
public:
    CTank();
    virtual ~CTank();

    // CObj을(를) 통해 상속됨
    void Initialize() override;
    void Update() override;
    void LateUpdate() override;
    void Render(HDC _hDC) override;
    void Release() override;

public:
    void KeyInput();//공격키
    void MakeBullet();

private:
    list<CObj*> m_pList;
};

