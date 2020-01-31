#pragma once
#include "iShape.h"      // iShape interface class
#include <game_math.h>   // single include header for all glm math stuffs

namespace phys
{
	// cPlane
	// 
	// Subclass of iShape.
	// eShapeType::plane
	// 
	// Represents an infinite, static plane.
	// 
	// Contains a normal, and a constant.
	// 
	// For a plane defined by the equation:
	//        Ax + By + Cz = D
	// The normal vector is <A,B,C>
	// The constant is  D
	// Any point on the plane is <x,y,z>
	//
	class cPlane : public iShape
	{
	public:
		// Destructor
		virtual ~cPlane() {}

		// Constructor
		// A plane must defined by a normal vector and a constant.
		// The normal vector is expected to already be normalized.
		cPlane(const glm::vec3& normal, float constant)
		: iShape(eShapeType::plane)
		, mNormal(normal)
		, mConstant(constant)
		{

		}
		// GetNormal
		// Get the normal vector that defines this plane.
		inline const glm::vec3& GetNormal() { return mNormal; }
		// GetConstant
		// Get the constant value that defines this plane.
		inline float GetConstant() { return mConstant; }

	private:
		// Normal vector.
		// The <A,B,C> in  Ax + By + Cz = D
		glm::vec3 mNormal;
		// Constant value of the plane.
		// The "D" in  Ax + By + Cz = D
		float mConstant;

		// Constructors not to be used.
		cPlane() = delete;
		cPlane(const cPlane& other) = delete;
		cPlane& operator=(const cPlane& other) = delete;
	};
	
	// cSphere
	// 
	// Subclass of iShape.
	// eShapeType::sphere
	// 
	// Represents a perfect sphere.
	// 
	// Contains a radius.
	// 
	class cSphere : public iShape
	{
	public:
		// Destructor
		virtual ~cSphere() {}
		// Constructor
		// A sphere is defined only by a radius.
		cSphere(float radius)
			: iShape(eShapeType::sphere)
			, mRadius(radius)
		{

		}

		// GetRadius
		// Get the radius that defines this sphere.
		inline float GetRadius() { return mRadius; }

	private:
		// The radius of this sphere.
		float mRadius;

		// Constructors not to be used.
		cSphere() = delete;
		cSphere(const cPlane& other) = delete;
		cSphere& operator=(const cSphere& other) = delete;
	};
}