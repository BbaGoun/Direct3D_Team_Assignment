#pragma once
#include "CObj.h"

template<typename T>
class AbstractFactory
{
public:
	static CObj* Create() {
		CObj* pObj = new T;
		pObj->Initialize();
		return pObj;
	}

	static CObj* Create(D3DXVECTOR3 _vDir, D3DXVECTOR3 _vPos, float _fSpeed) {
		CObj* pObj = new T;
		pObj->Initialize();

		pObj->SetDir(_vDir);
		pObj->SetPos(_vPos);
		//pObj->
		return pObj;
	}
};