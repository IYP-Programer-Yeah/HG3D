#include "Main Window.h"
#include "resource.h"  //RESOURCES DEFS

Windows Main_Windows;
LRESULT CALLBACK Main_Window_WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) //wnd proc to check messages
{
	switch (Msg)
	{
	case WM_MOVE: //in case of movement update the rect data
		Main_Windows.WNDx = LOWORD(lParam); //rect's x in lower bit
		Main_Windows.WNDy = HIWORD(lParam); //rect's y in higher bit
		Main_Windows.Needs_update = 1;//needs update
		break;
	case WM_SIZE: //in case of resize updtae the recs
		Main_Windows.WNDw = LOWORD(lParam); //rect's width in lower bit
		Main_Windows.WNDh = HIWORD(lParam); //rect's height in higher bit
		Main_Windows.Needs_update = 1;//needs update
		break;
	case WM_DESTROY:
		PostQuitMessage(WM_QUIT);           //with no main windows there are no reasons to keep the program running
		break;
	default:
		Main_Windows.Needs_update = 0;//needs update
		return DefWindowProc(hWnd, Msg, wParam, lParam); //return default reaction
	}
	return false;  //the reaction was done normally
}
bool init_main_window(HINSTANCE hInstance)
{
	CheckSystemInformation();

	//setting wnd class
	Main_Windows.WndClass.style = CS_HREDRAW | CS_VREDRAW;
	Main_Windows.WndClass.lpfnWndProc = Main_Window_WndProc; //set the wndproc of the window
	Main_Windows.WndClass.cbClsExtra = NULL;
	Main_Windows.WndClass.cbWndExtra = NULL;
	Main_Windows.WndClass.hInstance = hInstance;//set the instance
	Main_Windows.WndClass.hCursor = LoadCursor(NULL, IDC_ARROW); //set the cursor to arrow
	Main_Windows.WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); //clear the window with white
	Main_Windows.WndClass.lpszMenuName = NULL;//no menu
	Main_Windows.WndClass.lpszClassName = "HG3D_Main";//main window's class name
#ifndef _M_X64 //if 32bit
	Main_Windows.WndClass.hIcon = LoadIcon(hInstance,MAKEINTRESOURCEA(IDI_ICON1)); //set the icon 32bit
#else          //if 64bit
	Main_Windows.WndClass.hIcon = LoadIcon(hInstance, (LPCSTR)MAKEINTRESOURCEA(IDI_ICON1)); //set the icon 64bit
#endif
	return Main_Windows.init_window(hInstance, "HG3D", SW_SHOWMAXIMIZED); //make a maximized window


}
MSG* GetMSG()
{
	return &Main_Windows.msg; //return msg adress so i can change the content
}
HDC* GetHDC()
{
	return &Main_Windows.hdc; //return hdc adress so i can change the content
}
int GetW()//get width
{
	return Main_Windows.WNDw;
}
int GetH()//get height
{
	return Main_Windows.WNDh;
}
bool Get_Wind_Update_Stat()//get if needs update
{
	return Main_Windows.Needs_update;
}
int Get_Mouse_X()
{
	return Main_Windows.Mouse_X;
}
int Get_Mouse_Y()
{
	return Main_Windows.Mouse_Y;
}
int Get_Mouse_Last_X()
{
	return Main_Windows.Last_Mouse_X;
}
int Get_Mouse_Last_Y()
{
	return Main_Windows.Last_Mouse_Y;
}
int Get_Mouse_Wheel_Delta()
{
	return Main_Windows.Mouse_Wheel_Delta;
}
bool Get_Mouse_Stat(int Stat_ID)//get mouse stats
{
	if (Stat_ID == Mouse_Left_Stat)
		return Main_Windows.Mouse_Left;
	if (Stat_ID == Mouse_Right_Stat)
		return Main_Windows.Mouse_Right;
	if (Stat_ID == Mouse_Middle_Stat)
		return Main_Windows.Mouse_Middle;
	return false;
}
bool Update_Wnd()
{
	return Main_W indows.update_mouse();//update mouse
}

void CheckSystemInformation()
{
#ifdef NT_IS_DEBUGGING
	OutputDebugString("\nCreated SystemInformation::_instance.\n");
#endif
	SystemInformation::Instance()->Initialize();
}

void DestroyCachedSystemInformation()
{
#ifdef NT_IS_DEBUGGING
	//If you don't see this in the output window while debugging, then something is wrong
	OutputDebugString("\nDeleted SystemInformation::_instance.\n");
#endif
	SystemInformation::Instance()->Destroy();
}