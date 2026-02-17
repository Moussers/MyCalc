#include <windows.h>
#include <windowsx.h>
#include <cstdio>
#include "resource.h"
#include <string>
#include <cmath>

#define POS_STATIC_BOX_X 20
#define POS_STATIC_BOX_Y 15
#define MEASUR_STATIC_BOX_WIDTH 300
#define MEASUR_STATIC_BOX_HEIGHT 30
#define POS_ZERO_X 20
#define POS_ZERO_Y 215
#define POS_ZERO_HEIGHT 51
#define POS_POINT_X 95
#define POS_POINT_Y 213
#define POS_NUMBER_WIDTH 74
#define POS_NUMBER_HEIGHT 53
#define NUMBER_BUTTONS 18

enum TypeOpeation 
{
	NOOPER, OPPLUS, OPMINUS, OPMULTI, OPDIVIDE, OPEXPONENT, OPROOT
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
void removeNumber(HWND field)
{ 
	CONST INT SIZE = 256;
	WCHAR str[SIZE];
	if (stOper.status() != NEWNUM)
	{
		GetWindowText(field, str, SIZE);
		int i = 0;
		while (str[i++] != 0);		//ищем конец строки. через цикл.
		if (i > 1) 
		{
			str[i - 2] = 0;
		}
		SetWindowText(field, str);
	}
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
			case OPEXPONENT:
			{
				int p = std::round(r);
				double result = 1;
				for (int i = 0; i < p; ++i)
				{
					result *= stOper.currentNumber();
				}
				stOper.setNumber(result);
			}
			break;
			case OPROOT: 
			{
				double left = 0;
				double right = r + 1;
				double middle = 0;
				for (int i = 0; i < 40; ++i)
				{
					middle = (left + right) / 2;
					if (middle * middle < r)
					{
						left = middle;
					}
					else
					{
						right = middle;
					}
				}
				stOper.setNumber(left);
			}
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
		HWND zeroButton = CreateWindow(L"Button", L"", BS_PUSHBUTTON | WS_VISIBLE | WS_CHILD | BS_BITMAP, POS_ZERO_X, POS_ZERO_Y, POS_NUMBER_WIDTH, POS_ZERO_HEIGHT, hwnd, (HMENU)IDB_BUTTON_C_0, NULL, NULL);
		HBITMAP ZB = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BUTTON_C_0), IMAGE_BITMAP, 0, 0, 0);
		SendMessage(zeroButton, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)ZB);
		HWND pointButton = CreateWindow(L"Button", L"", BS_DEFPUSHBUTTON | WS_VISIBLE | WS_CHILD | BS_BITMAP, POS_POINT_X, POS_POINT_Y, POS_NUMBER_WIDTH, POS_NUMBER_HEIGHT, hwnd, (HMENU)IDB_BUTTON_C_POINT, NULL, NULL);
		HBITMAP pointB = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BUTTON_C_POINT), IMAGE_BITMAP, 0, 0, 0);
		SendMessage(pointButton, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)pointB);
		int x = 170;
		int y = 50;
		for (int j = 9; j > 0; j -= 3) {
			for (int i = 1; i <= 3; ++i)
			{
				HWND button = CreateWindow(L"Button", L"", BS_DEFPUSHBUTTON | WS_VISIBLE | WS_CHILD | BS_BITMAP, x, y, POS_NUMBER_WIDTH, POS_NUMBER_HEIGHT, hwnd, (HMENU)(IDB_BUTTON_C_1 + j - i), NULL, NULL);
				HBITMAP BT = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BUTTON_C_1 + j - i), IMAGE_BITMAP, 0, 0, 0);
				SendMessage(button, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)BT);
				x -= 75;
			}
			x = 170;
			y += 55;
		}
		HWND plusBt = CreateWindow(L"Button", L"", BS_DEFPUSHBUTTON | WS_VISIBLE | WS_CHILD | BS_BITMAP, 245, 50, POS_NUMBER_WIDTH, POS_NUMBER_HEIGHT, hwnd, (HMENU)IDB_BUTTON_C_PLUS, NULL, NULL);
		HBITMAP plusOp = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BUTTON_C_PLUS), IMAGE_BITMAP, 0, 0, 0);
		SendMessage(plusBt, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)plusOp);
		HWND minusBt = CreateWindow(L"Button", L"", BS_DEFPUSHBUTTON | WS_VISIBLE | WS_CHILD | BS_BITMAP, 245, 105, POS_NUMBER_WIDTH, 54, hwnd, (HMENU)IDB_BUTTON_C_MINUS, NULL, NULL);
		HBITMAP minusOp = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BUTTON_C_MINUS), IMAGE_BITMAP, 0, 0, 0);
		SendMessage(minusBt, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)minusOp);
		HWND asterButton = CreateWindow(L"Button", L"", BS_DEFPUSHBUTTON | WS_VISIBLE | WS_CHILD | BS_BITMAP, 245, 160, POS_NUMBER_WIDTH, 54, hwnd, (HMENU)IDB_BUTTON_C_ASTER, NULL, NULL);
		HBITMAP asterOp = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BUTTON_C_ASTER), IMAGE_BITMAP, 0, 0, 0);
		SendMessage(asterButton, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)asterOp);
		HWND divideBt = CreateWindow(L"Button", L"", BS_DEFPUSHBUTTON | WS_VISIBLE | WS_CHILD | BS_BITMAP, 245, 212, POS_NUMBER_WIDTH, 54, hwnd, (HMENU)IDB_BUTTON_C_DIVIDE, NULL, NULL);
		HBITMAP divideOp = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BUTTON_C_DIVIDE), IMAGE_BITMAP, 0, 0, 0);
		SendMessage(divideBt, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)divideOp);
		HWND equalBt = CreateWindow(L"Button", L"", BS_DEFPUSHBUTTON | WS_VISIBLE | WS_CHILD | BS_BITMAP, 20, 267, 149, 51, hwnd, (HMENU)IDB_BUTTON_C_EQUAL, NULL, NULL);
		HBITMAP equalOp = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BUTTON_C_EQUAL), IMAGE_BITMAP, 0, 0, 0);
		SendMessage(equalBt, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)equalOp);
		HWND clrBt = CreateWindow(L"Button", L"", BS_DEFPUSHBUTTON | WS_VISIBLE | WS_CHILD | BS_BITMAP, 170, 267, POS_NUMBER_WIDTH, 51, hwnd, (HMENU)IDB_BUTTON_C_CLR, NULL, NULL);
		HBITMAP clrOp = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BUTTON_C_CLR), IMAGE_BITMAP, 0, 0, 0);
		SendMessage(clrBt, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)clrOp);
		HWND exponBt = CreateWindow(L"Button", L"", BS_DEFPUSHBUTTON | WS_VISIBLE | WS_CHILD | BS_BITMAP, 245, 267, POS_NUMBER_WIDTH, 51, hwnd, (HMENU)IDB_BUTTON_C_EXPON, NULL, NULL);
		HBITMAP exponOp = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BUTTON_C_EXPON), IMAGE_BITMAP, 0, 0, 0);
		SendMessage(exponBt, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)exponOp);
		HWND rootBt = CreateWindow(L"Button", L"", BS_DEFPUSHBUTTON | WS_VISIBLE | WS_CHILD | BS_BITMAP, 170, 213, POS_NUMBER_WIDTH, POS_NUMBER_HEIGHT, hwnd, (HMENU)IDB_BUTTON_C_ROOT, NULL, NULL);
		HBITMAP rootOp = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BUTTON_C_ROOT), IMAGE_BITMAP, 0, 0, 0);
		SendMessage(rootBt, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)rootOp);
	}
	break;
	case WM_CONTEXTMENU: 
	{
		//WM_RBUTTONDOWN - обрабатывает событие нажатия правой кнопкой мыши по пустому окну.
		//WN_CINTEXTMENU - обрабатывает событие нажатия правой кнопкой мыши по кнопкам.
		HMENU calcMenu = CreatePopupMenu();							//CreatePopupMenu создает handle меню для popup menu
		AppendMenu(calcMenu, MF_STRING, IDB_FIRST_POPUP, L"Кастомный фон");
		AppendMenu(calcMenu, MF_STRING, IDB_SECOND_POPUP, L"Красный фон");
		INT xPos = GET_X_LPARAM(lParam);							//Получаем координаты x для отображения вплывающего окна
		INT yPos = GET_Y_LPARAM(lParam);							//Получаем координаты y для отображения вплывающего окна
		TrackPopupMenu(calcMenu, TPM_LEFTALIGN | TPM_TOPALIGN, xPos, yPos, 0, hwnd, NULL);		//TrackpopupMenu отображает само высплывающее окно
		DestroyMenu(calcMenu);
	}
		break;
	case WM_COMMAND:			//Функция в которой выполняются все нажатые клавиши на клавиатуре или на виртуальных кпопках программы
	{
		int id = LOWORD(wParam);
		switch (id)
		{
		case IDB_BUTTON_C_0:
			setNumber(0, GetDlgItem(hwnd, IDR_EDIT));
			break;
		case IDB_BUTTON_C_1:
			setNumber(1, GetDlgItem(hwnd, IDR_EDIT));
			break;
		case IDB_BUTTON_C_2:
			setNumber(2, GetDlgItem(hwnd, IDR_EDIT));
			break;
		case IDB_BUTTON_C_3:
			setNumber(3, GetDlgItem(hwnd, IDR_EDIT));
			break;
		case IDB_BUTTON_C_4:
			setNumber(4, GetDlgItem(hwnd, IDR_EDIT));
			break;
		case IDB_BUTTON_C_5:
			setNumber(5, GetDlgItem(hwnd, IDR_EDIT));
			break;
		case IDB_BUTTON_C_6:
			setNumber(6, GetDlgItem(hwnd, IDR_EDIT));
			break;
		case IDB_BUTTON_C_7:
			setNumber(7, GetDlgItem(hwnd, IDR_EDIT));
			break;
		case IDB_BUTTON_C_8:
			setNumber(8, GetDlgItem(hwnd, IDR_EDIT));
			break;
		case IDB_BUTTON_C_9:
			setNumber(9, GetDlgItem(hwnd, IDR_EDIT));
			break;
		case IDB_BUTTON_C_PLUS:
			executeOperation(GetDlgItem(hwnd, IDR_EDIT));
			stOper.setOper(OPPLUS);
			break;
		case IDB_BUTTON_C_MINUS:
			executeOperation(GetDlgItem(hwnd, IDR_EDIT));
			stOper.setOper(OPMINUS);
		case IDB_BUTTON_C_ASTER:
			executeOperation(GetDlgItem(hwnd, IDR_EDIT));
			stOper.setOper(OPMULTI);
			break;
		case IDB_BUTTON_C_DIVIDE:
			executeOperation(GetDlgItem(hwnd, IDR_EDIT));
			stOper.setOper(OPDIVIDE);
			break;
		case IDB_BUTTON_C_EQUAL:
			executeOperation(GetDlgItem(hwnd, IDR_EDIT));
			stOper.setOper(OPDIVIDE);
			break;
		case IDB_BUTTON_C_POINT:
			setPoint(GetDlgItem(hwnd, IDR_EDIT));
			break;
		case IDB_BUTTON_C_EXPON:
			executeOperation(GetDlgItem(hwnd, IDR_EDIT));
			stOper.setOper(OPEXPONENT);
			break;
		case IDB_BUTTON_C_ROOT:
			executeOperation(GetDlgItem(hwnd, IDR_EDIT));
			stOper.setOper(OPROOT);
			break;
		case IDB_FIRST_POPUP:
		{
			for (int i = 0; i < NUMBER_BUTTONS; ++i) 
			{
				HWND hBC = GetDlgItem(hwnd, IDB_BUTTON_C_0 + i);
				HBITMAP HBpZ = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BUTTON_C_0 + i), IMAGE_BITMAP, 0, 0, 0);
				SendMessage(hBC, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)HBpZ);
			}
		}
			break;
		case IDB_SECOND_POPUP:
		{
			for (int i = 0; i < NUMBER_BUTTONS; ++i)
			{
				HWND hBR = GetDlgItem(hwnd, IDB_BUTTON_C_0 + i);
				HBITMAP HBpZ = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BUTTON_R_0 + i), IMAGE_BITMAP, 0, 0, 0);
				SendMessage(hBR, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)HBpZ);
			}
		}
			break;
		case IDB_BUTTON_C_CLR:
			CONST INT SIZE = 256;
			WCHAR str[SIZE] = {};
			wsprintf(str, L"%s%d", str, 0);
			HWND hClr = GetDlgItem(hwnd, IDR_EDIT);
			SetWindowText(hClr, str);
			stOper.setStatus(NEWNUM);
			stOper.setOper(NOOPER);
			break;
		}
		SetFocus(hwnd);
		//SetFocus устанавливает снова потрянный фокус с клавиатуры.
	}
	break;
	case WM_KEYDOWN:
	//WM_KEYDOWN контроилирует какая кнопка нажата в данный момент.
	{
		switch (wParam)
		{
		//в wParam хранится информация о нажатой кнопке.
		case VK_NUMPAD0:
			setNumber(0, GetDlgItem(hwnd, IDR_EDIT));
			break;
		case '0':
			setNumber(0, GetDlgItem(hwnd, IDR_EDIT));
			break;
		case VK_NUMPAD1:
			setNumber(1, GetDlgItem(hwnd, IDR_EDIT));
			break;
		case '1':
			setNumber(1, GetDlgItem(hwnd, IDR_EDIT));
			break;
		case VK_NUMPAD2:
			setNumber(2, GetDlgItem(hwnd, IDR_EDIT));
			break;
		case '2':
			setNumber(2, GetDlgItem(hwnd, IDR_EDIT));
			break;
		case VK_NUMPAD3:
			setNumber(3, GetDlgItem(hwnd, IDR_EDIT));
			break;
		case '3':
			setNumber(3, GetDlgItem(hwnd, IDR_EDIT));
			break;
		case VK_NUMPAD4:
			setNumber(4, GetDlgItem(hwnd, IDR_EDIT));
			break;
		case '4':
			setNumber(4, GetDlgItem(hwnd, IDR_EDIT));
			break;
		case VK_NUMPAD5:
			setNumber(5, GetDlgItem(hwnd, IDR_EDIT));
			break;
		case '5':
			setNumber(5, GetDlgItem(hwnd, IDR_EDIT));
			break;
		case VK_NUMPAD6:
			setNumber(6, GetDlgItem(hwnd, IDR_EDIT));
			break;
		case '6':
			setNumber(6, GetDlgItem(hwnd, IDR_EDIT));
			break;
		case VK_NUMPAD7:
			setNumber(1, GetDlgItem(hwnd, IDR_EDIT));
			break;
		case '7':
			setNumber(7, GetDlgItem(hwnd, IDR_EDIT));
			break;
		case VK_NUMPAD8:
			setNumber(8, GetDlgItem(hwnd, IDR_EDIT));
			break;
		case '8':
			if (GetKeyState(VK_SHIFT)) 
			{
				executeOperation(GetDlgItem(hwnd, IDR_EDIT));
				stOper.setOper(OPMULTI);
			}
			else
			{
				setNumber(8, GetDlgItem(hwnd, IDR_EDIT));
			}
			break;
		case VK_NUMPAD9:
			setNumber(9, GetDlgItem(hwnd, IDR_EDIT));
			break;
		case '9':
			setNumber(9, GetDlgItem(hwnd, IDR_EDIT));
			break;
		case VK_OEM_PLUS:
			//GetKeyState -  проверяет нажата кнопка или нетб то есть состояние кнопки.
			if (GetKeyState(VK_SHIFT))
			{
				executeOperation(GetDlgItem(hwnd, IDR_EDIT));
				stOper.setOper(OPPLUS);
			}
			else
			{
				executeOperation(GetDlgItem(hwnd, IDR_EDIT));
			}
			break;
		case VK_OEM_MINUS:
			//vk_oem - отвечает за нажатие кнопопок на левой части клавиатуры символов, цифр и других водянистых знаков.
			executeOperation(GetDlgItem(hwnd, IDR_EDIT));
			stOper.setOper(OPMINUS);
			break;
		case VK_SUBTRACT:
		//VK_SUBSRUCT знак минус на numpad.
			executeOperation(GetDlgItem(hwnd, IDR_EDIT));
			stOper.setOper(OPMINUS);
			break;
		case VK_MULTIPLY:
			executeOperation(GetDlgItem(hwnd, IDR_EDIT));
			stOper.setOper(OPMULTI);
			break;
		case VK_OEM_2:
			//VK_OEM_2 - это знак деления на numpad.
			executeOperation(GetDlgItem(hwnd, IDR_EDIT));
			stOper.setOper(OPDIVIDE);
			break;
		case VK_RETURN:
			//нажатие клавиши равно и с левой и правой части.
			executeOperation(GetDlgItem(hwnd, IDR_EDIT));
			break;
		case VK_BACK:
			removeNumber(GetDlgItem(hwnd, IDR_EDIT));
			break;
		case VK_OEM_PERIOD:
			//клавиша точка на левой части клавиатуры.
			setPoint(GetDlgItem(hwnd, IDR_EDIT));
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