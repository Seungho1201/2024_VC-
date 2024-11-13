// GameProject.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "GameProject.h"

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
    LoadStringW(hInstance, IDC_GAMEPROJECT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GAMEPROJECT));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GAMEPROJECT));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_GAMEPROJECT);
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

MakeButton bt_GAMESTART(330, 150, 500, 100, GAMESTART, L"GAME START");
MakeButton bt_GAMEINFO(330, 300, 500, 100, GAMEINFO, L"GAME INFO");
MakeButton bt_GAMEEXIT(330, 450, 500, 100, GAMEEXIT, L"GAME EXIT");

HWND g_Hwnd;

WCHAR inGameText[50];                       /// 게임 내 텍스트

int playerSpeed = 10;                       ///  플레이어 이동 속도
int enemySpeed = 10;

RECT playArea = { 10, 70, 1210, 635 };      /// 플레이 영역
RECT player = { 100, 100, 150, 150 };       /// 플레이어 
RECT timerBox = { 100, 40, 1000, 60 };      /// 남은 시간 박스
RECT enemy = { 700, 500, 750, 550 };        /// 적
RECT enemy2 = { 750, 550, 800, 600 };       /// 적
RECT itemBox = { 200, 200, 235, 235 };      /// 아이템 박스


RECT rest;
bool playStart = false;                     /// 게임 시작 여부 
bool mainTitle = true;                      /// 메인 타이틀 출력 여부
bool gameInfo = false;                      /// 게임 정보 출력 여부
bool enemy2Start = false;                   /// 2페이즈 진입시 적 한명 더 소환


