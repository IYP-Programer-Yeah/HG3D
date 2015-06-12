#include "Object.h"


namespace Physics
{
	PhysicsObject::PhysicsObject()
	{
		//Initialize everything to defaults

		m_Position = nullptr;
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
		m_Last_Position = *m_Position;
		m_Last_Velocity = m_Velocity;

		m_Gravity.build(0.0f, -9.8f, 0.0f);

		//Initialize to 1 to avoid divide by zero accidentally
		m_Mass = 1.0;

		m_Moveable = true;
		m_ApplyGravity = true;

		m_LastFrameDataInitialized = false;
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

	void PhysicsObject::SetPositionPointer(point& PointerAddress)
	{
		m_Position = &PointerAddress;
	}
	
	point PhysicsObject::GetPosition()
	{
#ifdef NT_IS_DEBUGGING
		//make sure m_Position points to a valid address
		assert(m_Position);
#endif
		return *m_Position;
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

		//assuming that m_Position points to a valid location
		assert(m_Position);

		//double-check
		//We assume that mass is always greater than zero
		assert(m_Mass > 0);
#endif	

		//Force = Mass * Acceleration
		//Acceleration = Force / Mass 
		m_Acceleration = m_Force / m_Mass;
		
	
		//Equivalent to m_Velocity += m_Acceleration * dt;
		m_Velocity = m_Velocity + m_Acceleration * dt;


		//Initialize for first frame
		if (!m_LastFrameDataInitialized)
		{
			m_Last_dt = dt;
			m_Last_Position = *m_Position;
			m_Last_Velocity = m_Velocity;

			m_LastFrameDataInitialized = true;
		}

		if (m_ApplyGravity)
		{
			//Equivalent to m_Velocity += m_Gravity * dt;
			m_Velocity = m_Velocity + m_Gravity * dt;
		}

		//Update position with our physics code
		vector NewPos;
		NewPos = m_Velocity * dt;

	
		m_Position->x += NewPos.x;
		m_Position->y += NewPos.y;
		m_Position->z += NewPos.z;

		//=========== SAVE FOR NEXT FRAME ====================//

		//save last position
		m_Last_Position = *m_Position;

		//save last dt
		m_Last_dt = dt;

		//save the last velosity
		m_Last_Velocity = m_Velocity;
	}

	void PhysicsObject::SetMesh(Mesh& mesh)
	{
		if (!m_MeshPtr)
		{
			m_MeshPtr = &mesh;
		}
#ifdef NT_IS_DEBUGGING
		else
		{
			OutputDebugString("Warning: Setting the PhysicsObject::SetMesh() more than one time.\n");
		}
#endif
	}

	void PhysicsObject::CalculateCollisionShapes()
	{
		point Min;
		point Max;

		point SphereCenter;
		SphereCenter.build(0.0f, 0.0f, 0.0f);

		const static float Infinite = FLT_MAX;

		Min.build(Infinite, Infinite, Infinite);
		Max.build(-Infinite, -Infinite, -Infinite);

		point vMin;
		point vMax;

		vMin = Min;
		vMax = Max;

		vertex* vertices = m_MeshPtr->verts;

		for (register unsigned long int i = 0; i < m_MeshPtr->vert_nums; ++i)
		{

			SphereCenter.x += vertices[i].x;
			SphereCenter.y += vertices[i].y;
			SphereCenter.z += vertices[i].z;

			//calculate the farthest vertex
			vMax.x = (vMax.x > vertices[i].x) ? vMax.x : vertices[i].x;
			vMax.y = (vMax.y > vertices[i].y) ? vMax.x : vertices[i].y;
			vMax.z = (vMax.z > vertices[i].z) ? vMax.x : vertices[i].z;

			//calculate the nearest vertex
			vMin.x = (vMin.x < vertices[i].x) ? vMin.x : vertices[i].x;
			vMin.y = (vMin.y < vertices[i].y) ? vMin.y : vertices[i].y;
			vMin.z = (vMin.z < vertices[i].z) ? vMin.z : vertices[i].z;

		}

		SphereCenter.x /= m_MeshPtr->vert_nums;
		SphereCenter.y /= m_MeshPtr->vert_nums;
		SphereCenter.z /= m_MeshPtr->vert_nums;
		
		//Now we know the mesh's approximate center for the sphere collision shape
		m_ColSphere.Center = SphereCenter;


		//Calculate AABB's final values
		m_ColBox.Center.x = (vMin.x + vMax.x) * 0.5;
		m_ColBox.Center.y = (vMin.y + vMax.y) * 0.5;
		m_ColBox.Center.z = (vMin.z + vMax.z) * 0.5;

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