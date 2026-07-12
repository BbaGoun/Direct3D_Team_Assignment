#include "pch.h"
#include "UIMgr.h"

UIMgr::UIMgr()
{
}

UIMgr::~UIMgr()
{
	Release();
}

void UIMgr::Update()
{
	DeleteListPostProcess();

	for (int i = 0; i < UI_END; ++i) {
		for (auto& pUI : m_UIList[i]) {
			pUI->Update();

			if (m_UIList[i].empty())
				break;
		}
	}

	DeleteListPostProcess();
}

void UIMgr::LateUpdate()
{
	DeleteListPostProcess();

	for (int i = 0; i < UI_END; ++i) {
		for (auto& pUI : m_UIList[i]) {
			pUI->LateUpdate();

			if (m_UIList[i].empty())
				break;
		}
	}

	DeleteListPostProcess();
}

void UIMgr::Render(HDC _HDC)
{
	for (int i = 0; i < UI_END; ++i) {
		for (auto& pUI : m_UIList[i]) {
			pUI->Render(_HDC);
		}
	}
}

void UIMgr::Release()
{
	for (auto& uiList : m_UIList) {
		for (auto& pUI : uiList) {
			Safe_Delete(pUI);
		}
		uiList.clear();
	}
}

void UIMgr::AddUI(UIID eID, CUI* _pUI)
{
	m_UIList[eID].push_back(_pUI);
}

void UIMgr::DeleteUI(UIID eID)
{
	for (auto& pUI : m_UIList[eID])
		Safe_Delete(pUI);

	m_UIList[eID].clear();
}

void UIMgr::DeleteSpecificUI(UIID eID, CUI* _pUI)
{
	m_DeleteList[eID].push_back(_pUI);
}

void UIMgr::DeleteListPostProcess()
{
	bool bFind = false;
	for (int i = 0; i < UI_END; ++i) {
		for (auto it = m_UIList[i].begin(); it != m_UIList[i].end(); ) {
			auto find_it = find(m_DeleteList[i].begin(), m_DeleteList[i].end(), *it);

			if (find_it != m_DeleteList[i].end()) {
				Safe_Delete(*it);
				it = m_UIList[i].erase(it);
			}
			else
				++it;
		}
		m_DeleteList[i].clear();
	}
}
