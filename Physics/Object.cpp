#include "Object.h"


namespace Physics
{
	Object::Object()
	{
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
	}

	Object::~Object()
	{

	}
}