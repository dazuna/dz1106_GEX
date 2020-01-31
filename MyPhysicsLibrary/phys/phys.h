#pragma once
// A single include header for everything
// 
// Also a handy place to put a general comments
// and information about this library. For example: 
// - What is this library?
// This is a physics library that implements RK4 for the integration steps, includes sphere and plane collisions.
// - Who created this library
// Created by David Zuniga for GDAP course 2019-2020
// - Dependencies of this library
// GLM library only
// - Why this library was created
// To learn how to implement physics simulation into an opengl program,
// and also learn how to build libraries completely independent from the rest of our gdap code.
// - The general approach to creating this library
// The main purpose of this implementation is for it to work as reference and learning material for me, the author.
// - Notes on usage
// - Notes about any good-to-know factos or gotchas, perhaps tidbits like:
//       "cWorld will never delete a cRigidBody"
//       "shapes can be shared between multiple cRigidBody instances"
//
// All the files required to make full use of this library:
#include "eShapeType.h"  // enum class for all supported shape types
#include "iShape.h"      // iShape is the base interface class for all shapes
#include "cRigidBody.h"  // cRigidBody is this library's rigid body representation
#include "shapes.h"      // Concrete classes for all supported shape types
#include "cWorld.h"      // cWorld is the primary container and operator of a simulation