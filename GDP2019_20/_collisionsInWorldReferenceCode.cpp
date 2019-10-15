
		//**********************************************************
		// 	Collisions in World
		//**********************************************************
		// Physics Stuff (will move soon)
		// Let's draw all the closest points to the sphere
		// on the terrain mesh....
		// For each triangle in the terrain mesh:
		// - Run ClosestPointToTriangle
		// - Place the debug sphere "there"
		// - Draw it.
		//**********************************************************
		glm::vec3 closestPoint = glm::vec3(0.0f, 0.0f, 0.0f);
		cPhysics::sPhysicsTriangle closestTriangle;
		// Update the mesh to match it's world position
		glm::mat4 matWorld = calculateWorldMatrix(::g_map_GameObjects[""]);

		cMesh lowResCubeMesh_TRANSFORMED_WorldSpace;
		pPhsyics->CalculateTransformedMesh(lowrescubeMesh, matWorld, lowResCubeMesh_TRANSFORMED_WorldSpace);
		//		pPhsyics->CalculateTransformedMesh(hirescubeMesh, matWorld, lowResCubeMesh_TRANSFORMED_WorldSpace);

				//glm::mat4 matSpaceStation = glm::mat4(1.0f);
				//pPhsyics->CalculateTransformedMesh(spaceStationMesh, matSpaceStation, lowResCubeMesh_TRANSFORMED_WorldSpace);

				// NOTE that I'm using the LOW RESOLUTION "cube" mesh, but DRAWING the high resolution mesh
		//		pPhsyics->GetClosestTriangleToPoint(pShpere->positionXYZ, largeBunnyMesh, closestPoint, closestTriangle);
		//		pPhsyics->GetClosestTriangleToPoint(pShpere->positionXYZ, lowrescubeMesh, closestPoint, closestTriangle);
		//		pPhsyics->GetClosestTriangleToPoint(pShpere->positionXYZ, lowResCubeMesh_TRANSFORMED_WorldSpace, closestPoint, closestTriangle);
		pPhsyics->GetClosestTriangleToPoint(pShpere->positionXYZ, singleTriangleMesh, closestPoint, closestTriangle);

		// Highlight the triangle that I'm closest to
		pDebugRenderer->addTriangle(closestTriangle.verts[0],
			closestTriangle.verts[1],
			closestTriangle.verts[2],
			glm::vec3(1.0f, 0.0f, 0.0f));

		// Highlight the triangle that I'm closest to
		// To draw the normal, calculate the average of the 3 vertices, 
		// then draw that average + the normal (the normal starts at the 0,0,0 OF THE TRIANGLE)
		glm::vec3 centreOfTriangle = (closestTriangle.verts[0] +
			closestTriangle.verts[1] +
			closestTriangle.verts[2]) / 3.0f;		// Average

		glm::vec3 normalInWorld = centreOfTriangle + (closestTriangle.normal * 20.0f);	// Normal x 10 length

		pDebugRenderer->addLine(centreOfTriangle,
			normalInWorld,
			glm::vec3(1.0f, 1.0f, 0.0f));

		// Are we hitting the triangle? 
		float distance = glm::length(pShpere->positionXYZ - closestPoint);

		if (distance <= pShpere->SPHERE_radius)
		{

			// ************************************************************************

			// If you want, move the sphere back to where it just penetrated...
			// So that it will collide exactly where it's supposed to. 
			// But, that's not a big problem.

			// 1. Calculate vector from centre of sphere to closest point
			glm::vec3 vecSphereToClosestPoint = closestPoint - pShpere->positionXYZ;

			// 2. Get the length of this vector
			float centreToContractDistance = glm::length(vecSphereToClosestPoint);

			// 3. Create a vector from closest point to radius
			float lengthPositionAdjustment = pShpere->SPHERE_radius - centreToContractDistance;

			// 4. Sphere is moving in the direction of the velocity, so 
			//    we want to move the sphere BACK along this velocity vector
			glm::vec3 vecDirection = glm::normalize(pShpere->velocity);

			glm::vec3 vecPositionAdjust = (-vecDirection) * lengthPositionAdjustment;

			// 5. Reposition sphere 
			pShpere->positionXYZ += (vecPositionAdjust);
			//			pShpere->inverseMass = 0.0f;

						// ************************************************************************


						// Is in contact with the triangle... 
						// Calculate the response vector off the triangle. 
			glm::vec3 velocityVector = glm::normalize(pShpere->velocity);

			// closestTriangle.normal
			glm::vec3 reflectionVec = glm::reflect(velocityVector, closestTriangle.normal);
			reflectionVec = glm::normalize(reflectionVec);

			// Stop the sphere and draw the two vectors...
//			pShpere->inverseMass = 0.0f;	// Stopped

			glm::vec3 velVecX20 = velocityVector * 10.0f;
			pDebugRenderer->addLine(closestPoint, velVecX20,
				glm::vec3(1.0f, 0.0f, 0.0f), 30.0f /*seconds*/);

			glm::vec3 reflectionVecX20 = reflectionVec * 10.0f;
			pDebugRenderer->addLine(closestPoint, reflectionVecX20,
				glm::vec3(0.0f, 1.0f, 1.0f), 30.0f /*seconds*/);

			// Change the direction of the ball (the bounce off the triangle)

			// Get lenght of the velocity vector
			float speed = glm::length(pShpere->velocity);

			pShpere->velocity = reflectionVec * speed;

		}
		//bool DidBallCollideWithGround = false;
		//HACK_BounceOffSomePlanes(pShpere, DidBallCollideWithGround );

		// A more general 
		pPhsyics->TestForCollisions(::g_map_GameObjects);

		// How far did we penetrate the surface?
		glm::vec3 CentreToClosestPoint = pShpere->positionXYZ - closestPoint;
		// Direction that ball is going is normalized velocity
		glm::vec3 directionBall = glm::normalize(pShpere->velocity);	// 1.0f
		// Calcualte direction to move it back the way it came from
		glm::vec3 oppositeDirection = -directionBall;				// 1.0f
		float distanceToClosestPoint = glm::length(CentreToClosestPoint);

		pDebugRenderer->addLine(pShpere->positionXYZ,
			closestPoint,
			glm::vec3(0.0f, 1.0f, 0.0f),
			1.0f);

		ssTitle
			<< pShpere->velocity.x << ", "
			<< pShpere->velocity.y << ", "
			<< pShpere->velocity.z << "   dist = "
			<< distanceToClosestPoint << std::endl;
		//howMuchToMoveItBack = 1.0 - lenthOfThatVector