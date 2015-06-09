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
	

		void DLLEXPORT Update(const long double& Deltatime);
	public:
		bool m_Moveable;
		point m_Position;
		point m_Last_Position;

		vector m_Force;
		vector m_Velocity;
		vector m_Last_Velocity;
		vector m_Acceleration;

		long double m_Mass;
		long double m_Last_dt;
	};
}