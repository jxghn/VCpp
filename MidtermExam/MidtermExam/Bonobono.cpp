#include "yuhanCG.h"

Draw shape; // 현재 선택된 도형

POINT startPoint = { 0 }; // 시작점 좌표
POINT endPoint = { 0 }; // 끝점 좌표
POINT lastPoint = { 0 }; // 마지막 커서 좌표
POINT point = { 0 };

RECT Box = { 8, 8, 792, 472 };
RECT drawArea = { 16, 112, 784, 464 };

bool isDrawing = false; // 좌클릭 여부
bool isMoving = false; // 우클릭 여부
bool isInBox = false; // 커서가 범위 안에 있는지 여부
bool isCircleSize = false; // 원 우클릭 여부
bool isCubeSize = false; // 큐브 좌클릭 여부

int blink = 0;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_COMMAND:
        InvalidateRect(hWnd, NULL, TRUE); // WM_PAINT 호출
        // Box 버튼 클릭
        if (LOWORD(wParam) == 1) {
            shape = Draw::BOX; // Box 선택
        }
        // Circle 버튼 클릭
        else if (LOWORD(wParam) == 2) {
            shape = Draw::CIRCLE; // Circle 선택
        }
        // Cube 버튼 클릭
        else if (LOWORD(wParam) == 3) {
            shape = Draw::CUBE; // Cube 선택
        }
        // Bonobono 버튼 클릭
        else if (LOWORD(wParam) == 4) {
            shape = Draw::BONOBONO; // Bonobono 선택
        }
        // Ryan 버튼 클릭
        else if (LOWORD(wParam) == 5) {
            shape = Draw::RYAN; // Ryan 선택
        }
        // 좌표 초기화
        startPoint = { 0 };
        endPoint = { 0 };
        SetFocus(hWnd); // 윈도우로 포커스 이동
        break;
    case WM_LBUTTONDOWN: {
        isDrawing = true;
        if (shape == CUBE && isDrawArea(startPoint, endPoint, drawArea)) {
            lastPoint.x = LOWORD(lParam);
            lastPoint.y = HIWORD(lParam);
            if (IsMouseInBox(lastPoint, startPoint, endPoint)) {
                isCubeSize = true;
                isDrawing = false;
            }
        }
        if (isDrawing) {
            startPoint.x = LOWORD(lParam);
            startPoint.y = HIWORD(lParam);
        }
        break;
    }
    case WM_LBUTTONUP: {
        isDrawing = false;
        isCubeSize = false;
        break;
    }
    case WM_RBUTTONDOWN: {

        if (shape == BOX) {
            isMoving = true;

            // 마우스 커서 위치 받기
            lastPoint.x = LOWORD(lParam);
            lastPoint.y = HIWORD(lParam);

            // 커서가 도형 범위 안에 있는지 확인
            if (IsMouseInBox(lastPoint, startPoint, endPoint))
            {
                isInBox = true;
            }
            else
            {
                isInBox = false;
            }
        }
        else if (shape == CIRCLE) {
            isCircleSize = true;
        }
        else if (shape == CUBE) {
            isMoving = true;

            lastPoint.x = LOWORD(lParam);
            lastPoint.y = HIWORD(lParam);

            // 커서가 도형 범위 안에 있는지 확인
            if (IsMouseInBox(lastPoint, startPoint, endPoint))
            {
                isInBox = true;
            }
            else
            {
                isInBox = false;
            }
        }
        break;
    }
    case WM_RBUTTONUP: {
        if (shape == BOX) {
            isMoving = false;
        }
        else if (shape == CIRCLE) {
            isCircleSize = false;
        }
        else if (shape == CUBE) {
            isMoving = false;
        }
        break;
    }
    case WM_KEYDOWN:
        if (wParam == VK_SPACE) {
            blink = 1;
            if (shape == BONOBONO) {
                InvalidateRect(hWnd, NULL, TRUE);
            }
        }
        break;
    case WM_KEYUP:
        blink = 0;
        if (shape == BONOBONO) {
            InvalidateRect(hWnd, NULL, TRUE);
        }
        break;
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 0));

        FrameRect(hdc, &Box, hBrush); // Box 테두리 그리기
        FrameRect(hdc, &drawArea, hBrush); // drawArea 테두리 그리기
        FillRect(hdc, &drawArea, CreateSolidBrush(RGB(255, 255, 255))); // 흰 배경으로 채우기

        if (isDrawArea) {
            DrawShape(hWnd, hdc, startPoint, endPoint, shape, blink);
        }

        DeleteObject(hBrush);
        EndPaint(hWnd, &ps);
        break;
    }
    case WM_MOUSEMOVE: {
        POINT mousePos;
        GetCursorPos(&mousePos);
        ScreenToClient(hWnd, &mousePos);

        // 커서 바꾸기
        if (PtInRect(&drawArea, mousePos)) {
            SetCursor(LoadCursor(NULL, IDC_CROSS));
        }
        else {
            SetCursor(LoadCursor(NULL, IDC_ARROW));
        }

        if (isDrawing) {
            endPoint.x = LOWORD(lParam);
            endPoint.y = HIWORD(lParam);
            InvalidateRect(hWnd, NULL, TRUE);
        }

        // Box 범위 안에서 우클릭 시
        if (shape == BOX && isMoving && isInBox) {
            int deltaX = LOWORD(lParam) - lastPoint.x;
            int deltaY = HIWORD(lParam) - lastPoint.y;

            startPoint.x += deltaX;
            startPoint.y += deltaY;
            endPoint.x += deltaX;
            endPoint.y += deltaY;

            lastPoint.x = LOWORD(lParam);
            lastPoint.y = HIWORD(lParam);

            InvalidateRect(hWnd, NULL, TRUE);
        }

        // Cube 범위 안에서 우클릭 시
        if (shape == CUBE && isMoving && isInBox) {
            int deltaX = LOWORD(lParam) - lastPoint.x;
            int deltaY = HIWORD(lParam) - lastPoint.y;

            startPoint.x += deltaX;
            startPoint.y += deltaY;
            endPoint.x += deltaX;
            endPoint.y += deltaY;

            lastPoint.x = LOWORD(lParam);
            lastPoint.y = HIWORD(lParam);

            InvalidateRect(hWnd, NULL, TRUE);
        }

        // Cube 범위 안에서 좌클릭 시
        if (isCubeSize) {
            int maxX = startPoint.x > endPoint.x ? startPoint.x : endPoint.x;

            int deltaX = LOWORD(lParam) - maxX;

            // 커서를 오른쪽으로 갈 때
            if (deltaX > 0) {
                if (endPoint.x > startPoint.x && endPoint.y > startPoint.y) {
                    endPoint.x = endPoint.x + (long)(endPoint.x * 0.1);
                }
                else if (endPoint.x > startPoint.x && endPoint.y < startPoint.y) {
                    endPoint.x = endPoint.x + (long)(endPoint.x * 0.1);
                }
                else if (endPoint.x < startPoint.x && endPoint.y > startPoint.y) {
                    startPoint.x = startPoint.x + (long)(startPoint.x * 0.1);
                }
                else {
                    startPoint.x = startPoint.x + (long)(startPoint.x * 0.1);
                }
            }
            // 커서를 왼쪽으로 갈 때
            else {
                if (endPoint.x > startPoint.x && endPoint.y > startPoint.y) {
                    endPoint.x = endPoint.x - (long)(endPoint.x * 0.1);
                }
                else if (endPoint.x > startPoint.x && endPoint.y < startPoint.y) {
                    endPoint.x = endPoint.x - (long)(endPoint.x * 0.1);
                }
                else if (endPoint.x < startPoint.x && endPoint.y > startPoint.y) {
                    startPoint.x = startPoint.x - (long)(startPoint.x * 0.1);
                }
                else {
                    startPoint.x = startPoint.x - (long)(startPoint.x * 0.1);
                }
            }

            lastPoint.x = LOWORD(lParam);
            lastPoint.y = HIWORD(lParam);

            InvalidateRect(hWnd, NULL, TRUE);
        }

        // Circle 범위 안에서 우클릭 시
        if (isCircleSize) {
            int maxX = startPoint.x > endPoint.x ? startPoint.x : endPoint.x;

            int deltaX = LOWORD(lParam) - maxX;

            // 커서를 오른쪽으로 갈 때
            if (deltaX > 0) {
                if (endPoint.x > startPoint.x && endPoint.y > startPoint.y) {
                    endPoint.x = endPoint.x + (long)(endPoint.x * 0.1);
                }
                else if (endPoint.x > startPoint.x && endPoint.y < startPoint.y) {
                    endPoint.x = endPoint.x + (long)(endPoint.x * 0.1);
                }
                else if (endPoint.x < startPoint.x && endPoint.y > startPoint.y) {
                    startPoint.x = startPoint.x + (long)(startPoint.x * 0.1);
                }
                else {
                    startPoint.x = startPoint.x + (long)(startPoint.x * 0.1);
                }
            }
            // 커서를 왼쪽으로 갈 때
            else {
                if (endPoint.x > startPoint.x && endPoint.y > startPoint.y) {
                    endPoint.x = endPoint.x - (long)(endPoint.x * 0.1);
                }
                else if (endPoint.x > startPoint.x && endPoint.y < startPoint.y) {
                    endPoint.x = endPoint.x - (long)(endPoint.x * 0.1);
                }
                else if (endPoint.x < startPoint.x && endPoint.y > startPoint.y) {
                    startPoint.x = startPoint.x - (long)(startPoint.x * 0.1);
                }
                else {
                    startPoint.x = startPoint.x - (long)(startPoint.x * 0.1);
                }
            }

            lastPoint.x = LOWORD(lParam);
            lastPoint.y = HIWORD(lParam);

            InvalidateRect(hWnd, NULL, TRUE);
        }
        break;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {

    HWND hWnd;
    HWND hButton1, hButton2, hButton3, hButton4, hButton5;

    WNDCLASSEX wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(CreateSolidBrush(RGB(255, 240, 200)));
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = L"ButtonWindowClass";
    wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

    if (!RegisterClassEx(&wcex)) {
        return 1;
    }

    RECT rect = { 0, 0, 800, 480 };
    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, 0);
    int width = rect.right - rect.left;
    int height = rect.bottom - rect.top;

    hWnd = CreateWindow(
        L"ButtonWindowClass", L"곰돌이", WS_OVERLAPPEDWINDOW & ~(WS_MAXIMIZEBOX | WS_THICKFRAME),
        0, 0, width, height, NULL, NULL, hInstance, NULL);

    if (!hWnd) {
        return FALSE;
    }

    hButton1 = CreateWindow(
        L"BUTTON", L"Box", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        32, 32, 121.6, 64, hWnd, (HMENU)1, hInstance, NULL);

    hButton2 = CreateWindow(
        L"BUTTON", L"Circle", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        185.6, 32, 121.6, 64, hWnd, (HMENU)2, hInstance, NULL);

    hButton3 = CreateWindow(
        L"BUTTON", L"Cube", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        339.2, 32, 121.6, 64, hWnd, (HMENU)3, hInstance, NULL);

    hButton4 = CreateWindow(
        L"BUTTON", L"Bonobono", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        492.8, 32, 121.6, 64, hWnd, (HMENU)4, hInstance, NULL);

    hButton5 = CreateWindow(
        L"BUTTON", L"Ryan", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        646.4, 32, 121.6, 64, hWnd, (HMENU)5, hInstance, NULL);

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    MSG msg;
    HWND hLastFocused = NULL;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int)msg.wParam;
}