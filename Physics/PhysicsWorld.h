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
		DLLEXPORT PhysicsWorld();
		DLLEXPORT ~PhysicsWorld();

		void DLLEXPORT Update(const long double& Deltatime);
	
		void DLLEXPORT AddObject(PhysicsObject& Object, Mesh& mesh);
		void DLLEXPORT RemoveLastObject();

		void DLLEXPORT LoadWorld(Renderer* World, long double* Masses);

		PhysicsObject& GetPhysicsObject(UINT Index);
	private:
		std::vector<PhysicsObject> m_Objects;

		Renderer* m_RendererPtr;
	};
}



#endif