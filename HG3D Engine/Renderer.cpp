#include "HG3D Engine.h"
#include "..\File_HG_FS\File_HG_FS.h"
#include "..\Shared Headers\hstring.h"
#include "GLew.h"
static unsigned long int renderer_class_nums = 0;
static unsigned long int shader_nums = 0;
/******************************************************************/
/**************************test console****************************/
/******************************************************************/
/******************************************************************/
string Final_str[2];
/******************************************************************/
/******************************************************************/
/**************************test console****************************/
/******************************************************************/
GLuint LoadShaders(const char *V_Shader_Path, const char *F_Shader_Path)
{
	GLint Result = GL_FALSE;
	GLuint ProgramID = glCreateProgram();//creat program
	File_HG_FS::File V_Shader, F_Shader;//shader's file
	GLuint VertexShaderID = 0, FragmentShaderID = 0;
	Final_str[0] = "vertex shader #";
	Final_str[0] = Final_str[0] + inttostring(shader_nums);
	Final_str[0] = Final_str[0] + " log: \n";
	if (V_Shader_Path != NULL) //check if the path exists
	{
		V_Shader.open(V_Shader_Path);
		VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(VertexShaderID, 1, &V_Shader.data, (int*)&V_Shader.size);
		glCompileShader(VertexShaderID);
		glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
		if (Result == GL_FALSE)
		{
			GLint maxLength = 0;
			string Log;
			Log = "";
			glGetShaderInfoLog(VertexShaderID, 1000, &maxLength, Log.string1); //get the log
			Final_str[0] = Final_str[0] + Log; //add the log to the string
			Final_str[0] = Final_str[0] + "\n\n\n"; //add triple new lines
		}
		else
			Final_str[0] = Final_str[0] + "well compiled. \n";  //every thing went well
		glAttachShader(ProgramID, VertexShaderID);
	}
	else
		Final_str[0] = Final_str[0] + "not found. \n"; //no such path idiot
	Final_str[1] = "fragment shader #"; 
	Final_str[1] = Final_str[1] + inttostring(shader_nums);
	Final_str[1] = Final_str[1] + " log: \n";
	if (F_Shader_Path != NULL)
	{
		F_Shader.open(F_Shader_Path); //check if the path exists
		FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(FragmentShaderID, 1, &F_Shader.data, (int*)&F_Shader.size);
		glCompileShader(FragmentShaderID);
		glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
		if (Result == GL_FALSE)
		{
			GLsizei maxLength = 0;
			string Log;
			Log = "";
			glGetShaderInfoLog(FragmentShaderID, 1000, &maxLength, Log.string1); //get the log
			Final_str[1] = Final_str[1] + Log; //add the log to the string
			Final_str[1] = Final_str[1] + "\n\n\n"; //add triple new lines
		}
		else
			Final_str[1] = Final_str[1] + "well compiled. \n"; //every thing went well
		glAttachShader(ProgramID, FragmentShaderID);
	}
	else
		Final_str[1] = Final_str[1] + "not found. \n";//no such path idiot
	glBindAttribLocation(ProgramID, 0, "Vertex");
	glBindAttribLocation(ProgramID, 1, "Normal");
	glBindAttribLocation(ProgramID, 2, "Coord");
	glBindAttribLocation(ProgramID, 3, "bone_ID");
	glBindFragDataLocation(ProgramID, 0, "Output1");//bind the out put 
	glLinkProgram(ProgramID);//link shaders
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	if (V_Shader_Path != NULL)
		glDeleteShader(VertexShaderID);//delete if only one was created
	if (F_Shader_Path != NULL)
		glDeleteShader(FragmentShaderID);//delete if only one was created
	shader_nums++;
	return ProgramID;
}
//end of shader loader
void SetupPixelFormat_WND_DC(HDC hDC) //setup pixel format
{
	int nPixelFormat;
	static PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),          //size of structure
		1,                                      //default version 
		PFD_DRAW_TO_WINDOW |                    //window drawing support
		PFD_SUPPORT_OPENGL |                    //opengl support
		PFD_DOUBLEBUFFER,                       //double buffering support
		PFD_TYPE_RGBA,                          //RGBA color mode
		32,                                     //32 bit color mode
		0, 0, 0, 0, 0, 0,                       //ignore color bits
		0,                                      //no alpha buffer
		1,                                      //ignore shift bit
		0,                                      //no accumulation buffer
		0, 0, 0, 0,                             //ignore accumulation bits
		32,                                     //32 bit z-buffer size
		0,                                      //no stencil buffer
		0,                                      //no aux buffer
		PFD_MAIN_PLANE,                         //main drawing plane
		0,                                      //reserved
		0, 0, 0 };                              //layer masks ignored
	nPixelFormat = ChoosePixelFormat(hDC, &pfd);                            //layer masks ignored
	SetPixelFormat(hDC, nPixelFormat, &pfd);
}
namespace HG3D_Engine
{
	void camera::update_camera()//update camera
	{
		ViewMatrix = LookAt(camera_position, forward, up);//update view matrix
		ProjectionMatrix = Projection(Left, Right, Buttom, Top, Near, Far);//update projection
		needs_update = 0;
	}
	void camera::fps_camera(float pitch, float yaw, vector head_up)//update camera
	{
		_4x4matrix rotationP;
		_4x4matrix rotationY;
		point o;
		o.build(0.0f, 0.0f, 0.0f);
		rotationP.LoadRotation(head_up, o, pitch);//calculate rotaton matrix for pitch
		rotationY.LoadRotation(cross(head_up, forward), o, yaw);//calculate rotaton matrix for yaw
		forward = rotationY*(rotationP*forward);//multiply by mats pitch first
		up = rotationY*(rotationP*up);//multiply by mats pitch first
		needs_update = 1;
	}

