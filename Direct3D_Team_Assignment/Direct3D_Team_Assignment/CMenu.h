#pragma once
#include "CScene.h"

class CMenu :
	public CScene 
{
public:
	CMenu();
	~CMenu() override;

public:
	// CScene을(를) 통해 상속됨
	void Initialize() override;
	void Update() override;
	void LateUpdate() override;
	void Render(HDC _HDC) override;
	void Release() override;

private:

private:
};