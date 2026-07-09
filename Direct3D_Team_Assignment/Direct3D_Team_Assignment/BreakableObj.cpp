#include "pch.h"
#include "BreakableObj.h"

BreakableObj::BreakableObj()
{
}

BreakableObj::~BreakableObj()
{
}

void BreakableObj::Initialize()
{
	if (dis(gen) < 25) { // 삼각형
		//\(V_1 = (0, \frac{\sqrt{3}}{6}a, 0)\)\(V_2 = (-\frac{1}{2}a, -\frac{\sqrt{3}}{6}a, 0)\)\(V_3 = (\frac{1}{2}a, -\frac{\sqrt{3}}{6}a, 0)\)
		float deltaX = 25 * 0.5f;
		float deltaY = 25.f * sqrtf(3) / 6;
		m_vList.push_back({ 0, -deltaY, 0});
		m_vList.push_back({ deltaX, deltaY, 0 });
		m_vList.push_back({ -deltaX, deltaY, 0 });
	}
	else if (dis(gen) < 50) { // 사각형

	}
	else if (dis(gen) < 75) { // 오각형

	}
	else { // 육각형

	}
}

void BreakableObj::Update()
{
}

void BreakableObj::LateUpdate()
{
}

void BreakableObj::Render(HDC _hDC)
{
}

void BreakableObj::Release()
{
}
