#include "Object.h"
#include <iostream>

namespace Physics
{
	PhysicsObject::PhysicsObject()
	{
		//Initialize everything to defaults

		static unsigned long int lolID = 0;

		lolID++;

		m_UniqueObjectID = lolID;

#ifdef NT_IS_DEBUGGING
		char s[256];
		sprintf_s(s, "Unique ID of Physics Object: %d\n", m_UniqueObjectID);
		
		OutputDebugString(s);
#endif

		m_MeshPtr = nullptr;

		m_Acceleration.x = 0.0;
		m_Acceleration.y = 0.0;
		m_Acceleration.z = 0.0;

		m_Velocity.x = 0.0;
		m_Velocity.y = 0.0;
		m_Velocity.z = 0.0;

		m_Force.x = 0.0;
		m_Force.y = 0.0;
		m_Force.z = 0.0;

		//We DO need to initialize this... 
		m_Last_dt = 0.0;
		m_Last_Position.build(0.0f,0.0f,0.0f);
		m_Last_Velocity = m_Velocity;

		m_Gravity.build(0.0f, -9.8f, 0.0f);

		//Initialize to 1 to avoid divide by zero accidentally
		m_Mass = 1.0;

		m_Moveable = true;
		m_ApplyGravity = false;
		m_Valid = true;

		m_LastFrameDataInitialized = false;
		m_MeshID = 0;
	}

	PhysicsObject::~PhysicsObject()
	{

	}

	//This is AddForce, not SetForce
	//So, Something like:
	//AddForce(0.0f, 1.0f, 0.0f);
	//AddForce(0.0f, 2.0f, 0.0f);
	//is equivalent to
	//SetForce(0.0f, 3.0f, 0.0f); 

	void PhysicsObject::AddForce(long double x, long double y, long double z)
	{
		m_Force.x += x;
		m_Force.y += y;
		m_Force.z += z;
	}

	
	point PhysicsObject::GetPosition()
	{
		return m_Position;
	}

	//Vector over loaded
	void PhysicsObject::AddForce(const vector& Force)
	{
		m_Force = m_Force + Force;
	}

	//This will overwrite anything done by AddForce() function
	void PhysicsObject::SetForce(long double x, long double y, long double z)
	{
		m_Force.build(x, y, z);
	}

	//Vector over loaded
	void PhysicsObject::SetForce(const vector& Force)
	{
		m_Force = Force;
	}

	void PhysicsObject::SetMass(long double Mass)
	{
#ifdef NT_IS_DEBUGGING
		//avoid divide by zero
		if (Mass == 0.0)
		{
			OutputDebugString("Mass cannot be null, you idiot.");
			return;
		}
#endif

		m_Mass = Mass;
	}

	//If gravity need's to be changed for this object
	//Gravity is enabled by default at 9.8 m/s in negative y-axis

	void PhysicsObject::SetGravity(long double x, long double y, long double z)
	{
		m_Gravity.build(x, y, z);
	}

	void PhysicsObject::Update(const long double& dt)
	{
#ifdef NT_IS_DEBUGGING
		//double-check
		//We assume that mass is always greater than zero
		assert(m_Mass > 0);

#endif	

		//Force = Mass * Acceleration
		//Acceleration = Force / Mass 
		m_Acceleration = m_Force / m_Mass;
		
		if (m_ApplyGravity)
		{
			//Equivalent to m_Acceleration += m_Gravity;
			m_Acceleration = m_Acceleration + m_Gravity;
		}

		//Equivalent to m_Velocity += m_Acceleration * dt;
		m_Velocity = m_Velocity + m_Acceleration * dt;


		//Initialize for first frame
		if (!m_LastFrameDataInitialized)
		{
			m_Last_dt = dt;
			m_Last_Position = m_Position;
			m_Last_Velocity = m_Velocity;

			m_LastFrameDataInitialized = true;
		}

		//Update position with our physics code
		vector NewPos;
		NewPos = m_Velocity * dt;

		m_Position.x += NewPos.x;
		m_Position.y += NewPos.y;
		m_Position.z += NewPos.z;

#ifdef NT_IS_DEBUGGING
		char s[256];

		sprintf_s(s, "X: %.2f Y: %.2f Z: %.2f\n", (float)m_Position.x, (float)m_Position.y, (float)m_Position.z);
		OutputDebugString(s);

#endif

		vector movement;
		movement.build(m_Last_Position, m_Position);
		m_MeshPtr->move(movement);

		//=========== SAVE FOR NEXT FRAME ====================//

		//save last position
		m_Last_Position = m_Position;

		//save last dt
		m_Last_dt = dt;

		//save the last velosity
		m_Last_Velocity = m_Velocity;

		//====================================================//
	}

