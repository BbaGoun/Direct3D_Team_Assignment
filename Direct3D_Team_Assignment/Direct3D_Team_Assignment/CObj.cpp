#include "pch.h"
#include "CObj.h"
#include "ObjMgr.h"
#include "TimeMgr.h"

CObj::CObj() : m_eObjID(OBJ_END),
m_fSpeed(5), m_fRadian(0), m_iHP(100), m_iMaxHP(100), 
m_iDamage(10), m_bDead(false), m_pParent(nullptr),
m_iCurrentExp(1), m_iMaxExp(3), m_iLevel(1), m_iDropExp(0),
m_fInvincibleTime(0.25f), m_fInvincibleTimer(0)
{
}

CObj::~CObj()
{
}

void CObj::TakeDamageByBullet(CObj* _pBullet)
{
	m_iHP -= _pBullet->GetDamage();
	m_fInvincibleTimer = m_fInvincibleTime;
	if (m_iHP <= 0) {
		_pBullet->GetParent()->GainExp(m_iDropExp);
		Dead();
	}
}

void CObj::TakeDamageByObj(CObj* _pPlayer)
{
	m_iHP -= _pPlayer->GetDamage();
	m_fInvincibleTimer = m_fInvincibleTime;
	if (m_iHP <= 0) {
		_pPlayer->GainExp(m_iDropExp);
		Dead();
	}
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

void CObj::Dead()
{
	ObjMgr::GetInstance().DeleteSpecificObj(m_eObjID, this);
}

void CObj::UpdateTimers()
{
	m_fInvincibleTimer = max(0.f, m_fInvincibleTimer - TimeMgr::GetInstance().GetDeltaTime());
}
