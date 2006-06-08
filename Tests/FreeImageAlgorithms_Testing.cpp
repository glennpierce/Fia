#include "FreeImageAlgorithms.h"
#include "FreeImageAlgorithms_Testing.h"
#include "FreeImageAlgorithms_IO.h"
#include "FreeImageAlgorithms_HBitmap.h"
#include "FreeImageAlgorithms_Palettes.h"
#include "FreeImageAlgorithms_LinearScale.h"
#include "FreeImageAlgorithms_Statistics.h"
#include "FreeImageAlgorithms_Utilities.h"
#include "FreeImageAlgorithms_Utils.h"

#include <iostream>
#include <assert.h>

#include <windows.h>
#include <tchar.h>
#include <commctrl.h>

#define APP_TITLE   _T("CanvasTest")

HWND		hwndMain;
HDC			hdc;
HDC			hbitmap_hdc;
HBITMAP		hbitmap;

DIBSECTION	dib_section;

FIBITMAP	*copy_dib;

TCHAR szFileName[MAX_PATH];
TCHAR szFileTitle[MAX_PATH];
TCHAR		szAppName[] = APP_TITLE;

static void PaintRect(HWND hwnd, HDC hdc, COLORREF colour)
{
	RECT rect;
    COLORREF oldcr = SetBkColor(hdc, colour);
    
	GetClientRect(hwnd, &rect);
	
	ExtTextOut(hdc, 0, 0, ETO_OPAQUE, &rect, "", 0, 0);
    SetBkColor(hdc, oldcr);
}


//
//	Main window procedure
//
static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static int width, height;

	switch(msg)
	{
		case WM_PAINT:
		{
			//PaintRect(hwnd, hdc, RGB(255,0,0));
			HDC          hdc;
			PAINTSTRUCT  ps;
	
			// Get a device context for this window
			hdc = BeginPaint(hwnd, &ps);

			SetStretchBltMode(hdc, COLORONCOLOR);

			BitBlt(hdc, 0, 0, 800, 600, hbitmap_hdc, 0, 0, SRCCOPY); 

			// Release the device context
			EndPaint(hwnd, &ps);

			return 0;
		}

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_CLOSE:
		DestroyWindow(hwnd);
		return 0;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

//
//	Register main window class
//
static void InitMainWnd()
{
	WNDCLASSEX wcx;
	HANDLE hInst = GetModuleHandle(0);

	// Window class for the main application parent window
	wcx.cbSize			= sizeof(wcx);
	wcx.style			= 0;
	wcx.lpfnWndProc		= WndProc;
	wcx.cbClsExtra		= 0;
	wcx.cbWndExtra		= 0;
	wcx.hInstance		= (HINSTANCE) hInst;
	wcx.hCursor			= LoadCursor (NULL, IDC_ARROW);
	wcx.hbrBackground	= (HBRUSH)0;
	wcx.lpszMenuName	= 0;
	wcx.lpszClassName	= szAppName;
	wcx.hIcon			= 0;
	wcx.hIconSm			= 0;

	RegisterClassEx(&wcx);
}

//
//	Create a top-level window
//
static HWND CreateMainWnd()
{
	return CreateWindowEx(0,
				szAppName,				// window class name
				szAppName,				// window caption
				WS_OVERLAPPEDWINDOW|WS_CLIPCHILDREN,
				CW_USEDEFAULT,			// initial x position
				CW_USEDEFAULT,			// initial y position
				800,					// initial x size
				600,					// initial y size
				NULL,					// parent window handle
				NULL,					// use window class menu
				GetModuleHandle(0),		// program instance handle
				NULL);					// creation parameters
}


void
ShowImage(FIBITMAP *src)
{
	MSG msg;

	// initialize window classes
	InitMainWnd();

	hwndMain = CreateMainWnd();
	hdc = GetDC(hwndMain);

	hbitmap = FreeImageAlgorithms_GetDibSection(src, hdc, 0, 0,
						FreeImage_GetWidth(src), FreeImage_GetHeight(src));
		
	hbitmap_hdc = CreateCompatibleDC(hdc);

	// Associate the new DC with the bitmap for drawing 
	SelectObject( hbitmap_hdc, hbitmap );

	ShowWindow(hwndMain, 1);

	// message-loop
	while(GetMessage(&msg, NULL, 0, 0) > 0)
	{
		DispatchMessage(&msg);
	}
}

void
ShowImageFromFile(char *filepath)
{
	FIBITMAP* dib = FreeImageAlgorithms_LoadFIBFromFile(filepath);

	assert(dib != NULL);

	ShowImage(dib);

	FreeImage_Unload(dib);
}