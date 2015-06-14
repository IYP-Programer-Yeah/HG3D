#include "Physics.h"
namespace Physics_Engine
{
	void Object::operator =(Object input)//copy data except the pointer
	{
		Mass = input.Mass;
		Velocity = input.Velocity;
		Force = input.Force;
		Acceleration = input.Acceleration;
		Last_Delta_Time = input.Last_Delta_Time;

		Movable = input.Movable;
		needs_update = input.needs_update;

		Position = input.Position;
		LastPosition = input.LastPosition;

		Mesh_ID = input.Mesh_ID;

	}
	void Object::build(HG3D_Engine::Mesh *imesh, long double iMass, unsigned long int iMesh_ID)//build the object
	{
		Object_Mesh = imesh;
		Mass = iMass;
		HG3D_Engine::vector temp_vector[2];//temp variable for calculation
		for (register unsigned long int i = 0; i < Object_Mesh->vert_nums; i++)
		{
			temp_vector[0].build(Object_Mesh->verts[i].x, Object_Mesh->verts[i].y, Object_Mesh->verts[i].z);//current vertex's vector
			temp_vector[1] = temp_vector[1] + temp_vector[0];//some point if the model for mass center calculation
		}
		temp_vector[1] = temp_vector[1] / long double(Object_Mesh->vert_nums);//find the mass center
		Mass_Center.build(temp_vector[1].x, temp_vector[1].y, temp_vector[1].z);//build the mass center
		needs_update = 0;//just made every thing there is no need for update

		Velocity.build(0.0f, 0.0f, 0.0f);//initialize to 0
		Force.build(0.0f, 0.0f, 0.0f);
		Acceleration.build(0.0f, 0.0f, 0.0f);

		Mesh_ID=iMesh_ID;//set the mesh ID sent from the physics class
		Movable = 1;
	}
	void Object::apply_physics(long double Delta_Time)
	{
		if (Movable)
		{
			LastPosition = Position;
			Last_Delta_Time = Delta_Time;

			Acceleration = Force / Mass;//calculate acceleration
			
			Velocity = Velocity + Acceleration*Delta_Time;//calculate the speed
			Position.build(Position.x + Velocity.x*Delta_Time, Position.y + Velocity.y*Delta_Time, Position.z + Velocity.z*Delta_Time);//calculate new position
		}
		else
		{
			Last_Delta_Time = Delta_Time;
			LastPosition = Position;
			Velocity.build(0.0f, 0.0f, 0.0f);//no speed for this
		}
	}
	void Object::check_data()
	{
		needs_update = (Object_Mesh->needs_update || Object_Mesh->subdata_changed);
	}
	void Object::update()
	{
		needs_update = 0;
	}

}