#pragma once
#include "CObj.h"
#include "Define.h"

//#include <vector>

#include "CTank.h"
#include "CTankType.h"

enum TANKID
{
    TANK_NOMAL,
    TANK_SHOTGUN,
    TANK_GUIDED,
    TANK_BOOSTER,
    TANK_SOMMONER,
    TANK_END
};

class CEnemy :
    public CObj
{
public:
    CEnemy();
    ~CEnemy() override;

public:

    // CObjÀ»(¸¦) ÅëÇØ »ó¼ÓµÊ
    void Initialize() override;
    void Update() override;
    void LateUpdate() override;
    void Render(HDC _hDC) override;
    void Release() override;

public:
    //vector<D3DXVECTOR3> GetWorldBodyPoints() {
    //    return m_vWorldBodyPoints;}
    void ReUpdateWorldVertex() override;
private:

public:
    void TakeDamage(int _iDamage) override {}

private:
    void DecelerationCurrentSpeed();
    void Accelerate(float _fFactor);

    void SetDelay(float _fDelay) { m_fDelay = _fDelay;}
    void SetBeMove(bool _bMove) { m_bMove = _bMove;}

    bool GetDelay() { return m_bMove;}
    float GetMove() { return m_fDelay;}

private:
    void KeyInput();
    void ChaingeTankType(TANKID _eID);
private:
   
    D3DXVECTOR3 m_vLocalPosinPoint;
    D3DXVECTOR3 m_vWorldPosinPoint;

    TANKID m_eNextTankID;
    TANKID m_eCurTankID;
    CTank* m_pTankStat;
    //int m_iEXP;
    //int m_iMaxEXP;
    //int m_iLevel;

    float m_fDelay;

    float m_fCurrentSpeed;
    float m_fAccel;        
    float m_fDecel;        

    bool m_bMove;


    // CObj��(��) ���� ��ӵ�
    void TakeDamage(int _iDamage) override;

    //�������ͽ� ������ ���� ���� ��ũ ���� ����
    //�������ͽ� ������ ���� ���� ��ũ ���¿� ���� ���� ����.
    //������ ��ġ�� ���¸� �������ͽ� ������ �����ϵ��� �ϴ°ɷ�.
};

