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
		bool m_ApplyGravity;

		point m_Position;

		vector m_Force;
		vector m_Velocity;
		vector m_Acceleration;
		vector m_Gravity;

		long double m_Mass;
	};
}