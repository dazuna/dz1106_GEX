#pragma once
#include <game_math.h>   // single include header for all glm math stuffs

namespace phys
{
	// cIntegrator
	// 
	// A single place for numerical integration schemes.
	//
	// Contains single entry-points for each supported integrations scheme.
	class cIntegrator
	{
	private:
		// TODO: Define any structs used internally for specific schemes.
		//       Such as,
		struct State;
		struct Derivative;
		struct sRK4State
		{
			glm::vec3 pos; // position
			glm::vec3 vel; // velocity (dx)
			glm::vec3 acc; // acceleration (dv)

			sRK4State()
			: pos(glm::vec3()), vel(glm::vec3()), acc(glm::vec3()) {}
			
			sRK4State(glm::vec3 position,glm::vec3 velocity,glm::vec3 acceleration)
			: pos(position), vel(velocity), acc(acceleration) {}

		};
		// RK4 helper functions
		sRK4State RungeKuttaEvaluate(const sRK4State & initial, float dt, const sRK4State & derivative);

	public:
		// Constructor
		cIntegrator();

		// TODO: Fully comment the inputs/outputs of any
		//       integration method you do end up including.
	
		// FOURTH-ORDER RUNGE-KUTTA
		void RK4(glm::vec3& pos, glm::vec3& vel, glm::vec3& acc, float dt);
		void RK4(glm::vec3& pos, glm::vec3& vel, glm::vec3& acc, const glm::vec3& appliedAcc, float dt);
	};
}