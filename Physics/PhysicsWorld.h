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
		void DLLEXPORT UpdateCollision();

		void DLLEXPORT LoadWorld(Renderer* renderer, float* Masses, std::string* Names,
			unsigned long int* MeshIDs, unsigned long int NumMeshes);

		void DLLEXPORT AddObject(const std::string Name, PhysicsObject& Object, Mesh& mesh);
		void DLLEXPORT RemoveLastObject();

		DLLEXPORT PhysicsObject& GetPhysicsObject(unsigned long int Index);
		DLLEXPORT PhysicsObject& GetPhysicsObject(const std::string Name);
	private:
		std::vector<PhysicsObject> m_Objects;
		std::vector<std::string> m_ObjectNames;
		Renderer* m_RendrerPtr;
	};
}



#endif