#include <windows.h>
#include <cstdio>
#include "resource.h"

#define POS_STATIC_BOX_X 20
#define POS_STATIC_BOX_Y 15
#define MEASUR_STATIC_BOX_WIDTH 300
#define MEASUR_STATIC_BOX_HEIGHT 30
#define POS_ZERO_X 20
#define POS_ZERO_Y 215
#define POS_ZERO_WIDTH 150
#define POS_ZERO_HEIGHT 51

CONST WCHAR className[] = L"CalcApplication";

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, INT nCmdShow)
{
	WNDCLASSW wClass = { 0 };

	wClass.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wClass.hInstance = hInstance;
	wClass.lpszClassName = (LPCWSTR)className;
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
		CreateWindow(L"STATIC", NULL, WS_VISIBLE | WS_CHILD | WS_BORDER, POS_STATIC_BOX_X, POS_STATIC_BOX_Y, MEASUR_STATIC_BOX_WIDTH, MEASUR_STATIC_BOX_HEIGHT, hwnd, (HMENU)IDR_EDIT, NULL, NULL);
		HWND zeroButton = CreateWindow(L"Button", L"", BS_PUSHBUTTON | WS_VISIBLE | WS_CHILD | BS_BITMAP, POS_ZERO_X, POS_ZERO_Y, POS_ZERO_WIDTH, POS_ZERO_HEIGHT, hwnd, (HMENU)IDB_BUTTON_0, NULL, NULL);
		HBITMAP ZB = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BUTTON_0), IMAGE_BITMAP, 0, 0, 0);
		SendMessage(zeroButton, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)ZB);
		HWND pointButton = CreateWindow(L"Button", L"", BS_DEFPUSHBUTTON | WS_VISIBLE | WS_CHILD | BS_BITMAP, 171, 213, 74, 53, hwnd, (HMENU)IDB_BUTTON_POINT, NULL, NULL);
		HBITMAP pointB = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BUTTON_POINT), IMAGE_BITMAP, 0, 0, 0);
		SendMessage(pointButton, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)pointB);
		int x = 170;
		int y = 50;
		for (int j = 9; j > 0; j -= 3) {
			for (int i = 1; i <= 3; ++i)
			{
				HWND button = CreateWindow(L"Button", L"", BS_DEFPUSHBUTTON | WS_VISIBLE | WS_CHILD | BS_BITMAP, x, y, 74, 53, hwnd, (HMENU)(IDB_BUTTON_1 + j - i), NULL, NULL);
				HBITMAP BT = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BUTTON_1 + j - i), IMAGE_BITMAP, 0, 0, 0);
				SendMessage(button, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)BT);
				x -= 75;
			}
			x = 170;
			y += 55;
		}
		HWND plusButton = CreateWindow(L"Button",  L"", BS_DEFPUSHBUTTON | WS_VISIBLE | WS_CHILD | BS_BITMAP, 245, 50, 74, 53, hwnd, (HMENU)IDB_BUTTON_PLUS, NULL, NULL);
		HBITMAP plusOp = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BUTTON_PLUS), IMAGE_BITMAP, 0, 0, 0);
		SendMessage(plusButton, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)plusOp);
		HWND minusButton = CreateWindow(L"Button", L"", BS_DEFPUSHBUTTON | WS_VISIBLE | WS_CHILD | BS_BITMAP, 245, 105, 74, 54, hwnd, (HMENU)IDB_BUTTON_MINUS, NULL, NULL);
		HBITMAP minusOp = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BUTTON_MINUS), IMAGE_BITMAP, 0, 0, 0);
		SendMessage(minusButton, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)minusOp);
		HWND asterButton = CreateWindow(L"Button", L"", BS_DEFPUSHBUTTON | WS_VISIBLE | WS_CHILD | BS_BITMAP, 245, 160, 74, 54, hwnd, (HMENU)IDB_BUTTON_ASTER, NULL, NULL);
		HBITMAP asterOp = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BUTTON_ASTER), IMAGE_BITMAP, 0, 0, 0);
		SendMessage(asterButton, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)asterOp);
		HWND divideButton = CreateWindow(L"Button", L"", BS_DEFPUSHBUTTON | WS_VISIBLE | WS_CHILD | BS_BITMAP, 245, 212, 74, 54, hwnd, (HMENU)IDB_BUTTON_DIVIDE, NULL, NULL);
	}
		break;
	case WM_COMMAND:			//Функция в которой выполняются все нажатые клавиши на клавиатуре или на виртуальных кпопках программы
	{
		int id = LOWORD(wParam);
		switch (id)
		{
		case IDB_BUTTON_0:
			MessageBox(NULL, L"ZERO", L"ZERO", MB_OK);
			break;
		case IDB_BUTTON_1:
			MessageBox(NULL, L"ONE", L"ONE", MB_OK);
			break;
		case IDB_BUTTON_2:
			MessageBox(NULL, L"TWO", L"TWO", MB_OK);
			break;
		case IDB_BUTTON_3:
			MessageBox(NULL, L"THREE", L"THREE", MB_OK);
			break;
		case IDB_BUTTON_4:
			MessageBox(NULL, L"FOUR", L"FOUR", MB_OK);
			break;
		case IDB_BUTTON_5:
			MessageBox(NULL, L"FIVE", L"FIVE", MB_OK);
		case IDB_BUTTON_6:
			MessageBox(NULL, L"SIX", L"SIX", MB_OK);
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