/// <summary>
///  버튼 핸들 추척용 전역 변수
/// </summary>
HWND hWnd_GAMESTART;
HWND hWnd_GAMEINFO;
HWND hWnd_GAMEEXIT;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
        g_Hwnd = hWnd;

        /// 각 핸들 추적용 전역 변수에 각자 버튼 핸들 대입
        /// 버튼 생성
        hWnd_GAMESTART = bt_GAMESTART.mkButton(g_Hwnd);
        hWnd_GAMEINFO = bt_GAMEINFO.mkButton(g_Hwnd);
        hWnd_GAMEEXIT = bt_GAMEEXIT.mkButton(g_Hwnd);

        break;

    case WM_KEYDOWN:
        switch (wParam)     /// 키보드 부가 정보는 WPARAM
        {
        case VK_LEFT:
            player.left -= playerSpeed;
            player.right -= playerSpeed;
            break;

        case VK_RIGHT:
            player.left += playerSpeed;
            player.right += playerSpeed;
            break;

        case VK_UP:
            player.top -= playerSpeed;
            player.bottom -= playerSpeed;
            break;

        case VK_DOWN:
            player.top += playerSpeed;
            player.bottom += playerSpeed;
            break;
        }

        /// 플레이 영역 판단
        if (player.left <= playArea.left)
        {
            player.left = playArea.left;
            player.right = playArea.left + 50;
        }
        if (player.right >= playArea.right)
        {
            player.right = playArea.right;
            player.left = player.right - 50;;
        }
        if (player.top <= playArea.top)
        {
            player.top = playArea.top;
            player.bottom = player.top + 50;
        }
        if (player.bottom >= playArea.bottom)
        {
            player.bottom = playArea.bottom;
            player.top = player.bottom - 50;
        }

        /// 아이템 충돌 판정
        if (IntersectRect(&rest, &itemBox, &player))
        {
            /// 아이템 습득시 플레이어의 이동속도를 30으로 지정한다
            playerSpeed = 30;

            /// 아이템 충돌 판정시 안보이는 곳으로 이동
            itemBox.left = 1500;
            itemBox.top = 1500;
            itemBox.right = itemBox.left + 35;
            itemBox.bottom = itemBox.top + 35;

            /// 3초 후 랜덤한 곳으로 스폰
            SetTimer(hWnd, SETITEM,3000, NULL);

            /// 유지시간은 5초로 설정
            SetTimer(hWnd,SETSPEED,5000, NULL);

            InvalidateRect(hWnd, &playArea, TRUE);
            break;
        }

        InvalidateRect(hWnd, &playArea, TRUE);
        break;

      
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // 메뉴 선택을 구문 분석합니다:
        switch (wmId)
        {
            
        case GAMESTART:
            /// 메인 타이틀을 게임 시작시 출력 안함
            mainTitle = false;
            /// 플레이 시작
            playStart = true;
            gameInfo = false;

            /// 버튼 숨기기
            ShowWindow(hWnd_GAMEINFO, SW_HIDE);
            ShowWindow(hWnd_GAMEEXIT, SW_HIDE);
            ShowWindow(hWnd_GAMESTART, SW_HIDE);

            SetTimer(g_Hwnd, SETPLAYTIME, 500, NULL);
            SetTimer(g_Hwnd, SETENEMY, 250, NULL);

            // 화면 갱신
            InvalidateRect(g_Hwnd, NULL, TRUE);
            break;
        case GAMEINFO:
            /// 버튼 숨기기
            ShowWindow(hWnd_GAMEINFO, SW_HIDE);
            ShowWindow(hWnd_GAMEEXIT, SW_HIDE);

            gameInfo = true;

            // 화면 갱신
            InvalidateRect(g_Hwnd, NULL, TRUE);
            break;

        case GAMEEXIT:
            /// 프로그램 종료
            PostQuitMessage(0);
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

    case WM_TIMER:
        /// 플레이 시간
        if (wParam == SETPLAYTIME)      /// 페이즈 1
        {
            timerBox.right -= 10;
            InvalidateRect(hWnd, NULL, TRUE);

            /// 타이머 종료 시
            /// 타이머를 종료 후 메세지를 출력한다
            /// 메세지 이후 프로그램 종료 처리
            if (timerBox.right <= timerBox.left)
            {   
                /// 현재 타이머 종료
                KillTimer(hWnd, SETPLAYTIME);

                /// 적 타이머 모두 종료
                KillTimer(hWnd, SETENEMY);
                KillTimer(hWnd, SETENEMY2);

                MessageBox(hWnd, L"클리어", L"축하합니다", MB_OK);
                /// 프로그램 종료
                PostQuitMessage(0);
            }
            
            /// 타이머가 절반 이하일시 현재 타이머 삭제 후 새 타이머 작동
            /// 적 추가 등장 + 
            if (timerBox.right <= 500)
            {
                /// 현재 ENEMY 타이머를 삭제 후 
                /// 새 타이머를 작동하여 갱신을 더 자주 일어나게 한다 (== 이동속도 증가)
                KillTimer(hWnd, SETENEMY);
                SetTimer(hWnd, SETENEMY, 100, NULL);

                /// 두번 째 적 타이머 세팅
                SetTimer(hWnd, SETENEMY2, 300, NULL);

                /// 2페이즈 시작이기에 두번째 적을 소환한다.
                enemy2Start = true;
               
            }
        }
        
        /// 적 이동 설정
        if (wParam == SETENEMY)
        {
            if (player.left <= enemy.left)
            {
                enemy.left -= enemySpeed;
                enemy.right -= enemySpeed;
            }
            else {
                enemy.left += enemySpeed;
                enemy.right += enemySpeed;
            }

            if (player.top <= enemy.top)
            {
                enemy.top -= enemySpeed;
                enemy.bottom -= enemySpeed;
            }
            else {
                enemy.top += enemySpeed;
                enemy.bottom += enemySpeed;
            }

            /// 화면 갱신
            InvalidateRect(g_Hwnd, &playArea, TRUE);
            
            /// 적과 플레이어가 접촉했을 시
            /// 모든 타이머를 작동 중지 후 메세지를 출력
            /// 메세지 이후엔 프로그램 종료
            if (IntersectRect(&rest, &enemy, &player))
            {
                /// 현재 타이머 종료
                KillTimer(hWnd, SETPLAYTIME);

                KillTimer(hWnd, SETENEMY);
                KillTimer(hWnd, SETENEMY2);
                MessageBox(hWnd, L"게임 오버", L"사망하셨습니다", MB_OK);

                /// 프로그램 종료
                PostQuitMessage(0);
            }
        }

        /// 두번째 적 이동 설정
        if (wParam == SETENEMY2)
        {
            if (player.left <= enemy2.left)
            {
                enemy2.left -= 30;
                enemy2.right -= 30;
            }
            else {
                enemy2.left += 30;
                enemy2.right += 30;
            }

            if (player.top <= enemy2.top)
            {
                enemy2.top -= 30;
                enemy2.bottom -= 30;
            }
            else {
                enemy2.top += 30;
                enemy2.bottom += 30;
            }

            /// 화면 갱신
            InvalidateRect(g_Hwnd, &playArea, TRUE);

            /// 첫번째 적과 두번째 적이 접촉시 뭉치는걸 방지 위해
            /// 다른 좌표로 이동
            if (IntersectRect(&rest, &enemy, &enemy2))
            {
                enemy2.top = playArea.top;
                enemy2.left = playArea.left;
                enemy2.right = enemy2.left + 50;
                enemy2.bottom = enemy2.top + 50;
            }

            /// 적과 플레이어가 접촉했을 시
            /// 모든 타이머를 작동 중지 후 메세지를 출력
            /// 메세지 이후엔 프로그램 종료
            if (IntersectRect(&rest, &enemy2, &player))
            {
                /// 현재 타이머 종료
                KillTimer(hWnd, SETPLAYTIME);
                KillTimer(hWnd, SETENEMY);
                KillTimer(hWnd, SETENEMY2);

                MessageBox(hWnd, L"게임 오버", L"사망하셨습니다", MB_OK);
                /// 프로그램 종료
                PostQuitMessage(0);
            }
        }

        /// 아이템 세팅
        if (wParam == SETITEM)
        {
            // 아이템의 너비와 높이를 정의
            int itemWidth = 35;
            int itemHeight = 35;

            // 플레이 영역 내에서 랜덤한 좌표로 아이템을 이동
            itemBox.left = playArea.left + (rand() % (playArea.right - playArea.left - itemWidth));
            itemBox.top = playArea.top + (rand() % (playArea.bottom - playArea.top - itemHeight));
            itemBox.right = itemBox.left + itemWidth;
            itemBox.bottom = itemBox.top + itemHeight;

            /// 3번 타이머(아이템 충돌 처리) 삭제
            KillTimer(hWnd, SETITEM);
        }

        /// 유저 속도 세팅 (아이템과 연관)
        if (wParam == SETSPEED)
        {
            /// 아이템 유지시간이 끝난 후 원래 속도로 되돌린 후 1번 타이머를 삭제한다.
            playerSpeed = 10;
            KillTimer(hWnd, SETSPEED);
            InvalidateRect(hWnd, NULL, TRUE);
        }
        break;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...

        /// 메인 타이틀 텍스트
        if (mainTitle)
        {
            wsprintf(inGameText, L"5주자 과체 게임 제작");
            TextOut(hdc, 500, 15, inGameText, lstrlen(inGameText));
        }

        if (gameInfo)
        {
            // 적을 빨간색으로 그리기
            HBRUSH enemyColorRed = CreateSolidBrush(RGB(255, 0, 0));  // 빨간색 브러시 생성
            HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, enemyColorRed);  // 기존 브러시 저장

            Rectangle(hdc,400, 350, 450, 400);  // 적 그리기

            SelectObject(hdc, hOldBrush);  // 기존 브러시로 복원
            DeleteObject(enemyColorRed);  // 빨간색 브러시 삭제

            wsprintf(inGameText, L"ENEMY 적을 피해다니세요!");
            TextOut(hdc, 500, 365, inGameText, lstrlen(inGameText));

            /// 아이템 그리기
            HBRUSH itemColorBlue = CreateSolidBrush(RGB(0, 0, 255));  // 빨간색 브러시 생성
            hOldBrush = (HBRUSH)SelectObject(hdc, itemColorBlue);  // 기존 브러시 저장

            Ellipse(hdc, 400, 450, 450,500);

            SelectObject(hdc, hOldBrush);  // 기존 브러시로 복원
            DeleteObject(itemColorBlue);  // 빨간색 브러시 삭제

            wsprintf(inGameText, L"ITEM 아이템을 먹을 시 이동속도가 30으로 증가합니다!");
            TextOut(hdc, 500,465, inGameText, lstrlen(inGameText));

        }

        /// 플레이 시작시 처리
        if (playStart)
        {
            wsprintf(inGameText, L"학번 : 202002056");
            TextOut(hdc, 1050, 15, inGameText, lstrlen(inGameText));

            wsprintf(inGameText, L"이름 : 오승호");
            TextOut(hdc, 1050, 35, inGameText, lstrlen(inGameText));

            wsprintf(inGameText, L"현재 속도 : ");
            TextOut(hdc, 15, 15, inGameText, lstrlen(inGameText));

            wsprintf(inGameText, L"%d", playerSpeed);
            TextOut(hdc, 100, 15, inGameText, lstrlen(inGameText));

            wsprintf(inGameText, L"남은 시간 : ");
            TextOut(hdc, 15, 40, inGameText, lstrlen(inGameText));

            /// 플레이영역 그리기
            Rectangle(hdc, playArea.left, playArea.top, playArea.right, playArea.bottom);

            /// 플레이어 그리기
            Rectangle(hdc, player.left, player.top, player.right, player.bottom);

            // 적을 빨간색으로 그리기
            HBRUSH enemyColorRed = CreateSolidBrush(RGB(255, 0, 0));  // 빨간색 브러시 생성
            HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, enemyColorRed);  // 기존 브러시 저장

            Rectangle(hdc, enemy.left, enemy.top, enemy.right, enemy.bottom);  // 적 그리기

            if (enemy2Start)
            {
                Rectangle(hdc, enemy2.left, enemy2.top, enemy2.right, enemy2.bottom);  // 적 그리기
            }

            SelectObject(hdc, hOldBrush);  // 기존 브러시로 복원
            DeleteObject(enemyColorRed);  // 빨간색 브러시 삭제


            HBRUSH itemColorBlue = CreateSolidBrush(RGB(0, 0, 255));  // 빨간색 브러시 생성
            HBRUSH hOldBrush1 = (HBRUSH)SelectObject(hdc, itemColorBlue);  // 기존 브러시 저장

            Ellipse(hdc, itemBox.left, itemBox.top, itemBox.right, itemBox.bottom);

            SelectObject(hdc, hOldBrush1);  // 기존 브러시로 복원
            DeleteObject(itemColorBlue);  //  파란색 브러시 삭제

            // 타이머 박스 내부 채우기
            HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 0));  // 검은색 브러시 생성
            FillRect(hdc, &timerBox, hBrush);
            DeleteObject(hBrush);  // 브러시 삭제


        }

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
