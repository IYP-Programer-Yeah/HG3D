#include "Object.h"


namespace Physics
{
	PhysicsObject::PhysicsObject()
	{
		//Initialize everything to defaults

		m_Position = nullptr;

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
}