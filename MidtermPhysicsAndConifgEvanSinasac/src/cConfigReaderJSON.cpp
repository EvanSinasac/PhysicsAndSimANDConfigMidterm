//Evan Sinasac - 1081418
//INFO6019 Physics and Simulation and INFO6025 Configuration and Deployment Midterm
//cConfigReader.cpp description:
//						Reads config.json file and stores information in private structures


#include "cConfigReaderJSON.h"
#include <iostream>
#include <sstream>

cConfigReaderJSON::cConfigReaderJSON()
{
	//Should probably put default safety values here... yeah
}
cConfigReaderJSON::~cConfigReaderJSON()
{

}
//read the config file
bool cConfigReaderJSON::readConfigFile(const std::string& filePath)
{
	using namespace rapidjson;

	FILE* fp = 0;
	fopen_s(&fp, filePath.c_str(), "rb");

	if (!fp)
	{
		std::cout << "fp didn't open" << std::endl;
		return false;
	}

	//char readBuffer[65536];
	char* readBuffer = new char[15000];
	FileReadStream is(fp, readBuffer, sizeof(readBuffer));

	Document d;
	d.ParseStream(is);

	if (!d["Cannon"].IsObject())
	{
		std::cout << "Cannon object didn't exist" << std::endl;
		return false;
	}
	else
	{
		if (!d["Cannon"]["YawMax"].IsFloat())
		{
			std::cout << "YawMax wasn't a float!" << std::endl;
		}
		else
		{
			cannonInfo.yawMax = d["Cannon"]["YawMax"].GetFloat();
		}

		if (!d["Cannon"]["YawMin"].IsFloat())
		{
			std::cout << "YawMin wasn't a float!" << std::endl;
		}
		else
		{
			cannonInfo.yawMin = d["Cannon"]["YawMin"].GetFloat();
		}

		if (!d["Cannon"]["PitchMax"].IsFloat())
		{
			std::cout << "PitchMax wasn't a float!" << std::endl;
		}
		else
		{
			cannonInfo.pitchMax = d["Cannon"]["PitchMax"].GetFloat();
		}

		if (!d["Cannon"]["PitchMin"].IsFloat())
		{
			std::cout << "PitchMin wasn't a float!" << std::endl;
		}
		else
		{
			cannonInfo.pitchMin = d["Cannon"]["PitchMin"].GetFloat();
		}

		if (!d["Cannon"]["XPosition"].IsFloat())
		{
			std::cout << "XPosition wasn't a float!" << std::endl;
		}
		else
		{
			cannonInfo.position.x = d["Cannon"]["XPosition"].GetFloat();
		}

		if (!d["Cannon"]["YPosition"].IsFloat())
		{
			std::cout << "YPosition wasn't a float!" << std::endl;
		}
		else
		{
			cannonInfo.position.y = d["Cannon"]["YPosition"].GetFloat();
		}

		if (!d["Cannon"]["ZPosition"].IsFloat())
		{
			std::cout << "ZPosition wasn't a float!" << std::endl;
		}
		else
		{
			cannonInfo.position.z = d["Cannon"]["ZPosition"].GetFloat();
		}
	}

	if (!d["Bullet"].IsObject())
	{
		std::cout << "Bullet isn't an object!" << std::endl;
		return false;
	}
	else
	{
		if (!d["Bullet"]["Size"].IsFloat())
		{
			std::cout << "Bullet Size isn't a float!" << std::endl;
			return false;
		}
		else
		{
			bulletInfo.size = d["Bullet"]["Size"].GetFloat();
		}

		if (!d["Bullet"]["Damping"].IsFloat())
		{
			std::cout << "Bullet Damping isn't a float!" << std::endl;
			return false;
		}
		else
		{
			bulletInfo.damping = d["Bullet"]["Damping"].GetFloat();
		}

		if (!d["Bullet"]["Mass"].IsFloat())
		{
			std::cout << "Bullet Mass isn't a float!" << std::endl;
			return false;
		}
		else
		{
			bulletInfo.mass = d["Bullet"]["Mass"].GetFloat();
		}

		if (!d["Bullet"]["MuzzleVelocity"].IsFloat())
		{
			std::cout << "Bullet MuzzleVelocity isn't a float!" << std::endl;
			return false;
		}
		else
		{
			bulletInfo.velocityMag = d["Bullet"]["MuzzleVelocity"].GetFloat();
		}
	}

	if (!d["Laser"].IsObject())
	{
		std::cout << "Laser isn't an object!" << std::endl;
		return false;
	}
	else
	{
		if (!d["Laser"]["Size"].IsFloat())
		{
			std::cout << "Laser Size isn't a float!" << std::endl;
			return false;
		}
		else
		{
			laserInfo.size = d["Laser"]["Size"].GetFloat();
		}

		if (!d["Laser"]["Damping"].IsFloat())
		{
			std::cout << "Laser Damping isn't a float!" << std::endl;
			return false;
		}
		else
		{
			laserInfo.damping = d["Laser"]["Damping"].GetFloat();
		}

		if (!d["Laser"]["Mass"].IsFloat())
		{
			std::cout << "Laser Mass isn't a float!" << std::endl;
			return false;
		}
		else
		{
			laserInfo.mass = d["Laser"]["Mass"].GetFloat();
		}

		if (!d["Laser"]["MuzzleVelocity"].IsFloat())
		{
			std::cout << "Laser MuzzleVelocity isn't a float!" << std::endl;
			return false;
		}
		else
		{
			laserInfo.velocityMag = d["Laser"]["MuzzleVelocity"].GetFloat();
		}

		if (!d["Laser"]["DistanceLimit"].IsFloat())
		{
			std::cout << "Laser DistanceLimit isn't a float!" << std::endl;
			return false;
		}
		else
		{
			laserInfo.distanceLimit = d["Laser"]["DistanceLimit"].GetFloat();
		}
	}

	if (!d["CannonBall"].IsObject())
	{
		std::cout << "CannonBall isn't an object!" << std::endl;
		return false;
	}
	else
	{
		if (!d["CannonBall"]["Size"].IsFloat())
		{
			std::cout << "CannonBall Size isn't a float!" << std::endl;
			return false;
		}
		else
		{
			cannonBallInfo.size = d["CannonBall"]["Size"].GetFloat();
		}

		if (!d["CannonBall"]["Damping"].IsFloat())
		{
			std::cout << "CannonBall Damping isn't a float!" << std::endl;
			return false;
		}
		else
		{
			cannonBallInfo.damping = d["CannonBall"]["Damping"].GetFloat();
		}

		if (!d["CannonBall"]["Mass"].IsFloat())
		{
			std::cout << "CannonBall Mass isn't a float!" << std::endl;
			return false;
		}
		else
		{
			cannonBallInfo.mass = d["CannonBall"]["Mass"].GetFloat();
		}

		if (!d["CannonBall"]["MuzzleVelocity"].IsFloat())
		{
			std::cout << "CannonBall MuzzleVelocity isn't a float!" << std::endl;
			return false;
		}
		else
		{
			cannonBallInfo.velocityMag = d["CannonBall"]["MuzzleVelocity"].GetFloat();
		}
	}

	if (!d["EnergyBall"].IsObject())
	{
		std::cout << "EnergyBall isn't an object!" << std::endl;
		return false;
	}
	else
	{
		if (!d["EnergyBall"]["Size"].IsFloat())
		{
			std::cout << "EnergyBall Size isn't a float!" << std::endl;
			return false;
		}
		else
		{
			energyBallInfo.size = d["EnergyBall"]["Size"].GetFloat();
		}

		if (!d["EnergyBall"]["Damping"].IsFloat())
		{
			std::cout << "EnergyBall Damping isn't a float!" << std::endl;
			return false;
		}
		else
		{
			energyBallInfo.damping = d["EnergyBall"]["Damping"].GetFloat();
		}

		if (!d["EnergyBall"]["Mass"].IsFloat())
		{
			std::cout << "EnergyBall Mass isn't a float!" << std::endl;
			return false;
		}
		else
		{
			energyBallInfo.mass = d["EnergyBall"]["Mass"].GetFloat();
		}

		if (!d["EnergyBall"]["MuzzleVelocity"].IsFloat())
		{
			std::cout << "EnergyBall MuzzleVelocity isn't a float!" << std::endl;
			return false;
		}
		else
		{
			energyBallInfo.velocityMag = d["EnergyBall"]["MuzzleVelocity"].GetFloat();
		}

		if (!d["EnergyBall"]["TimeLimit"].IsFloat())
		{
			std::cout << "EnergyBall TimeLimit isn't a float!" << std::endl;
			return false;
		}
		else
		{
			energyBallInfo.timeLimit = d["EnergyBall"]["TimeLimit"].GetFloat();
		}

		if (!d["EnergyBall"]["DistanceLimit"].IsFloat())
		{
			std::cout << "EnergyBall DistanceLimit isn't a float!" << std::endl;
			return false;
		}
		else
		{
			energyBallInfo.distanceLimit = d["EnergyBall"]["DistanceLimit"].GetFloat();
		}
	}

	fclose(fp);
	return true;
}

