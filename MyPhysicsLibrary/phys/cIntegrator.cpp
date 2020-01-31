#include "cIntegrator.h"  // My header

namespace phys
{
	cIntegrator::cIntegrator() = default;

	// FOURTH-ORDER RUNGE-KUTTA
	void cIntegrator::RK4(glm::vec3& pos, glm::vec3& vel, glm::vec3& acc, float dt)
	{
		// TODO: math!
		// For an excellent article on RK4 implementation:
		// https://gafferongames.com/post/integration_basics/
		float hDt = dt*0.5f;
		sRK4State state(pos,vel,acc);
		sRK4State a = RungeKuttaEvaluate(state, 0.f, sRK4State());
		sRK4State b = RungeKuttaEvaluate(state, hDt, a);
		sRK4State c = RungeKuttaEvaluate(state, hDt, b);
		sRK4State d = RungeKuttaEvaluate(state,dt,c);

		glm::vec3 rkVel = (1.f / 6.f) * (a.vel + ((b.vel + c.vel) * 2.f) + d.vel);
		glm::vec3 rkAcc = (1.f / 6.f) * (a.acc + ((b.acc + c.acc) * 2.f) + d.acc);

		pos += rkVel * dt;
		vel += rkAcc * dt;
	}

	void cIntegrator::RK4(glm::vec3& pos, glm::vec3& vel, glm::vec3& acc, const glm::vec3& appliedAcc, float dt)
	{
		// TODO: math!
		// For an excellent article on RK4 implementation:
		// https://gafferongames.com/post/integration_basics/
		float hDt = dt*0.5f;
		sRK4State state(pos,vel,acc+appliedAcc);
		sRK4State a = RungeKuttaEvaluate(state,0.f,sRK4State());
		sRK4State b = RungeKuttaEvaluate(state,hDt,a);
		sRK4State c = RungeKuttaEvaluate(state,hDt,b);
		sRK4State d = RungeKuttaEvaluate(state,dt,c);

		glm::vec3 rkVel = (1.f / 6.f) * (a.vel + ((b.vel + c.vel) * 2.f) + d.vel);
		glm::vec3 rkAcc = (1.f / 6.f) * (a.acc + ((b.acc + c.acc) * 2.f) + d.acc);

		pos += rkVel * dt;
		vel += rkAcc * dt;
	}

	cIntegrator::sRK4State cIntegrator::RungeKuttaEvaluate(const sRK4State & initial, float dt, const sRK4State & derivative)
	{
		sRK4State state;
		state.pos = initial.pos + derivative.vel*dt;
		state.vel = initial.vel + derivative.acc*dt;
		state.acc = initial.acc;
		
		return state;
	}
}