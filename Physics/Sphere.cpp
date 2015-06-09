#include "Sphere.h"

namespace Physics
{
	bool CollisionSphere::CheckCollisionWith(CollisionSphere& OtherSphere)
	{
		vector Difference;

		Difference.x = Center.x - OtherSphere.Center.x;
		Difference.y = Center.y - OtherSphere.Center.y;
		Difference.z = Center.z - OtherSphere.Center.z;

		float LengthBetweenCentersSq = static_cast<float>(Difference.getsizeSq());

		float TotalRadius = Radius + OtherSphere.Radius;

		if (LengthBetweenCentersSq < TotalRadius*TotalRadius)
			return true;

		return false;
	}
}