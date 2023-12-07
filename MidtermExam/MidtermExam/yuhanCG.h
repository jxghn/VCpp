#pragma once
#include <windows.h>

enum Draw { NON, BOX, CIRCLE, CUBE, BONOBONO, RYAN }; // Drawing Shape Values

void DrawShape(HWND hWnd, HDC hdc, POINT start, POINT end, Draw shape, int blink);

void DrawBoxCircle(HDC hdc, POINT max, POINT min, Draw shape); 
void DrawCube(HDC hdc, const POINT start, const POINT end); 
void DrawBonobono(HWND hWnd, HDC hdc, int blink); 
void DrawRyan(HWND hWnd, HDC hdc, int left, int top, int right, int bottom); 

bool isDrawArea(POINT start, POINT end, RECT drawArea);
bool IsMouseInBox(POINT last, POINT start, POINT end);