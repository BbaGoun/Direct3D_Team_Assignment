#pragma once
#include "Define.h"

constexpr int VK_MAX = 0xff;

class KeyMgr
{
private:
	KeyMgr();
	KeyMgr(const KeyMgr& rhs) = delete;
	KeyMgr& operator=(const KeyMgr& rhs) = delete;
	~KeyMgr();

public:
	static KeyMgr& GetInstance() {
		static KeyMgr Instance;
		return Instance;
	}

public:
	bool KeyPressing(int iKey);
	bool KeyDown(int iKey);
	bool KeyUp(int iKey);
	void Key_Update();
	void Key_LateUpdate();

private:
	bool m_bGetAsyncKey[VK_MAX];
	bool m_bKeyState[VK_MAX];
	bool m_bKeyNextState[VK_MAX];
};

