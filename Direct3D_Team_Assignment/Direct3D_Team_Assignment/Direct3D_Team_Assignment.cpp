// DefaultWindow.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include "Direct3D_Team_Assignment.h"
#include "CMainGame.h"
#include "Define.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.
HWND g_hWnd;

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);   // 창 스타일 지정 옵션
BOOL                InitInstance(HINSTANCE, int);           // 창 생성 관련 함수
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);    // 메세지 처리기
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);      // 쓸데 없음

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,         // 창이 생성되면서 발생한 인스턴스 정보를 저장하는 곳
    _In_opt_ HINSTANCE hPrevInstance,  // 이 프로그램이 시작되기 전에 이전 프로그램의 인스턴스를 받는 곳(없으면 NULL)
    _In_ LPWSTR    lpCmdLine,          // LP(포인터) W(유니코드) STR(문자열)
    _In_ int       nCmdShow)           // 창 생성 옵션
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    HMODULE hUser = LoadLibraryW(L"user32.dll");
    if (hUser)
    {
        auto pSetCtx = reinterpret_cast<BOOL(WINAPI*)(DPI_AWARENESS_CONTEXT)>(
            GetProcAddress(hUser, "SetProcessDpiAwarenessContext"));
        if (pSetCtx)
        {
            // 최신 권장: 모니터별 DPI v2
            pSetCtx(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
        }
        else
        {
            // 폴백(구 API)
            SetProcessDPIAware();
        }
        FreeLibrary(hUser);
    }

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_DIRECT3DTEAMASSIGNMENT, szWindowClass, MAX_LOADSTRING);

    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DIRECT3DTEAMASSIGNMENT));

    MSG msg;
    msg.message = WM_NULL;

    CMainGame MainGame;
    MainGame.Initialize();

    // 기본 메시지 루프입니다:
    while (true)
    {
        // PM_REMOVE   : 메세지 큐에 메세지가 있을 경우, 가지고 오면서 큐로부터 메세지를 삭제
        // PM_NOREMOVE : 메세지 큐에 메세지 유무 상태만 파악, 만약 큐에 있는 메세지를 가져오려면 개별적으로 GetMessage 함수를 다시 호출해야 함

        // PeekMessage : 메세지가 없을 경우 거짓을 반환

        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
                break;

            if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }

        else
        {
            MainGame.RunGame();
        }
    }

    return (int)msg.wParam;
}   // 창 생성 옵션



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;       // 윈도우 창 만들 때, 필요한 구조체

    wcex.cbSize = sizeof(WNDCLASSEX);       // 자기 자신의 사이즈를 저장해 놓음

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    // 윈도우 창의 스타일을 정의, 어떤 형태를 가질 것인지 지정

    // CS_HREDRAW : 가로 다시 그리기
    // CS_VREDRAW : 세로 다시 그리기


    wcex.lpfnWndProc = WndProc;  // 저장해둔 함수가 호출되어 메세지를 처리

    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    // 윈도우가 특수한 목적으로 사용하는 여분의 공간

    wcex.hInstance = hInstance;

    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DIRECT3DTEAMASSIGNMENT));
    // 윈도우 창이 사용할 아이콘 지정

    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    // 마우스 커서

    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    // 윈도우 창 배경 색상

    wcex.lpszMenuName = NULL; //MAKEINTRESOURCEW(IDC_DEFAULTWINDOW);

    wcex.lpszClassName = szWindowClass;
    // 실행파일 이름 문자열(대개 프로젝트 이름을 기본 값 설정)

    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
    // 창 상단에 출력하는 아이콘 모양

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

    // 핸들(HANDLE) : 리소스마다 부여되는 고유 식별 번호, 16진수 형태의 정수로 제공
    RECT rcWindow{ 0, 0, WINCX, WINCY };

    AdjustWindowRect(&rcWindow, WS_OVERLAPPEDWINDOW, FALSE);

    // rcWindow = rcWindow + 기본 창 옵션을 고려한 사이즈 적용 + 메뉴 바의 크기 고려 여부

    HWND hWnd = CreateWindowW(szWindowClass,
        szTitle,
        WS_OVERLAPPEDWINDOW,        // 만들고자 하는 윈도우 창의 형태(스타일) 지정 옵션, 현재 값은 기본 창의 형태 옵션을 의미
        CW_USEDEFAULT, 0,           // LEFT, TOP 좌표
        rcWindow.right - rcWindow.left,
        rcWindow.bottom - rcWindow.top,                   // 가로, 세로 사이즈
        nullptr,                    // 만약 부모 윈도우가 있다면 부모 윈도우의 핸들을 지정, 없을 경우 NULLPTR
        nullptr,                    // 윈도우에서 사용할 메뉴의 핸들 지정  
        hInstance,
        nullptr);                   // 운영체제가 특수한 목적으로 사용

    if (!hWnd)
    {
        return FALSE;
    }

    g_hWnd = hWnd;

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_KEYDOWN:
        switch (wParam)
        {
        case VK_ESCAPE:
            PostQuitMessage(0);
            break;
        }
        break;

    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // 메뉴 선택을 구문 분석합니다:
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}