//Evan Sinasac - 1081418
//INFO6019 Physics and Simulation and INFO6025 Configuration and Deployment Midterm
//cLaser.cpp description:
//					The cpp file for cLaser class, implementation for the distance from the cannon death condition,
//					Laser's also die if they hit the ground

#include <physics/cLaser.h>
#include <iostream>

namespace nPhysics
{

	cLaser::cLaser(float mass, const glm::vec3& position) : cCannonShot(mass, position)
	{
		SetAcceleration(glm::vec3(0.0f, -3.2f, 0.0f));
		SetIsAlive(true);
		distanceToDeath = 100.0f;
		cannonPosition = glm::vec3(0.0f, 2.0f, 0.0f);
	}
	cLaser::~cLaser()
	{

	}

	void cLaser::Integrate(float deltaTime)
	{
		if (GetIsAlive())
		{
			if (GetInverseMass() == 0.f)
			{
				return; // static things don't move!
			}

			SetPosition(glm::vec3(GetPosition() + GetVelocity() * deltaTime));
			// F*(1/m) = a
			SetVelocity(glm::vec3(GetVelocity() + (GetAcceleration() / GetInverseMass() * deltaTime)));

			// apply damping
			SetVelocity(glm::vec3(GetVelocity() * glm::pow(GetDamping(), deltaTime)));


			if (GetPosition().y <= 0 || DistanceFromCanon(cannonPosition) >= distanceToDeath)
			{
				SetIsAlive(false);
			}
		}
	}

	void cLaser::SetDistanceToDeath(float dToD)
	{
		distanceToDeath = dToD;
	}

	float cLaser::DistanceFromCanon(glm::vec3 cannonPos)
	{
		return glm::distance(cannonPos, GetPosition());
	}

	void cLaser::SetCannonPosition(glm::vec3 cannonPos)
	{
		cannonPosition = cannonPos;
	}

}