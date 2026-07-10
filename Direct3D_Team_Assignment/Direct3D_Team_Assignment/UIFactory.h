#pragma once
#include "CUI.h"

template<typename T>
class UIFactory
{
public:
	static CUI* Create() {
		CUI* pUI = new T;
		pUI->Initialize();
		return pUI;
	}
};