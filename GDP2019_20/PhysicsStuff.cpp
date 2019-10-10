// PhysicsStuff.cpp
#include <vector>
#include "cGameObject.h"
#include <iostream>
#include "PhysicsStuff.h"

//extern bool g_BallCollided;

void PhysicsUpdate(std::vector<cGameObject*>& vec_pGameObjects,
				   float deltaTime)
{

	// 9.81 
	glm::vec3 gravity = glm::vec3( 0.0f, -6.0f, 0.0f );


	for (unsigned int index = 0;
		 index != vec_pGameObjects.size(); index++)
	{


		cGameObject* pCurObj = vec_pGameObjects[index];

		if (pCurObj->inverseMass != 0.0f)
		{

			// Forward Explicit Euler Inetegration
			//NewVelocty += Velocity + ( Ax * DeltaTime )

			// 
			pCurObj->accel = gravity;


			pCurObj->velocity.x += pCurObj->accel.x * deltaTime;
			pCurObj->velocity.y += pCurObj->accel.y * deltaTime;
			pCurObj->velocity.z += pCurObj->accel.z * deltaTime;
	//		// Or you can do this...
	//		CurObj.velocity += CurObj.accel * deltaTime;

			//NewPosition = Posistion + ( Vx * DeltaTime )

			pCurObj->positionXYZ.x += pCurObj->velocity.x * deltaTime;
			pCurObj->positionXYZ.y += pCurObj->velocity.y * deltaTime;
			pCurObj->positionXYZ.z += pCurObj->velocity.z * deltaTime;


			float groundPlane = 0.0f;
			float rightPlane = 100.0f;
			float leftPlane = -100.0f;

			float radius = 1.0f;
//			direction = vec3( 0.0, +1.0, 0.0 )
//		    velocity = 10.0f
			if ( (pCurObj->positionXYZ.y - radius) <= groundPlane)
			{
				// It's hit the cube. Set the velocity to -ve of what it is
				// fabs() is floating point absolute value
//				pCurObj->velocity.y = fabs(pCurObj->velocity.y);

				// Stop it (it's collided with a triangle)
//				pCurObj->velocity = glm::vec3(0.0f,0.0f,0.0f);
				pCurObj->inverseMass = 0.0f;	 // Stop it
				
				// HACK
//				::g_BallCollided = true;
				
				
				//				float vel = fabs(pCurObj->velocity.y)
//
//				pCurObj->velocity = glm::reflect(pCurObj->velocity, 
//												 normalOfTriangle);


				// Damp the velocity 
				pCurObj->velocity.y *= 0.95f;
			}

			// Left plane 
			if ( (pCurObj->positionXYZ.x) <= leftPlane)
			{
				// It's hit the cube. Set the velocity to -ve of what it is
				// fabs() is floating point absolute value
				pCurObj->velocity.x = +fabs(pCurObj->velocity.x);
			}
			// Right plane 
			if ( (pCurObj->positionXYZ.x) >= rightPlane)
			{
				// It's hit the cube. Set the velocity to -ve of what it is
				// fabs() is floating point absolute value
				pCurObj->velocity.x = -fabs(pCurObj->velocity.x);
			}

		}// if (CurObj.inverseMass != 0.0f)

	}

	return;
}



// Amazing notes
	// Determine the object new position

	// Position   ( x = 0 )

	// Velocity   Vx = 1.0f / second 

	//Where is the object at time = 1? 

	// DeltaX  = Vx * DeltaTime 
	//    1.0     =  1.0f/second  * 1.0 second

//Position += (DeltaX)
//Position += Vx * DeltaTime

// Velocity = 0.0f / second 
// Accel = 1.0f / Second /second 

// DeltaV  = Ax * DeltaTime 
// DeltaV  = 1.0f/second/second * 1.0f second 
// DeltaV  = 1.0f/second/ * 1.0f  
// Velocty += DeltaV
// Velocty += Ax * DeltaTime 

// Alias to a type "existing type" "new type name"
//typedef glm::vec3 Point;
//typedef glm::vec3 Vector;

