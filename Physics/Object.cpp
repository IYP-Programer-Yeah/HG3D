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

		m_Gravity.build(0.0f, -9.8f, 0.0f);

		//Initialize to 1 to avoid divide by zero accidentally
		m_Mass = 1.0;

		m_Moveable = true;
		m_ApplyGravity = true;
	}

	PhysicsObject::~PhysicsObject()
	{

	}


	void PhysicsObject::Update(const long double& dt)
	{
		//Force = Mass * Acceleration
		//Acceleration = Force / Mass 
		m_Acceleration = m_Force / m_Mass;


		//Equivalent to m_Velocity += m_Acceleration * dt;
		m_Velocity = m_Velocity + m_Acceleration * dt;

		if (m_ApplyGravity)
		{
			//Equivalent to m_Velocity += m_Gravity * dt;
			m_Velocity = m_Velocity + m_Gravity * dt;
		}

		vector NewPos;
		NewPos = m_Velocity * dt;

		m_Position.x += NewPos.x;
		m_Position.y += NewPos.y;
		m_Position.z += NewPos.z;


	}
}