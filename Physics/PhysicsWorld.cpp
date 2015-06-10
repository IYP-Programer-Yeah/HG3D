#include "PhysicsWorld.h"

#define G  6.67384 //gravitational constant
#define GE -11 //gravitational constant exponent
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

		//IYP code

		//calculate gravitational force and add it to objects
		//TODO : do this on a multi threaded way
		for (unsigned long int i = 0; i < m_Objects.size(); i++)
			for (unsigned long int j = i; j < m_Objects.size(); j++)//this is optimised
			{
				vector gravitational_force;//final force containor
				vector object_object;//object to object vector
				object_object.build(m_Objects[i].m_Position, m_Objects[j].m_Position);//get the object to object vector
				long double r_P3 = pow(object_object.getsize(), 3.0);//power is 3 to normalize the vector too
				gravitational_force = object_object*(m_Objects[i].m_Mass*m_Objects[j].m_Mass*G*pow(10.0, GE) / r_P3);//calculate gravitational force
				m_Objects[i].AddForce(gravitational_force.x, gravitational_force.y, gravitational_force.z);//apply force
				m_Objects[j].AddForce(-gravitational_force.x, -gravitational_force.y, -gravitational_force.z);//apply force the second object gets the force in opposite direction  
			}
		//IYP code
		/*NOTE : use register unsigned long int not uint for 3 reasons :
		#1 uint is not register (slower loop) 
		#2 our ogject number may excced the range 
		#3 dont waste memory both uint and unsigned long int are the same size (32bit) */
		for (unsigned long int i = 0; i < m_Objects.size(); ++i)//TODO : #1 find the best number of threads for multi threading #2 make a class for threads #3 do updates on threads 
		{
			if (m_Objects[i].m_Moveable)
			{
				m_Objects[i].Update(dt);
			}
		}
	}
}
