#include <Windows.h>
#include <gl/glew.h>
#include <gl/gl.h> 
#include <gl/glu.h>
#include <math.h>

// load GL libs
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

#ifndef HG3D_ENG  //check if included before
#define HG3D_ENG


#define Vert_Pos_Size      12                                //size of positions
#define Vert_Normal_Size   12                                //size of normals
#define Vert_Coord_Size    8                                 //size of coord
#define vertex_size        32                                //vertex size
#define face_size          12								 //face size

namespace HG3D_Engine
{
	//math libs
	class point //point class
	{
	public:
		long double x, y, z;
		void __declspec(dllexport) __fastcall build(long double ix, long double iy, long double iz); //build point
		void __declspec(dllexport) __fastcall build(float ix, float iy, float iz); //build point with float (idiot nvidia)
		void __declspec(dllexport) __fastcall move(long double ax = long double(0.0), long double ay = long double(0.0), long double az = long double(0.0)); //move point
		void __declspec(dllexport) __fastcall rotate(point from, long double ax = long double(0.0), long double ay = long double(0.0), long double az = long double(0.0)); //rotate point
		void __declspec(dllexport) __fastcall move(float ax = 0.0f, float ay = 0.0f, float az = 0.0f); //move point float
		void __declspec(dllexport) __fastcall rotate(point from, float ax = 0.0f, float ay = 0.0f, float az = 0.0f); //rotate point float
		void __declspec(dllexport) __fastcall operator =(point intery); //operator =
	};
	//end of point class
	class vector //3d vector class
	{
	public:
		long double x, y, z;
		void __fastcall build(point start, point end);//point to point vector
		void __fastcall build(long double ix, long double iy, long double iz);//simpile build
		void __fastcall build(float ix, float iy, float iz);//simpile build with float
		void __fastcall operator =(vector intery);//set value
		vector __fastcall operator +(vector entry);//vector vector sum
		vector __fastcall operator *(long double entry);//vector number multipliction
		vector __fastcall operator /(long double entry);//vector number division
		vector __fastcall operator *(float entry);//vector number multipliction
		vector __fastcall operator /(float entry);//vector number division
		long double __fastcall getsize();//get vector size
	};
	//end of 3d vector class
	long double __fastcall dot(vector v1, vector v2); //dot product

	vector __fastcall cross(vector v1, vector v2); //cross product

	vector __fastcall normalize(vector v1); //normalize

	class _4x4matrix //4*4 matrix
	{
	public:
		float x[16];
		_4x4matrix __declspec(dllexport) __fastcall operator * (_4x4matrix input);//mat4 multiply
		void __declspec(dllexport) __fastcall operator = (_4x4matrix input);//=
		void __declspec(dllexport) __fastcall LoadIdentity();//load matric with i
		void __declspec(dllexport) __fastcall LoadZero();//load matric with 0
		void __declspec(dllexport) __fastcall LoadScaler(float sx, float sy, float sz);//load matric with 0
	};
	//end of rotation class
	_4x4matrix LookAt(point cam_pos,vector forward,vector up);//get view mat

	_4x4matrix Projection(float left, float right, float buttom, float top, float nearz, float farz);//projection matrix
	
	//end of math libs

	//mesh libs
	class face //face class
	{
	public:
		unsigned long int index[3];   //face indicies
	};
	//end of face class
	class vertex //vertex class
	{
	public:
		float x, y, z;       //positions
		float nx, ny, nz;    //normals
		float cx, cy;        //coords
	};
	//end of vertex class
	class Mesh//mesh class
	{
	public:
		unsigned long int vert_nums;      //number of verts
		unsigned long int faces_nums;     //number of faces
		unsigned long int total_size;     //total size of the verts data

		unsigned long int *indecies;      //vert indecies

		GLuint text_ID_diff;              //diffuse texture ID
		GLuint text_ID_spec;              //specular texture ID
		GLuint text_ID_normal;            //normal map texure ID
		GLuint text_ID_height;            //height map texture ID
		GLuint text_ID_mask;              //mask texture ID

		bool have_diff_text;              //have diffuse texture? 
		bool have_spec_text;              //have specular texture? 
		bool have_NM_text;                //have normals map texture? 
		bool have_HM_text;                //have height map texture? 
		bool have_mask_text;              //have mask texture? 
		bool hidden;                      //is mesh hidden?
		bool clear_last_buff;             //clear last buffer in updat?
		bool needs_update;                //mesh needs an update

		point mesh_cube[8];               //mesh's container cube
		point mesh_center;                //mesh-cube's center

		_4x4matrix model_matrix;          //model matrix will be sent to shader

		face *faces;                      //mesh faces
		vertex *verts;                    //mesh verts

		GLuint VAO_ID;                    //mesh vao ID
		GLuint VBO_ID;                    //mesh vbo ID

		void __declspec(dllexport) load_mesh(char *path);       //.obj mesh loader 3 verts for each face
		void __declspec(dllexport) load_optimized(char *path);  //.obj mesh loader reuse the verts (time consumeing process)
		void __declspec(dllexport) free_mesh();                 //free the mesh memory

		void __declspec(dllexport) update_vbo();				//update mesh vbo

	};
	//end of mesh class
	//end of mesh libs

	//renderer libs
	class camera//camera class
	{
	public:
		_4x4matrix ViewMatrix;//modelview matrix
		_4x4matrix ProjectionMatrix;//projection

		float Left, Right, Buttom, Top, Near, Far;//projection property
		float camera_viewpost[4];//view port property

		point camer_position;//camera position

		vector forward;//forwar vector
		vector up;//up vector

		bool needs_update;//camera needs an update

		void __declspec(dllexport) update_camra();//update camera

	};
	//end of camera class
	class Renderer //renderer class
	{
	public:
		HDC hdc;								//device context

		HGLRC hrc;								//render condext

		Mesh *meshes;							//meshes

		camera *cameras;						//allow sveral cameras

		unsigned long int current_camera_nums;	//current used camera
		unsigned long int cameras_nums;			//number of cameras
		unsigned long int mesh_nums;			//number of meshs
		unsigned long int vert_nums;			//total number of verts
		unsigned long int total_size;			//total size of the verts data
		unsigned long int rendere_ID;			//will be initialized in init

		unsigned long int *mesh_draw_order;		//the order in which meshs are drawn
		unsigned long int *current_cameras;		//current used camera

		int __declspec(dllexport) add_camra();	//add a camera
		int __declspec(dllexport) add_mesh();	//add a mesh

		void __declspec(dllexport) init();		//initialize function
		void __declspec(dllexport) render();	//render the scene using current camera



		/******************************************************************/
		/**************************test console****************************/
		/******************************************************************/
		/******************************************************************/
		HANDLE myConsoleHandle;
		DWORD cCharsWritten;
		/******************************************************************/
		/******************************************************************/
		/**************************test console****************************/
		/******************************************************************/
	};
	//end of renderer class
	//end of renderer libs
}
#endif