#include "Windows Handler.h"
BOOL WINAPI DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	return 1;
}
namespace Windows_Handler
{
	bool Windows::init_window(HINSTANCE hInstance, char* Win_Name, int nCmdShow, BOOL POPUP, unsigned int x, unsigned int y, unsigned int w, unsigned int h)
	{
		RegisterClassA(&WndClass);//register the wnd class
		hwnd = CreateWindowA(
			WndClass.lpszClassName,//wnd class name
			(LPCSTR)Win_Name,//wnd caption
			POPUP?WS_POPUP:WS_OVERLAPPEDWINDOW, //FULL SCREEN or NORMAL
			x,//WINX
			y,//WINY
			w,//WINW
			h,//WINDH
			NULL,
			NULL,
			hInstance,
			NULL);
		if (hwnd == 0)
			return 0; //couldn't make the wnd
		ShowWindow(hwnd, nCmdShow); //show the wnd
		UpdateWindow(hwnd); //update wnd
		hdc=GetDC(hwnd);
		WNDx = x;
		WNDy = y;
		WNDw = w;
		WNDh = h;
		return 1;//every thing done well
	}
}