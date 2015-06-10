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
		//IYP code
		//changed floats to long double
		//added vector over loads
		//Set Object's properties
		void DLLEXPORT AddForce(long double x, long double y, long double z);
		void DLLEXPORT SetForce(long double x, long double y, long double z);
		void DLLEXPORT AddForce(vector Force);
		void DLLEXPORT SetForce(vector Force);
		void DLLEXPORT SetGravity(long double x, long double y, long double z);
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
		//IYP code
		long double m_Mass;//mas is used every where dont make it private

	private:
		bool m_LastFrameDataInitialized;

		//This stuff is now set by it's functions
		vector m_Force;
		vector m_Gravity;

	};
}