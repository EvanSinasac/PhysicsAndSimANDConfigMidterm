//Evan Sinasac - 1081418
//INFO6019 Physics and Simulation and INFO6025 Configuration and Deployment Midterm
//cParticle.h description:
//			        More or less the same as James Lucas' cParticle class, with applied forces removed as I'm not using them
//					Changes to Integrate are made in the classes that inherit from cParticle, and everything else
//					here is what is needed in every 'Particle"


#pragma once

#include <../../MidtermPhysicsAndConifgEvanSinasac/src/GLMCommon.h>

namespace nPhysics
{
	class cParticle
	{
	private:
		glm::vec3 mPosition;
		glm::vec3 mVelocity;
		glm::vec3 mAcceleration;
		float mInverseMass;
		float mDamping;

	public:
		cParticle(float mass, const glm::vec3& position);
		virtual ~cParticle();

		cParticle() = delete;
		cParticle(cParticle& other) = delete;
		cParticle& operator=(cParticle& other) = delete;

		void SetDamping(float damping);
		float GetDamping() const;

		float GetMass() const;
		float GetInverseMass() const;

		bool IsStaticObject() const;

		glm::vec3 GetPosition() const;
		void GetPosition(glm::vec3& position);
		void SetPosition(const glm::vec3& position);

		glm::vec3 GetVelocity() const;
		void GetVelocity(glm::vec3& velocity);
		void SetVelocity(const glm::vec3& velocity);

		glm::vec3 GetAcceleration() const;
		void GetAcceleration(glm::vec3& acceleration);
		void SetAcceleration(const glm::vec3& acceleration);


		virtual void Integrate(float deltaTime);
	};
}