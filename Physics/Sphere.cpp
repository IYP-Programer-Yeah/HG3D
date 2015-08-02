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

	bool CollisionSphere::Contains(Point& p)
	{
		vector VecCenterToPoint;

		VecCenterToPoint.x = p.x - Center.x;
		VecCenterToPoint.y = p.y - Center.y;
		VecCenterToPoint.z = p.z - Center.z;

		float LengthSq = static_cast<float>(VecCenterToPoint.getsizeSq());

		if (LengthSq < Radius*Radius)
			return true;

		return false;
	}
}