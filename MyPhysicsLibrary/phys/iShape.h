#pragma once
#include "eShapeType.h"  // eShapeType class enum

namespace phys
{
	// iShape
	// 
	// A base interface for all specific shapes.
	// 
	// Contains an eShapeType to identify the specific shape type for casting.
	class iShape
	{
	public:

		// Destructor
		virtual ~iShape() {}

		// GetShapeType
		// Returns the eShapeType indicating the specific shape.
		inline const eShapeType& GetShapeType() { return mShapeType; }
		
	protected:

		// Constructor
		// Expected to be called only by the constructors of subclasses.
		// The calling subclass is expected to pass the correct eShapeType.
		iShape(eShapeType shapeType)
			: mShapeType(shapeType)
		{

		}
	private:
		// The shape type identifier.
		eShapeType mShapeType;

		// Constructors not to be used.
		iShape() = delete;
		iShape(const iShape& other) = delete;
		iShape& operator=(const iShape& other) = delete;
	};
}