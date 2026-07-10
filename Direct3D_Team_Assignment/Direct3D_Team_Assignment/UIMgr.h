#pragma once
#include "Define.h"
#include "CUI.h"

enum UIID {
	UI_NORMAL,
	UI_END
};

class UIMgr
{
private:
	UIMgr();
	UIMgr(const UIMgr& rhs) = delete;
	UIMgr& operator=(const UIMgr& rhs) = delete;
	~UIMgr();

public:
	static UIMgr& GetInstance() {
		static UIMgr Instance;
		return Instance;
	}

public:
	void Update() ;
	void LateUpdate();
	void Render(HDC _HDC);
	void Release();

public:
	void AddUI(UIID eID, CUI* _pUI);
	void DeleteUI(UIID eID);
	void DeleteSpecificUI(UIID eID, CUI* _pUI);
	void DeleteListPostProcess();

private:
	list<CUI*>		m_UIList[UI_END];
	list<CUI*>		m_DeleteList[UI_END];
};

