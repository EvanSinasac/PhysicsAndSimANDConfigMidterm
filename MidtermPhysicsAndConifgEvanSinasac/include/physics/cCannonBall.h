//Evan Sinasac - 1081418
//INFO6019 Physics and Simulation and INFO6025 Configuration and Deployment Midterm
//cCannonBall.h description:
//						cCannonBall header file, Cannon Balls are very similar to bullets but have higher mass and 
//						slower initial velocity.

#pragma once

#include <physics/cCannonShot.h>

namespace nPhysics
{
	class cCannonBall : public cCannonShot
	{
	public:
		cCannonBall(float mass, const glm::vec3& position);
		virtual ~cCannonBall();

		virtual void Integrate(float deltaTime);

	};
}