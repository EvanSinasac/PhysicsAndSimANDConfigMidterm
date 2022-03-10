//Evan Sinasac - 1081418
//INFO6019 Physics and Simulation and INFO6025 Configuration and Deployment Midterm
//cEnergyBall.h description:
//						The header file for the Energy Ball, which has the additional death conditions.  Energy
//						Balls can die when they hit the ground, are a certain distance from the cannon or
//						if they've been out for a certain amount of time

#pragma once

#include <physics/cCannonShot.h>

namespace nPhysics
{
	class cEnergyBall : public cCannonShot
	{
	private:
		float timeToDie;
		float timeAlive;
		float distanceToDie;
		glm::vec3 cannonPosition;
	public:
		cEnergyBall(float mass, const glm::vec3& position);
		virtual ~cEnergyBall();

		virtual void Integrate(float deltaTime);

		void SetDistanceToDeath(float dToD);
		void SetTimeToDie(float tToD);

		float DistanceFromCanon(glm::vec3 cannonPos);
		void SetCannonPosition(glm::vec3 cannonPos);

	};
}