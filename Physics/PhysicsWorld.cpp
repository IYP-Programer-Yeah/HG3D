#include "PhysicsWorld.h"

namespace Physics
{
	PhysicsWorld::PhysicsWorld()
	{
		
	}

	PhysicsWorld::~PhysicsWorld()
	{
	}

	void PhysicsWorld::AddObject(PhysicsObject& Object)
	{
		m_Objects.push_back(Object);
	}

	void PhysicsWorld::RemoveLastObject()
	{
		m_Objects.pop_back();
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
		for (UINT i = 0; i < m_Objects.size(); ++i)
		{
			if (m_Objects[i].m_Moveable)
			{

			}
		}
	}
}
