#include <Windows.h>
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

		bool __declspec(dllexport) init_window(HINSTANCE hInstance, char* Win_Name, int nCmdShow = SW_SHOWNORMAL, BOOL POPUP = 0, unsigned int x = 0, unsigned int y = 0, unsigned int w = 640, unsigned int h = 480); //initialize the window
	};
}
#endif