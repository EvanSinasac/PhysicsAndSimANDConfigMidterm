//Evan Sinasac - 1081418
//INFO6019 Physics and Simulation and INFO6025 Configuration and Deployment Midterm
//cCannonBall.cpp description:
//						Honestly, can probably just use the cBullet as a Cannon Ball and change it's properties,
//						I guess I'm keeping it as a seperate class in case I use them for different things in the future

#include <physics/cCannonBall.h>
#include <iostream>

namespace nPhysics
{
	cCannonBall::cCannonBall(float mass, const glm::vec3& position) : cCannonShot(mass, position)
	{
		SetAcceleration(glm::vec3(0.0f, -9.8f, 0.0f));
		SetIsAlive(true);
	}
	cCannonBall::~cCannonBall()
	{

	}

	void cCannonBall::Integrate(float deltaTime)
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


			if (GetPosition().y <= 0)
			{
				SetIsAlive(false);
			}
		}
	}

}