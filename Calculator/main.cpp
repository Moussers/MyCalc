#include <windows.h>
#include <cstdio>
#include "resource.h"
#include <string>

#define POS_STATIC_BOX_X 20
#define POS_STATIC_BOX_Y 15
#define MEASUR_STATIC_BOX_WIDTH 300
#define MEASUR_STATIC_BOX_HEIGHT 30
#define POS_ZERO_X 20
#define POS_ZERO_Y 215
#define POS_ZERO_WIDTH 150
#define POS_ZERO_HEIGHT 51

enum TypeOpeation 
{
	NOOPER, OPPLUS, OPMINUS, OPMULTI, OPDIVIDE
};
enum StatusInput
{
	NEWNUM, OLDNUM
};
class StatusOperation
{
	//enum class StatusInputNumber {NEW_NUM, OLD_NUM};
	INT m_status;
	INT m_oper;
	DOUBLE m_current;
public:
	StatusOperation() :m_status(NEWNUM), m_oper(NOOPER), m_current(0.0) {};
public:
	void setStatus(int status);
	int status();
	void setOper(int oper);
	int oper();
	void setNumber(double currentNum);
	double currentNumber();
};

void StatusOperation::setStatus(int status) 
{
	m_status = status;
}
int StatusOperation::status() 
{
	return m_status;
}
void StatusOperation::setOper(int oper)
{
	m_oper = oper;
}
int StatusOperation::oper() 
{
	return m_oper;
}
void StatusOperation::setNumber(double currentNum)
{
	m_current = currentNum;
}
double StatusOperation::currentNumber()
{
	return m_current;
}
StatusOperation stOper;
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
		350, 370,
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
void setNumber(INT num, HWND field)
{
	CONST INT SIZE = 256;
	WCHAR str[SIZE] = {};
	if (stOper.status() != NEWNUM) 
	{
		GetWindowText(field, str, SIZE);
	}
	stOper.setStatus(OLDNUM);
	wsprintf(str, L"%s%d", str, num);
	//первый параметр читается 
	//третий параметр записывается
	SetWindowText(field, str);
}
void setPoint(HWND field)
{
	CONST WCHAR SIZE = 256;
	if (stOper.status() != NEWNUM) 
	{
		WCHAR str[SIZE];
		GetWindowText(field, str, SIZE);
		int numPoint = 0;
		for (int i = 0; str[i] != '\0'; ++i)
		{
			if (str[i] == L'.')
			{
				numPoint++;
			}
		}
		if (numPoint < 1) 
		{

			wsprintf(str, L"%s.", str);
			SetWindowText(field, str);
		}
	}
}
void executeOperation(HWND field)
{
	CONST INT SIZE = 256;
	if (stOper.oper() != NOOPER)
	{
		WCHAR str[SIZE];
		double r = 0;
		if (stOper.status() != NEWNUM)
		{
			GetWindowText(field, str, SIZE);
			r = wcstod(str, NULL);
			switch (stOper.oper())
			{
			case OPPLUS:
				stOper.setNumber(stOper.currentNumber() + r);
				break;
			case OPMINUS:
				stOper.setNumber(stOper.currentNumber() - r);
				break;
			case OPMULTI:
				stOper.setNumber(stOper.currentNumber() * r);
				break;
			case OPDIVIDE:
				stOper.setNumber(stOper.currentNumber() / r);
				break;
			default:
				MessageBox(NULL, L"Ошибка! Введена не существующая операция", L"Внимание", MB_ICONWARNING | MB_OK);
				return;
			}
			lstrcpy(str, std::to_wstring(stOper.currentNumber()).c_str());
			//lstrcpy перемещает строку из дного массива строк в другой массив по символьно.
			SetWindowText(field, str);
		}
		stOper.setStatus(NEWNUM);
		stOper.setOper(NOOPER);
	}
	else 
	{
		WCHAR str[SIZE];
		if (stOper.status() != NEWNUM)
		{
			GetWindowText(field, str, SIZE - 1);
			stOper.setNumber(wcstod(str, NULL));
		}
		stOper.setStatus(NEWNUM);
	}
}
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CREATE:								//Функцция создание окна
	{
		CreateWindow(L"STATIC", NULL, WS_VISIBLE | WS_CHILD | WS_BORDER, POS_STATIC_BOX_X, POS_STATIC_BOX_Y, MEASUR_STATIC_BOX_WIDTH, MEASUR_STATIC_BOX_HEIGHT, hwnd, (HMENU)IDR_EDIT, NULL, NULL);
		HWND hZero = GetDlgItem(hwnd, IDR_EDIT);
		SetWindowText(hZero, L"0");
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
		HBITMAP divideOp = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BUTTON_DIVIDE), IMAGE_BITMAP, 0, 0, 0);
		SendMessage(divideButton, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)divideOp);
		HWND equalButton = CreateWindow(L"Button", L"", BS_DEFPUSHBUTTON | WS_VISIBLE | WS_CHILD | BS_BITMAP, 20, 267, 149, 51, hwnd, (HMENU)IDB_BUTTON_EQUAL, NULL, NULL);
		HBITMAP equalOp = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BUTTON_EQUAL), IMAGE_BITMAP, 0, 0, 0);
		SendMessage(equalButton, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)equalOp);
		HWND clrButton = CreateWindow(L"Button", L"", BS_DEFPUSHBUTTON | WS_VISIBLE | WS_CHILD | BS_BITMAP, 170, 267, 149, 51, hwnd, (HMENU)IDB_BUTTON_CLR, NULL, NULL);
		HBITMAP clrOp = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BUTTON_CLR), IMAGE_BITMAP, 0, 0, 0);
		SendMessage(clrButton, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)clrOp);
	}
		break;
	case WM_COMMAND:			//Функция в которой выполняются все нажатые клавиши на клавиатуре или на виртуальных кпопках программы
	{
		int id = LOWORD(wParam);
		switch (id)
		{
		case IDB_BUTTON_0:
			setNumber(0, GetDlgItem(hwnd, IDR_EDIT));
			break;
		case IDB_BUTTON_1:
			setNumber(1, GetDlgItem(hwnd, IDR_EDIT));
			break;
		case IDB_BUTTON_2:
			setNumber(2, GetDlgItem(hwnd, IDR_EDIT));
			break;
		case IDB_BUTTON_3:
			setNumber(3, GetDlgItem(hwnd, IDR_EDIT));
			break;
		case IDB_BUTTON_4:
			setNumber(4, GetDlgItem(hwnd, IDR_EDIT));
			break;
		case IDB_BUTTON_5:
			setNumber(4, GetDlgItem(hwnd, IDR_EDIT));
		case IDB_BUTTON_6:
			setNumber(5, GetDlgItem(hwnd, IDR_EDIT));
			break;
		case IDB_BUTTON_7:
			setNumber(7, GetDlgItem(hwnd, IDR_EDIT));
			break;
		case IDB_BUTTON_8:
			setNumber(8, GetDlgItem(hwnd, IDR_EDIT));
			break;
		case IDB_BUTTON_9:
			setNumber(9, GetDlgItem(hwnd, IDR_EDIT));
			break;
		case IDB_BUTTON_PLUS:
			executeOperation(GetDlgItem(hwnd,IDR_EDIT));
			stOper.setOper(OPPLUS);
			break;
		case IDB_BUTTON_MINUS:
			executeOperation(GetDlgItem(hwnd, IDR_EDIT));
			stOper.setOper(OPMINUS);
		case IDB_BUTTON_ASTER:
			executeOperation(GetDlgItem(hwnd, IDR_EDIT));
			stOper.setOper(OPMULTI);
			break;
		case IDB_BUTTON_DIVIDE:
			executeOperation(GetDlgItem(hwnd, IDR_EDIT));
			break;
		case IDB_BUTTON_EQUAL:
			executeOperation(GetDlgItem(hwnd, IDR_EDIT));
			stOper.setOper(OPDIVIDE);
			break;
		case IDB_BUTTON_POINT:
			setPoint(GetDlgItem(hwnd, IDR_EDIT));
			break;
		case IDB_BUTTON_CLR:
			CONST INT SIZE = 256;
			WCHAR str[SIZE] = {};
			wsprintf(str, L"%s%d", str, 0);
			HWND hClr = GetDlgItem(hwnd, IDR_EDIT);
			SetWindowText(hClr, str);
			stOper.setStatus(NEWNUM);
			stOper.setOper(NOOPER);
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