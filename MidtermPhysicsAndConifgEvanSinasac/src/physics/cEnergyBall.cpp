//Evan Sinasac - 1081418
//INFO6019 Physics and Simulation and INFO6025 Configuration and Deployment Midterm
//cEnergyBall.cpp description:
//						The Energy Ball class, which has the most conditions for death.  Time, distance or hitting the
//						ground.  It also has the unique property that it's acceleration is meant to be in the same
//						direction that it is fired in, so it get's faster and faster with time

#include <physics/cEnergyBall.h>
#include <iostream>

namespace nPhysics
{

	cEnergyBall::cEnergyBall(float mass, const glm::vec3& position) : cCannonShot(mass, position)
	{
		SetIsAlive(true);
		distanceToDie = 50.0f;
		timeAlive = 0.0f;
		timeToDie = 30.0f;
		cannonPosition = glm::vec3(0.0f, 2.0f, 0.0f);
	}
	cEnergyBall::~cEnergyBall()
	{

	}

	void cEnergyBall::Integrate(float deltaTime)
	{
		if (GetIsAlive())
		{
			if (GetInverseMass() == 0.f)
			{
				return; // static things don't move!
			}

			SetPosition(glm::vec3(GetPosition() + GetVelocity() * deltaTime));
			// F*(1/m) = a
			SetVelocity(glm::vec3(GetVelocity() + (GetAcceleration() * deltaTime)));

			// apply damping
			SetVelocity(glm::vec3(GetVelocity() * glm::pow(GetDamping(), deltaTime)));


			timeAlive += deltaTime;

			if (GetPosition().y <= 0 || DistanceFromCanon(cannonPosition) >= distanceToDie || timeAlive >= timeToDie)
			{
				SetIsAlive(false);
			}
		}
	}

	void cEnergyBall::SetDistanceToDeath(float dToD)
	{
		distanceToDie = dToD;
	}
	void cEnergyBall::SetTimeToDie(float tToD)
	{
		timeToDie = tToD;
	}

	float cEnergyBall::DistanceFromCanon(glm::vec3 cannonPos)
	{
		return glm::distance(cannonPos, GetPosition());
	}
	void cEnergyBall::SetCannonPosition(glm::vec3 cannonPos)
	{
		cannonPosition = cannonPos;
	}

}