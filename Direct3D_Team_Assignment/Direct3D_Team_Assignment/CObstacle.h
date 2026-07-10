#pragma once
#include "CObj.h"

enum OBSTACLE_ID {
    OBSTACLE_TRIANGLE,
    OBSTACLE_SQUARE,
    OBSTACLE_PENTAGON,
    OBSTACLE_HEXAGON,
    OBSTACLE_VERTICAL_WALL,
    OBSTACLE_HORIZONTAL_WALL
};

class CObstacle :
    public CObj
{
public:
    CObstacle();
    ~CObstacle() override;

public:
    // CObj을(를) 통해 상속됨
    void Initialize() override;
    void Update() override;
    void LateUpdate() override;
    void Render(HDC _hDC) override;
    void Release() override;
    void TakeDamage(int _iDamage) override;

public:
    void SetObstacle(OBSTACLE_ID _eID, D3DXVECTOR3 _vPos,float _fRadian);

private:
    OBSTACLE_ID m_eType;
};

