#include "stdafx.h"
#include "TankFinal.h"
#include <time.h>
#include "Init.h"
#include "GameProc.h"
#include "Entity.h"
#include <windows.h>
#include<Mmsystem.h>
#pragma comment(lib,"winmm.lib")
using namespace std;




#define MAX_LOADSTRING 100


HINSTANCE hInst;								
TCHAR szTitle[MAX_LOADSTRING];					
TCHAR szWindowClass[MAX_LOADSTRING];			

int reset_time = 0;								


ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR    lpCmdLine,
	int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	
	MSG msg;
	HACCEL hAccelTable;

	
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_TANKFINAL, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TANKFINAL));

	srand(time(NULL));			
	ChangeLevel(OPEN);			

	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}

int main(void)
{
	PlaySound(L"1.wav", NULL, SND_ASYNC | SND_NODEFAULT);
}


ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TANKFINAL));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;/*MAKEINTRESOURCE(IDC_TANKFINAL);*/	
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}


BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;

	hInst = hInstance; 

	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		return FALSE;
	}
	SetWindowLong(hWnd, GWL_STYLE, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU);	
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
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
		break;
	case WM_CREATE:			
		SetTimer(hWnd, 1, GetTimeStep(), NULL);
		SetTimer(hWnd, 2, GetBirthTime(), NULL);
		SetTimer(hWnd, 3, GetAnimStep(), NULL);
		break;
	case WM_TIMER:			
		InvalidateRect(hWnd, NULL, TRUE);	
		switch (wParam)
		{
		case 1:				
			Update(GetTimeStep() / 10);
			if (reset_time)	
			{
				reset_time = 0;
				KillTimer(hWnd, 1);		
				KillTimer(hWnd, 2);
				KillTimer(hWnd, 3);
				SetTimer(hWnd, 1, GetTimeStep(), NULL);
				SetTimer(hWnd, 2, GetBirthTime(), NULL);
				SetTimer(hWnd, 3, GetAnimStep(), NULL);
			}
			break;
		case 2:				
			EnemyBirth();
			break;
		case 3:				
			NextFrame(GetAnimStep());
			break;
		}
		break;
	case WM_SIZE:			
		ChangeWndSize(LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_KEYDOWN:		
		InvalidateRect(hWnd, NULL, TRUE);
		EnterKey(wParam);
		break;
	case WM_ERASEBKGND:		
		break;
	case WM_PAINT:
	{
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		Draw(hdc, hWnd);		
		EndPaint(hWnd, &ps);
		break;
	}
	case WM_DESTROY:
		KillTimer(hWnd, 1);		
		KillTimer(hWnd, 2);
		KillTimer(hWnd, 3);
		Destroy();
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
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
