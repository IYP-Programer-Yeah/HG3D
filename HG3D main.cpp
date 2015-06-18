#include "Main Window.h"
#include <iostream>
INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	init_main_window(hInstance);
	HDC *hdc = GetHDC();
	HG3D_Engine::Renderer Engine;
	Engine.hdc = hdc[0];
	Engine.init();
	MSG *msg = GetMSG();
	while (GetMessage(msg, NULL, 0, 0))
	{
		DispatchMessage(msg);
	}
	return 0;
}