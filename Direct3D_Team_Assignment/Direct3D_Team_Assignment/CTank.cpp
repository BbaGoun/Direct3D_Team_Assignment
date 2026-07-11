#include "pch.h"
#include "CTank.h"
#include "CEnemy.h"

CTank::CTank()
{
}

CTank::~CTank()
{
}

void CTank::SetDelayAndRebound(CEnemy* _Enemy)
{
	_Enemy->Accelerate(m_fRebound);
	_Enemy->SetDelayTime();
}