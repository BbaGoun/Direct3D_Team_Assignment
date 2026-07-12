#include "pch.h"
#include "CScene.h"

CScene::CScene() : m_bIsCameraRestricted(false),
m_CameraRestrictZone({}), m_bPaused(false)
{
}

CScene::~CScene()
{
}
