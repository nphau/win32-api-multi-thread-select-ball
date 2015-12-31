// G.4.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "G.4.h"
#include <stdlib.h>
#include <time.h>
#include <windowsx.h>

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_G4, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_G4));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
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
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_G4));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_G4);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateDialog(hInst, MAKEINTRESOURCE(IDD_MAINDLG), NULL, DlgProc);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	TCHAR buffer[200];
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		OnInitDialog(hDlg);
		_itow(iNumOfBall, buffer, 10);
		Edit_SetText(GetDlgItem(hDlg, IDC_EDITTONGSOBONG), buffer);
		_itow(iTimers, buffer, 10);
		Edit_SetText(GetDlgItem(hDlg, IDC_EDITTONGTHOIGIAN), buffer);
		return (INT_PTR)TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_BTNSTART:
			OnStart(hDlg);
			break;
		case IDC_BTNEND:
			KillTimer(hDlg, 1);
			break;
		}
		break;
	case WM_TIMER:
		ThoiGianConLai -= 200;
		if (ThoiGianConLai <= 0 || iNumOfRestBall <= 0)
			KillTimer(hDlg, 1);
		
		_itow(iNumOfGetBall[0], buffer, 10);
		Edit_SetText(GetDlgItem(hDlg, IDC_EDITDALAY1), buffer);
		
		_itow(iNumOfGetBall[1], buffer, 10);
		Edit_SetText(GetDlgItem(hDlg, IDC_EDITDALAY2), buffer);
		
		_itow(SoLanGiuKho[0], buffer, 10);
		Edit_SetText(GetDlgItem(hDlg, IDC_EDITGIUKHO1), buffer);

		_itow(SoLanGiuKho[1], buffer, 10);
		Edit_SetText(GetDlgItem(hDlg, IDC_EDITGIUKHO2), buffer);

		_itow(iNumOfRestBall, buffer, 10);
		Edit_SetText(GetDlgItem(hDlg, IDC_EDITBONGCONLAI), buffer);

		Edit_SetText(GetDlgItem(hDlg, IDC_EDITNAME1), teamName[0]);
		Edit_SetText(GetDlgItem(hDlg, IDC_EDITNAME2), teamName[1]);
		
		_itow(ThoiGianConLai, buffer, 10);
		Edit_SetText(GetDlgItem(hDlg, IDC_EDITTHOIGIANCONLAI), buffer);

		Edit_SetText(GetDlgItem(hDlg, IDC_EDITDOIHIENTAI), teamName[iCurrentTeam]);

		break;
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	}
	return (INT_PTR)FALSE;
}
