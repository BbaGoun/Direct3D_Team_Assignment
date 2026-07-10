#include "pch.h"
#include "CPreview.h"

CPreview::CPreview()
{
}

CPreview::~CPreview()
{
}

void CPreview::Initialize()
{
	m_tINFO.vPos = { 50, 50, 0 };

	m_vLocalVec.push_back({ -50.f, -50.f, 0 });
	m_vLocalVec.push_back({ 50.f, -50.f, 0 });
	m_vLocalVec.push_back({ 50.f, 50.f, 0 });
	m_vLocalVec.push_back({ -50.f, 50.f, 0 });
}

void CPreview::Update()
{
	D3DXMATRIX matWorld;
	D3DXMatrixIdentity(&matWorld);

	D3DXMATRIX matTrans;
	D3DXMatrixTranslation(&matTrans, m_tINFO.vPos.x, m_tINFO.vPos.y, m_tINFO.vPos.z);

	matWorld = matTrans;

	for (int i = 0; i < m_vLocalVec.size(); ++i) {
		D3DXVec3TransformCoord(&m_vWorldVec[i], &m_vLocalVec[i], &matWorld);
	}
}

void CPreview::LateUpdate()
{
}

void CPreview::Render(HDC _hDC)
{
}

void CPreview::Release()
{
}
