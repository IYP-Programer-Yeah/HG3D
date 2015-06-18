#include "Main Window.h"
#include <iostream>
#include "..\Shared Headers\hstring.h"


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
	Engine.add_camera();
	Engine.add_current_camera(0);
	Engine.add_current_camera(1);

	//================ PHYSICS ENGINE USAGE ====================/
	Physics::PhysicsObject Horse;
	
	Engine.add_mesh("..\\HG3D 2.1\\Resource\\Models\\horse.obj");

	vector HorsePos;

	//Don't forget to initialize any of these if you don't use them
	HorsePos.build(0.0f, 100.0f, -5.0f);
	Engine.meshes[0].move(HorsePos);
	Engine.meshes[0].scale_model(100.0f, 100.0f, 100.0f);
	vector axis;
	axis.build(1.0f, 0.0f, 0.0f);
	Engine.meshes[0].rotate_model_AIC(-3.14f/2.0f, axis);
	Engine.meshes[0].model_matrix = Engine.meshes[0].model_matrix;

	//==========================================================================//

	vector SpherePos;
	
	SpherePos.build(24.0f, 0.0f, 0.0f);

	Engine.add_mesh("..\\HG3D 2.1\\Resource\\Models\\sphere.obj");

	Engine.meshes[1].move(SpherePos);
	Engine.meshes[1].scale_model(5.0f, 5.0f, 5.0f);

	//=========================================================================//

	vector SpherePos2;

	Engine.add_mesh("..\\HG3D 2.1\\Resource\\Models\\sphere.obj");

	SpherePos2.build(0.0f, -6370850.0f, 0.0f);
	Engine.meshes[2].move(SpherePos2);
	Engine.meshes[2].scale_model(6371000.0f, 6371000.0f, 6371000.0f);//earth radius
	//==========================================================================//

	Engine.cameras[0].camera_position.build(0.0f, 0.0f, 0.0f);//put the camera to x=12
	Engine.cameras[0].forward.build(-1.0f, 0.0f, 0.0f);//look int x=-1 direction 
	Engine.cameras[0].camera_viewport[2] = GetW();//update view port
	Engine.cameras[0].camera_viewport[3] = GetH();//update view port
	Engine.cameras[0].Right = float(GetW()) / float(GetH());//update projection
	Engine.cameras[0].Left = -1.0f*float(GetW()) / float(GetH());//update projection
	Engine.cameras[0].update_camera();//update camera


	Engine.cameras[1].camera_position.build(0.0f, 0.0f, 0.0f);//put the camera to x=12
	Engine.cameras[1].forward.build(-1.0f, 0.0f, 0.0f);//look int x=-1 direction 
	Engine.cameras[1].camera_viewport[0] = GetW() - 400;//update view port
	Engine.cameras[1].camera_viewport[1] = GetH() - 400;//update view port
	Engine.cameras[1].camera_viewport[2] = 400;//update view port
	Engine.cameras[1].camera_viewport[3] = 400;//update view port
	Engine.cameras[1].update_camera();//update camera


	long double last_time = clock(), current_time = clock();

	//World.AddObject("Horse", Horse, Engine.meshes[0]);
	//World.AddObject("Sphere", Sphere, Engine.meshes[1]);

	float Masses[] =
	{ 60.0f /*Horse Mass*/,
	5.0f /*Sphere mass*/,
	5.972f*pow(10.0f, 24.0f)//*earth mass*/
	};

	std::string Names[] = { 
		"Horse", 
		"Sphere",
		"Earth"
	};

	unsigned long int IDs[] = {
		0, //Mesh Index of Horse in renderer->meshes
		1, //Mesh Index of Sphere
		2  //Mesh index of Earth
	};

	World.LoadWorld(&Engine, Masses, Names, IDs, ARRAYSIZE(Masses)); //or ARRAYSIZE(Names)


	Physics::PhysicsObject& SphereObj = World.GetPhysicsObject("Sphere");

#ifdef NT_IS_DEBUGGING
	if (!SphereObj.m_Valid)
		OutputDebugString("SphereObj is not valid.\n");
#endif


	SphereObj.AddVelocity(-40.0, 60.0, 0.0);

	while (msg->message != WM_QUIT)  
	{
		if (PeekMessage(msg, NULL, 0, 0, PM_REMOVE))
		{
			Update_Wnd();
			TranslateMessage(msg);
			DispatchMessage(msg);
			int MWD = Get_Mouse_Wheel_Delta();

			if (MWD != 0 && !Get_Mouse_Stat(Mouse_Right_Stat))//check if mouse will moved
			{
				HG3D_Engine::point LastPos;//last pos

				LastPos = Engine.cameras[0].camera_position;//set the value

				HG3D_Engine::vector Movement;//set movement

				Movement = normalize(Engine.cameras[0].forward)*long double(MWD) / long double(20.0);//get the movement

				Engine.cameras[0].camera_position.build(LastPos.x + Movement.x, LastPos.y + Movement.y, LastPos.z + Movement.z);//move

				Engine.cameras[0].needs_update = true;//need update
			}
			if (MWD != 0 && Get_Mouse_Stat(Mouse_Right_Stat))//check if mouse will moved
			{
				HG3D_Engine::point LastPos;//last pos

				LastPos = Engine.cameras[1].camera_position;//set the value

				HG3D_Engine::vector Movement;//set movement

				Movement = normalize(Engine.cameras[1].forward)*long double(MWD) / long double(20.0);//get the movement

				Engine.cameras[1].camera_position.build(LastPos.x + Movement.x, LastPos.y + Movement.y, LastPos.z + Movement.z);//move

				Engine.cameras[1].needs_update = true;//need update
			}
			//Update camera data
			Engine.cameras[0].camera_viewport[2] = GetW();

			Engine.cameras[0].camera_viewport[3] = GetH();

			Engine.cameras[0].Right = float(GetW()) / float(GetH());         //update projection

			Engine.cameras[0].Left = -1.0f * float(GetW()) / float(GetH());    //update projection

			Engine.cameras[1].camera_viewport[0] = GetW() - 400;//update view port

			Engine.cameras[1].camera_viewport[1] = GetH() - 400;//update view port

			Engine.cameras[1].camera_viewport[2] = 400;//update view port

			Engine.cameras[1].camera_viewport[3] = 400;//update view port


			if (Get_Mouse_Stat(Mouse_Middle_Stat))///check if the mddile button is pressed
			{
				Engine.cameras[0].fps_camera(float(Get_Mouse_X() - Get_Mouse_Last_X()) / 180.0f, 
					float(Get_Mouse_Last_Y() - Get_Mouse_Y()) / 180.0f, 
					Engine.cameras[0].up); //a simple fps camera
			}
			if (Get_Mouse_Stat(Mouse_Right_Stat))///check if the mddile button is pressed
			{
				Engine.cameras[1].fps_camera(float(Get_Mouse_X() - Get_Mouse_Last_X()) / 180.0f,
					float(Get_Mouse_Last_Y() - Get_Mouse_Y()) / 180.0f,
					Engine.cameras[1].up); //a simple fps camera
			}
		}
		else
		{
			last_time = current_time;
			current_time = clock();

			long double dt = (current_time - last_time) / 1000.0f;
			if (Get_Mouse_Stat(Mouse_Left_Stat))
				World.Update(dt);

			Engine.test_render(); //render scene


		}

	}

	DestroyCachedSystemInformation();

	return (int)msg->wParam;
}