#pragma once

class CTexture;

class CCore
{
	SINGLE(CCore); // 동적할당 방식이 아니기 때문에 해제를 신경쓰지 않아도 된다. => 지울 수 없다.

private:
	HWND		m_hWnd;			// 메인 윈도우 핸들
	POINT		m_ptResolution; // 메인 윈도우 해상도
	HDC			m_hDC;			// 메인 윈도우에 Draw 할 DC

	CTexture*	m_pMemTex;		// 백버퍼 텍스쳐

	// 자주 사용하는 GDI Object
	HBRUSH		m_arrBrush[(UINT)BRUSH_TYPE::END];
	HPEN		m_arrPen[(UINT)PEN_TYPE::END];

	// 메뉴
	HMENU		m_hMenu;		// Tool Scene 에서만 사용

public:
	int init(HWND _hWnd, POINT _ptResolution);
	void progress();

private:
	void Clear();
	void CreateBrushPen();

public:
	void DockMenu();
	void DivideMenu();
	void ChangeWindowSize(Vec2 _vResolution, bool _bMenu);

public:
	HWND GetMainHWnd() { return m_hWnd; }
	HDC GetMainDC() { return m_hDC; }

	POINT GetResolution() { return m_ptResolution; }
	HBRUSH GetBrush(BRUSH_TYPE _eType) { return m_arrBrush[(UINT)_eType]; }
	HPEN GetPen(PEN_TYPE _eType) { return m_arrPen[(UINT)_eType]; }
};