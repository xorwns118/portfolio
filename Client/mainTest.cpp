// main ���� ����

//#include "framework.h"
//#include "Client.h"
#include "pch.h"
//#include "CCore.h"

// ���� ����:
//HINSTANCE hInst;                                // ���� �ν��Ͻ��Դϴ�.
//HWND      g_hwnd;                               // ���� ������
//WCHAR szTitle[MAX_LOADSTRING];                  // ���� ǥ���� �ؽ�Ʈ�Դϴ�.
//WCHAR szWindowClass[MAX_LOADSTRING];            // �⺻ â Ŭ���� �̸��Դϴ�.

// �� �ڵ� ��⿡ ���Ե� �Լ��� ������ �����մϴ�:
//ATOM                MyRegisterClass(HINSTANCE hInstance);
//BOOL                InitInstance(HINSTANCE, int);
//LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
//INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

// ����
// ����
// ���� (������ ����)
// 1. �Լ� �ȿ�
// 2. ����
// 3. Ŭ���� �ȿ�
// �ܺ�
//class CClass
//{
//private:
//    int m_i;
//
//    static int m_iStatic; // ���� ��� (������ ����)
//
//public:
//    void func()
//    {
//        m_i = 0;
//        m_iStatic = 0;
//    }
//
//    // ���� ����Լ�, ��ü���� ȣ�� ����, this �� ����(��� ���� �Ұ�), ���� ����� ���� ����
//    static void Func()
//    {
//        // this�� private�� ��� ���� ȣ�� �Ұ����ϱ� ������ ���� ����Լ��� ȣ�� �� �� �ִ°�
//        m_iStatic = 0; // this Ű���� ��� �Ұ�
//    }
//};

