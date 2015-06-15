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

	//================ PHYSICS ENGINE USAGE ====================/
	Physics::PhysicsObject Horse;
	Horse.SetMass(1.0f);
	Engine.add_mesh("..\\HG3D 2.1\\Resource\\Models\\horse.obj");

	vector translation;

	//Don't forget to initialize any of these if you don't use them
	translation.build(0.0f, 0.0f, 0.0f);
	Engine.meshes[0].move(translation);
	Engine.meshes[0].scale_model(100.0f, 100.0f, 100.0f);
	//Should be same as Translate.LoadTranslate(x, y, z)
	Horse.SetPosition(0.0f, 0.0f, 0.0f);

	Horse.SetForce(0.0f, 0.0f, 3.0f);

	World.AddObject(Horse, Engine.meshes[0]);

	//==========================================================================//

	Engine.add_mesh("..\\HG3D 2.1\\Resource\\Models\\sphere.obj");

	translation.build(24.0f, 0.0f, 0.0f);
	Engine.meshes[1].move(translation);
	Engine.meshes[1].scale_model(5.0f, 5.0f, 5.0f);

	//=========================================================================//

	Engine.add_mesh("..\\HG3D 2.1\\Resource\\Models\\sphere.obj");

	translation.build(0.0f, -6370850.0f, 0.0f);
	Engine.meshes[2].move(translation);
	Engine.meshes[2].scale_model(6371000.0f, 6371000.0f, 6371000.0f);//earth radius
	//==========================================================================//

	Engine.cameras[0].camera_position.build(12.0f, 0.0f, 0.0f);//put the camera to x=12
	Engine.cameras[0].forward.build(-1.0f, 0.0f, 0.0f);//look int x=-1 direction 
	Engine.cameras[0].camera_viewport[2] = GetW();//update view port
	Engine.cameras[0].camera_viewport[3] = GetH();//update view port
	Engine.cameras[0].Right = float(GetW()) / float(GetH());//update projection
	Engine.cameras[0].Left = -1.0f*float(GetW()) / float(GetH());//update projection
	Engine.cameras[0].update_camera();//update camera


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

			long double dt = (static_cast<float>(current_time)-last_time) / 1000.0f;

			World.Update(dt);

			Engine.test_render(); //render scene


		}

	}

	DestroyCachedSystemInformation();

	return (int)msg->wParam;
}