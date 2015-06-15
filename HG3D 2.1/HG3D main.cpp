#include "Main Window.h"
#include <iostream>


INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	init_main_window(hInstance);

	HDC *hdc = GetHDC();

	HG3D_Engine::Renderer Engine;
	Physics::PhysicsWorld World;

	Engine.hdc = hdc[0];
	Engine.init();

	MSG *msg = GetMSG();
	Engine.add_camera();


	Physics::PhysicsObject Horse;
	Horse.SetMass(60.0);

	Engine.add_mesh("..\\HG3D 2.1\\Resource\\Models\\horse.obj");
	
	HG3D_Engine::_4x4matrix HorseWorld[2];
	HorseWorld[0].LoadTranslate(0.0f, 10.0f, 0.0f);
	HorseWorld[0].LoadTranslate(0.0f, -10.0f, 0.0f);
	HorseWorld[1].LoadScaler(100.0f, 100.0f, 100.0f);

	Engine.meshes[0].model_matrix = HorseWorld[0] * HorseWorld[1];

	World.AddObject(Horse, Engine.meshes[0]);

	Engine.add_mesh("..\\HG3D 2.1\\Resource\\Models\\sphere.obj");

	HG3D_Engine::_4x4matrix temp[2];
	temp[0].LoadTranslate(24.0f,0.0f,0.0f);//move it 24 in direction of x
	temp[1].LoadScaler(5.0f, 5.0f, 5.0f);//largen 5 times
	Engine.meshes[1].model_matrix = temp[0]* temp[1];//multiply the scale from the right (always do this) and translate from right

	Engine.add_mesh("..\\HG3D 2.1\\Resource\\Models\\sphere.obj");//add the other mesh (this is earth)

	temp[0].LoadTranslate(0.0f, -6370850.0f, 0.0f);//this is earth
	temp[1].LoadScaler(6371000.0f, 6371000.0f, 6371000.0f);//earth radius
	Engine.meshes[2].model_matrix = temp[0] * temp[1];//multiply the scale from the right (always do this) and translate from right

	Engine.cameras[0].camera_position.build(12.0f, 0.0f, 0.0f);//put the camera to x=12
	Engine.cameras[0].forward.build(-1.0f, 0.0f, 0.0f);//look int x=-1 direction 
	Engine.cameras[0].camera_viewport[2] = GetW();//update view port
	Engine.cameras[0].camera_viewport[3] = GetH();//update view port
	Engine.cameras[0].Right = float(GetW()) / float(GetH());//update projection
	Engine.cameras[0].Left = -1.0f*float(GetW()) / float(GetH());//update projection
	Engine.cameras[0].update_camera();//update camera

	Physics::PhysicsWorld PH_Engine;
	long double masses[3] = { 10.0, 10.0, pow(10.0, 24.0)*5.972 };
	PH_Engine.LoadWorld(&Engine, masses);
	

	long double last_time = clock(), current_time = clock();
	while (msg->message != WM_QUIT)  
	{
		if (PeekMessage(msg, NULL, 0, 0, PM_REMOVE))
		{
			Update_Wnd();
			TranslateMessage(msg);
			DispatchMessage(msg);
			int MWD = Get_Mouse_Wheel_Delta();

			if (MWD != 0)//check if mouse will moved
			{
				HG3D_Engine::point LastPos;//last pos

				LastPos = Engine.cameras[0].camera_position;//set the value

				HG3D_Engine::vector Movement;//set movement

				Movement = normalize(Engine.cameras[0].forward)*long double(MWD) / long double(20.0);//get the movement


				Engine.cameras[0].camera_position.build(LastPos.x + Movement.x, LastPos.y + Movement.y, LastPos.z + Movement.z);//move

				Engine.cameras[0].needs_update = true;//need update
			}
			//Update camera data
			Engine.cameras[0].camera_viewport[2] = GetW();

			Engine.cameras[0].camera_viewport[3] = GetH();

			Engine.cameras[0].Right = float(GetW()) / float(GetH());         //update projection

			Engine.cameras[0].Left = -1.0f * float(GetW()) / float(GetH());    //update projection

			if (Get_Mouse_Stat(Mouse_Middle_Stat))///check if the mddile button is pressed
			{
				Engine.cameras[0].fps_camera(float(Get_Mouse_X() - Get_Mouse_Last_X()) / 180.0f, 
					float(Get_Mouse_Last_Y() - Get_Mouse_Y()) / 180.0f, 
					Engine.cameras[0].up); //a simple fps camera
			}
		}
		else
		{
			last_time = current_time;
			current_time = clock();
			PH_Engine.Update((current_time-last_time)/1000.0);
			Engine.test_render(); //render scene
		}

	}

	DestroyCachedSystemInformation();

	return (int)msg->wParam;
}