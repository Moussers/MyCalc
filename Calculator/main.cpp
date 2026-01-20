#include <windows.h>
#include <cstdio>

#include "resource.h"

CONST WCHAR className[] = L"CalcApplication";

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, INT nCmdShow)
{
	WNDCLASSW wClass = { 0 };

	wClass.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wClass.hInstance = hInstance;
	wClass.lpszClassName = className;
	wClass.lpfnWndProc = WndProc;
	if (!RegisterClassW(&wClass))
	{
		MessageBox(NULL, L"Ошибка при регистрации окна", L"Ошибка", MB_OK);
		return -1;
	}
	CreateWindow
	(
		className,
		L"Calculator",
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		100, 100,
		500, 500,
		NULL, NULL,
		NULL, NULL
	);
	MSG msg = { 0 };
	while (GetMessage(&msg, NULL, NULL, NULL))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CREATE:								//Функцция создание окна
	{
		CreateWindow(L"STATIC", NULL, WS_VISIBLE | WS_CHILD | WS_BORDER, 20, 15, 225, 30, hwnd, (HMENU)IDR_EDIT, NULL, NULL);
		HWND zeroButton = CreateWindow(L"Button", L"", BS_PUSHBUTTON | WS_VISIBLE | WS_CHILD | BS_BITMAP, 20, 215, 120, 51, hwnd, (HMENU)IDB_BUTTON_0, NULL, NULL);
		HBITMAP BM = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BUTTON_0), IMAGE_BITMAP, 0, 0, 0);
		SendMessage(zeroButton, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)BM);
		int x = 20;
		int y = 160;
		for (int j = 9; j > 0; j -= 3) {
			for (int i = 1; i <= 3; ++i)
			{
				HWND button = CreateWindow(L"Button", L"", BS_DEFPUSHBUTTON | WS_VISIBLE | WS_CHILD | BS_BITMAP, x, y, 74, 53, hwnd, (HMENU)IDB_BUTTON_0 + j - i, NULL, NULL);
				HBITMAP BT = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BUTTON_1 + j - i), IMAGE_BITMAP, 0, 0, 0);
				SendMessage(button, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)BT);
				x += 75;
			}
			x = 20;
			y -= 55;
		}
	}
		break;
	case WM_COMMAND:
	{
		int id = LOWORD(wParam);
		switch (id)
		{
		case IDB_BUTTON_0:
			MessageBox(NULL, L"ZERO", L"ZERO", MB_OK);
			break;
		}
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProcW(hwnd, msg, wParam, lParam);
	}
}