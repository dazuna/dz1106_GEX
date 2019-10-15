// UserInput
//if ( key == GLFW_KEY_B )
		//{ 
		//	// Shoot a bullet from the pirate ship
		//	cGameObject* pShip = pFindObjectByFriendlyNameMap("PirateShip");
		//	// Find the sphere#2
		//	cGameObject* pBall = pFindObjectByFriendlyNameMap("Sphere#2");
		//	// Set the location velocity for sphere#2
		//	pBall->positionXYZ = pShip->positionXYZ;
		//	pBall->inverseMass = 1.0f;		// So it's updated
		//	pBall->velocity = glm::vec3( 15.0f, 20.0f, 0.0f );
		//	pBall->accel = glm::vec3(0.0f,0.0f,0.0f);
		//	pBall->diffuseColour = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
		//}//if ( key == GLFW_KEY_B )

// lines 352

/* pirateShip moving code */
		{
			// ********************************************************
			// Move the pirate in the direction it's pointing in...
			// Vec4 = mat4x4 * vec4				vertFinal = matModel * vertStart;
			cGameObject* pPirate = pFindObjectByFriendlyName("PirateShip");

			glm::mat4 matRotY = glm::rotate(glm::mat4(1.0f),
				pPirate->HACK_AngleAroundYAxis,	//(float)glfwGetTime(),					// Angle 
				glm::vec3(0.0f, 1.0f, 0.0f));

			// Assume the ship is at 0,0,0
			glm::vec4 frontOfShip = glm::vec4(0.0f, 0.0f, -1.0f, 1.0f);	// pointing to the "front" of the ship

			// Vec4 = mat4x4 * vec4				vertFinal = matModel * vertStart;
			glm::vec4 frontOfShipInWorld = matRotY * frontOfShip;

			// this value would be the velocity, ifyou wanted the phsyics update do to it
			glm::vec3 newSpeedOfShipIN_THE_DIRECTION_WE_WANT_TO_GO
				= frontOfShipInWorld * pPirate->HACK_speed;

			// Draw a line showing where we are going...
			pDebugRenderer->addLine(pPirate->positionXYZ,
				pPirate->positionXYZ + (glm::vec3(frontOfShipInWorld) * pPirate->HACK_speed * 5.0f),
				glm::vec3(1.0f, 0.0f, 0.0f), 0.5f);

			// Draw a line showing how fast we are going...
			// Update the pirate ship
			pPirate->positionXYZ += newSpeedOfShipIN_THE_DIRECTION_WE_WANT_TO_GO;
		}




