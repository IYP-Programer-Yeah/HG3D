#include "Object.h"


namespace Physics
{
	PhysicsObject::PhysicsObject()
	{
		//Initialize everything to defaults

		m_Position.x = 0.0;
		m_Position.y = 0.0;
		m_Position.z = 0.0;

		m_Acceleration.x = 0.0;
		m_Acceleration.y = 0.0;
		m_Acceleration.z = 0.0;

		m_Velocity.x = 0.0;
		m_Velocity.y = 0.0;
		m_Velocity.z = 0.0;

		m_Force.x = 0.0;
		m_Force.y = 0.0;
		m_Force.z = 0.0;
		//dont initialize the m_last_** values its waste of time
		//Initialize to 1 to avoid divide by zero accidentally
		m_Mass = 1.0;

		m_Moveable = true;
	}

	PhysicsObject::~PhysicsObject()
	{

	}


	void PhysicsObject::Update(const long double& dt)
	{
		m_Last_dt = dt;//save last dt
		//Force = Mass * Acceleration
		//Acceleration = Force / Mass 
		m_Acceleration = m_Force / m_Mass;
		
		m_Last_Velocity = m_Velocity;//save the last velosity
		//Equivalent to m_Velocity += m_Acceleration * dt;
		m_Velocity = m_Velocity + m_Acceleration * dt;

		vector NewPos;
		NewPos = m_Velocity * dt;

		m_Last_Position = m_Position;//save last position

		m_Position.x += NewPos.x;
		m_Position.y += NewPos.y;
		m_Position.z += NewPos.z;


	}
}