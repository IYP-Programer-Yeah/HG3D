#include "PhysicsWorld.h"


namespace Physics
{
	PhysicsWorld::PhysicsWorld()
	{
		
	}

	PhysicsWorld::~PhysicsWorld()
	{
	}

	void PhysicsWorld::AddObject(PhysicsObject& Object, Mesh& mesh)
	{
		m_Objects.push_back(Object);

		m_Objects[m_Objects.size() - 1].SetMesh(mesh);
		m_Objects[m_Objects.size() - 1].CalculateCollisionShapes();
	}

	void PhysicsWorld::RemoveLastObject()
	{
		m_Objects.pop_back();
	}

	PhysicsObject& PhysicsWorld::GetPhysicsObject(UINT Index)
	{
#ifdef NT_IS_DEBUGGING
		if (Index > m_Objects.size() || m_Objects.empty())
		{
			OutputDebugString("Come here, something is wrong.");
		}
#endif

		return m_Objects[Index];
	}

	void PhysicsWorld::Update(const long double& dt)
	{
#ifdef NT_IS_DEBUGGING
		if (m_Objects.empty())
		{
			OutputDebugString("PhysicsWorld::m_Objects is empty.");

			return;
		}
#endif
		for (register unsigned long int i = 0; i < m_Objects.size(); i++)
		{
			if (m_Objects[i].m_MeshPtr_is_valid)//the pointer is valid
			{
				m_Objects[i].SetPositionPointer(m_RendererPtr->meshes[m_Objects[i].m_Mesh_ID].model_matrix*m_Objects[i].m_ColSphere.Center);//set the position
			}
			else//the pointer is not valid any more
			{
				m_Objects[i].SetMesh(m_RendererPtr->meshes[m_Objects[i].m_Mesh_ID]);//set the mesh adress
				m_Objects[i].SetPositionPointer(m_RendererPtr->meshes[m_Objects[i].m_Mesh_ID].model_matrix*m_Objects[i].m_ColSphere.Center);//set the position
				m_Objects[i].m_MeshPtr_is_valid = true;//the pointer is valid now
			}
			if(m_RendererPtr->meshes[m_Objects[i].m_Mesh_ID].needs_update)//mesh data must have been changed we need to update our collision shapes
				m_Objects[i].CalculateCollisionShapes();
		}
		//Calculate gravitational force and add it to objects

		//TODO : do this on a multi threaded way
		for (register unsigned long int i = 0; i < m_Objects.size(); i++)
		{
			for (register unsigned long int j = i; j < m_Objects.size(); j++)
			{
				//Final force containor
				vector GravitationalForce;

				//Object to object vector
				vector Object_Object;

				//Get the object to object vector
				point a = m_Objects[i].GetPosition();
				point b = m_Objects[j].GetPosition();
				Object_Object.build(a, b);

				//Power is 3 to normalize the vector too
				long double r_P3 = pow(Object_Object.getsize(), 3.0);

				//Calculate gravitational force
				GravitationalForce = Object_Object * (m_Objects[i].m_Mass * m_Objects[j].m_Mass * G *pow(10.0, GE) / r_P3);

				//Apply force
				m_Objects[i].AddForce(GravitationalForce.x, GravitationalForce.y, GravitationalForce.z);

				//Apply force the second object gets the force in opposite direction  
				m_Objects[j].AddForce(-GravitationalForce.x, -GravitationalForce.y, -GravitationalForce.z);
			}

			
		}

		//TODO : #1 find the best number of threads for multi threading 
		//       #2 make a class for threads #3 do updates on threads 

		for (register unsigned long int i = 0; i < m_Objects.size(); ++i)
		{
			if (m_Objects[i].m_Moveable)
			{
				m_Objects[i].Update(dt);
				vector movement;
				movement.build(m_Objects[i].m_Last_Position, m_Objects[i].GetPosition());
				_4x4matrix movemet_matrix;//the translate matrix
				movemet_matrix.LoadTranslate(float(movement.x), float(movement.y), float(movement.z));//make the translate matrix
				m_RendererPtr->meshes[m_Objects[i].m_Mesh_ID].model_matrix = movemet_matrix* m_RendererPtr->meshes[m_Objects[i].m_Mesh_ID].model_matrix;//tramslate the model
			}
		}

	}
	void PhysicsWorld::Load_World(Renderer *world, long double *Masses)
	{ 
		m_RendererPtr = world;
		PhysicsObject temp_object;
		for (register unsigned long int i = 0; i < world->mesh_nums; i++)
		{
			temp_object.SetMass(Masses[i]);//set the damned mass
			temp_object.m_MeshPtr_is_valid = true;//now its valid
			temp_object.m_Moveable = 1;
			temp_object.m_Mesh_ID = i;
			AddObject(temp_object, world->meshes[i]);//add the object
		}
	}
}
