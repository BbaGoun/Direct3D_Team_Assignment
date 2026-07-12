#pragma once
#include "CObj.h"
#include "Define.h"

class CBullet1
	:public CObj
{
public:
	CBullet1();
	virtual ~CBullet1();

	// CObj을(를) 통해 상속됨
	void Initialize() override;
	void Update() override;
	void LateUpdate() override;
	void Render(HDC _hDC) override;
	void Release() override;


private:
	float m_fDuration;
	bool m_bBeTraking;

	//각종 총알용 스탯 정보 
};