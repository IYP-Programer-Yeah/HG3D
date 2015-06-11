#include "Main Window.h"
#include <iostream>
INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	init_main_window(hInstance);//init the wondpw
	HDC *hdc = GetHDC();
	HG3D_Engine::Renderer Engine;//define a rendere
	Engine.hdc = hdc[0];//get the dc
	Engine.init();//initialize the engine
	MSG *msg = GetMSG();//get the message point the to actual msg in wnd class
	Engine.add_camera();//add a camera to ebgine
	Engine.add_mesh("..\\HG3D 2.1\\Resource\\Models\\horse.obj");//add the mesh to engine
	Engine.meshes[0].model_matrix.LoadScaler(100.0f, 100.0f, 100.0f);//the mesh needs to be 100 times larger
	Engine.add_mesh("..\\HG3D 2.1\\Resource\\Models\\cube.obj");//add the other mesh

	HG3D_Engine::_4x4matrix temp[2];;
	temp[0].LoadTranslate(24.0f,0.0f,0.0f);//move it 24 in direction of x
	temp[1].LoadScaler(5.0f, 5.0f, 5.0f);//largen 5 times
	Engine.meshes[1].model_matrix = temp[0]* temp[1];//multiply the scale from the right (always do this) and translate from right


	Engine.cameras[0].camera_position.build(12.0f, 0.0f, 0.0f);//put the camera to x=12
	Engine.cameras[0].forward.build(-1.0f, 0.0f, 0.0f);//look int x=-1 direction 
	Engine.cameras[0].camera_viewport[2] = GetW();//update view port
	Engine.cameras[0].camera_viewport[3] = GetH();//update view port
	Engine.cameras[0].Right = float(GetW()) / float(GetH());//update projection
	Engine.cameras[0].Left = -1.0f*float(GetW()) / float(GetH());//update projection
	Engine.cameras[0].update_camera();//updtae camera
	while (1)//or use GetMessage(msg, NULL, 0, 0)
	{
		DispatchMessage(msg);
		//update camera data
		Engine.cameras[0].camera_viewport[2] = GetW();
		Engine.cameras[0].camera_viewport[3] = GetH();
		Engine.cameras[0].Right = float(GetW()) / float(GetH());//update projection
		Engine.cameras[0].Left = -1.0f*float(GetW()) / float(GetH());//update projection
		Engine.cameras[0].fps_camera(0.01f, 0.0f, Engine.cameras[0].up);//a simple fps camera

		Engine.test_render();//render scene
		PeekMessage(msg, 0, 0, 0, 1);//get msg
		//GetMessage(msg, NULL, 1, 0);
	}

	DestroyCachedSystemInformation();

	return 0;
}