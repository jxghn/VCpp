#include "yuhanCG.h"

Draw shape; // ���� ���õ� ����

POINT startPoint = { 0 }; // ������ ��ǥ
POINT endPoint = { 0 }; // ���� ��ǥ
POINT lastPoint = { 0 }; // ������ Ŀ�� ��ǥ
POINT point = { 0 };

RECT Box = { 8, 8, 792, 472 };
RECT drawArea = { 16, 112, 784, 464 };

bool isDrawing = false; // ��Ŭ�� ����
bool isMoving = false; // ��Ŭ�� ����
bool isInBox = false; // Ŀ���� ���� �ȿ� �ִ��� ����
bool isCircleSize = false; // �� ��Ŭ�� ����
bool isCubeSize = false; // ť�� ��Ŭ�� ����

int blink = 0;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_COMMAND:
        InvalidateRect(hWnd, NULL, TRUE); // WM_PAINT ȣ��
        // Box ��ư Ŭ��
        if (LOWORD(wParam) == 1) {
            shape = Draw::BOX; // Box ����
        }
        // Circle ��ư Ŭ��
        else if (LOWORD(wParam) == 2) {
            shape = Draw::CIRCLE; // Circle ����
        }
        // Cube ��ư Ŭ��
        else if (LOWORD(wParam) == 3) {
            shape = Draw::CUBE; // Cube ����
        }
        // Bonobono ��ư Ŭ��
        else if (LOWORD(wParam) == 4) {
            shape = Draw::BONOBONO; // Bonobono ����
        }
        // Ryan ��ư Ŭ��
        else if (LOWORD(wParam) == 5) {
            shape = Draw::RYAN; // Ryan ����
        }
        // ��ǥ �ʱ�ȭ
        startPoint = { 0 };
        endPoint = { 0 };
        SetFocus(hWnd); // ������� ��Ŀ�� �̵�
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

            // ���콺 Ŀ�� ��ġ �ޱ�
            lastPoint.x = LOWORD(lParam);
            lastPoint.y = HIWORD(lParam);

            // Ŀ���� ���� ���� �ȿ� �ִ��� Ȯ��
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

            // Ŀ���� ���� ���� �ȿ� �ִ��� Ȯ��
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

        FrameRect(hdc, &Box, hBrush); // Box �׵θ� �׸���
        FrameRect(hdc, &drawArea, hBrush); // drawArea �׵θ� �׸���
        FillRect(hdc, &drawArea, CreateSolidBrush(RGB(255, 255, 255))); // �� ������� ä���

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

        // Ŀ�� �ٲٱ�
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

        // Box ���� �ȿ��� ��Ŭ�� ��
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

        // Cube ���� �ȿ��� ��Ŭ�� ��
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

        // Cube ���� �ȿ��� ��Ŭ�� ��
        if (isCubeSize) {
            int maxX = startPoint.x > endPoint.x ? startPoint.x : endPoint.x;

            int deltaX = LOWORD(lParam) - maxX;

            // Ŀ���� ���������� �� ��
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
            // Ŀ���� �������� �� ��
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

        // Circle ���� �ȿ��� ��Ŭ�� ��
        if (isCircleSize) {
            int maxX = startPoint.x > endPoint.x ? startPoint.x : endPoint.x;

            int deltaX = LOWORD(lParam) - maxX;

            // Ŀ���� ���������� �� ��
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
            // Ŀ���� �������� �� ��
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
        L"ButtonWindowClass", L"������", WS_OVERLAPPEDWINDOW & ~(WS_MAXIMIZEBOX | WS_THICKFRAME),
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