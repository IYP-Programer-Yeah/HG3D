#ifndef _PHYSICS_WORLD_H_
#define _PHYSICS_WORLD_H_

#include "BasicIncludes.h"
#include "Object.h"
#include <iostream>
#include <vector>

namespace Physics
{
	class PhysicsWorld
	{
	public:
		PhysicsWorld();
		~PhysicsWorld();

		void DLLEXPORT Update(const long double& Deltatime);
		void DLLEXPORT AddObject(PhysicsObject& Object);
		void DLLEXPORT RemoveLastObject();


	private:
		std::vector<PhysicsObject> m_Objects;
	};
}



#endif