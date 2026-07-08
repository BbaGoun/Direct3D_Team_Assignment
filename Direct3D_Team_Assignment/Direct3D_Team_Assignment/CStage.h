#pragma once
#include "CScene.h"
#include "CObj.h"

class CStage :
    public CScene 
{
public:
    CStage();
    ~CStage() override;

public:
    // CScene을(를) 통해 상속됨
    void Initialize() override;
    void Update() override;
    void LateUpdate() override;
    void Render(HDC _HDC) override;
    void Release() override;

private:

private:

};