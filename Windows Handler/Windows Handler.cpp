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
			WndClass.lpszClassName, //wnd class name
			(LPCSTR)Win_Name, //wnd caption
			POPUP?WS_POPUP:WS_OVERLAPPEDWINDOW, //FULL SCREEN or NORMAL
			x, //WINX
			y, //WINY
			w, //WINW
			h, //WINDH
			NULL,
			NULL,
			hInstance,
			NULL);
		if (hwnd == 0)
			return 0; //couldn't make the wnd
		ShowWindow(hwnd, nCmdShow); //show the wnd
		UpdateWindow(hwnd); //update wnd
		hdc=GetDC(hwnd);
		RECT Temp_Wind_Rect;
		if (GetWindowRect(hwnd, &Temp_Wind_Rect))
		{
			WNDx = Temp_Wind_Rect.right;
			WNDy = Temp_Wind_Rect.left;;
			WNDw = Temp_Wind_Rect.right - Temp_Wind_Rect.left;
			WNDh = Temp_Wind_Rect.bottom - Temp_Wind_Rect.top;
		}
		//init the values
		Last_Mouse_X = 0;
		Last_Mouse_Y = 0;
		Mouse_X = 0;
		Mouse_Y = 0;
		Mouse_Wheel_Delta = 0;
		Mouse_Middle = false;
		Mouse_Right = false;
		Mouse_Left = false;
		return true;//every thing done well
	}
	void Windows::update_mouse()
	{
		Last_Mouse_X = Mouse_X;//set the last positions
		Last_Mouse_Y = Mouse_Y;
		if (msg.message == WM_MOUSEMOVE)
		{
			//get new postions
			Mouse_X = GET_X_LPARAM(msg.lParam);
			Mouse_Y = GET_Y_LPARAM(msg.lParam);
		}
		if (msg.message ==WM_MOUSEWHEEL)
		{
			//get new postions
			Mouse_X = GET_X_LPARAM(msg.lParam);
			Mouse_Y = GET_Y_LPARAM(msg.lParam);
			Mouse_Wheel_Delta = GET_WHEEL_DELTA_WPARAM(msg.wParam);//get the delta v
		}
		else
			Mouse_Wheel_Delta = 0;
		if (msg.message == WM_LBUTTONDOWN)
		{
			//get new postions
			Mouse_X = GET_X_LPARAM(msg.lParam);
			Mouse_Y = GET_Y_LPARAM(msg.lParam);
			Mouse_Left = true;
		}
		if (msg.message == WM_LBUTTONUP)
		{
			//get new postions
			Mouse_X = GET_X_LPARAM(msg.lParam);
			Mouse_Y = GET_Y_LPARAM(msg.lParam);
			Mouse_Left = false;
		}
		if (msg.message == WM_RBUTTONDOWN)
		{
			//get new postions
			Mouse_X = GET_X_LPARAM(msg.lParam);
			Mouse_Y = GET_Y_LPARAM(msg.lParam);
			Mouse_Right = true;
		}
		if (msg.message == WM_RBUTTONUP)
		{
			//get new postions
			Mouse_X = GET_X_LPARAM(msg.lParam);
			Mouse_Y = GET_Y_LPARAM(msg.lParam);
			Mouse_Right = false;
		}
		if (msg.message == WM_MBUTTONDOWN)
		{
			//get new postions
			Mouse_X = GET_X_LPARAM(msg.lParam);
			Mouse_Y = GET_Y_LPARAM(msg.lParam);
			Mouse_Middle = true;
		}
		if (msg.message == WM_MBUTTONUP)
		{
			//get new postions
			Mouse_X = GET_X_LPARAM(msg.lParam);
			Mouse_Y = GET_Y_LPARAM(msg.lParam);
			Mouse_Middle = false;
		}

	}


	//===========================================================================================


	
}