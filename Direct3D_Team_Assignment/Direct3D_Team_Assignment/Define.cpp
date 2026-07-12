#include "pch.h"
#include "Define.h"

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<int> dis(0, 99);

// dis(gen)을 하면 0~99 사이의 숫자가 나옴
// 이를 통해 랜덤을 구현