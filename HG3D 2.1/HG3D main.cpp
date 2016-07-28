#include "Main Window.h"
#include <iostream>
#include "..\Shared Headers\hstring.h"
#include "..\Shared Headers\bitmap.h"
#include "..\File_HG_FS\File_HG_FS.h"

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	hbitmap texture;
	texture.getimage("..\\HG3D 2.1\\Resource\\Test Texture.bmp");
	init_main_window(hInstance);

	HDC *hdc = GetHDC();
	
	HG3D_Engine::Renderer Engine;
	Physics::PhysicsWorld World;

	Engine.hdc = hdc[0];
	//Engine.test_init();
	Engine.init();

	MSG *msg = GetMSG();
	Engine.add_camera();
	Engine.add_camera();
	Engine.add_current_camera(0);
	//Engine.add_current_camera(1);
	point horse_pos,light_pos;
	horse_pos.build(0.0f, 0.0f, -50.0f);
	vector light_dir;
	for (int i = 0; i < 3; i++)
	{
		Engine.lights[i].light_enabled = true;
		Engine.last_light_ID++;
		Engine.lights[i].Attenuation[0] = 0.001f / float(i % 8 + 1);
		Engine.lights[i].Attenuation[1] = 0.001f / float(i % 5 + 1);
		Engine.lights[i].Attenuation[2] = 0.025f / float(i % 11 + 1);
		Engine.lights[i].light_color[0] = float(i % 8 + 1) / 8.0f;
		Engine.lights[i].light_color[1] = float(i % 5 + 1) / 5.0f;;
		Engine.lights[i].light_color[2] = float(i % 3 + 1) / 3.0f;;
		Engine.lights[i].light_position[0] = pow(-1.0f, float(i))*float((i * 30 + 50) % 50);
		Engine.lights[i].light_position[1] = float((i * 40 + 70) % 150);
		Engine.lights[i].light_position[2] = pow(-1.0f, float(i + 1))*float((i * 20 + 45) % 50);
		light_pos.build(Engine.lights[i].light_position[0], Engine.lights[i].light_position[1], Engine.lights[i].light_position[2]);
		light_dir.build(light_pos, horse_pos);
		light_dir = normalize(light_dir);
		Engine.lights[i].direction[0] = float(light_dir.x);
		Engine.lights[i].direction[1] = float(light_dir.y);
		Engine.lights[i].direction[2] = float(light_dir.z);
		Engine.lights[i].calculate_max_radius();
		Engine.lights[i].shadow_map = false;
	}

	vector direction;
	direction.build(0.0f, 0.0f, -1.0f);
	direction = normalize(direction);
	Engine.lights[0].light_enabled = true;
	Engine.lights[0].Attenuation[0] = 7.0f;
	Engine.lights[0].Attenuation[1] = 0.01f;
	Engine.lights[0].Attenuation[2] = 0.0008f;
	Engine.lights[0].light_color[0] = 1.0f;
	Engine.lights[0].light_color[1] = 1.0f;
	Engine.lights[0].light_color[2] = 1.0f;
	Engine.lights[0].light_position[0] = 0.0f;
	Engine.lights[0].light_position[1] = 100.0f;
	Engine.lights[0].light_position[2] = 50.0f;
	Engine.lights[0].direction[0] = float(direction.x);
	Engine.lights[0].direction[1] = float(direction.y);
	Engine.lights[0].direction[2] = float(direction.z);
	Engine.lights[0].cut_off_cos = cos(3.14f / 5.0f);
	Engine.lights[0].edge_cut_off_cos_delta = cos(3.14f / 5.0f) - cos(3.14f / 4.0f);
	Engine.lights[0].calculate_max_radius();
	Engine.lights[0].shadow_map = true;

	direction.build(0.0f, -1.0f, 0.0f);
	direction = normalize(direction);

	Engine.lights[1].light_enabled = true;
	Engine.lights[1].Attenuation[0] = 5.0f;
	Engine.lights[1].Attenuation[1] = 0.0f;
	Engine.lights[1].Attenuation[2] = 0.0f;
	Engine.lights[1].light_color[0] = 1.0f;
	Engine.lights[1].light_color[1] = 1.0f;
	Engine.lights[1].light_color[2] = 1.0f;
	Engine.lights[1].light_position[0] = 500.0f;
	Engine.lights[1].light_position[1] = 500.0f;
	Engine.lights[1].light_position[2] = 0.0f;
	Engine.lights[1].direction[0] = float(direction.x);
	Engine.lights[1].direction[1] = float(direction.y);
	Engine.lights[1].direction[2] = float(direction.z);
	//Engine.lights[1].cut_off_cos = cos(3.14f / 9.0f);
	//Engine.lights[1].edge_cut_off_cos_delta = cos(3.14f / 10.0f) - cos(3.14f / 9.0f);
	Engine.lights[1].calculate_max_radius();
	//Engine.lights[1].shadow_map = true;

	direction.build(0.0f, -1.0f, 0.0f);
	Engine.lights[2].light_enabled = true;
	Engine.lights[2].Attenuation[0] = 1.0f;
	Engine.lights[2].Attenuation[1] = 0.0f;
	Engine.lights[2].Attenuation[2] = 0.0f;
	Engine.lights[2].light_color[0] = 1.0f;
	Engine.lights[2].light_color[1] = 1.0f;
	Engine.lights[2].light_color[2] = 1.0f;
	Engine.lights[2].light_position[0] = 1000.0f;
	Engine.lights[2].light_position[1] = 1500.0f;
	Engine.lights[2].light_position[2] = 0.0f;
	Engine.lights[2].direction[0] = float(direction.x);
	Engine.lights[2].direction[1] = float(direction.y);
	Engine.lights[2].direction[2] = float(direction.z);
	//Engine.lights[1].cut_off_cos = cos(3.14f / 9.0f);
	//Engine.lights[1].edge_cut_off_cos_delta = cos(3.14f / 10.0f) - cos(3.14f / 9.0f);
	Engine.lights[2].calculate_max_radius();
	Engine.lights[2].shadow_map = true;
	//Engine.lights[1].directional = 1;

	Engine.light_data_changed = 1;
	/*BYTE *data;
	data = (BYTE*)malloc(size_t(texture.Width[0] * texture.Height[0] * 3));
	for (register int i = 0; i < texture.Width[0] * texture.Height[0]; i++)
	{
		data[i * 3] = texture.rgb[i].r;
		data[i * 3 + 1] = texture.rgb[i].g;
		data[i * 3 + 2] = texture.rgb[i].b;
	}
	Engine.add_texture(data, unsigned (texture.Width[0]), unsigned(texture.Height[0]), 3, 0, 0, 0);
	*/
	//===========================================================/

	Engine.add_mesh("..\\HG3D 2.1\\Resource\\Models\\horse.obj");

	vector HorsePos;

	//Don't forget to initialize any of these if you don't use them
	HorsePos.build(0.0f, 100.0f, -50.0f);
	Engine.meshes[0].move(HorsePos);
	Engine.meshes[0].scale_model(100.0f, 100.0f, 100.0f);
	vector axis;
	axis.build(1.0f, 0.0f, 0.0f);
	vector yaxis;
	yaxis.build(0.0f, 1.0f, 0.0f);
	Engine.meshes[0].rotate_model_AIC(-3.14f / 2.0f, axis);
	Engine.meshes[0].rotate_model_AIC(-3.14f / 2.0f, yaxis);
	//==========================================================================//
	//===========================================================/
	point lightpos;
	lightpos.build(0.0f, 100.0f, 0.0f);
	for (register int i = 1; i < 5; i++)
	{
		Engine.add_mesh("..\\HG3D 2.1\\Resource\\Models\\horse.obj");
		HorsePos.build(0.0f,100.0f, -50.0f);
		Engine.meshes[i * 2 - 1].move(HorsePos);
		Engine.meshes[i * 2 - 1].scale_model(100.0f, 100.0f, 100.0f);
		Engine.meshes[i * 2 - 1].rotate_model_AIC(-3.14f / 2.0f, axis);
		Engine.meshes[i * 2 - 1].rotate_model(3.14f / 8.0f*float(i), lightpos, yaxis);
		Engine.meshes[i * 2 - 1].rotate_model_AIC(-3.14f / 2.0f, yaxis);
		Engine.add_mesh("..\\HG3D 2.1\\Resource\\Models\\horse.obj");
		HorsePos.build(0.0f, 100.0f, -50.0f);
		Engine.meshes[i * 2].move(HorsePos);
		Engine.meshes[i * 2].scale_model(100.0f, 100.0f, 100.0f);
		Engine.meshes[i * 2].rotate_model_AIC(-3.14f / 2.0f, axis);
		Engine.meshes[i * 2].rotate_model(-3.14f / 8.0f*float(i), lightpos, yaxis);
		Engine.meshes[i * 2].rotate_model_AIC(-3.14f / 2.0f, yaxis);
	}

	//==========================================================================//





	int temp;


	vector SpherePos;
	
	SpherePos.build(0.0f, 100.0f, 0.0f);

	temp = Engine.add_mesh("..\\HG3D 2.1\\Resource\\Models\\sphere.obj");

	Engine.meshes[temp].move(SpherePos);
	Engine.meshes[temp].scale_model(5.0f, 5.0f, 5.0f);

	//=========================================================================//
	
	vector SpherePos2;

	temp = Engine.add_mesh("..\\HG3D 2.1\\Resource\\Models\\sphere.obj");

	SpherePos2.build(0.0f, -6370850.0f, 0.0f);
	Engine.meshes[temp].move(SpherePos2);
	Engine.meshes[temp].scale_model(6371000.0f, 6371000.0f, 6371000.0f);//earth radius
	//==========================================================================//

	temp = Engine.add_mesh("..\\HG3D 2.1\\Resource\\Models\\kernel box.obj");
	Engine.meshes[temp].scale_model(1.0f, 1.0f, 1.0f);


	temp = Engine.add_mesh("..\\HG3D 2.1\\Resource\\Models\\bridge.obj");
	vector bridge;
	bridge.build(5.0f, 80.0f, 40.0f);
	Engine.meshes[temp].move(bridge);
	Engine.meshes[temp].scale_model(5.0f, 5.0f, 5.0f);
	Engine.meshes[temp].rotate_model_AIC(PI / 4.0f, axis);


	Engine.cameras[0].camera_position.build(0.0f, 100.0f, -80.0f);//put the camera to x=12
	Engine.cameras[0].camera_position.build(1100.0f, 900.0f, 0.0f);//put the camera to x=12
	//Engine.cameras[0].forward.build(0.0f, 0.0f, 1.0f);//look int x=-1 direction 
	Engine.cameras[0].camera_viewport[2] = GetW();//update view port
	Engine.cameras[0].camera_viewport[3] = GetH();//update view port
	Engine.cameras[0].Right = float(GetW()) / float(GetH());//update projection
	Engine.cameras[0].Left = -1.0f*float(GetW()) / float(GetH());//update projection
	Engine.cameras[0].needs_update = true;

	Engine.cameras[1].camera_position.build(0.0f, 0.0f, 0.0f);//put the camera to x=12
	Engine.cameras[1].forward.build(-1.0f, 0.0f, 0.0f);//look int x=-1 direction 
	Engine.cameras[1].camera_viewport[0] = GetW() - 400;//update view port
	Engine.cameras[1].camera_viewport[1] = GetH() - 400;//update view port
	Engine.cameras[1].camera_viewport[2] = 400;//update view port
	Engine.cameras[1].camera_viewport[3] = 400;//update view port
	Engine.cameras[1].needs_update = true;


	long double last_time = clock(), current_time = clock();

	//World.AddObject("Horse", Horse, Engine.meshes[0]);
	//World.AddObject("Sphere", Sphere, Engine.meshes[1]);

	float Masses[] =
	{ 60.0f /*Horse Mass*/,
	5.0f /*Sphere mass*/,
	//5.972f*pow(10.0f, 24.0f)//*earth mass*/
	};

	std::string Names[] = { 
		"Horse", 
		"Sphere",
	//	"Earth"
	};

	unsigned long int IDs[] = {
		0, //Mesh Index of Horse in renderer->meshes
		1, //Mesh Index of Sphere
	//	2  //Mesh index of Earth
	};

	World.LoadWorld(&Engine, Masses, Names, IDs, ARRAYSIZE(Masses)); //or ARRAYSIZE(Names)


	Physics::PhysicsObject* SphereObj = World.GetPhysicsObject("Sphere");

