//Evan Sinasac - 1081418
//INFO6019 Physics and Simulation and INFO6025 Configuration and Deployment Midterm
//cConfigReader.h description:
//						Reads config.json file and stores information in private structures

#pragma once
#include <string>
#include <rapidjson/filereadstream.h>
#include <rapidjson/document.h>
#include <vector>
#include <../src/GLMCommon.h>

struct CannonInfo
{
	float yawMin;
	float yawMax;
	float pitchMax;
	float pitchMin;
	glm::vec3 position;
};

struct BulletInfo
{
	float size;
	float damping;
	float mass;
	float velocityMag;
};

struct LaserInfo
{
	float size;
	float damping;
	float mass;
	float velocityMag;
	float distanceLimit;
};

struct CannonBallInfo
{
	float size;
	float damping;
	float mass;
	float velocityMag;
};

struct EnergyBallInfo
{
	float size;
	float damping;
	float mass;
	float velocityMag;
	float timeLimit;
	float distanceLimit;
};

class cConfigReaderJSON
{
private:
	CannonInfo cannonInfo;
	BulletInfo bulletInfo;
	LaserInfo laserInfo;
	CannonBallInfo cannonBallInfo;
	EnergyBallInfo energyBallInfo;

public:
	cConfigReaderJSON();
	~cConfigReaderJSON();

	bool readConfigFile(const std::string& filePath);
	
	//Cannon Info
	float GetCannonYawMax();
	float GetCannonYawMin();
	float GetCannonPitchMax();
	float GetCannonPitchMin();
	glm::vec3 GetCannonPosition();

	//Bullet Info
	float GetBulletSize();
	float GetBulletDamping();
	float GetBulletMass();
	float GetBulletVelocity();

	//Laser Info
	float GetLaserSize();
	float GetLaserDamping();
	float GetLaserMass();
	float GetLaserVelocity();
	float GetLaserDistanceLimit();

	//Cannon Ball Info
	float GetCannonBallSize();
	float GetCannonBallDamping();
	float GetCannonBallMass();
	float GetCannonBallVelocity();

	//Energy Ball Info
	float GetEnergyBallSize();
	float GetEnergyBallDamping();
	float GetEnergyBallMass();
	float GetEnergyBallVelocity();
	float GetEnergyBallTimeLimit();
	float GetEnergyBallDistanceLimit();
};
