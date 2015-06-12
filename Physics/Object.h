#include "BasicIncludes.h"
#include "..\HG3D Engine\HG3D Engine.h"
#include "Sphere.h"
#include "AABB.h"

#include <float.h>
#include <assert.h>

using namespace HG3D_Engine;



namespace Physics
{
	class PhysicsObject
	{
	public:
		PhysicsObject();
		~PhysicsObject();
	
		//This function is to be called by a PhysicsWorld
		//Not available to user
		void Update(const long double& Deltatime);
		
		//It's called by the PhysicalWorld class
		//This function calculates 
		void CalculateCollisionShapes();

		//Set Object's properties
		void DLLEXPORT AddForce(long double x, long double y, long double z);
		void DLLEXPORT SetForce(long double x, long double y, long double z);
		void DLLEXPORT AddForce(const vector& Force);
		void DLLEXPORT SetForce(const vector& Force);
		void DLLEXPORT SetGravity(long double x, long double y, long double z);
		void DLLEXPORT SetMass(long double Mass);

		//To be called by PhysicalWorld
		void SetMesh(Mesh& mesh);


		//our m_Position vector will point to this address
		//This PointerAddress must be ACTUAL Object's position
		//So, if we update position with our physics code, then the actual position of the object
		//will be updated too
		void DLLEXPORT SetPositionPointer(point& PointerAddress);
		point DLLEXPORT GetPosition();

	public:
		bool m_ApplyGravity;
		bool m_Moveable;

		point m_Last_Position;

		vector m_Velocity;
		vector m_Last_Velocity;
		vector m_Acceleration;

		long double m_Last_dt;
		long double m_Mass;

	private:
		bool m_LastFrameDataInitialized;

		//This stuff is now set by it's functions
		vector m_Force;
		vector m_Gravity;

		//Pointer because actual position of the object too needs be updated
		//So, this needs to be private
		point* m_Position;

		CollisionSphere m_ColSphere;
		CollisionBox m_ColBox;

		Mesh* m_MeshPtr;
	};
}