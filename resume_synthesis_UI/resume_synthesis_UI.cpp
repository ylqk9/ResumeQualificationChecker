// resume_synthesis_UI.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "resume_synthesis_UI.h"
#include "Qualification.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
#define ID_MENURUN 201
HWND hWndTextbox_key;                           // TextBox for keywords
HWND hWndTextbox_out;                           // TextBox for output
qualification myqual;                           // Qualification Object

// Forward declarations of functions included in this code module:
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

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_RESUME_SYNTHESIS_UI, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_RESUME_SYNTHESIS_UI));

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
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
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
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_RESUME_SYNTHESIS_UI));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_RESUME_SYNTHESIS_UI);
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

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

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
	HMENU hMatchMenu = CreateMenu();
	//HMENU hMatchSub  = CreateMenu();
	RECT rect;
	int width, height;
	GetWindowRect(hWnd, &rect);
	width  = rect.right  - rect.left - 15;
	height = rect.bottom - rect.top - 18;

	switch (message)
	{
	case WM_CREATE:
		hWndTextbox_key = CreateWindow("edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE | WS_HSCROLL | WS_VSCROLL,
				0, 0, width, 130, hWnd, NULL, hInst, NULL);
		if (height - 140 > 0)
			hWndTextbox_out = CreateWindow("edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE | WS_HSCROLL | WS_VSCROLL, 
				0, 140, width, height - 180, hWnd, NULL, hInst, NULL);
		else
			hWndTextbox_out = CreateWindow("edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE | WS_HSCROLL | WS_VSCROLL, 
				0, 140, width, 100, hWnd, NULL, hInst, NULL);
		AppendMenu(hMatchMenu, MF_POPUP | MF_STRING , ID_MENURUN, "Match!");
		SetMenu(hWnd, hMatchMenu);
		//AppendMenu(hMatchSub, MF_STRING, ID_MENURUN, "Run");
		break;
	case WM_SIZE:
		MoveWindow (hWndTextbox_key,0,0,width,130,1);
		if (height - 158 > 0)
			MoveWindow (hWndTextbox_out,0,140,width, height - 180,1);
		else
			MoveWindow (hWndTextbox_out,0,140,width,100,1);
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case ID_MENURUN:
			char buf[9192];
			myqual.clean_content();
			SetWindowText(hWndTextbox_out, "\0");
			GetWindowText(hWndTextbox_key, buf, 9193);
			myqual.get_keywords(buf);
			myqual.get_qual_list();
			myqual.match_keywords_qual();
			myqual.print_char();
			SetWindowText(hWndTextbox_out, myqual.output_all.c_str());
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