//Cannon Info
float cConfigReaderJSON::GetCannonYawMax()
{
	return cannonInfo.yawMax;
}
float cConfigReaderJSON::GetCannonYawMin()
{
	return cannonInfo.yawMin;
}
float cConfigReaderJSON::GetCannonPitchMax()
{
	return cannonInfo.pitchMax;
}
float cConfigReaderJSON::GetCannonPitchMin()
{
	return cannonInfo.pitchMin;
}
glm::vec3 cConfigReaderJSON::GetCannonPosition()
{
	return cannonInfo.position;
}

//Bullet Info
float cConfigReaderJSON::GetBulletSize()
{
	return bulletInfo.size;
}
float cConfigReaderJSON::GetBulletDamping()
{
	return bulletInfo.damping;
}
float cConfigReaderJSON::GetBulletMass()
{
	return bulletInfo.mass;
}
float cConfigReaderJSON::GetBulletVelocity()
{
	return bulletInfo.velocityMag;
}

//Laser Info
float cConfigReaderJSON::GetLaserSize()
{
	return laserInfo.size;
}
float cConfigReaderJSON::GetLaserDamping()
{
	return laserInfo.damping;
}
float cConfigReaderJSON::GetLaserMass()
{
	return laserInfo.mass;
}
float cConfigReaderJSON::GetLaserVelocity()
{
	return laserInfo.velocityMag;
}
float cConfigReaderJSON::GetLaserDistanceLimit()
{
	return laserInfo.distanceLimit;
}

//Cannon Ball Info
float cConfigReaderJSON::GetCannonBallSize()
{
	return cannonBallInfo.size;
}
float cConfigReaderJSON::GetCannonBallDamping()
{
	return cannonBallInfo.damping;
}
float cConfigReaderJSON::GetCannonBallMass()
{
	return cannonBallInfo.mass;
}
float cConfigReaderJSON::GetCannonBallVelocity()
{
	return cannonBallInfo.velocityMag;
}

//Energy Ball Info
float cConfigReaderJSON::GetEnergyBallSize()
{
	return energyBallInfo.size;
}
float cConfigReaderJSON::GetEnergyBallDamping()
{
	return energyBallInfo.damping;
}
float cConfigReaderJSON::GetEnergyBallMass()
{
	return energyBallInfo.mass;
}
float cConfigReaderJSON::GetEnergyBallVelocity()
{
	return energyBallInfo.velocityMag;
}
float cConfigReaderJSON::GetEnergyBallTimeLimit()
{
	return energyBallInfo.timeLimit;
}
float cConfigReaderJSON::GetEnergyBallDistanceLimit()
{
	return energyBallInfo.distanceLimit;
}