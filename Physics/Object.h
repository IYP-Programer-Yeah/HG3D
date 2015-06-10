#include "BasicIncludes.h"
#include "..\HG3D Engine\HG3D Engine.h"

using namespace HG3D_Engine;

namespace Physics
{
	class PhysicsObject
	{
	public:
		PhysicsObject();
		~PhysicsObject();
	
		//This function is to be called by a PhysicsWorld
		//Not available to user
		void Update(const long double& Deltatime);

		//Set Object's properties
		void DLLEXPORT AddForce(float x, float y, float z);
		void DLLEXPORT SetForce(float x, float y, float z);
		void DLLEXPORT SetGravity(float x, float y, float z);
		void DLLEXPORT SetMass(long double Mass);

	public:
		bool m_ApplyGravity;
		bool m_Moveable;

		point m_Position;
		point m_Last_Position;

		vector m_Velocity;
		vector m_Last_Velocity;
		vector m_Acceleration;

		long double m_Last_dt;

	private:
		bool m_LastFrameDataInitialized;

		//This stuff is now set by it's functions
		vector m_Force;
		vector m_Gravity;

		long double m_Mass;
	};
}