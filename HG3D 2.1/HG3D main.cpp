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
	Engine.add_mesh("..\\HG3D 2.1\\Resource\\Models\\sphere.obj");//add the other mesh

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

	while (msg->message != WM_QUIT)  
	{
		Update_Wnd();
		if (PeekMessage(msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(msg);
			DispatchMessage(msg);
		}
		else
		{
			//Update camera data
			Engine.cameras[0].camera_viewport[2] = GetW();

			Engine.cameras[0].camera_viewport[3] = GetH();

			Engine.cameras[0].Right = float(GetW()) / float(GetH());         //update projection

			Engine.cameras[0].Left = -1.0f * float(GetW()) / float(GetH());    //update projection

			int MWD = Get_Mouse_Wheel_Delta();

			if (MWD != 0)//check if mouse will moved
			{
				HG3D_Engine::point LastPos;//last pos

				LastPos = Engine.cameras[0].camera_position;//set the value

				HG3D_Engine::vector Movement;//set movement

				Movement = normalize(Engine.cameras[0].forward)*long double(MWD) / long double(480.0);//get the movement

				Engine.cameras[0].camera_position.build(LastPos.x + Movement.x, LastPos.y + Movement.y, LastPos.z + Movement.z);//move

				Engine.cameras[0].needs_update = 1;//need update
			}
			if (Get_Mouse_Stat(Mouse_Middle_Stat))///check if the mddile button is pressed
			{
				Engine.cameras[0].fps_camera(float(Get_Mouse_X() - Get_Mouse_Last_X()) / 180.0f, float(Get_Mouse_Last_Y() - Get_Mouse_Y()) / 180.0f, Engine.cameras[0].up); //a simple fps camera
			}

			Engine.test_render(); //render scene

		}

	}

	//LEARN WINDOWS PROGRAMMING! -_-
	//YOU HAVE coded it in such a way that the code NEVER REACHES HERE
	//SO, IT WAS CAUSING A MEMORY LEAK HERE BECAUSE THIS FUNCTION WAS NOT BEING CALLED:
	DestroyCachedSystemInformation();

	//I FIXED IT
	//DON'T DO ANY OTHER SHIT NOW

	return (int)msg->wParam;
}