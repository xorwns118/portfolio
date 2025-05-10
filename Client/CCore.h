#pragma once

// �̱��� ����
// ��ü�� ������ 1���� ����
// ��𼭵� ���� ���� ����
//class CCore
//{
//private:
//	static CCore* g_pInst;
//
//public:
//	 ���� ��� �Լ�
//	static CCore* GetInstance()
//	{
//		 ���� ȣ�� �� ���
//		if (g_pInst == nullptr)
//		{
//			g_pInst = new CCore;
//		}
//
//		 �޸� ������ �̹� �ִٸ�, nullptr ��ȯ
//		return g_pInst;
//	}
//
//	static void Release()
//	{
//		if (g_pInst != nullptr)
//		{
//			delete g_pInst;
//			g_pInst = nullptr;
//		}
//	}
//
//private:
//	CCore()
//	{}
//	~CCore()
//	{}
//};

class CTexture;

class CCore
{
	SINGLE(CCore); // �����Ҵ� ����� �ƴϱ� ������ ������ �Ű澲�� �ʾƵ� �ȴ�. => ���� �� ����.

private:
	HWND		m_hWnd;			// ���� ������ �ڵ�
	POINT		m_ptResolution; // ���� ������ �ػ�
	HDC			m_hDC;			// ���� �����쿡 Draw �� DC

	CTexture*	m_pMemTex;		// ����� �ؽ���

	// ���� ����ϴ� GDI Object
	HBRUSH		m_arrBrush[(UINT)BRUSH_TYPE::END];
	HPEN		m_arrPen[(UINT)PEN_TYPE::END];

	// �޴�
	HMENU		m_hMenu;		// Tool Scene ������ ���

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