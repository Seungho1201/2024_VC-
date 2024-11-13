// ProjectSecond.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "ProjectSecond.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_PROJECTSECOND, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PROJECTSECOND));

    MSG msg;

    // 기본 메시지 루프입니다:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PROJECTSECOND));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_PROJECTSECOND);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
/// 현재 좌표(lParam)  
int x, y;
/// 이전 좌표
int preX, preY;
// 그리기 여부
bool drawStart = false;

// 실행시 기본 값은 자유선 그리기로 정한다
int drawMode = 1;


HBRUSH myBrush, osBrush;
HPEN myPen, osPen;

COLORREF paintColor = RGB(255, 255, 255);
COLORREF lineColor = RGB(0, 0, 0);


HDC hdc;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
 
    switch (message)
    {
    /// WM_LBUTTONDOWN
    case WM_USER:
    {
        hdc = GetDC(hWnd);

        preX = LOWORD(lParam);
        preY = HIWORD(lParam);

        /// 좌클릭시 마우스 좌표 저장
        MoveToEx(hdc, preX, preY, NULL);
    }
    break;

    /// WM_MOUSEMOVE
    case WM_USER + 1:
    {
        /// 자유곡선 모드가 아닐시 그리기 중단
        if (drawMode != 1) { break; }

        myPen = CreatePen(PS_SOLID, 1, lineColor);
        osPen = (HPEN)SelectObject(hdc, myPen);

        /// 현재 좌표 설정 및 그리기
        x = LOWORD(lParam);
        y = HIWORD(lParam);

        LineTo(hdc, x, y);
    }
    break;

    /// WM_LBUTTONUP
    case WM_USER + 2:
    {
        /// 자유선 그리기일 땐 UP시 break
        if (drawMode == 1) { drawStart = false; }

        /// 현재 마우스 좌표 갱신
        x = LOWORD(lParam);
        y = HIWORD(lParam);

        myBrush = CreateSolidBrush(paintColor);
        osBrush = (HBRUSH)SelectObject(hdc, myBrush);

        /// 직선 그리기
        if (drawMode == 2)      { LineTo(hdc, x, y); }   

        /// 사각형 그리기
        else if (drawMode == 3) { Rectangle(hdc, preX, preY, x, y); } 

        /// 타원 그리기
        else if (drawMode == 4) { Ellipse(hdc, preX, preY, x, y); }      

    }
    break;

    /// 그리기 모드
    case WM_USER + 3:
    {
        switch (wParam)
        {
            case 1: 
                drawMode = 1;   /// 자유선
                break;
            case 2: 
                drawMode = 2;   /// 직선
                break;
            case 3: 
                drawMode = 3;   /// 사각형
                break;
            case 4: 
                drawMode = 4;   /// 타원
                break;
        }
    }
    break;

    /// 선 색상 선택
    case WM_USER + 4:
    {
        switch (wParam)
        {
        case 1:
            lineColor = RGB(255, 0, 0);
            break;
        case 2:
            lineColor = RGB(0, 255, 0);
            break;
        case 3:
            lineColor = RGB(0, 0, 255);
            break;
        case 4:
            lineColor = RGB(0, 0, 0);
            break;

        }
    }
    break;

    /// 면 색상 선택
    case WM_USER + 5:
    {
        switch (wParam)
        {
        case 1:
            paintColor = RGB(255, 0, 0);
            break;
        case 2:
            paintColor = RGB(0, 255, 0);
            break;
        case 3:
            paintColor = RGB(0, 0, 255);
            break;
        case 4:
            paintColor = RGB(255, 255, 255);
            break;
        case 5:
            paintColor = RGB(0, 0, 0);
            break;
        }
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

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
        EndPaint(hWnd, &ps);
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
