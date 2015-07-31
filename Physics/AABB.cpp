#include "AABB.h"


namespace Physics
{
	bool CollisionBox::CheckCollisionWith(CollisionBox& OtherAABB)
	{

		point CenterA = Center;
		point CenterB = OtherAABB.Center;

		point ExtendsA = Extends;
		point ExtendsB = OtherAABB.Extends;

		point MinA;
		point MaxA;

		MinA.x = CenterA.x - ExtendsA.x;
		MinA.y = CenterA.y - ExtendsA.y;
		MinA.z = CenterA.z - ExtendsA.z;

		MaxA.x = CenterA.x + ExtendsA.x;
		MaxA.y = CenterA.y + ExtendsA.y;
		MaxA.z = CenterA.z + ExtendsA.z;

		point MinB;
		point MaxB;

		MinB.x = CenterB.x - ExtendsB.x;
		MinB.y = CenterB.y - ExtendsB.y;
		MinB.z = CenterB.z - ExtendsB.z;

		MaxB.x = CenterB.x + ExtendsB.x;
		MaxB.y = CenterB.y + ExtendsB.y;
		MaxB.z = CenterB.z + ExtendsB.z;


		return true;
	}
}