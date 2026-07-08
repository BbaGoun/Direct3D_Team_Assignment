#pragma once

enum SCENEID {
	SC_MENU,
	SC_STAGE1,
	SC_END
};

class CScene
{
public:
	CScene();
	virtual ~CScene() PURE;

public:
	virtual void	Initialize()	PURE;
	virtual void	Update()		PURE;
	virtual void	LateUpdate()	PURE;
	virtual void	Render(HDC _HDC) PURE;
	virtual void	Release()		PURE;

public:
	void Set_CameraRestricted(bool _b) { m_bIsCameraRestricted = _b; }
	void Set_CameraRestrictZone(RECT _rect) { m_CameraRestrictZone = _rect; }
	void Set_Paused(bool _b) { m_bPaused = _b; }

	bool Get_CameraRestricted() { return m_bIsCameraRestricted; }
	RECT Get_CameraRestrictZone() { return m_CameraRestrictZone; }
	bool Get_Paused() { return m_bPaused; }

protected:
	bool	m_bIsCameraRestricted;
	RECT	m_CameraRestrictZone;

	bool	m_bPaused;
};