// line 418
		// Let's draw all the closest points to the sphere
		// on the terrain mesh....
		// 
		// For each triangle in the terrain mesh:
		// - Run ClosestPointToTriangle
		// - Place the debug sphere "there"
		// - Draw it.

		//**********************************************************
		//**********************************************************
		glm::vec3 closestPoint = glm::vec3(0.0f, 0.0f, 0.0f);
		cPhysics::sPhysicsTriangle closestTriangle;

		//pPhsyics->GetClosestTriangleToPoint(pShpere->positionXYZ, *(g_map_Mesh["largeBunnyMesh"]), closestPoint, closestTriangle);

		// Highlight the triangle that I'm closest to
		//pDebugRenderer->addTriangle(closestTriangle.verts[0],
		//	closestTriangle.verts[1],
		//	closestTriangle.verts[2],
		//	glm::vec3(1.0f, 0.0f, 0.0f));

		// Highlight the triangle that I'm closest to
		// To draw the normal, calculate the average of the 3 vertices, 
		// then draw that average + the normal (the normal starts at the 0,0,0 OF THE TRIANGLE)
		glm::vec3 centreOfTriangle = (closestTriangle.verts[0] +
			closestTriangle.verts[1] +
			closestTriangle.verts[2]) / 3.0f;		// Average

		glm::vec3 normalInWorld = centreOfTriangle + (closestTriangle.normal * 20.0f);	// Normal x 10 length

		//pDebugRenderer->addLine(centreOfTriangle,
		//	normalInWorld,
		//	glm::vec3(1.0f, 1.0f, 0.0f));

		// Are we hitting the triangle? 
		float distance = glm::length(::g_vec_pGameObjects->positionXYZ - closestPoint);

		if (distance <= pShpere->SPHERE_radius)
		{
			// If you want, move the sphere back to where it just penetrated...
			// So that it will collide exactly where it's supposed to. 
			// But, that's not a big problem.

			// Is in contact with the triangle... 
			// Calculate the response vector off the triangle. 
			glm::vec3 velocityVector = glm::normalize(pShpere->velocity);

			// closestTriangle.normal
			glm::vec3 reflectionVec = glm::reflect(velocityVector, closestTriangle.normal);
			reflectionVec = glm::normalize(reflectionVec);

			//	Stop the sphere and draw the two vectors...
			//	pShpere->inverseMass = 0.0f;	// Stopped

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

		bool DidBallCollideWithGround = false;
		//HACK_BounceOffSomePlanes(pShpere, DidBallCollideWithGround);

		// A more general 
		pPhsyics->TestForCollisions(::g_vec_pGameObjects);

		//	********* Aqui Habia un comentario enorme.. maybe checalo en original.
		//{// Draw closest point
		//	glm::mat4 matModel = glm::mat4(1.0f);
		//	pDebugSphere->positionXYZ = closestPoint;
		//	pDebugSphere->scale = 1.0f;
		//	pDebugSphere->debugColour = glm::vec4(0.0f, 1.0f, 1.0f, 1.0f);
		//	pDebugSphere->isWireframe = true;
		//	DrawObject(matModel, pDebugSphere,
		//		shaderProgID, pTheVAOManager);
		//}

		// How far did we penetrate the surface?
		glm::vec3 CentreToClosestPoint = pShpere->positionXYZ - closestPoint;

		// Direction that ball is going is normalized velocity
		glm::vec3 directionBall = glm::normalize(pShpere->velocity);	// 1.0f

		// Calcualte direction to move it back the way it came from
		glm::vec3 oppositeDirection = -directionBall;				// 1.0f

		float distanceToClosestPoint = glm::length(CentreToClosestPoint);

		//pDebugRenderer->addLine(pShpere->positionXYZ,
		//	closestPoint,
		//	glm::vec3(0.0f, 1.0f, 0.0f),
		//	1.0f);

		//		// HACK
		//		if (DidBallCollideWithGround)
		//		{ 
		//			float sphereRadius = 1.0f;
		//			float distanceToMoveBack = sphereRadius - distanceToClosestPoint;
		//			glm::vec3 adjustmentVector = oppositeDirection * distanceToMoveBack;
		//			// Let's move the sphere that amount...
		//			pShpere->positionXYZ += adjustmentVector;
		//			//	NOW, I can calculate the correct response vector... 
		//			//	pShpere->velocity = glm::reflect(pShpere->velocity, triangleNormal)
		//		}

		//std::cout
		//	<< pShpere->velocity.x << ", "
		//	<< pShpere->velocity.y << ", "
		//	<< pShpere->velocity.z << "   dist = "
		//	<< distanceToClosestPoint << std::endl;

		//howMuchToMoveItBack = 1.0 - lenthOfThatVector

		if (bLightDebugSheresOn)
		{
			{// Draw where the light is at
				glm::mat4 matModel = glm::mat4(1.0f);
				pDebugSphere->positionXYZ = selectedLight->second.positionXYZ;
				pDebugSphere->scale = 0.5f;
				pDebugSphere->debugColour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
				pDebugSphere->isWireframe = true;
				DrawObject(matModel, pDebugSphere,
					shaderProgID, pTheVAOManager);
			}

			// Draw spheres to represent the attenuation...
			{   // Draw a sphere at 1% brightness
				glm::mat4 matModel = glm::mat4(1.0f);
				pDebugSphere->positionXYZ = selectedLight->second.positionXYZ;
				float sphereSize = pLightHelper->calcApproxDistFromAtten(
					0.01f,		// 1% brightness (essentially black)
					0.001f,		// Within 0.1%  
					100000.0f,	// Will quit when it's at this distance
					selectedLight->second.ConstAtten,
					selectedLight->second.LinearAtten,
					selectedLight->second.QuadraticAtten);
				pDebugSphere->scale = sphereSize;
				pDebugSphere->debugColour = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
				pDebugSphere->isWireframe = true;
				DrawObject(matModel, pDebugSphere,
					shaderProgID, pTheVAOManager);
			}
			{   // Draw a sphere at 25% brightness
				glm::mat4 matModel = glm::mat4(1.0f);
				pDebugSphere->positionXYZ = selectedLight->second.positionXYZ;
				float sphereSize = pLightHelper->calcApproxDistFromAtten(
					0.25f,		// 1% brightness (essentially black)
					0.001f,		// Within 0.1%  
					100000.0f,	// Will quit when it's at this distance
					selectedLight->second.ConstAtten,
					selectedLight->second.LinearAtten,
					selectedLight->second.QuadraticAtten);
				pDebugSphere->scale = sphereSize;
				pDebugSphere->debugColour = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
				pDebugSphere->isWireframe = true;
				DrawObject(matModel, pDebugSphere,
					shaderProgID, pTheVAOManager);
			}
			{   // Draw a sphere at 50% brightness
				glm::mat4 matModel = glm::mat4(1.0f);
				pDebugSphere->positionXYZ = selectedLight->second.positionXYZ;
				float sphereSize = pLightHelper->calcApproxDistFromAtten(
					0.50f,		// 1% brightness (essentially black)
					0.001f,		// Within 0.1%  
					100000.0f,	// Will quit when it's at this distance
					selectedLight->second.ConstAtten,
					selectedLight->second.LinearAtten,
					selectedLight->second.QuadraticAtten);
				pDebugSphere->scale = sphereSize;
				pDebugSphere->debugColour = glm::vec4(0.0f, 1.0f, 1.0f, 1.0f);
				pDebugSphere->isWireframe = true;
				DrawObject(matModel, pDebugSphere,
					shaderProgID, pTheVAOManager);
			}
			{   // Draw a sphere at 75% brightness
				glm::mat4 matModel = glm::mat4(1.0f);
				pDebugSphere->positionXYZ = selectedLight->second.positionXYZ;
				float sphereSize = pLightHelper->calcApproxDistFromAtten(
					0.75f,		// 1% brightness (essentially black)
					0.001f,		// Within 0.1%  
					100000.0f,	// Will quit when it's at this distance
					selectedLight->second.ConstAtten,
					selectedLight->second.LinearAtten,
					selectedLight->second.QuadraticAtten);
				pDebugSphere->scale = sphereSize;
				pDebugSphere->debugColour = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
				pDebugSphere->isWireframe = true;
				DrawObject(matModel, pDebugSphere,
					shaderProgID, pTheVAOManager);
			}
			{   // Draw a sphere at 95% brightness
				glm::mat4 matModel = glm::mat4(1.0f);
				pDebugSphere->positionXYZ = selectedLight->second.positionXYZ;
				float sphereSize = pLightHelper->calcApproxDistFromAtten(
					0.95f,		// 1% brightness (essentially black)
					0.001f,		// Within 0.1%  
					100000.0f,	// Will quit when it's at this distance
					selectedLight->second.ConstAtten,
					selectedLight->second.LinearAtten,
					selectedLight->second.QuadraticAtten);
				pDebugSphere->scale = sphereSize;
				pDebugSphere->debugColour = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
				pDebugSphere->isWireframe = true;
				DrawObject(matModel, pDebugSphere,
					shaderProgID, pTheVAOManager);
			}
		}// if (bLightDebugSheresOn) 