	void texture::build(unsigned char *irgba, unsigned int w, unsigned int h, unsigned short int NOC, bool icompressed, unsigned long int icompression_method, unsigned long int idata_size)
	{
		width = w;
		height = h;//set the input
		number_of_components = NOC;
		if (!icompressed)
			total_size = width*height*number_of_components;//get the total memory size
		else
			total_size = idata_size;
		rgba = (unsigned char*)malloc(total_size);//allocate memory
		for (register unsigned long int i = 0; i < total_size; i++)//copy the data
			rgba[i] = irgba[i];
		needs_update = 1;//this will need updat after building
		generate_mipmaps = 1;//yes you probaly want them
		Repeat_X = 0;
		Repeat_Y = 0;
		glGenTextures(1, &texture_name);//generate the texture so you can free it
		compressed = icompressed;
		compression_method = icompression_method;
	}
	void texture::free_data()
	{
		free(rgba);//free allocation
		glDeleteTextures(1, &texture_name);//delete the texture
	}
	void texture::update()
	{
		glBindTexture(GL_TEXTURE_2D, texture_name);//bind the texture to update the data
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, Repeat_X ? GL_REPEAT : GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, Repeat_Y ? GL_REPEAT : GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		if (generate_mipmaps)//set min filter to use the mipmap if it was wanted
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		else
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		if (!compressed)//is it compressed data?
		{
			unsigned long int format = GL_RGBA;
			if (number_of_components == 3)
				format = GL_RGB;
			else if (number_of_components == 2)
				format = GL_RG;
			else if (number_of_components == 1)
				format = GL_R;
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, rgba);//set the data
			if (generate_mipmaps)//creat the miomaps if wanted
				gluBuild2DMipmaps(GL_TEXTURE_2D, format, width, height, format, GL_UNSIGNED_BYTE, rgba);
		}
		else
		{
			if (generate_mipmaps)//creat the miomaps if wanted
				glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP_SGIS, true);
			glCompressedTexImage2D(GL_TEXTURE_2D, 0, compression_method, width, height, 0, total_size, rgba);
		}
		glBindTexture(GL_TEXTURE_2D, 0);//unbind the texture 
		needs_update = 0;//just updated no need to redo it
	}
	void texture::clone_NMA(texture input)
	{
		texture_name = input.texture_name;//clone  every thing dont reallocate new memory
		Repeat_X = input.Repeat_X;
		needs_update = input.needs_update;
		generate_mipmaps = input.generate_mipmaps;
		number_of_components = input.number_of_components;
		width = input.width;
		height = input.height;
		total_size = input.total_size;
		rgba = input.rgba;//get the pointer the data is still usefull
	}

	void light::build()//initialize the way that the light be instantly usable
	{
		for (register int i = 0; i < 3; i++)
		{
			light_position[i] = 0.0f;
			light_color[i] = 1.0f;
			Attenuation[i] = 0.0f;
			direction[i] = 0.0f;
		}
		Attenuation[0] = 1.0f;
		max_radius = 10000.0f;
		cut_off_cos = -1.0f;
		light_enabled = 0;
		update_shadow_maps = 1;
		shadow_map = 1;
	}
	void light::calculate_max_radius()
	{
		if (Attenuation[2] == 0.0f && Attenuation[1] == 0.0f)
			max_radius = 10000.0f;//it will never not be contirbuting
		else
		{
			if (Attenuation[2] == 0.0f)
			{
				max_radius = (256.0f - Attenuation[0]) / Attenuation[1];
				return;
			}
			float min_value_rad = -1.0f * Attenuation[1] / Attenuation[2] / 2.0f;//the minimum value's occurrence radius 
			if (min_value_rad*(min_value_rad * Attenuation[2] + Attenuation[1]) +Attenuation[0] > 256.0f)
				max_radius = 0.0f;//it will always be more that 256
			else
			{
				float delta = Attenuation[1] * Attenuation[1] + 4.0f * (256.0f - Attenuation[0])*Attenuation[2];
				if (delta < 0.0f)
					max_radius = max_radius = 10000.0f;//it will never not be contirbuting;
				else
					max_radius = min_value_rad + sqrt(delta) / (2.0f * Attenuation[2]);//what ever the color after this atinoution the light contribution is 0
			}
		}
	}
	void light::operator=(light input)//needed in add light to duplicate the light data
	{
		for (register int i = 0; i < 3; i++)
		{
			light_position[i] = input.light_position[i];
			light_color[i] = input.light_color[i];
			Attenuation[i] = input.Attenuation[i];
			direction[i] = input.direction[i];
		}
		max_radius = input.max_radius;
		cut_off_cos = input.cut_off_cos;
		light_enabled = input.light_enabled;
		shadow_map = input.shadow_map;
		update_shadow_maps = input.update_shadow_maps;
	}

	void Renderer::init()
	{
		mesh_nums = 0;
		vert_nums = 0;
		total_size = 0;            //initialize values
		cameras_nums = 0;
		current_camera_nums = 0;
		last_light_ID = 0;
		texture_nums = 0;
		light_data_changed = 0;
		meshes =(Mesh*) malloc(0);
		cameras = (camera*)malloc(0);//so they can be freed
		textures = (texture*)malloc(0);
		mesh_draw_order = (unsigned long int*)malloc(0);
		current_cameras = (unsigned long int*)malloc(0);
		for (register int i = 0; i < 100; i++)
			lights[i].build();
		rendere_ID = renderer_class_nums;//give it an ID
		renderer_class_nums++;//a renderer added
		SetupPixelFormat_WND_DC(hdc);//setup the pixel format
		hrc=wglCreateContext(hdc); //creat a render context
		wglMakeCurrent(hdc, hrc);//make the rc current
		Init_glew();//initialize glew functions
		glEnable(GL_DEPTH_TEST); //enable 
		glDepthFunc(GL_LEQUAL); //less or equal(closer) z are writen
		glEnable(GL_CULL_FACE); //cull the back face 
		glCullFace(GL_BACK);//cull the back face
		glClearDepth(1.0f);//clear the depth with 1
		
		glGenBuffers(1, &light_data_UBO_ID);//generate the UBP
		glBindBufferBase(GL_UNIFORM_BUFFER, lights_UBO_binding_point, light_data_UBO_ID);

		Shaders[0] = LoadShaders("..\\HG3D Engine\\VS00.txt", "..\\HG3D Engine\\FS00.txt");//load shaders
		Light_Block_Index[0] = glGetUniformBlockIndex(Shaders[0], "lights");//get lights index
		glUniformBlockBinding(Shaders[0], Light_Block_Index[0], lights_UBO_binding_point);
		//get uniforms loactions
		Model_Matrix_Location[0] = glGetUniformLocation(Shaders[0], "ModelMatrix");
		Normal_Matrix_Location[0] = glGetUniformLocation(Shaders[0], "NormalMatrix");
		Projection_Matrix_Location[0] = glGetUniformLocation(Shaders[0], "ProjectionMatrix");
		View_Matrix_Location[0] = glGetUniformLocation(Shaders[0], "ViewMatrix");
		Lights_Nums_Location[0] = glGetUniformLocation(Shaders[0], "Lights_Nums");
		Lights_View_Matrix_Location[0] = glGetUniformLocation(Shaders[0], "light_view_matrix");
		Shadowmap_Sampler_Location[0] = glGetUniformLocation(Shaders[0], "SMTex_Sampler");

		/******************************************************************/
		/**************************test console****************************/
		/******************************************************************/
		/******************************************************************/
		AllocConsole();
		myConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		string tempstring;
		tempstring = "Renderer ID: ";
		tempstring = tempstring + inttostring(rendere_ID);
		tempstring = tempstring + '\n';
		WriteConsole(myConsoleHandle, tempstring.string1, (DWORD)strlen(tempstring.string1), &cCharsWritten, NULL);
		WriteConsole(myConsoleHandle, Final_str[0].string1, (DWORD)strlen(Final_str[0].string1), &cCharsWritten, NULL);
		WriteConsole(myConsoleHandle, Final_str[1].string1, (DWORD)strlen(Final_str[1].string1), &cCharsWritten, NULL);
		/******************************************************************/
		/******************************************************************/
		/**************************test console****************************/
		/******************************************************************/
		//load the shadow mapping shader
		Shaders[1] = LoadShaders("..\\HG3D Engine\\VS01.txt", "..\\HG3D Engine\\FS01.txt");//load shaders
		Model_Matrix_Location[1] = glGetUniformLocation(Shaders[1], "ModelMatrix");
		View_Matrix_Location[1] = glGetUniformLocation(Shaders[1], "ViewMatrix");
		Lightradius_Location[1] = glGetUniformLocation(Shaders[1], "radius");
		/******************************************************************/
		/**************************test console****************************/
		/******************************************************************/
		/******************************************************************/
		WriteConsole(myConsoleHandle, Final_str[0].string1, (DWORD)strlen(Final_str[0].string1), &cCharsWritten, NULL);
		WriteConsole(myConsoleHandle, Final_str[1].string1, (DWORD)strlen(Final_str[1].string1), &cCharsWritten, NULL);
		/******************************************************************/
		/******************************************************************/
		/**************************test console****************************/
		/******************************************************************/

		/********************************************************************/
		/*					setting up shadow map textures					*/
		/********************************************************************/
		for (register int i = 0; i < MaxShadowmapsNums; i++)
		{
			glGenTextures(1, &Shadow_Maps_Tex_ID[i]);//generate the texture so you can free it
			glBindTexture(GL_TEXTURE_2D, Shadow_Maps_Tex_ID[i]);//bind the texture
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, Shadowmap_Res, Shadowmap_Res, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//linear filter (we'll use it)
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP); //clamping dosnt really matter since we wont read out of the range
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		}
		glBindTexture(GL_TEXTURE_2D, 0);//unbind the texture
		/********************************************************************/
		/*					setting up shadow map textures					*/
		/********************************************************************/
		/********************************************************************/
		/*						setting up shadow map fbo					*/
		/********************************************************************/
		glGenRenderbuffers(1, &Shadowmap_RBO_ID);
		glBindRenderbuffer(GL_RENDERBUFFER, Shadowmap_RBO_ID);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, Shadowmap_Res, Shadowmap_Res);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glGenFramebuffers(1, &Shadowmap_FBO_ID);//set the fbo
		glBindFramebuffer(GL_FRAMEBUFFER, Shadowmap_FBO_ID);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, Shadowmap_RBO_ID);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		/********************************************************************/
		/*						setting up shadow map fbo					*/
		/********************************************************************/
		wglMakeCurrent(hdc, 0);//make the rc current

	}
	unsigned long int Renderer::add_mesh(char* path)	//add a mesh 
	{
		mesh_nums++;//add number of meshes by 1
		Mesh *meshes_the_next = (Mesh *)malloc(sizeof(Mesh)*mesh_nums);//allocate new meshes' memory


		for (register unsigned long int i = 0; i < mesh_nums - 1; i++)
			meshes_the_next[i].clone_NMA(meshes[i]);//clone last data


		meshes_the_next[mesh_nums - 1].clear_last_buff = 0;//don't clear last buffer this is first run of this mesh
		meshes_the_next[mesh_nums - 1].load_mesh(path);//load mesh from path

		unsigned long int temp = mesh_nums - 1;
		meshes_the_next[mesh_nums - 1].MeshID[3] = unsigned char(temp % 256);//mesh id to be writen as 4 8 byte int
		temp = temp / 256;
		meshes_the_next[mesh_nums - 1].MeshID[2] = unsigned char(temp % 256);
		temp = temp / 256;
		meshes_the_next[mesh_nums - 1].MeshID[1] = unsigned char(temp % 256);
		temp = temp / 256;
		meshes_the_next[mesh_nums - 1].MeshID[0] = unsigned char(temp % 256);

		total_size += meshes_the_next[mesh_nums - 1].total_size;//update total size
		vert_nums += meshes_the_next[mesh_nums - 1].vert_nums;//update total vert nums

		free(mesh_draw_order);//it'll ve re allocated
		mesh_draw_order = (unsigned long int*)malloc(sizeof(unsigned long int)*mesh_nums);
		free(meshes);//free last data
		meshes = meshes_the_next;//replace the pointer to new mesh data

		return mesh_nums - 1;//return the added mesh's ID
	}
	unsigned long int Renderer::add_camera()//update camera 
	{
		cameras_nums++;//add number of textures by 1
		camera *cameras_the_next = (camera *)malloc(sizeof(camera)*cameras_nums);//allocate new meshes' memory


		for (register unsigned long int i = 0; i < cameras_nums - 1; i++)
			cameras_the_next[i] = cameras[i];//clone last data


		cameras_the_next[cameras_nums - 1].Near=1.0f;//init to default
		cameras_the_next[cameras_nums - 1].Far = 100000.0f;//init to default
		cameras_the_next[cameras_nums - 1].Left = -1.0f;//init to default
		cameras_the_next[cameras_nums - 1].Right = 1.0f;//init to default
		cameras_the_next[cameras_nums - 1].Top = 1.0f;//init to default
		cameras_the_next[cameras_nums - 1].Buttom = -1.0f;//init to default
		cameras_the_next[cameras_nums - 1].forward.build(0.0f, 0.0f, 1.0f);//init to default
		cameras_the_next[cameras_nums - 1].up.build(0.0f, 1.0f, 0.0f);//init to default
		cameras_the_next[cameras_nums - 1].camera_position.build(0.0f, 0.0f, 0.0f);//init to default
		cameras_the_next[cameras_nums - 1].camera_viewport[0] = 0;//init to default
		cameras_the_next[cameras_nums - 1].camera_viewport[1] = 0;//init to default
		cameras_the_next[cameras_nums - 1].camera_viewport[2] = 600;//init to default
		cameras_the_next[cameras_nums - 1].camera_viewport[3] = 600;//init to default
		cameras_the_next[cameras_nums - 1].update_camera();//update camera

		free(cameras);//free last data
		cameras = cameras_the_next;//replace the pointer to new camera data
		return cameras_nums - 1;//return the added camera's ID
	}
	void Renderer::add_current_camera(unsigned long int camera_ID)
	{
		if (camera_ID >= cameras_nums)
			return;
		current_camera_nums++;//add the numbers
		unsigned long int *temp_current_camera = (unsigned long int*)malloc(sizeof(unsigned long int)*current_camera_nums);//allocated new data
		for (register unsigned long int i = 0; i < current_camera_nums - 1; i++)
			temp_current_camera[i] = current_cameras[i];//copy last data
		free(current_cameras);//free last data
		temp_current_camera[current_camera_nums - 1] = camera_ID;//add new camera
		current_cameras = temp_current_camera;//pass the new pointer
	}
	void Renderer::delete_current_camera(unsigned long int camera_ID)
	{
		if (camera_ID >= cameras_nums)
			return;
		unsigned long int TCCID = 0;//the id of the cammera in current cameras
		bool camre_in_the_list = 0;//is camera in the list
		for (register unsigned long int i = 0; i < current_camera_nums; i++)
			if (camera_ID == current_cameras[i])
			{
				TCCID = i;
				camre_in_the_list = 1;
			}
		if (camre_in_the_list)
		{
			unsigned long int *temp_current_camera = (unsigned long int*)malloc(sizeof(unsigned long int)*(current_camera_nums - 1));
			for (register unsigned long int i = 0; i < current_camera_nums - 1; i++)
				if (i >= TCCID)
					temp_current_camera[i] = current_cameras[i + 1];
				else
					temp_current_camera[i] = current_cameras[i];
			free(current_cameras);
			current_cameras = temp_current_camera;
			current_camera_nums--;
		}
	}
	unsigned long int Renderer::add_texture(unsigned char *irgba, unsigned int w, unsigned int h, unsigned short int NOC, bool icompressed, unsigned long int icompression_method, unsigned long int idata_size)
	{
		texture_nums++;//add number of cameras by 1
		texture *textures_the_next = (texture *)malloc(sizeof(texture) * texture_nums);//allocate new meshes' memory


		for (register unsigned long int i = 0; i < texture_nums - 1; i++)
			textures_the_next[i].clone_NMA(textures[i]);//clone last data with no new memory allocation

		wglMakeCurrent(hdc, hrc);//make the rc current
		textures_the_next[texture_nums - 1].build(irgba, w, h, NOC, icompressed, icompression_method, icompression_method);//build the texture
		wglMakeCurrent(hdc, 0);//make the rc current

		free(textures);//free last data
		textures = textures_the_next;//replace the pointer to new camera data
		return texture_nums - 1;//return the added camera's ID

	}
	/******************************************************************/
	/**************************test functions**************************/
	/******************************************************************/
	/******************************************************************/
	void Renderer::test_render()
	{
		wglMakeCurrent(hdc, hrc);//make the rc current
		int number_of_lights = 0;
		int shadowmapped_lights_num = 0;
		light lights_in_the_scene[MaxLightNums];
		for (register unsigned long int i = 0; i < last_light_ID; i++)//sort light by enablity 
			if (lights[i].light_enabled)
			{
				lights_in_the_scene[number_of_lights] = lights[i];
				number_of_lights++;
				if (number_of_lights == MaxLightNums)//reached the limit?
					break;
			}
		light temp_light;
		for (register int i = 0; i < number_of_lights; i++)//sort light by shadow maping
			if (lights_in_the_scene[i].shadow_map)
			{
				if (i != shadowmapped_lights_num)//no need to sort the lightis in place
				{
					temp_light = lights_in_the_scene[shadowmapped_lights_num];
					lights_in_the_scene[shadowmapped_lights_num] = lights_in_the_scene[i];
					lights_in_the_scene[i] = temp_light;
				}
				shadowmapped_lights_num++;//a light needs shadow map
				if (shadowmapped_lights_num == MaxShadowmapsNums)
					break;
			}
		_4x4matrix light_view[8];
		glUseProgram(Shaders[1]);
		glBindFramebuffer(GL_FRAMEBUFFER, Shadowmap_FBO_ID);//bind the frame buffer
		glViewport(0, 0, Shadowmap_Res, Shadowmap_Res);//draw on the whole texture
		//declare temp variables
		point light_temp_pos;
		vector arbitrary_vecs[2],forward;
		arbitrary_vecs[0].build(1.0f, 0.0f, 0.0f);
		arbitrary_vecs[1].build(0.0f, 1.0f, 0.0f);
		glCullFace(GL_FRONT);//cull the front face
		for (register int i = 0; i < shadowmapped_lights_num; i++)
		{
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, Shadow_Maps_Tex_ID[i], 0);//set the texture as depth buffer and start drawing
			glClear(GL_DEPTH_BUFFER_BIT);//clear the depth

			light_temp_pos.build(lights_in_the_scene[i].light_position[0], lights_in_the_scene[i].light_position[1], lights_in_the_scene[i].light_position[2]);//build the light pos point
			forward.build(lights_in_the_scene[i].direction[0], lights_in_the_scene[i].direction[1], lights_in_the_scene[i].direction[2]);//build the light direction vector

			if (abs(lights_in_the_scene[i].light_position[1]) < 0.01f&&abs(lights_in_the_scene[i].light_position[2]) < 0.01f)//check not to let later errors and then calculate the view matrix
				light_view[i] = LookAt(light_temp_pos, forward, cross(forward, arbitrary_vecs[1]));
			else
				light_view[i] = LookAt(light_temp_pos, forward, cross(forward, arbitrary_vecs[0]));

			glUniformMatrix4fv(View_Matrix_Location[1], 1, 1, light_view[i].x);//send the viewmatrix to shader
			glUniform1f(Lightradius_Location[1], lights_in_the_scene[i].max_radius);//send the max radius to the shader 

			for (register unsigned long int j = 0; j < mesh_nums; j++)
			{
				if (meshes[j].needs_update)//update data if needed
					meshes[j].update_vbo();
				if (meshes[j].subdata_changed)//update data if needed
					meshes[j].remap_vbo();
				glUniformMatrix4fv(Model_Matrix_Location[1], 1, 1, meshes[j].model_matrix.x);//set model matrix
				glBindVertexArray(meshes[j].VAO_ID);//bind vao to draw
				glDrawElements(GL_TRIANGLES, meshes[j].vert_nums, GL_UNSIGNED_INT, meshes[j].indices);
				glBindVertexArray(0);
			}
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glCullFace(GL_BACK);//cull the back face
		//final image
		glUseProgram(Shaders[0]);
		if (textures[0].needs_update)
			textures[0].update();
		if (light_data_changed)//light data needs to be updated
		{
			glUniform1i(Lights_Nums_Location[0], number_of_lights);//send light data to UBO
			glBindBuffer(GL_UNIFORM_BUFFER, light_data_UBO_ID);
			glBufferData(GL_UNIFORM_BUFFER, sizeof(light) * MaxLightNums, (void*)lights_in_the_scene, GL_DYNAMIC_DRAW);
			glBindBuffer(GL_UNIFORM_BUFFER, 0);

			for (int register i = 0; i < MaxShadowmapsNums; i++)
			{
				glUniform1i(Shadowmap_Sampler_Location[0] + i, i);//send light data to UBO
				glActiveTexture(GL_TEXTURE0 + i);
				glBindTexture(GL_TEXTURE_2D, Shadow_Maps_Tex_ID[i]);
			}
			glUniformMatrix4fv(Lights_View_Matrix_Location[0], 8, 1, (float*)light_view);//set view and projection matrices
		}
		glClear(GL_COLOR_BUFFER_BIT);
		for (register unsigned long int j = 0; j < current_camera_nums;j++)
		{
			glClear(GL_DEPTH_BUFFER_BIT);
			if (cameras[current_cameras[j]].needs_update)
				cameras[current_cameras[j]].update_camera();

			glViewport(cameras[current_cameras[j]].camera_viewport[0],//set the view port
				cameras[current_cameras[j]].camera_viewport[1],
				cameras[current_cameras[j]].camera_viewport[2],
				cameras[current_cameras[j]].camera_viewport[3]);

			glUniformMatrix4fv(Projection_Matrix_Location[0], 1, 1, cameras[current_cameras[j]].ProjectionMatrix.x);//set view and projection matrices
			glUniformMatrix4fv(View_Matrix_Location[0], 1, 1, cameras[current_cameras[j]].ViewMatrix.x);
			for (register unsigned long int i = 0; i < mesh_nums; i++)
			{
				if (meshes[i].needs_update)//update data if needed
					meshes[i].update_vbo();
				if (meshes[i].subdata_changed)//update data if needed
					meshes[i].remap_vbo();
				glUniformMatrix4fv(Model_Matrix_Location[0], 1, 1, meshes[i].model_matrix.x);//set model and normal matrices
				glUniformMatrix4fv(Normal_Matrix_Location[0], 1, 0, Inverse(meshes[i].model_matrix).x);
				glBindVertexArray(meshes[i].VAO_ID);//bind vao to draw
				glDrawElements(GL_TRIANGLES, meshes[i].vert_nums, GL_UNSIGNED_INT, meshes[i].indices);
				glBindVertexArray(0);
			}
		}



		glUseProgram(0);
		glDisable(GL_CULL_FACE);
		glEnable(GL_TEXTURE_2D);
		glViewport(0, 0, 1366, 768);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-1.0, 1.0, -1.0, 1.0, 1.0f, 100000.0f);
		glMatrixMode(GL_MODELVIEW);
		glShadeModel(GL_SMOOTH);
		glLoadIdentity();
		gluLookAt(0, 0, -3, 0, 0, 1, 0, 1, 0);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_DEPTH_BUFFER_BIT);

		GLfloat color[4];
		color[0] = 1;
		color[1] = 1;
		color[2] = 1;
		color[3] = 1;
		glColor4fv(color);
		glBegin(GL_TRIANGLES);
		float hwratio = 1.0;
		glTexCoord2f(0, 0);  glVertex2f(0, 0);
		glTexCoord2f(1, 0);  glVertex2f(1, 0);
		glTexCoord2f(1, 1);  glVertex2f(1, 1);
		glTexCoord2f(1, 1);  glVertex2f(1, 1);
		glTexCoord2f(0, 1);  glVertex2f(0, 1);
		glTexCoord2f(0, 0);  glVertex2f(0, 0);
		glEnd();
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);
		glEnable(GL_CULL_FACE);

















		glFlush();
		SwapBuffers(hdc);
		wglMakeCurrent(hdc, 0);//make the rc current
	}
	/******************************************************************/
	/******************************************************************/
	/**************************test functions**************************/
	/******************************************************************/
}