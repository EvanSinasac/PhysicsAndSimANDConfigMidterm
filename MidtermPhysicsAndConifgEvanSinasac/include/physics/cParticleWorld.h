//Evan Sinasac - 1081418
//INFO6019 Physics and Simulation and INFO6025 Configuration and Deployment Midterm
//cParticleWorld.h description:
//			        More or less the same as James Lucas' cParticleWorld class, with the force registry stuff removed as I'm not using them
//					

#pragma once
#include <physics/cParticle.h>
#include <vector>

namespace nPhysics
{
	class cParticleWorld
	{
	private:
		std::vector<cParticle*> mParticles;
	public:
		cParticleWorld();
		virtual ~cParticleWorld();

		bool AddParticle(cParticle* particle);
		bool RemoveParticle(cParticle* particle);

		void TimeStep(float deltaTime);

		void IntegrateParticles(float deltaTime);

	};
}