#ifdef NT_IS_DEBUGGING
	if (!SphereObj)
		OutputDebugString("SphereObj is not valid.\n");
#endif

	SphereObj->AddVelocity(0.0, 0.0, -10.0);
	hbitmap Bitmap;
	vector Sponza;
	Sponza.build(1000.0f, 500.0f, 0.0f);
	string Path;
	Path = "..\\HG3D 2.1\\Resource\\Models\\Sponza\\sponza";
	unsigned long int TexIndex;
	int W = 2048, H = 2048;
	unsigned char *RGBA = (unsigned char*)malloc(H * W * 3);
	for (int i = 0; i < 26; i++)
	{
		temp = Engine.add_mesh((Path+inttostring(i)+"\'.obj").string1);
		Engine.meshes[temp].move(Sponza);
		Engine.meshes[temp].scale_model(0.5, 0.5, 0.5);
	}

	Bitmap.getimage("..\\HG3D 2.1\\Resource\\Models\\Sponza\\textures\\roof.bmp");
	W = Bitmap.Width[0];
	H = Bitmap.Height[0];

	for (int i = 0; i < H; i++)
		for (int j = 0; j < W; j++)
		{
			RGBA[(i * W + j) * 3] = Bitmap.rgb[i * W + j].r;
			RGBA[(i * W + j) * 3 + 1] = Bitmap.rgb[i * W + j].g;
			RGBA[(i * W + j) * 3 + 2] = Bitmap.rgb[i * W + j].b;
		}
	TexIndex = Engine.add_texture(RGBA, W, H, 3, false, 0, 0);
	free(Bitmap.rgb);
	Engine.textures[TexIndex].Repeat_X = true;
	Engine.textures[TexIndex].Repeat_Y = true;
	Engine.meshes[temp].have_diff_text = true;
	Engine.meshes[temp].text_ID_diff = TexIndex;
	temp--;
	Bitmap.getimage("..\\HG3D 2.1\\Resource\\Models\\Sponza\\textures\\lion.bmp");
	W = Bitmap.Width[0];
	H = Bitmap.Height[0];

	for (int i = 0; i < H; i++)
		for (int j = 0; j < W; j++)
		{
			RGBA[(i * W + j) * 3] = Bitmap.rgb[i * W + j].r;
			RGBA[(i * W + j) * 3 + 1] = Bitmap.rgb[i * W + j].g;
			RGBA[(i * W + j) * 3 + 2] = Bitmap.rgb[i * W + j].b;
		}
	TexIndex = Engine.add_texture(RGBA, W, H, 3, false, 0, 0);
	free(Bitmap.rgb);

	Engine.meshes[temp].have_diff_text = true;
	Engine.meshes[temp].text_ID_diff = TexIndex;

	Bitmap.getimage("..\\HG3D 2.1\\Resource\\Models\\Sponza\\textures\\vase_h.bmp");
	W = Bitmap.Width[0];
	H = Bitmap.Height[0];

	for (int i = 0; i < H; i++)
		for (int j = 0; j < W; j++)
		{
			RGBA[(i * W + j) * 3] = Bitmap.rgb[i * W + j].r;
			RGBA[(i * W + j) * 3 + 1] = Bitmap.rgb[i * W + j].g;
			RGBA[(i * W + j) * 3 + 2] = Bitmap.rgb[i * W + j].b;
		}
	TexIndex = Engine.add_texture(RGBA, W, H, 3, false, 0, 0);
	free(Bitmap.rgb);

	Engine.meshes[temp - 1].have_diff_text = true;
	Engine.meshes[temp - 1].text_ID_diff = TexIndex;

	Bitmap.getimage("..\\HG3D 2.1\\Resource\\Models\\Sponza\\textures\\vase_hanging.bmp");
	W = Bitmap.Width[0];
	H = Bitmap.Height[0];

	for (int i = 0; i < H; i++)
		for (int j = 0; j < W; j++)
		{
			RGBA[(i * W + j) * 3] = Bitmap.rgb[i * W + j].r;
			RGBA[(i * W + j) * 3 + 1] = Bitmap.rgb[i * W + j].g;
			RGBA[(i * W + j) * 3 + 2] = Bitmap.rgb[i * W + j].b;
		}
	TexIndex = Engine.add_texture(RGBA, W, H, 3, false, 0, 0);
	free(Bitmap.rgb);

	Engine.meshes[temp - 2].have_diff_text = true;
	Engine.meshes[temp - 2].text_ID_diff = TexIndex;
	
	Bitmap.getimage("..\\HG3D 2.1\\Resource\\Models\\Sponza\\textures\\curtain_green.bmp");
	W = Bitmap.Width[0];
	H = Bitmap.Height[0];
	
	for (int i = 0; i < H; i++)
		for (int j = 0; j < W; j++)
		{
			RGBA[(i * W + j) * 3] = Bitmap.rgb[i * W + j].r;
			RGBA[(i * W + j) * 3 + 1] = Bitmap.rgb[i * W + j].g;
			RGBA[(i * W + j) * 3 + 2] = Bitmap.rgb[i * W + j].b;
		}
	TexIndex = Engine.add_texture(RGBA, W, H, 3, false, 0, 0);
	free(Bitmap.rgb);
	
	Engine.meshes[temp - 4].have_diff_text = true;
	Engine.meshes[temp - 4].text_ID_diff = TexIndex;

	Bitmap.getimage("..\\HG3D 2.1\\Resource\\Models\\Sponza\\textures\\curtain_blue.bmp");
	W = Bitmap.Width[0];
	H = Bitmap.Height[0];

	for (int i = 0; i < H; i++)
		for (int j = 0; j < W; j++)
		{
			RGBA[(i * W + j) * 3] = Bitmap.rgb[i * W + j].r;
			RGBA[(i * W + j) * 3 + 1] = Bitmap.rgb[i * W + j].g;
			RGBA[(i * W + j) * 3 + 2] = Bitmap.rgb[i * W + j].b;
		}
	TexIndex = Engine.add_texture(RGBA, W, H, 3, false, 0, 0);
	free(Bitmap.rgb);

	Engine.meshes[temp - 5].have_diff_text = true;
	Engine.meshes[temp - 5].text_ID_diff = TexIndex;

	Bitmap.getimage("..\\HG3D 2.1\\Resource\\Models\\Sponza\\textures\\curtain_red.bmp");
	W = Bitmap.Width[0];
	H = Bitmap.Height[0];

	for (int i = 0; i < H; i++)
		for (int j = 0; j < W; j++)
		{
			RGBA[(i * W + j) * 3] = Bitmap.rgb[i * W + j].r;
			RGBA[(i * W + j) * 3 + 1] = Bitmap.rgb[i * W + j].g;
			RGBA[(i * W + j) * 3 + 2] = Bitmap.rgb[i * W + j].b;
		}
	TexIndex = Engine.add_texture(RGBA, W, H, 3, false, 0, 0);
	free(Bitmap.rgb);

	Engine.meshes[temp - 6].have_diff_text = true;
	Engine.meshes[temp - 6].text_ID_diff = TexIndex;

	Bitmap.getimage("..\\HG3D 2.1\\Resource\\Models\\Sponza\\textures\\fabric_green.bmp");
	W = Bitmap.Width[0];
	H = Bitmap.Height[0];

	for (int i = 0; i < H; i++)
		for (int j = 0; j < W; j++)
		{
			RGBA[(i * W + j) * 3] = Bitmap.rgb[i * W + j].r;
			RGBA[(i * W + j) * 3 + 1] = Bitmap.rgb[i * W + j].g;
			RGBA[(i * W + j) * 3 + 2] = Bitmap.rgb[i * W + j].b;
		}
	TexIndex = Engine.add_texture(RGBA, W, H, 3, false, 0, 0);
	free(Bitmap.rgb);
	Engine.textures[TexIndex].Repeat_X = true;
	Engine.textures[TexIndex].Repeat_Y = true;
	Engine.meshes[temp - 7].have_diff_text = true;
	Engine.meshes[temp - 7].text_ID_diff = TexIndex;

	Bitmap.getimage("..\\HG3D 2.1\\Resource\\Models\\Sponza\\textures\\fabric_red.bmp");
	W = Bitmap.Width[0];
	H = Bitmap.Height[0];

	for (int i = 0; i < H; i++)
		for (int j = 0; j < W; j++)
		{
			RGBA[(i * W + j) * 3] = Bitmap.rgb[i * W + j].r;
			RGBA[(i * W + j) * 3 + 1] = Bitmap.rgb[i * W + j].g;
			RGBA[(i * W + j) * 3 + 2] = Bitmap.rgb[i * W + j].b;
		}
	TexIndex = Engine.add_texture(RGBA, W, H, 3, false, 0, 0);
	free(Bitmap.rgb);
	Engine.textures[TexIndex].Repeat_X = true;
	Engine.textures[TexIndex].Repeat_Y = true;
	Engine.meshes[temp - 8].have_diff_text = true;
	Engine.meshes[temp - 8].text_ID_diff = TexIndex;

	Bitmap.getimage("..\\HG3D 2.1\\Resource\\Models\\Sponza\\textures\\fabric_blue.bmp");
	W = Bitmap.Width[0];
	H = Bitmap.Height[0];

	for (int i = 0; i < H; i++)
		for (int j = 0; j < W; j++)
		{
			RGBA[(i * W + j) * 3] = Bitmap.rgb[i * W + j].r;
			RGBA[(i * W + j) * 3 + 1] = Bitmap.rgb[i * W + j].g;
			RGBA[(i * W + j) * 3 + 2] = Bitmap.rgb[i * W + j].b;
		}
	TexIndex = Engine.add_texture(RGBA, W, H, 3, false, 0, 0);
	free(Bitmap.rgb);
	Engine.textures[TexIndex].Repeat_X = true;
	Engine.textures[TexIndex].Repeat_Y = true;
	Engine.meshes[temp - 9].have_diff_text = true;
	Engine.meshes[temp - 9].text_ID_diff = TexIndex;

	Bitmap.getimage("..\\HG3D 2.1\\Resource\\Models\\Sponza\\textures\\flagpole.bmp");
	W = Bitmap.Width[0];
	H = Bitmap.Height[0];

	for (int i = 0; i < H; i++)
		for (int j = 0; j < W; j++)
		{
			RGBA[(i * W + j) * 3] = Bitmap.rgb[i * W + j].r;
			RGBA[(i * W + j) * 3 + 1] = Bitmap.rgb[i * W + j].g;
			RGBA[(i * W + j) * 3 + 2] = Bitmap.rgb[i * W + j].b;
		}
	TexIndex = Engine.add_texture(RGBA, W, H, 3, false, 0, 0);
	free(Bitmap.rgb);

	Engine.meshes[temp - 10].have_diff_text = true;
	Engine.meshes[temp - 10].text_ID_diff = TexIndex;

	Bitmap.getimage("..\\HG3D 2.1\\Resource\\Models\\Sponza\\textures\\details.bmp");
	W = Bitmap.Width[0];
	H = Bitmap.Height[0];

	for (int i = 0; i < H; i++)
		for (int j = 0; j < W; j++)
		{
			RGBA[(i * W + j) * 3] = Bitmap.rgb[i * W + j].r;
			RGBA[(i * W + j) * 3 + 1] = Bitmap.rgb[i * W + j].g;
			RGBA[(i * W + j) * 3 + 2] = Bitmap.rgb[i * W + j].b;
		}
	TexIndex = Engine.add_texture(RGBA, W, H, 3, false, 0, 0);
	free(Bitmap.rgb);

	Engine.meshes[temp - 11].have_diff_text = true;
	Engine.meshes[temp - 11].text_ID_diff = TexIndex;

	Bitmap.getimage("..\\HG3D 2.1\\Resource\\Models\\Sponza\\textures\\column_r_b.bmp");
	W = Bitmap.Width[0];
	H = Bitmap.Height[0];

	for (int i = 0; i < H; i++)
		for (int j = 0; j < W; j++)
		{
			RGBA[(i * W + j) * 3] = Bitmap.rgb[i * W + j].r;
			RGBA[(i * W + j) * 3 + 1] = Bitmap.rgb[i * W + j].g;
			RGBA[(i * W + j) * 3 + 2] = Bitmap.rgb[i * W + j].b;
		}
	TexIndex = Engine.add_texture(RGBA, W, H, 3, false, 0, 0);
	free(Bitmap.rgb);

	Engine.meshes[temp - 12].have_diff_text = true;
	Engine.meshes[temp - 12].text_ID_diff = TexIndex;

	Bitmap.getimage("..\\HG3D 2.1\\Resource\\Models\\Sponza\\textures\\details.bmp");
	W = Bitmap.Width[0];
	H = Bitmap.Height[0];

	for (int i = 0; i < H; i++)
		for (int j = 0; j < W; j++)
		{
			RGBA[(i * W + j) * 3] = Bitmap.rgb[i * W + j].r;
			RGBA[(i * W + j) * 3 + 1] = Bitmap.rgb[i * W + j].g;
			RGBA[(i * W + j) * 3 + 2] = Bitmap.rgb[i * W + j].b;
		}
	TexIndex = Engine.add_texture(RGBA, W, H, 3, false, 0, 0);
	free(Bitmap.rgb);

	Engine.meshes[temp - 13].have_diff_text = true;
	Engine.meshes[temp - 13].text_ID_diff = TexIndex;

	Bitmap.getimage("..\\HG3D 2.1\\Resource\\Models\\Sponza\\textures\\column_s_c.bmp");
	W = Bitmap.Width[0];
	H = Bitmap.Height[0];

	for (int i = 0; i < H; i++)
		for (int j = 0; j < W; j++)
		{
			RGBA[(i * W + j) * 3] = Bitmap.rgb[i * W + j].r;
			RGBA[(i * W + j) * 3 + 1] = Bitmap.rgb[i * W + j].g;
			RGBA[(i * W + j) * 3 + 2] = Bitmap.rgb[i * W + j].b;
		}
	TexIndex = Engine.add_texture(RGBA, W, H, 3, false, 0, 0);
	free(Bitmap.rgb);

	Engine.meshes[temp - 14].have_diff_text = true;
	Engine.meshes[temp - 14].text_ID_diff = TexIndex;

	Bitmap.getimage("..\\HG3D 2.1\\Resource\\Models\\Sponza\\textures\\floor.bmp");
	W = Bitmap.Width[0];
	H = Bitmap.Height[0];

	for (int i = 0; i < H; i++)
		for (int j = 0; j < W; j++)
		{
			RGBA[(i * W + j) * 3] = Bitmap.rgb[i * W + j].r;
			RGBA[(i * W + j) * 3 + 1] = Bitmap.rgb[i * W + j].g;
			RGBA[(i * W + j) * 3 + 2] = Bitmap.rgb[i * W + j].b;
		}
	TexIndex = Engine.add_texture(RGBA, W, H, 3, false, 0, 0);
	free(Bitmap.rgb);
	Engine.textures[TexIndex].Repeat_X = true;
	Engine.textures[TexIndex].Repeat_Y = true;
	Engine.meshes[temp - 15].have_diff_text = true;
	Engine.meshes[temp - 15].text_ID_diff = TexIndex;

	Bitmap.getimage("..\\HG3D 2.1\\Resource\\Models\\Sponza\\textures\\column_h_a.bmp");
	W = Bitmap.Width[0];
	H = Bitmap.Height[0];

	for (int i = 0; i < H; i++)
		for (int j = 0; j < W; j++)
		{
			RGBA[(i * W + j) * 3] = Bitmap.rgb[i * W + j].r;
			RGBA[(i * W + j) * 3 + 1] = Bitmap.rgb[i * W + j].g;
			RGBA[(i * W + j) * 3 + 2] = Bitmap.rgb[i * W + j].b;
		}
	TexIndex = Engine.add_texture(RGBA, W, H, 3, false, 0, 0);
	free(Bitmap.rgb);

	Engine.meshes[temp - 16].have_diff_text = true;
	Engine.meshes[temp - 16].text_ID_diff = TexIndex;

	Bitmap.getimage("..\\HG3D 2.1\\Resource\\Models\\Sponza\\textures\\ceiling.bmp");
	W = Bitmap.Width[0];
	H = Bitmap.Height[0];

	for (int i = 0; i < H; i++)
		for (int j = 0; j < W; j++)
		{
			RGBA[(i * W + j) * 3] = Bitmap.rgb[i * W + j].r;
			RGBA[(i * W + j) * 3 + 1] = Bitmap.rgb[i * W + j].g;
			RGBA[(i * W + j) * 3 + 2] = Bitmap.rgb[i * W + j].b;
		}
	TexIndex = Engine.add_texture(RGBA, W, H, 3, false, 0, 0);
	free(Bitmap.rgb);
	Engine.textures[TexIndex].Repeat_X = true;
	Engine.textures[TexIndex].Repeat_Y = true;
	Engine.meshes[temp - 17].have_diff_text = true;
	Engine.meshes[temp - 17].text_ID_diff = TexIndex;

	Bitmap.getimage("..\\HG3D 2.1\\Resource\\Models\\Sponza\\textures\\arch.bmp");
	W = Bitmap.Width[0];
	H = Bitmap.Height[0];

	for (int i = 0; i < H; i++)
		for (int j = 0; j < W; j++)
		{
			RGBA[(i * W + j) * 3] = Bitmap.rgb[i * W + j].r;
			RGBA[(i * W + j) * 3 + 1] = Bitmap.rgb[i * W + j].g;
			RGBA[(i * W + j) * 3 + 2] = Bitmap.rgb[i * W + j].b;
		}
	TexIndex = Engine.add_texture(RGBA, W, H, 3, false, 0, 0);
	free(Bitmap.rgb);

	Engine.meshes[temp - 18].have_diff_text = true;
	Engine.meshes[temp - 18].text_ID_diff = TexIndex;

	Bitmap.getimage("..\\HG3D 2.1\\Resource\\Models\\Sponza\\textures\\bricks.bmp");
	W = Bitmap.Width[0];
	H = Bitmap.Height[0];

	for (int i = 0; i < H; i++)
		for (int j = 0; j < W; j++)
		{
			RGBA[(i * W + j) * 3] = Bitmap.rgb[i * W + j].r;
			RGBA[(i * W + j) * 3 + 1] = Bitmap.rgb[i * W + j].g;
			RGBA[(i * W + j) * 3 + 2] = Bitmap.rgb[i * W + j].b;
		}
	TexIndex = Engine.add_texture(RGBA, W, H, 3, false, 0, 0);
	free(Bitmap.rgb);
	Engine.textures[TexIndex].Repeat_X = true;
	Engine.textures[TexIndex].Repeat_Y = true;
	Engine.meshes[temp - 19].have_diff_text = true;
	Engine.meshes[temp - 19].text_ID_diff = TexIndex;

	Bitmap.getimage("..\\HG3D 2.1\\Resource\\Models\\Sponza\\textures\\fabric_red.bmp");
	W = Bitmap.Width[0];
	H = Bitmap.Height[0];

	for (int i = 0; i < H; i++)
		for (int j = 0; j < W; j++)
		{
			RGBA[(i * W + j) * 3] = Bitmap.rgb[i * W + j].r;
			RGBA[(i * W + j) * 3 + 1] = Bitmap.rgb[i * W + j].g;
			RGBA[(i * W + j) * 3 + 2] = Bitmap.rgb[i * W + j].b;
		}
	TexIndex = Engine.add_texture(RGBA, W, H, 3, false, 0, 0);
	free(Bitmap.rgb);

	Engine.meshes[temp - 20].have_diff_text = true;
	Engine.meshes[temp - 20].text_ID_diff = TexIndex;

	Bitmap.getimage("..\\HG3D 2.1\\Resource\\Models\\Sponza\\textures\\lionb.bmp");
	W = Bitmap.Width[0];
	H = Bitmap.Height[0];

	for (int i = 0; i < H; i++)
		for (int j = 0; j < W; j++)
		{
			RGBA[(i * W + j) * 3] = Bitmap.rgb[i * W + j].r;
			RGBA[(i * W + j) * 3 + 1] = Bitmap.rgb[i * W + j].g;
			RGBA[(i * W + j) * 3 + 2] = Bitmap.rgb[i * W + j].b;
		}
	TexIndex = Engine.add_texture(RGBA, W, H, 3, false, 0, 0);
	free(Bitmap.rgb);

	Engine.meshes[temp - 21].have_diff_text = true;
	Engine.meshes[temp - 21].text_ID_diff = TexIndex;

	Bitmap.getimage("..\\HG3D 2.1\\Resource\\Models\\Sponza\\textures\\vase_round.bmp");
	W = Bitmap.Width[0];
	H = Bitmap.Height[0];

	for (int i = 0; i < H; i++)
		for (int j = 0; j < W; j++)
		{
			RGBA[(i * W + j) * 3] = Bitmap.rgb[i * W + j].r;
			RGBA[(i * W + j) * 3 + 1] = Bitmap.rgb[i * W + j].g;
			RGBA[(i * W + j) * 3 + 2] = Bitmap.rgb[i * W + j].b;
		}
	TexIndex = Engine.add_texture(RGBA, W, H, 3, false, 0, 0);
	free(Bitmap.rgb);

	Engine.meshes[temp - 22].have_diff_text = true;
	Engine.meshes[temp - 22].text_ID_diff = TexIndex;

	Bitmap.getimage("..\\HG3D 2.1\\Resource\\Models\\Sponza\\textures\\fabric_red.bmp");
	W = Bitmap.Width[0];
	H = Bitmap.Height[0];

	for (int i = 0; i < H; i++)
		for (int j = 0; j < W; j++)
		{
			RGBA[(i * W + j) * 3] = Bitmap.rgb[i * W + j].r;
			RGBA[(i * W + j) * 3 + 1] = Bitmap.rgb[i * W + j].g;
			RGBA[(i * W + j) * 3 + 2] = Bitmap.rgb[i * W + j].b;
		}
	TexIndex = Engine.add_texture(RGBA, W, H, 3, false, 0, 0);
	free(Bitmap.rgb);

	Engine.meshes[temp - 23].have_diff_text = true;
	Engine.meshes[temp - 23].text_ID_diff = TexIndex;

	Bitmap.getimage("..\\HG3D 2.1\\Resource\\Models\\Sponza\\textures\\fabric_green.bmp");
	W = Bitmap.Width[0];
	H = Bitmap.Height[0];

	for (int i = 0; i < H; i++)
		for (int j = 0; j < W; j++)
		{
			RGBA[(i * W + j) * 3] = Bitmap.rgb[i * W + j].r;
			RGBA[(i * W + j) * 3 + 1] = Bitmap.rgb[i * W + j].g;
			RGBA[(i * W + j) * 3 + 2] = Bitmap.rgb[i * W + j].b;
		}
	TexIndex = Engine.add_texture(RGBA, W, H, 3, false, 0, 0);
	free(Bitmap.rgb);

	Engine.meshes[temp - 24].have_diff_text = true;
	Engine.meshes[temp - 24].text_ID_diff = TexIndex;

	bool trigered = false;

	point O;

	while (msg->message != WM_QUIT)  
	{
		if (PeekMessage(msg, NULL, 0, 0, PM_REMOVE))
		{
			Update_Wnd();
			TranslateMessage(msg);
			DispatchMessage(msg);
			int MWD = Get_Mouse_Wheel_Delta();
			if (Get_Mouse_Stat(Mouse_Right_Stat))
			{
				trigered = !trigered;
				point temp_point = Engine.cameras[0].camera_position;
				temp_point.move(Engine.cameras[0].forward.x*80.0, Engine.cameras[0].forward.y*80.0, Engine.cameras[0].forward.z*80.0);

				O = temp_point;
			}

			if (MWD != 0 && !Get_Mouse_Stat(Mouse_Right_Stat))//check if mouse will move
			{
				HG3D_Engine::point LastPos;//last pos

				LastPos = Engine.cameras[0].camera_position;//set the value

				HG3D_Engine::vector Movement;//set movement

				Movement = normalize(Engine.cameras[0].forward)*long double(MWD) / long double(20.0);//get the movement

				Engine.cameras[0].camera_position.build(LastPos.x + Movement.x, LastPos.y + Movement.y, LastPos.z + Movement.z);//move

				Engine.cameras[0].needs_update = true;//need update
			}
			if (MWD != 0 && Get_Mouse_Stat(Mouse_Right_Stat))//check if mouse will move
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
			{
			    World.UpdateCollision();
				World.Update(dt); 

			}

			//Engine.test_render(); //render scene

			if (trigered)
			{
				Engine.cameras[0].camera_position.rotate(O, 0.0f, 0.05f*float(dt), 0.0f);
				Engine.cameras[0].forward.build(O.x - Engine.cameras[0].camera_position.x, 0.0, O.z - Engine.cameras[0].camera_position.z);
				Engine.cameras[0].forward = normalize(Engine.cameras[0].forward);
				Engine.cameras[0].needs_update = true;
			}
			Engine.render();

		}

	}

	DestroyCachedSystemInformation();

	return (int)msg->wParam;
}