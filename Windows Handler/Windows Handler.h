#include <Windows.h>
#include <Windowsx.h>

#ifndef WND_HND
#define WND_HND


namespace Windows_Handler
{
	class Windows
	{
	public:
		HDC hdc;   //device context
		MSG msg;   //wnd message
		HWND hwnd; //wnd handle
		WNDCLASSA  WndClass; //wnd class
		unsigned int WNDx, WNDy, WNDw, WNDh; //wnd rect
		bool Needs_update;
		//mouse bottuns condition
		bool Mouse_Middle, Mouse_Right, Mouse_Left;
		//mouse position
		int Mouse_X, Mouse_Y;
		//mouse last position
		int Last_Mouse_X, Last_Mouse_Y;
		//mouse wheel value
		int Mouse_Wheel_Delta;

		bool __declspec(dllexport) init_window(HINSTANCE hInstance, char* Win_Name, int nCmdShow = SW_SHOWNORMAL, BOOL POPUP = 0, unsigned int x = 0, unsigned int y = 0, unsigned int w = 640, unsigned int h = 480); //initialize the window
		bool __declspec(dllexport) update_mouse();//update mouse values
	};
	
	
}
#endif