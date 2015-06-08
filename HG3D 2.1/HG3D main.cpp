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
	Engine.add_camera();
	Engine.add_mesh("..\\HG3D 2.1\\Resource\\Models\\horse.obj");
	Engine.meshes[0].model_matrix.LoadScaler(100.0f, 100.0f, 100.0f);
	
	/*
	Engine.add_mesh("..\\HG3D 2.1\\Resource\\Models\\horse.obj");
	HG3D_Engine::_4x4matrix temp[2];;
	temp[0].LoadTranslate(-10.0f,0.0f,0.0f);
	temp[1].LoadScaler(-100.0f, -100.0f, -100.0f);
	Engine.meshes[1].model_matrix = temp[0] * temp[1]*///not yet done


	Engine.cameras[0].camera_position.build(12.0f, 0.0f, 0.0f);
	Engine.cameras[0].forward.build(-1.0f, 0.0f, 0.0f);
	Engine.cameras[0].needs_update = 1;
	while (PeekMessage(msg,0,0,0,0))//or use GetMessage(msg, NULL, 0, 0)
	{
		DispatchMessage(msg);
		Engine.cameras[0].fps_camera(0.01f, 0.0f, Engine.cameras[0].up);
		Engine.cameras[0].needs_update = 1;
		Engine.test_render();
	}
	return 0;
}