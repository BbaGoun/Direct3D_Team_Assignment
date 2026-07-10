#include "pch.h"
#include "CObj.h"

CObj::CObj() :
m_fSpeed(5), m_fRadian(0), m_iHP(100), m_iMaxHP(100), 
m_iDamage(10), m_bDead(false), m_pParent(nullptr),
m_iCurrentExp(1), m_iMaxExp(3), m_iLevel(1), m_iDropExp(5),
m_fInvincibleTime(1), m_fInvincibleTimer(0)
{
}

CObj::~CObj()
{
}

void CObj::GainExp(int _iExp)
{
	m_iCurrentExp += _iExp;
	if (m_iCurrentExp >= m_iMaxExp) {
		m_iCurrentExp %= m_iMaxExp;
		m_iMaxExp += 1;
		m_iLevel += 1;
	}
}