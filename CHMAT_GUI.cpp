#include <windows.h>  
#include <gdiplus.h>
#include "chmat.h"
#include <string>
#include <string>
#include <cstring>
#include <iostream>
#include <iostream>
#include <conio.h>
#include <stdlib.h>
#include <time.h>

using namespace Gdiplus;

#define IDI_ICON1 101
#define IDC_OPEN	3000
#define IDC_BUTTON1 3001
#define IDC_BUTTON2 3002
//#define IDC_BUTTON3 3003

using namespace std;
HINSTANCE hInst;   // current instance
HANDLE hTimerQueue = NULL;
HWND     hWnd, hEdit,HButton1, HButton2, Hmainbmp;    //parent window
int keypressed, threadflag = 1;
int boxlocation = 100;
int boxlocationy = 150;
int pointblue = 0;
LPCTSTR lpszAppName = "GEN32";
LPCTSTR lpszTitle = "GENERIC32";
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
ULONG_PTR           gdiplusToken;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPTSTR lpCmdLine, int nCmdShow)
{
	MSG      msg;
	WNDCLASSEX wc;
	HMENU MainMenu, FileMenu;
	MainMenu = CreateMenu();
	FileMenu = CreatePopupMenu();
	AppendMenu(FileMenu, MF_STRING, IDC_OPEN, "Open");
	//AppendMenu(MainMenu, MF_POPUP, (UINT_PTR)FileMenu, "FILE");

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = (WNDPROC)WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW);
	wc.lpszMenuName = lpszAppName;
	wc.lpszClassName = lpszAppName;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hIconSm = (HICON)LoadImage(hInstance, lpszAppName,
		IMAGE_ICON, 16, 16,
		LR_DEFAULTCOLOR);

	if (!RegisterClassEx(&wc))
		return(FALSE);

	hInst = hInstance;
	hWnd = CreateWindowEx(0, lpszAppName,
		lpszTitle,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		900, 600,
		NULL,
		MainMenu,
		hInstance,
		NULL
	);
	if (!hWnd)
		return(FALSE);
	GdiplusStartupInput gdiplusStartupInput;
	// Initialize GDI+.
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return(msg.wParam);
}
CHMAT m(300, 300);
VOID CALLBACK SlidingBox(PVOID lpParam, BOOLEAN TimerOrWaitFired)
{
	SendMessage(Hmainbmp, STM_SETIMAGE, 0, (LPARAM)m.HBitmap);		
	SendMessage(Hmainbmp, STM_SETIMAGE, 0, (LPARAM)m.HBitmap);
	SendMessage(Hmainbmp, STM_SETIMAGE, 0, (LPARAM)m.HBitmap);
	//sepetin sağa sola hareketine göre konumunu değiştirir
	if (keypressed == 37) boxlocation -= 2;//sol
	if (keypressed == 39) boxlocation += 2;//sağ
	if (keypressed == 38) boxlocationy += 2;
	if (keypressed == 40) boxlocationy -= 2;
	SendMessage(Hmainbmp, STM_SETIMAGE, 0, (LPARAM)m.HBitmap);	
}
VOID* Alien(PVOID lpParam)
{
	Rectangle(m, 220, 80, 20, 200, 0xffffff, true);
	Rectangle(m, 20, 50, 200, 15, 0xffffff, true);
	//blue ball
	int bx = 100;
	int by = 150;
	srand(time(NULL));
	int directionbx = (1 + rand() % 10) > 5 ? 1 : -1;//directionbx=1 => blue ball move to right directionbx=-1 => blue ball move to left
	srand(time(NULL));
	int directionby = (1 + rand() % 8) > 4 ? 1 : -1;//directionby=1 => blue ball move to up directionbx=-1 => blue ball move to down
	//yellow ball
	int yx = 30;
	int yy = 220;
	srand(time(NULL));
	int directionyx = (1 + rand() % 2) > 1 ? 1 : -1;
	srand(time(NULL));
	int directionyy = (1 + rand() % 6) > 3 ? 1 : -1;
	//green ball
	int gx = 100;
	int gy = 110;
	srand(time(NULL));
	int directiongx = (1 + rand() % 12) > 6 ? 1 : -1;
	srand(time(NULL));
	int directiongy = (1 + rand() % 14) > 7 ? 1 : -1;
	//red ball
	int rx = 50;
	int ry = 130;
	srand(time(NULL));
	int directionrx = (1 + rand() % 18) > 9 ? 1 : -1;
	srand(time(NULL));
	int directionry = (1 + rand() % 4) > 2 ? 1 : -1;

	while (threadflag)
	{
		//Blue ball moving
		if (bx == 205)directionbx = -1;
		else if (bx == 0)directionbx = 1;
		if (by == 290)directionby = -1;
		else if (by == 75)directionby = 1;
		if (directionbx > 0 && directionby > 0) {
			Sleep(50);
			Ellipse(m, bx, by, 10, 10, 0x000000, true);
			Ellipse(m, bx + 5, by + 5, 10, 10, 0x0000ff, true);
			bx += 5;
			by += 5;
		}
		else if (directionbx > 0 && directionby < 0) {
			Sleep(50);
			Ellipse(m, bx, by, 10, 10, 0x000000, true);
			Ellipse(m, bx + 5, by - 5, 10, 10, 0x0000ff, true);
			bx += 5;
			by -= 5;
		}
		else if (directionbx < 0 && directionby>0) {
			Sleep(50);
			Ellipse(m, bx, by, 10, 10, 0x000000, true);
			Ellipse(m, bx - 5, by + 5, 10, 10, 0x0000ff, true);
			bx -= 5;
			by += 5;
		}
		else {
			Sleep(50);
			Ellipse(m, bx, by, 10, 10, 0x000000, true);
			Ellipse(m, bx - 5, by - 5, 10, 10, 0x0000ff, true);
			bx -= 5;
			by -= 5;
		}
		//Yellow ball moving
		if (yx == 205)directionyx = -1;
		else if (yx == 0)directionyx = 1;
		if (yy == 290)directionyy = -1;
		else if (yy == 75)directionyy = 1;
		if (directionyx > 0 && directionyy > 0) {
			Sleep(50);
			Ellipse(m, yx, yy, 10, 10, 0x000000, true);
			Ellipse(m, yx + 5, yy + 5, 10, 10, 0xffa500, true);
			yx += 5;
			yy += 5;
		}
		else if (directionyx > 0 && directionyy < 0) {
			Sleep(50);
			Ellipse(m, yx, yy, 10, 10, 0x000000, true);
			Ellipse(m, yx + 5, yy - 5, 10, 10, 0xffa500, true);
			yx += 5;
			yy -= 5;
		}
		else if (directionyx < 0 && directionyy>0) {
			Sleep(50);
			Ellipse(m, yx, yy, 10, 10, 0x000000, true);
			Ellipse(m, yx - 5, yy + 5, 10, 10, 0xffa500, true);
			yx -= 5;
			yy += 5;
		}
		else {
			Sleep(50);
			Ellipse(m, yx, yy, 10, 10, 0x000000, true);
			Ellipse(m, yx - 5, yy - 5, 10, 10, 0xffa500, true);
			yx -= 5;
			yy -= 5;
		}
		//Green ball moving
		if (gx == 205)directiongx = -1;
		else if (gx == 0)directiongx = 1;
		if (gy == 290)directiongy = -1;
		else if (gy == 75)directiongy = 1;
		if (directiongx > 0 && directiongy > 0) {
			Sleep(50);
			Ellipse(m, gx, gy, 10, 10, 0x000000, true);
			Ellipse(m, gx + 5, gy + 5, 10, 10, 0x00ff00, true);
			gx += 5;
			gy += 5;
		}
		else if (directiongx > 0 && directiongy < 0) {
			Sleep(50);
			Ellipse(m, gx, gy, 10, 10, 0x000000, true);
			Ellipse(m, gx + 5, gy - 5, 10, 10, 0x00ff00, true);
			gx += 5;
			gy -= 5;
		}
		else if (directiongx < 0 && directiongy>0) {
			Sleep(50);
			Ellipse(m, gx, gy, 10, 10, 0x000000, true);
			Ellipse(m, gx - 5, gy + 5, 10, 10, 0x00ff00, true);
			gx -= 5;
			gy += 5;
		}
		else {
			Sleep(50);
			Ellipse(m, gx, gy, 10, 10, 0x000000, true);
			Ellipse(m, gx - 5, gy - 5, 10, 10, 0x00ff00, true);
			gx -= 5;
			gy -= 5;
		}
		//Red ball moving
		if (rx == 205)directionrx = -1;
		else if (rx == 0)directionrx = 1;
		if (ry == 290)directionry = -1;
		else if (ry == 75)directionry = 1;
		if (directionrx > 0 && directionry > 0) {
			Sleep(50);
			Ellipse(m, rx, ry, 10, 10, 0x000000, true);
			Ellipse(m, rx + 5, ry + 5, 10, 10, 0x00ff0000, true);
			rx += 5;
			ry += 5;
		}
		else if (directionrx > 0 && directionry < 0) {
			Sleep(50);
			Ellipse(m, rx, ry, 10, 10, 0x000000, true);
			Ellipse(m, rx + 5, ry - 5, 10, 10, 0x00ff0000, true);
			rx += 5;
			ry -= 5;
		}
		else if (directionrx < 0 && directionry>0) {
			Sleep(50);
			Ellipse(m, rx, ry, 10, 10, 0x000000, true);
			Ellipse(m, rx - 5, ry + 5, 10, 10, 0x00ff0000, true);
			rx -= 5;
			ry += 5;
		}
		else {
			Sleep(50);
			Ellipse(m, rx, ry, 10, 10, 0x000000, true);
			Ellipse(m, rx - 5, ry - 5, 10, 10, 0x00ff0000, true);
			rx -= 5;
			ry -= 5;
		}

	}
	return(0);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		
	case WM_CREATE:
	{
		
		hEdit = CreateWindowEx(WS_EX_CLIENTEDGE,"Edit", "", WS_CHILD | WS_VISIBLE |
			ES_AUTOVSCROLL | ES_MULTILINE | ES_WANTRETURN | WS_VSCROLL, 735, 5, 150, 100, hWnd, NULL, hInst, NULL);
		HButton1 = CreateWindowEx(NULL, "BUTTON", "Start", WS_CHILD | WS_VISIBLE | SS_CENTER,
			2, 2, 80, 20, hWnd, (HMENU)IDC_BUTTON1, hInst, NULL);
		HButton2 = CreateWindowEx(NULL, "BUTTON", "Stop", WS_CHILD | WS_VISIBLE | SS_CENTER,
			82, 2, 80, 20, hWnd, (HMENU)IDC_BUTTON2, hInst, NULL);
		//HButton3 = CreateWindowEx(NULL, "BUTTON", "BUTTON3", WS_CHILD | WS_VISIBLE | SS_CENTER,
			//162, 2, 80, 20, hWnd, (HMENU)IDC_BUTTON3, hInst, NULL);
		Hmainbmp = CreateWindowEx(NULL, "STATIC", "", WS_CHILD | WS_VISIBLE | SS_BITMAP | WS_THICKFRAME, 1, 23, 600, 500, hWnd, NULL, hInst, NULL);
	}
	break;
	case WM_KEYDOWN:
	{
		static int t, parameter = 5;
		static DWORD dw;
		t = (int)wParam;
		if (t > 36 && t < 41) keypressed = t;
	}
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_OPEN:
		{
			OPENFILENAME ofn;

			char szFile[260];
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = hWnd;
			ofn.lpstrFile = szFile;
			//
			// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
			// use the contents of szFile to initialize itself.
			//
			ofn.lpstrFile[0] = '\0';
			ofn.nMaxFile = sizeof(szFile);
			ofn.lpstrFilter = "All\0*.*\0Text\0*.TXT\0";
			ofn.nFilterIndex = 1;
			ofn.lpstrFileTitle = NULL;
			ofn.nMaxFileTitle = 0;
			ofn.lpstrInitialDir = NULL;
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
			GetOpenFileName(&ofn);
			CHMAT m(30, 30);
			if (!LoadImage(szFile, m))
			{ 
				SetWindowText(hEdit, "CAN'T OPEN FILE!!!\n"); 
				break; 
			}
			SendMessage(Hmainbmp, STM_SETIMAGE, 0, (LPARAM)m.HBitmap);
			//SetWindowText(hEdit, szFile);
		}
		break;
		case IDC_BUTTON1:
		{
			
			HANDLE hTimer = NULL;

			hTimerQueue = CreateTimerQueue();
			if (NULL == hTimerQueue)
			{
				break;
			}
			DWORD dw;
			//starts 1000 ms later calls thread every 30 ms
			CreateTimerQueueTimer(&hTimer, hTimerQueue, (WAITORTIMERCALLBACK)SlidingBox, NULL, 0, 30, 0);
			CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Alien, NULL, 50, &dw);
			SendMessage(Hmainbmp, STM_SETIMAGE, 0, (LPARAM)m.HBitmap);
			SetWindowText(hEdit, "Game Started!");
		}
		break;
		case IDC_BUTTON2:
		{
			threadflag = 0;//ends threads.

			SendMessage(Hmainbmp, STM_SETIMAGE, 0, (LPARAM)m.HBitmap);
			SetWindowText(hEdit, "Game Over!");
		}
		break;
		default:
			break;
		}
		break;

		

	    case WM_DESTROY:
		    PostQuitMessage(0);
		    GdiplusShutdown(gdiplusToken);
		break;

	    default:
		    return(DefWindowProc(hWnd, uMsg, wParam, lParam));
	    }

	    return(0L);
}

