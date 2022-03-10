//Evan Sinasac - 1081418
//INFO6019 Physics and Simulation and INFO6025 Configuration and Deployment Midterm
//cParticle.cpp description:
//			        More or less the same as James Lucas' cParticle class, with applied forces removed as I'm not using them
//					Changes to Integrate are made in the classes that inherit from cParticle, and everything else
//					here is what is needed in every 'Particle"

#include <physics/cParticle.h>
#include <../src/GLMCommon.h>
#include <iostream>

namespace nPhysics
{
	cParticle::cParticle(float mass, const glm::vec3& position)
		: mPosition(position)
		, mVelocity(0.f)
		, mAcceleration(0.f)
		, mDamping(0.995f)
	{
		if (mass <= 0.f)
		{
			mInverseMass = 0.f;
		}
		else
		{
			mInverseMass = 1.f / mass;
		}
	}
	cParticle::~cParticle()
	{

	}

	void cParticle::SetDamping(float damping)
	{
		mDamping = glm::clamp(damping, 0.f, 1.f);
	}
	float cParticle::GetDamping() const
	{
		return mDamping;
	}

	float cParticle::GetMass() const
	{
		if (mInverseMass == 0.f)
		{
			return mInverseMass;
		}
		return 1.0f / mInverseMass;
	}

	float cParticle::GetInverseMass() const
	{
		return mInverseMass;
	}

	bool cParticle::IsStaticObject() const
	{
		return mInverseMass == 0.f;
	}

	glm::vec3 cParticle::GetPosition() const
	{
		return mPosition;
	}
	void cParticle::GetPosition(glm::vec3& position)
	{
		position = mPosition;
	}
	void cParticle::SetPosition(const glm::vec3& position)
	{
		mPosition = position;
	}

	glm::vec3 cParticle::GetVelocity() const
	{
		return mVelocity;
	}
	void cParticle::GetVelocity(glm::vec3& velocity)
	{
		velocity = mVelocity;
	}
	void cParticle::SetVelocity(const glm::vec3& velocity)
	{
		mVelocity = velocity;
	}

	glm::vec3 cParticle::GetAcceleration() const
	{
		return mAcceleration;
	}
	void cParticle::GetAcceleration(glm::vec3& acceleration)
	{
		acceleration = mAcceleration;
	}
	void cParticle::SetAcceleration(const glm::vec3& acceleration)
	{
		mAcceleration = acceleration;
	}


	void cParticle::Integrate(float deltaTime)
	{
		if (mInverseMass == 0.f)
		{
			return; // static things don't move!
		}

		mPosition += mVelocity * deltaTime;
		// F*(1/m) = a
		mVelocity += mAcceleration * deltaTime;

		// apply damping
		mVelocity *= glm::pow(mDamping, deltaTime);

		// clear applied forces
	}
}