#include "HG3D Engine.h"
#include "..\File_HG_FS\File_HG_FS.h"
#include "..\Shared Headers\hstring.h"
#include "GLew.h"
static unsigned long int renderer_class_nums = 0;
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
	Final_str[0] = "vertex shader log: \n";
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
			glGetShaderInfoLog(VertexShaderID, sizeof(Log.string1), &maxLength, Log.string1); //get the log
			Final_str[0] = Final_str[0] + Log; //add the log to the string
			Final_str[0] = Final_str[0] + "\n\n\n"; //add triple new lines
		}
		else
			Final_str[0] = Final_str[0] + "well compiled. \n";  //every thing went well
		glAttachShader(ProgramID, VertexShaderID);
	}
	else
		Final_str[0] = Final_str[0] + "not found. \n"; //no such path idiot
	Final_str[1] = "fragment shader log: \n";
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
			glGetShaderInfoLog(FragmentShaderID, sizeof(Log.string1)/2, &maxLength, Log.string1); //get the log
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
	glBindFragDataLocation(ProgramID, 0, "Output1");//bind the out put 
	glLinkProgram(ProgramID);//link shaders
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	if (V_Shader_Path != NULL)
		glDeleteShader(VertexShaderID);//delete if only one was created
	if (F_Shader_Path != NULL)
		glDeleteShader(FragmentShaderID);//delete if only one was created
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
		24,                                     //16 bit z-buffer size
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
		ViewMatrix = LookAt(camera_position, forward,up);//update view matrix
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

	void Renderer::init()
	{
		mesh_nums = 0;
		vert_nums = 0;
		total_size = 0;            //initialize values
		cameras_nums = 0;
		current_camera_nums = 0;
		meshes =(Mesh*) malloc(0);
		cameras = (camera*)malloc(0);
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
		Shaders[0] = LoadShaders("..\\HG3D Engine\\VS00.txt", "..\\HG3D Engine\\FS00.txt");//load shaders
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
	}
	unsigned long int Renderer::add_mesh(char* path)	//add a mesh 
	{
		mesh_nums++;//add number of meshes by 1
		Mesh *meshes_the_next = (Mesh *)malloc(sizeof(Mesh)*mesh_nums);//allocate new meshes' memory


		for (register unsigned long int i = 0; i < mesh_nums - 1; i++)
			meshes_the_next[i].clone_NMA(meshes[i]);//clone last data


		meshes_the_next[mesh_nums - 1].clear_last_buff = 0;//don't clear last buffer this is first run of this mesh
		meshes_the_next[mesh_nums-1].load_mesh(path);//load mesh from path


		total_size += meshes_the_next[mesh_nums - 1].total_size;//update total size
		vert_nums += meshes_the_next[mesh_nums - 1].vert_nums;//update total vert nums


		free(meshes);//free last data
		meshes = meshes_the_next;//replace the pointer to new mesh data

		return mesh_nums - 1;//return the added mesh's ID
	}
	unsigned long int Renderer::add_camera()//update camera 
	{
		cameras_nums++;//add number of cameras by 1
		camera *cameras_the_next = (camera *)malloc(sizeof(camera)*1/*cameras_num*s*/);//allocate new meshes' memory


		for (register unsigned long int i = 0; i < cameras_nums - 1; i++)
			cameras_the_next[i] = cameras[i];//clone last data


		cameras_the_next[cameras_nums - 1].Near=1.0f;//init to default
		cameras_the_next[cameras_nums - 1].Far = 1000.0f;//init to default
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
	/******************************************************************/
	/**************************test functions**************************/
	/******************************************************************/
	/******************************************************************/
	void Renderer::test_render()
	{
		glClearDepth(1);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		if (cameras_nums > 0)
		{
			if (cameras[0].needs_update)
				cameras[0].update_camera();
			glViewport(cameras[0].camera_viewport[0], cameras[0].camera_viewport[1], cameras[0].camera_viewport[2], cameras[0].camera_viewport[3]);
			glUseProgram(Shaders[0]);
			glUniformMatrix4fv(glGetUniformLocation(Shaders[0], "ProjectionMatrix"), 1, 1, cameras[0].ProjectionMatrix.x);
			glUniformMatrix4fv(glGetUniformLocation(Shaders[0], "ViewMatrix"), 1, 1, cameras[0].ViewMatrix.x);
			for (register unsigned long int i = 0; i < mesh_nums; i++)
			{
				if (meshes[i].needs_update)
					meshes[i].update_vbo();
				glUniformMatrix4fv(glGetUniformLocation(Shaders[0], "ModelMatrix"), 1, 1, meshes[i].model_matrix.x);
				glUniformMatrix4fv(glGetUniformLocation(Shaders[0], "NormalMatrix"), 1, 1, InverseTranspose(meshes[i].model_matrix).x);
				glBindVertexArray(meshes[i].VAO_ID);
				glDrawElements(GL_TRIANGLES, meshes[i].vert_nums, GL_UNSIGNED_INT, meshes[i].indices);
				glBindVertexArray(0);
			}
		}
		glFlush();
		SwapBuffers(hdc);
	}
	/******************************************************************/
	/******************************************************************/
	/**************************test functions**************************/
	/******************************************************************/
}