// Closest point in 3D space. XYZ 
Point ClosestPtPointTriangle(Point p, Point a, Point b, Point c)
{

	Vector ab = b - a;
	Vector ac = c - a;
	Vector bc = c - b;

	// Compute parametric position s for projection P' of P on AB,
	// P' = A + s*AB, s = snom/(snom+sdenom)
	float snom = glm::dot(p - a, ab), sdenom = glm::dot(p - b, a - b);

	// Compute parametric position t for projection P' of P on AC,
	// P' = A + t*AC, s = tnom/(tnom+tdenom)
	float tnom = glm::dot(p - a, ac), tdenom = glm::dot(p - c, a - c);

	if (snom <= 0.0f && tnom <= 0.0f) return a; // Vertex region early out

	// Compute parametric position u for projection P' of P on BC,
	// P' = B + u*BC, u = unom/(unom+udenom)
	float unom = glm::dot(p - b, bc), udenom = glm::dot(p - c, b - c);

	if (sdenom <= 0.0f && unom <= 0.0f) return b; // Vertex region early out
	if (tdenom <= 0.0f && udenom <= 0.0f) return c; // Vertex region early out


	// P is outside (or on) AB if the triple scalar product [N PA PB] <= 0
	Vector n = glm::cross(b - a, c - a);
	float vc = glm::dot(n, glm::cross(a - p, b - p));
	// If P outside AB and within feature region of AB,
	// return projection of P onto AB
	if (vc <= 0.0f && snom >= 0.0f && sdenom >= 0.0f)
		return a + snom / (snom + sdenom) * ab;

	// P is outside (or on) BC if the triple scalar product [N PB PC] <= 0
	float va = glm::dot(n, glm::cross(b - p, c - p));
	// If P outside BC and within feature region of BC,
	// return projection of P onto BC
	if (va <= 0.0f && unom >= 0.0f && udenom >= 0.0f)
		return b + unom / (unom + udenom) * bc;

	// P is outside (or on) CA if the triple scalar product [N PC PA] <= 0
	float vb = glm::dot(n, glm::cross(c - p, a - p));
	// If P outside CA and within feature region of CA,
	// return projection of P onto CA
	if (vb <= 0.0f && tnom >= 0.0f && tdenom >= 0.0f)
		return a + tnom / (tnom + tdenom) * ac;

	// P must project inside face region. Compute Q using barycentric coordinates
	float u = va / (va + vb + vc);
	float v = vb / (va + vb + vc);
	float w = 1.0f - u - v; // = vc / (va + vb + vc)
	return u * a + v * b + w * c;
}
// From Ericson's book:
// Returns true if sphere s intersects triangle ABC, false otherwise.
// The point p on abc closest to the sphere center is also returned
//struct Sphere
//{
//	glm::vec3 c;
//	float r;
//};
int TestSphereTriangle(Sphere s, Point a, Point b, Point c, Point& p)
{
	// Find point P on triangle ABC closest to sphere center
	p = ClosestPtPointTriangle(s.c, a, b, c);

	// Sphere and triangle intersect if the (squared) distance from sphere
	// center to point p is less than the (squared) sphere radius
	Vector v = p - s.c;
	return glm::dot(v, v) <= s.r * s.r;
}


void HACK_BounceOffSomePlanes(cGameObject* pTheBouncingBall, bool &didItCollideWithTheGround)
{

	float groundPlane = 0.0f;
	float rightPlane = 100.0f;
	float leftPlane = -100.0f;

	float radius = 1.0f;
	//			direction = vec3( 0.0, +1.0, 0.0 )
	//		    velocity = 10.0f
	if ((pTheBouncingBall->positionXYZ.y - radius) <= groundPlane)
	{
		// It's hit the cube. Set the velocity to -ve of what it is
		// fabs() is floating point absolute value
		pTheBouncingBall->velocity.y = fabs(pTheBouncingBall->velocity.y);

					// Stop it (it's collided with a triangle)
	//				pCurObj->velocity = glm::vec3(0.0f,0.0f,0.0f);
		//pTheBouncingBall->inverseMass = 0.0f;	 // Stop it

		//// HACK
		//didItCollideWithTheGround = true;


		//				float vel = fabs(pCurObj->velocity.y)
	//
	//				pCurObj->velocity = glm::reflect(pCurObj->velocity, 
	//												 normalOfTriangle);


					// Damp the velocity 
		//pTheBouncingBall->velocity.y *= 0.95f;
	}

	// Left plane 
	if ((pTheBouncingBall->positionXYZ.x) <= leftPlane)
	{
		// It's hit the cube. Set the velocity to -ve of what it is
		// fabs() is floating point absolute value
		pTheBouncingBall->velocity.x = +fabs(pTheBouncingBall->velocity.x);
	}
	// Right plane 
	if ((pTheBouncingBall->positionXYZ.x) >= rightPlane)
	{
		// It's hit the cube. Set the velocity to -ve of what it is
		// fabs() is floating point absolute value
		pTheBouncingBall->velocity.x = -fabs(pTheBouncingBall->velocity.x);
	}

	return;
}
