#ifndef _SPHERE_H_
#define _SPHERE_H_

#include "BasicIncludes.h"
#include "..\HG3D Engine\HG3D Engine.h"

using namespace HG3D_Engine;

namespace Physics
{
	struct CollisionSphere
	{
		CollisionSphere()
		{
			ZeroMemory(this, sizeof(CollisionSphere));
		}

		~CollisionSphere()
		{
		}

		point Center;
		float Radius;

		bool DLLEXPORT CheckCollisionWith(CollisionSphere& OtherSphere);
	};

}


#endif