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
};