//int CClass::m_iStatic = 0; // ������ �ʱ⿡ �ö󰡾��ϴ� ���� �ݵ�� �ʱ�ȭ�ؾ���. ���ϸ� ��ũ����
//
//// SAL �ּ� _In_ : �����Ͱ� �Լ��� �Էµȴ�. _In_opt_ : �ΰ����� ������ (optional)
//int APIENTRY wWinMain(_In_ HINSTANCE hInstance, /*���� �� ���μ����� ���� �ּ�(���� �޸��� �ּ�, �������� �ּҴ� �ٸ�)*/
//    _In_opt_ HINSTANCE hPrevInstance, /*���� ���μ��� ���� �ּ� (��â�� ������ ���, ������ ������ ����)*/
//    _In_ LPWSTR lpCmdLine, /*CMD ��ɾ�� �⺻ ���α׷� ������ �� �ɼ� ���� ��, ������ �ٷ�鼭 ���� ���� ����*/
//    _In_ int nCmdShow)
//{
//    UNREFERENCED_PARAMETER(hPrevInstance); // ���� �ǹ� ���ٴ� ���� �˷��� (������ �ʴ´�.)
//    UNREFERENCED_PARAMETER(lpCmdLine); // �����Ϸ��� �˾Ƽ� ������.
//
//    // TODO: ���⿡ �ڵ带 �Է��մϴ�.
//    //CCore* pCore = CCore::GetInstance();
//
//    // ���� ���ڿ��� �ʱ�ȭ�մϴ�.
//    //LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
//    //LoadStringW(hInstance, IDC_CLIENT, szWindowClass, MAX_LOADSTRING);
//
//    // ������ ���� ���
//    MyRegisterClass(hInstance);
//
//    // ������ ����
//    if (!InitInstance(hInstance, nCmdShow))
//    {
//        return FALSE;
//    }
//
//    // Core �ʱ�ȭ
//    if (FAILED(CCore::GetInst()->init(g_hwnd, POINT{ 1280, 768 })))
//    {
//        MessageBox(nullptr, L"Core ��ü �ʱ�ȭ ����", L"ERROR", MB_OK);
//
//        return FALSE;
//    }
//
//    // ����Ű ���̺� ���� �ε�
//    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CLIENT));
//
//    MSG msg;
//
//    // ������ �޼��� �̺�Ʈ�� ���� ��츦 ����� �̺�Ʈ�� ������ ����Ű�� Ÿ�̸�, Kernel Obj
//    //SetTimer(g_hwnd, 10, 0, nullptr);
//
//    // �⺻ �޽��� �����Դϴ�:
//    // msg ����ü�� ���� ä���ֱ� ���� �ּҰ��� �޾ƿ�
//
//    // GetMessage
//    // �޼��� ť���� �޼����� Ȯ�ε� �� ���� ���
//    // msg.message == WM_QUIT �� ��� false �� ��ȯ -> ���α׷� ����
//
//    // PeekMessage
//    // �޼��� ������ ������� ��ȯ
//    // �޼���ť���� �޼����� Ȯ���� ��� true, �޼����� ���� ��� false�� ��ȯ�Ѵ�.
//    // Peek ���� ����. => �޼����� ��¦ ���ڴ�. => �޼����� Ȯ�ε� �� ���� ��� �� �ʿ䰡 ����.
//
//    while (true) // �⺻���� �����Ǵ� GetMessage �� �̿��� ������ ���� �� ������ ��ȿ������, ������ ���·� �ٲ�
//    {
//        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
//        {
//            if (msg.message == WM_QUIT)
//                break;
//
//            if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
//            {
//                TranslateMessage(&msg);
//                DispatchMessage(&msg);
//            }
//        }
//        // �޼����� �߻����� �ʴ� ��κ��� �ð�
//        else
//        {
//            // Game code ����
//            // ������ ����(���� ����)
//            // ��Ȳ���� �ٸ��� ���� Ư���� ��� ǥ���ϱ� ������, �˾Ƶθ� ������ ���� ������ �������� ����.
//            // ********�̱��� ������ �߿�********
//
//            CCore::GetInst()->progress();
//        }
//    }
//
//    // kernel obj ����
//    //KillTimer(g_hwnd, 10);
//
//    return (int)msg.wParam;
//}
//
//
//
////
////  �Լ�: MyRegisterClass()
////
////  �뵵: â Ŭ������ ����մϴ�.
////
//ATOM MyRegisterClass(HINSTANCE hInstance)
//{
//    WNDCLASSEXW wcex;
//
//    wcex.cbSize = sizeof(WNDCLASSEX);
//
//    wcex.style = CS_HREDRAW | CS_VREDRAW;
//    wcex.lpfnWndProc = WndProc;
//    wcex.cbClsExtra = 0;
//    wcex.cbWndExtra = 0;
//    wcex.hInstance = hInstance;
//    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CLIENT));
//    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
//    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
//    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_CLIENT); // �޴���
//    //wcex.lpszClassName = szWindowClass;
//    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
//
//    return RegisterClassExW(&wcex);
//}
//
////
////   �Լ�: InitInstance(HINSTANCE, int)
////
////   �뵵: �ν��Ͻ� �ڵ��� �����ϰ� �� â�� ����ϴ�.
////
////   �ּ�:
////
////        �� �Լ��� ���� �ν��Ͻ� �ڵ��� ���� ������ �����ϰ�
////        �� ���α׷� â�� ���� ���� ǥ���մϴ�.
////
//BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
//{
//    hInst = hInstance; // �ν��Ͻ� �ڵ��� ���� ������ �����մϴ�.
//
//   // g_hwnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
//        //CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);
//
//    if (!g_hwnd)
//    {
//        return FALSE;
//    }
//
//    ShowWindow(g_hwnd, nCmdShow);
//    UpdateWindow(g_hwnd);
//
//    return TRUE;
//}
//
////
////  �Լ�: WndProc(HWND, UINT, WPARAM, LPARAM)
////
////  �뵵: �� â�� �޽����� ó���մϴ�.
////
////  WM_COMMAND  - ���ø����̼� �޴��� ó���մϴ�.
////  WM_PAINT    - �� â�� �׸��ϴ�.
////  WM_DESTROY  - ���� �޽����� �Խ��ϰ� ��ȯ�մϴ�.
//
//#include <vector>
//
//using std::vector;
//
//struct tObjInfo
//{
//    POINT g_ptObjPos;
//    POINT g_ptObjScale;
//};
//
//vector<tObjInfo> g_vecInfo;
//
//// �� ���
//POINT g_ptLT;
//// �� �ϴ�
//POINT g_ptRB;
//
//bool bLBtnDown = false;
//
//LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
//{
//    switch (message)
//    {
//    case WM_COMMAND:
//    {
//        int wmId = LOWORD(wParam);
//        // �޴� ������ ���� �м��մϴ�:
//        switch (wmId)
//        {
//        case IDM_ABOUT:
//            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
//            break;
//        case IDM_EXIT:
//            DestroyWindow(hWnd);
//            break;
//        default:
//            return DefWindowProc(hWnd, message, wParam, lParam);
//        }
//    }
//    break;
//    case WM_PAINT: // ��ȿȭ ����(Invalidate)�� �߻��� ���
//    {
//        PAINTSTRUCT ps;
//        // TODO: ���⿡ hdc�� ����ϴ� �׸��� �ڵ带 �߰��մϴ�...
//
//        // ������ �ڵ� : Ŀ�� ������Ʈ�� �ٷ� �� �ִ� ID(�ڵ�) ��
//        // ������ ��ǥ
//        // HDC ?
//
//        // Device Context ���� ID �� ��ȯ
//        HDC hdc = BeginPaint(hWnd, &ps); // Device Context (�׸���) �׸��� �۾� ����, �ʿ��� Data ����
//        // DC �� �������� hWnd
//        // DC �� ���� �⺻��(Black)
//        // DC �� �귯���� �⺻ �귯��(White)
//
//        // ���� ���� ���� DC �� ����
//        HPEN hRedPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
//        HBRUSH hBlueBrush = CreateSolidBrush(RGB(0, 0, 255));//GetStockObject(); �̸� �������� ���־��� ������Ʈ�� ������
//
//        // Ÿ�Կ� ���� �ٲ��� �ϱ� ������ ��ȯŸ���� Ư�� �����Ͱ� �ƴ� void ������ ����� ���, ���� ĳ������ �ʿ���
//        HPEN hDefaultPen = (HPEN)SelectObject(hdc, hRedPen); // �⺻ Pen ID ���� �޾Ƶ�
//        HBRUSH hDefaultBrush = (HBRUSH)SelectObject(hdc, hBlueBrush); // �⺻ Brush ID ���� �޾Ƶ�
//
//        // ����� ������ �簢�� �׸�
//        if (bLBtnDown)
//            Rectangle(hdc, g_ptLT.x, g_ptLT.y, g_ptRB.x, g_ptRB.y); // pixel ����
//
//        // �߰��� �簢���鵵 �׷��ش�.
//        for (size_t i = 0; i < g_vecInfo.size(); ++i)
//        {
//            Rectangle(hdc
//                , g_vecInfo[i].g_ptObjPos.x - g_vecInfo[i].g_ptObjScale.x / 2
//                , g_vecInfo[i].g_ptObjPos.y - g_vecInfo[i].g_ptObjScale.y / 2
//                , g_vecInfo[i].g_ptObjPos.x + g_vecInfo[i].g_ptObjScale.x / 2
//                , g_vecInfo[i].g_ptObjPos.y + g_vecInfo[i].g_ptObjScale.y / 2);
//        }
//
//        // DC�� ��� �귯���� ������ ������ �ǵ���
//        SelectObject(hdc, hDefaultPen);
//        SelectObject(hdc, hDefaultBrush);
//
//        // �� �� ��, �귯�� ���� ��û
//        DeleteObject(hRedPen);
//        DeleteObject(hDefaultBrush);
//
//        // �׸��� ����
//        EndPaint(hWnd, &ps);
//    }
//    break;
//    case WM_KEYDOWN:
//    {
//        switch (wParam)
//        {
//        case VK_UP:
//            //g_ptObjPos.y -= 10;
//            InvalidateRect(hWnd, nullptr, true);
//
//            break;
//
//        case VK_DOWN:
//            //g_ptObjPos.y += 10;
//            InvalidateRect(hWnd, nullptr, true);
//
//            break;
//
//        case VK_LEFT:
//            //g_ptObjPos.x -= 10;
//            InvalidateRect(hWnd, nullptr, true);
//
//            break;
//
//        case VK_RIGHT:
//            //g_ptObjPos.x += 10;
//            InvalidateRect(hWnd, nullptr, true);
//
//            break;
//        }
//    }
//    break;
//
//    case WM_LBUTTONDOWN:
//        g_ptLT.x = LOWORD(lParam); // ���콺 x ��ǥ
//        g_ptLT.y = HIWORD(lParam); // ���콺 y ��ǥ
//        bLBtnDown = true;
//        break;
//
//    case WM_MOUSEMOVE:
//        g_ptRB.x = LOWORD(lParam);
//        g_ptRB.y = HIWORD(lParam);
//        //InvalidateRect(hWnd, nullptr, true);
//        break;
//
//    case WM_LBUTTONUP:
//    {
//        tObjInfo info = {};
//        info.g_ptObjPos.x = (g_ptLT.x + g_ptRB.x) / 2;
//        info.g_ptObjPos.y = (g_ptLT.y + g_ptRB.y) / 2;
//
//        info.g_ptObjScale.x = abs(g_ptLT.x - g_ptRB.x);
//        info.g_ptObjScale.y = abs(g_ptLT.y - g_ptRB.y);
//
//        g_vecInfo.push_back(info);
//        bLBtnDown = false;
//        InvalidateRect(hWnd, nullptr, true);
//    }
//    break;
//
//    case WM_TIMER:
//        break;
//
//    case WM_DESTROY:
//        PostQuitMessage(0);
//        break;
//    default:
//        return DefWindowProc(hWnd, message, wParam, lParam);
//    }
//    return 0;
//}
//
//// ���� ��ȭ ������ �޽��� ó�����Դϴ�.
//INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
//{
//    UNREFERENCED_PARAMETER(lParam);
//    switch (message)
//    {
//    case WM_INITDIALOG:
//        return (INT_PTR)TRUE;
//
//    case WM_COMMAND:
//        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
//        {
//            EndDialog(hDlg, LOWORD(wParam));
//            return (INT_PTR)TRUE;
//        }
//        break;
//    }
//    return (INT_PTR)FALSE;
//}
//