	void PhysicsObject::SetVelocity(long double x, long double y, long double z)
	{
		m_Velocity.build(x, y, z);
	}

	void PhysicsObject::SetVelocity(const vector& vec)
	{
		m_Velocity = vec;
	}

	void PhysicsObject::AddVelocity(long double x, long double y, long double z)
	{
		m_Velocity.x += x;
		m_Velocity.y += y;
		m_Velocity.z += z;
	}

	void PhysicsObject::AddVelocity(const vector& vel)
	{
		m_Velocity.x += vel.x;
		m_Velocity.y += vel.y;
		m_Velocity.z += vel.z;
	}

	void PhysicsObject::SetPosition(long double x, long double y, long double z)
	{
		m_Position.build(x, y, z);
	}
	void PhysicsObject::SetMesh(Mesh& mesh)
	{
		m_MeshPtr = &mesh;
	}

	void PhysicsObject::CalculateCollisionShapes()
	{
		if (!m_MeshPtr)
			return;

		point SphereCenter;
		SphereCenter.build(0.0f, 0.0f, 0.0f);

		point vMin;
		point vMax;

		vMin = m_MeshPtr->mesh_cube[0];
		vMax = m_MeshPtr->mesh_cube[7];

		vertex* vertices = m_MeshPtr->verts;

		for (register unsigned long int i = 0; i < m_MeshPtr->vert_nums; ++i)
		{
			SphereCenter.x += vertices[i].x;
			SphereCenter.y += vertices[i].y;
			SphereCenter.z += vertices[i].z;
		}

		SphereCenter.x /= m_MeshPtr->vert_nums;
		SphereCenter.y /= m_MeshPtr->vert_nums;
		SphereCenter.z /= m_MeshPtr->vert_nums;
		
		//Now we know the mesh's approximate center for the sphere collision shape
		m_ColSphere.Center = SphereCenter;


		//Calculate AABB's final values
		m_ColBox.Center = m_MeshPtr->mesh_center;

		m_ColBox.Extends.x = (vMax.x - vMin.x) * 0.5;
		m_ColBox.Extends.y = (vMax.y - vMin.y) * 0.5;
		m_ColBox.Extends.z = (vMax.z - vMin.z) * 0.5;


		//Calculate the radius of sphere

		float PreviousRadius = 0;
		float FinalRadius = 0;
		float CurrentRadius = 0;

		point VertexPos;
		vector DistanceVector;

		for (register unsigned long int i = 0; i < m_MeshPtr->vert_nums; ++i)
		{
			//for each vertex in the mesh
			VertexPos.build(vertices[i].x, vertices[i].y, vertices[i].z);

			//calculate the distance from the sphere center to the current vertex
			DistanceVector.x = SphereCenter.x - VertexPos.x;
			DistanceVector.y = SphereCenter.y - VertexPos.y;
			DistanceVector.z = SphereCenter.z - VertexPos.z;

			//Use squared distance for comparision to avoid the square root function
			CurrentRadius = static_cast<float>(DistanceVector.getsizeSq());

			FinalRadius = (CurrentRadius > PreviousRadius) ? CurrentRadius : PreviousRadius;

			//SAVE for NEXT LOOP
			PreviousRadius = CurrentRadius;

		}

		//Save the final value of sphere's radius which is the greatest value
		m_ColSphere.Radius = FinalRadius;

 	}
}