#pragma once
#include "CObj.h"
class CDrone :
    public CObj
{
public:
    CDrone();
    ~CDrone();

    // CObj을(를) 통해 상속됨
    void Initialize() override;
    void Update() override;
    void LateUpdate() override;
    void Render(HDC _hDC) override;
    void Release() override;
    void TakeDamage(int _iDamage) override;

private:
    //다이렉션은 백터 추적으로,
    //1. 플레이어를 무조건 추적할지,2/ 근처 오브젝트를 추적할지 좀 고민해야할듯. 오브젝트 매니저에서 리스트 순회 하면서 pos 구하고 백터 정규화 전에 크기가 가장 작은 백터를 기준으로 다이렉션 잡고 총알 쏘면 될것같음.
};

