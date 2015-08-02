#ifndef _AABB_H_
#define _AABB_H_

#include "BasicIncludes.h"
#include "..\HG3D Engine\HG3D Engine.h"
#include "Point.h"

using namespace HG3D_Engine;

namespace Physics
{
	//Axis-Aligned Bounding box
	struct CollisionBox
	{
		CollisionBox()
		{
			ZeroMemory(this, sizeof(CollisionBox));
		}

		~CollisionBox()
		{
		}

		point Center;
		point Extends;

		bool DLLEXPORT CheckCollisionWith(CollisionBox& OtherAABB);
		bool DLLEXPORT Contains(Point& p);
	};
}


#endif