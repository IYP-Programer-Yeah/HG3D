#include "BasicIncludes.h"
#include "..\HG3D Engine\HG3D Engine.h"

using namespace HG3D_Engine;

namespace Physics
{
	class Object
	{
	public:
		Object();
		~Object();
	
		
	public:
		bool m_Moveable;
		point m_Position;

		vector m_Force;
		vector m_Velocity;
		vector m_Acceleration;
	};
}