#include "..\HG3D Engine\HG3D Engine.h"
#ifndef PHYSICS_ENG
#define PHYSICS_ENG
#define G_Constant				0.0000000000667384
namespace Physics_Engine
{
	//the object class
	class Object
	{
	public:
		long double Mass;//physical property
		long double Last_Delta_Time;

		HG3D_Engine::vector Velocity;
		HG3D_Engine::vector Force;
		HG3D_Engine::vector Acceleration;
		

		bool Movable;//is it influenced by physics
		bool needs_update;//does it need update

		HG3D_Engine::point Position;
		HG3D_Engine::point LastPosition;//last position is needed for calculation
		HG3D_Engine::point Mass_Center;//the mass center calculated at build

		HG3D_Engine::Mesh *Object_Mesh;//pointer to the mesh

		unsigned long int Mesh_ID;//the mesh's ID in the rendrer

		void __declspec(dllexport) operator =(Object input);//clone the mesh data
		void __declspec(dllexport) build(HG3D_Engine::Mesh *imesh, long double iMass, unsigned long int iMesh_ID);//build object from the mesh
		void __declspec(dllexport) apply_physics(long double Delta_Time);//build the mesh
		void __declspec(dllexport) check_data();//checkand see if data is up to date
		void __declspec(dllexport) update();//update the project

	};
	//end of object class
	//physics class
	class Physics
	{
	public:
		Object *objects;//the objects

		unsigned long int object_nums;//number of objects

		bool needs_update;//do objects need update

		HG3D_Engine::Renderer *renderer_world;// a pointer to rendrer's world

		void __declspec(dllexport) build(HG3D_Engine::Renderer *irenderer, bool *movable, long double *mass);//build the mesh
		void __declspec(dllexport) apply_physics(long double Delta_Time);//apply physics on the objects
		void __declspec(dllexport) update();//update data reagarding to engine change

	};
	//end of physics class
}
#endif