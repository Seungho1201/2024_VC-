// Week6_OSH.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "Week6_OSH.h"

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
    LoadStringW(hInstance, IDC_WEEK6OSH, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WEEK6OSH));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WEEK6OSH));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WEEK6OSH);
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

COLORREF paintColor = RGB(255,255,255);
COLORREF lineColor = RGB(0, 0, 0);


MakeButton bt_Clear(10, 10, 100, 100, ERASE, L"ERASE");

ColorChange palette;
ColorChange line_palette;

HWND child;


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
    {
        HDC hdc;

        child = FindWindow(NULL, L"ProjectSecond");

        if (child == NULL) {
            MessageBox(hWnd, L"찾기 실패", L"찾기 실패", MB_OK);
        }


        bt_Clear.mkButton(hWnd, IDI_ERASE_ICON);
        break;
    }
    case WM_LBUTTONDOWN:
    {
        HDC hdc = GetDC(hWnd);

        // 자유선
        if (drawMode == 1)
        {
            drawStart = true;

            x = LOWORD(lParam);
            y = HIWORD(lParam);

            MoveToEx(hdc, x, y, NULL);

            // 자식 윈도우로 이전 좌표 전송
            LPARAM prePosParam = MAKELPARAM(x, y);
            SendMessage(child, WM_USER, 0, prePosParam);
            break;
        /// 자유선 그리기 제외 도형 및 직선 그리기의 시작 좌표 지정
        } else {
            x = LOWORD(lParam);
            y = HIWORD(lParam);

            preX = x;
            preY = y;

            // 자식 윈도우로 이전 좌표 전송
            LPARAM prePosParam = MAKELPARAM(x, y);
            SendMessage(child, WM_USER, 0, prePosParam);

            break;
        }
        
        break;
    }

    case WM_MOUSEMOVE:
    {
        /// 그리기 모드가 1 즉 자유선 그리기 일 때만 작동
        if (drawMode == 1)
        {
            // 그리기 여부가 false일시 바로 break; (그리기 방지)
            if (drawStart == false) { break; }

            HDC hdc = GetDC(hWnd);
            
            myPen = CreatePen(PS_SOLID, 1, lineColor);
            osPen = (HPEN)SelectObject(hdc, myPen);

            preX = x;
            preY = y;

            x = LOWORD(lParam);
            y = HIWORD(lParam);

            MoveToEx(hdc, preX, preY, NULL); 

            LineTo(hdc, x, y);
       
            /// 자식 윈도우로 이전 좌표 전송
            /// WM_USER로 보내어 이전 좌표를 갱신하게 함
            LPARAM prePosParam = MAKELPARAM(preX, preY);
            SendMessage(child, WM_USER, 0, prePosParam);

            /// 자식 윈도우로 현재 좌표 전송
            SendMessage(child, WM_USER + 1, 0, lParam);

            /// 리소스 해제
            SelectObject(hdc, osPen);
            DeleteObject(myPen);
            ReleaseDC(hWnd, hdc);

            break;
        }
        break;
    }
     
    case WM_LBUTTONUP:
    {
        HDC hdc = GetDC(hWnd);
        myBrush = CreateSolidBrush(paintColor);
        osBrush = (HBRUSH)SelectObject(hdc, myBrush);


        /// 자유선 그리기일 땐 UP시 그리기 중단
        if (drawMode == 1)
        {
            drawStart = false;
            break;
        }

        /// 현재 마우스 좌표
        x = LOWORD(lParam);
        y = HIWORD(lParam);
        LPARAM prePosParam = MAKELPARAM(x, y);

        // 이전 마우스 좌표로 이동
        MoveToEx(hdc, preX, preY, NULL);

        if (drawMode == 2) 
        { 
            myPen = CreatePen(PS_SOLID, 1, lineColor);
            osPen = (HPEN)SelectObject(hdc, myPen);

            LineTo(hdc, x, y); 
        }                   // 직선 그리기    
        if (drawMode == 3) { Rectangle(hdc, preX, preY, x, y); }    // 직사각형 그리기
        if (drawMode == 4) { Ellipse(hdc, preX, preY, x, y);}       // 타원 그리기


        SendMessage(child, WM_USER + 2, NULL, prePosParam);

        break;
    }
       
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
            switch (wmId)
            {

            case ERASE:
                InvalidateRect(hWnd, NULL, TRUE);
                InvalidateRect(child, NULL, TRUE);
                break;
                
            case ID_32771:  // 자유선 그리기
                drawMode = 1;
                SendMessage(child, WM_USER+3, 1, NULL);

                break;   
            case ID_32772:  // 직선 그리기
                drawMode = 2;
                SendMessage(child, WM_USER+3, 2, NULL);

                break;         
            case ID_32773:  // 사각형 그리기
                drawMode = 3;
                SendMessage(child, WM_USER+3, 3, NULL);

                break;
            case ID_32778:  // 타원 그리기
                drawMode = 4;
                SendMessage(child, WM_USER+3, 4, NULL);

                break;
 
                /// 도형 색상
            case ID_32775:  // red
                paintColor = RGB(255, 0, 0);
                SendMessage(child, WM_USER + 5, 1, NULL);

                break;
            case ID_32776:  // green
                paintColor = RGB(0, 255, 0);
                SendMessage(child, WM_USER + 5, 2, NULL);

                break;
            case ID_32779:  // blue
                paintColor = RGB(0, 0, 255);
                SendMessage(child, WM_USER + 5, 3, NULL);

                break;
            case ID_32785:  // white
                paintColor = RGB(255, 255, 255);
                SendMessage(child, WM_USER + 5, 4, NULL);

                break;
            case ID_32784: // black
                paintColor = RGB(0, 0, 0);
                SendMessage(child, WM_USER + 5, 5, NULL);

                break;

                /// 선 색상
            case ID_32780:  // red
                lineColor = RGB(255, 0, 0);
                SendMessage(child, WM_USER + 4, 1, NULL);

                break;
            case ID_32781:  // green
                lineColor = RGB(0, 255, 0);
                SendMessage(child, WM_USER + 4, 2, NULL);

                break;
            case ID_32782:  // blue
                lineColor = RGB(0, 0, 255);
                SendMessage(child, WM_USER + 4, 3, NULL);
                break;
            case ID_32783:  // black
                lineColor = RGB(0, 0, 0);
                SendMessage(child, WM_USER + 4, 4, NULL);

                break;

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
