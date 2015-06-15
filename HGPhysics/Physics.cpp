#include "Physics.h"
namespace Physics_Engine
{
	void Physics::build(HG3D_Engine::Renderer *irenderer, bool *movable, long double *mass)//build the mesh
	{
		renderer_world = irenderer;
		object_nums = renderer_world->mesh_nums;//there will be an object for each mesh
		objects = (Object*)malloc(sizeof(Object)*object_nums);//allocate memmory
		for (register unsigned long int i = 0; i < object_nums; i++)
		{
			objects[i].build(&renderer_world->meshes[i], mass[i], i);//build object
			objects[i].Movable = movable[i];//copy the movable data
		}

		needs_update = 0;//doesnt need update
	}
	void Physics::apply_physics(long double Delta_Time)
	{
		for (register unsigned long int i = 0; i < object_nums; i++)
		{
			HG3D_Engine::point center;
			center.build(0.0f, 0.0f, 0.0f);
			objects[i].Position = objects[i].Object_Mesh->model_matrix*center;//objects[i].Mass_Center;//set the current position
			objects[i].Force.build(0.0f, 0.0f, 0.0f);
		}
		for (register unsigned long int i = 0; i < object_nums; i++)//each pair once
			for (register unsigned long int j = i + 1; j < object_nums; j++)
			{
				HG3D_Engine::vector Object_Object;
				Object_Object.build(objects[i].Position, objects[j].Position);//object to object vector
				long double r_P3 = pow(Object_Object.getsize(), long double(3.0));//power is 3 to normalize the vector too
				HG3D_Engine::vector GravitationalForce;
				GravitationalForce = Object_Object * (objects[i].Mass * objects[j].Mass * long double(G_Constant) / r_P3);//gravitational force
				objects[i].Force = objects[i].Force + GravitationalForce;//set the force
				objects[j].Force = objects[j].Force + GravitationalForce * -1.0f;
			}
		for (register unsigned long int i = 0; i < object_nums; i++)
		{
			objects[i].apply_physics(Delta_Time);//apply the physics
			HG3D_Engine::vector movement;
			movement.build(objects[i].LastPosition, objects[i].Position);//get the movement
			HG3D_Engine::_4x4matrix translate_mat;
			translate_mat.LoadTranslate(float(movement.x), float(movement.y), float(movement.z));//get the translate matrix
			objects[i].Object_Mesh->model_matrix = translate_mat * objects[i].Object_Mesh->model_matrix;//move the mesh
		}
	}
}