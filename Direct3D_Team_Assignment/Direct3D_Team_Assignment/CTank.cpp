#include "pch.h"
#include "CTank.h"
#include "CEnemy.h"
#include "CameraMgr.h"

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

void CTank::PosUpdate(CEnemy* _Enemy)
{
	D3DXMATRIX matScale, matRotZ, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, 1, 1, 1);
	D3DXMatrixRotationZ(&matRotZ, _Enemy->GetRadian());
	D3DXMatrixTranslation(&matTrans, _Enemy->GetPos().x, _Enemy->GetPos().y, _Enemy->GetPos().z);

	D3DXMatrixIdentity(&matWorld);
	matWorld = matScale * matRotZ * matTrans;
	for(int i = 0; i < m_vLocalPosinPoint.size(); ++i) {
		D3DXVec3TransformCoord(&m_vWorldPosinPoint[i], &m_vLocalPosinPoint[i], &matWorld);
	}
}