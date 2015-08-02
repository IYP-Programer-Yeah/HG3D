#ifndef _POINT_H_
#define _POINT_H_

#include "BasicIncludes.h"

namespace Physics
{
	struct Point
	{
		Point() :
			x(0), y(0), z(0)
		{
			
		}

		float x;
		float y;
		float z;
	};
}

#endif