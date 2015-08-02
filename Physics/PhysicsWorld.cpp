#include "PhysicsWorld.h"


namespace Physics
{
	PhysicsWorld::PhysicsWorld()
	{
		
	}

	PhysicsWorld::~PhysicsWorld()
	{
	}

	void PhysicsWorld::AddObject(const std::string Name, PhysicsObject& Object, Mesh& mesh)
	{

		Object.SetMesh(mesh);
		Object.CalculateCollisionShapes();

		m_Objects.push_back(Object);
		m_ObjectNames.push_back(Name);
	}

	void PhysicsWorld::RemoveLastObject()
	{
		m_Objects.pop_back();
	}

	void PhysicsWorld::LoadWorld(Renderer* renderer, float* Masses, std::string* Names,
		unsigned long int* MeshIDs, unsigned long int NumMeshes)
	{
#ifdef NT_IS_DEBUGGING
		if (renderer->mesh_nums < 1)
		{
			OutputDebugString("Something is wrong. Invalid data is being passed in PhysicsWorld::LoadWorld() function.\n");
			return;
		}

		if (!m_Objects.empty() || !m_ObjectNames.empty())
		{
			OutputDebugString("Warning: m_Objects or m_ObjectNames std::vector is not empty.");
			return;
		}


#endif

		//We need 'NumMeshes' instead of renderer->mesh_nums because not all meshes will use physics
		for (register unsigned long int i = 0; i < NumMeshes; ++i)
		{
			unsigned long int ID = MeshIDs[i];

			Mesh& CurrentMesh = renderer->meshes[ID];

			PhysicsObject Temp;

			Temp.SetMesh(CurrentMesh);
			Temp.CalculateCollisionShapes();
			Temp.SetMass(Masses[i]);

			point Pos = CurrentMesh.model_matrix * Temp.m_ColSphere.Center;

#ifdef NT_IS_DEBUGGING
			char s[256];
			sprintf_s(s, "%s Mass: %.2f, Pos: %.2f %.2f %.2f\n", Names[i].c_str(), Masses[i], Pos.x, Pos.y, Pos.z);
			
			OutputDebugString(s);
#endif
			Temp.SetPosition(Pos.x, Pos.y, Pos.z);

			Temp.m_MeshID = ID;

			m_ObjectNames.push_back(Names[i]);
			m_Objects.push_back(Temp);
		}

		m_RendrerPtr = renderer;
	}

	PhysicsObject* PhysicsWorld::GetPhysicsObject(const std::string Name)
	{
#ifdef NT_IS_DEBUGGING
		if (m_Objects.empty() || (m_Objects.size() != m_ObjectNames.size()))
		{
			OutputDebugString("Come here, something is wrong. Don't ask me now how to come here.\n");
		}
#endif

		for (register unsigned long int i = 0; i < m_Objects.size(); ++i)
		{
			if (m_ObjectNames[i] == Name)
			{
				return &m_Objects[i];
			}
		}

		return nullptr;
	}


	PhysicsObject* PhysicsWorld::GetPhysicsObject(unsigned long int Index)
	{
#ifdef NT_IS_DEBUGGING
		if (Index > m_Objects.size() || m_Objects.empty())
		{
			OutputDebugString("Come here, something is wrong.\n");
		}
#endif

		return &m_Objects[Index];
	}


	void PhysicsWorld::UpdateCollision()
	{

		for (register unsigned long int i = 0; i < m_Objects.size(); ++i)
		{
			PhysicsObject& CurrentObject = m_Objects[i];

			if (!CurrentObject.m_DoCollision)
				continue;

			CollisionSphere CurrentObjSphere = CurrentObject.m_ColSphere;

			CurrentObjSphere.Center = CurrentObject.m_MeshPtr->model_matrix * CurrentObject.m_ColSphere.Center;

			//Do collision of this object with all other objects (but not itself)
			for (register unsigned long int j = 0; j < m_Objects.size(); ++j)
			{
				//Are we calculating collision of this object with itself? OMG? What the hell were we doing?
				if (CurrentObject.m_UniqueObjectID == m_Objects[j].m_UniqueObjectID)
					continue;

				if (!m_Objects[j].m_DoCollision)
					continue;

				CollisionSphere OtherObjSphere = m_Objects[j].m_ColSphere;


				OtherObjSphere.Center = m_Objects[j].m_MeshPtr->model_matrix * m_Objects[j].m_ColSphere.Center;

				//========= SOMETHING INCOMPLETE HERE ==============/

				//Scale radius accordingly to the mesh

				//==================================================/
			
				if (CurrentObjSphere.CheckCollisionWith(OtherObjSphere))
				{
#ifdef NT_IS_DEBUGGING
					char s[256];
					sprintf_s(s, "Object (%d) has collided with Object (%d)", CurrentObject.m_UniqueObjectID, m_Objects[j].m_UniqueObjectID); 

					OutputDebugString(s);
#endif
				}

			}
		}
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
		for (register unsigned long int i = 0; i < m_Objects.size(); ++i)
		{
			m_Objects[i].SetForce(0.0, 0.0, 0.0);

			point TempPos;

			TempPos = m_RendrerPtr->meshes[m_Objects[i].m_MeshID].model_matrix  * m_Objects[i].m_ColSphere.Center;

			m_Objects[i].SetPosition(TempPos.x, TempPos.y, TempPos.z);
		}
			 
		//Calculate gravitational force and add it to objects

		//TODO : do this on a multi threaded way
		for (register unsigned long int i = 0; i < m_Objects.size(); ++i)
		{
			for (register unsigned long int j = i + 1; j < m_Objects.size(); ++j)
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
				GravitationalForce = Object_Object * (m_Objects[i].m_Mass * m_Objects[j].m_Mass * G / r_P3);

				//Apply force
				m_Objects[i].AddForce(GravitationalForce.x, GravitationalForce.y, GravitationalForce.z);

				//Apply force the second object gets the force in opposite direction  
				m_Objects[j].AddForce(-GravitationalForce.x, -GravitationalForce.y, -GravitationalForce.z);
			}
			
			
		}
		

		for (register unsigned long int i = 0; i < m_Objects.size(); ++i)
		{
			if (m_Objects[i].m_Moveable)
			{
				m_Objects[i].Update(dt);
			}
		}

	}
	
}
