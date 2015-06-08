#ifndef _PHYSICS_H_
#define _PHYSICS_H_

#include "BasicIncludes.h"

#ifdef _DLLEXPORT
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT __declspec(dllimport)
#endif


namespace Physics
{
	class Engine
	{
	public:
		Engine();
		~Engine();

	private:

	};
}

#endif