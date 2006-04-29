#include "FreeImageAlgorithms.h"
#include "FreeImageAlgorithms_IO.h"
#include "FreeImageAlgorithms_HBitmap.h"
#include "FreeImageAlgorithms_Palettes.h"
#include "FreeImageAlgorithms_LinearScale.h"
#include "FreeImageAlgorithms_MetaData.h"
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

FIBITMAP *dib;


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
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
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
void InitMainWnd()
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
HWND CreateMainWnd()
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


static void LinearScaleTest()
{
	double min_found, max_found;

	FIBITMAP* dib2 = FreeImageAlgorithms_LinearScaleToStandardType(dib, 533, 1275, &min_found, &max_found);

	FreeImageAlgorithms_SaveFIBToFile(dib2, "C:\\Documents and Settings\\Pierce\\Desktop\\scale_test.ics");
}


static void SaveIcsTest()
{
	dib = FreeImageAlgorithms_LoadFIBFromFile("C:\\Documents and Settings\\Pierce\\Desktop\\wallpaper_river.jpg", 0);

	if(FreeImageAlgorithms_SaveFIBToFile(dib, "C:\\Documents and Settings\\Pierce\\Desktop\\test.ics") == FREEIMAGE_ALGORITHMS_ERROR)
		std::cout << "Error" << std::endl;
}


//
//	Entry-point for text-editor application
//
//int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR lpCmdLine, int iShowCmd)
//{




void ImageViewer_SetPalette(HDC hdc, HBITMAP hbitmap, RGBQUAD *palette2) 
{
	SetDIBColorTable(hdc, 0, 256, palette2);
}


int main()
{
	MSG			msg;
	
	// initialize window classes
	InitMainWnd();
	
	// create the main window!
	hwndMain = CreateMainWnd();
	hdc = GetDC(hwndMain);

	//dib = FreeImageAlgorithms_LoadFIBFromFile("C:\\Documents and Settings\\Pierce\\Desktop\\rjl.ics", 0);

	//char units[500];     

	//FreeImageIcsPointer fip;	
	//FreeImageAlgorithms_OpenIcsFile(&fip, "C:\\Documents and Settinigs\\Pierce\\Desktop\\rjl.ics", "r");     

	//FreeImageAlgorithms_GetFirstIcsHistoryValueWithKey(fip, "units", units); 






	unsigned char *hist = (unsigned char *) malloc (1 * (21 + 13 + 1) ); 

	hist[0] = 160;
	hist[1] = 160;
	hist[2] = 160;
	hist[3] = 160;
	hist[4] = 160;
	hist[5] = 160;
	hist[6] = 160;
	hist[7] = 160;
	hist[8] = 160;
	hist[9] = 160;
	hist[10] = 160;
	hist[11] = 160;
	hist[12] = 160;
	hist[13] = 144;
	hist[14] = 144;
	hist[15] = 144;
	hist[16] = 144;
	hist[17] = 144;
	hist[18] = 144;
	hist[19] = 144;
	hist[20] = 144;

	FreeImageAlgorithms_ArrayReverse(hist, 17);

	for(int i=0; i<21; i++)
		std::cout << i << " " << i << "  value " << (int) hist[i] << std::endl;

	double *xdata = (double *)calloc(21, sizeof(double));  


	/*
	hbitmap = FreeImageAlgorithms_GetDibSection(dib, hdc, 0, 0,
						FreeImage_GetWidth(dib), FreeImage_GetHeight(dib));
		

	hbitmap_hdc = CreateCompatibleDC(hdc);
	*/

	/* Associate the new DC with the bitmap for drawing */

	/*
	SelectObject( hbitmap_hdc, hbitmap );


	RGBQUAD *palette = (RGBQUAD *) malloc(sizeof(RGBQUAD) * 256);

	FreeImageAlgorithms_GetRainBowPalette(palette);

	ImageViewer_SetPalette(hbitmap_hdc, hbitmap, palette); 



	if(GetDIBColorTable(hbitmap_hdc, 0, 256, palette) <= 0)
		printf("What\n");

	DWORD err = GetLastError();

	//for(i=0; i < 256; i++)
	//	printf("%d %d %d \n", window->current_palette[i].rgbRed, window->current_palette[i].rgbGreen, window->current_palette[i].rgbBlue);	
		

	for(int i=0; i < 256; i++)
		printf("%d %d %d \n", palette[i].rgbRed, palette[i].rgbGreen, palette[i].rgbBlue);	

	ShowWindow(hwndMain, 1);
	*/

	// message-loop
	while(GetMessage(&msg, NULL, 0, 0) > 0)
	{
		DispatchMessage(&msg);
	}


	return 0;
}