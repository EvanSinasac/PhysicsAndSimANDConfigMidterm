//Evan Sinasac - 1081418
//INFO6019 Physics and Simulation and INFO6025 Configuration and Deployment Midterm
//main.cpp description:
//			        Midterm program made over 2 days, main classes and functions used and modified from previous projects
//					(mainly Project2Physics and Project2ConfigAndDeployment).  We are making a little cannon program that
//					can fire 4 different kinds of shots, their properties loaded from external configuration files

//common includes for GL and GLM
#include "GLCommon.h"
#include "GLMCommon.h"
//standard includes
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>
//graphics includes
#include "../shaderManager/cVAOManager.h"
#include "../shaderManager/cMesh.h"
#include "../shaderManager/cShaderManager.h"
//physics includes
#include <physics/cParticleWorld.h>
#include <physics/random_Helpers.h>
//Midterm
#include <physics/cBullet.h>
#include <physics/cLaser.h>
#include <physics/cCannonBall.h>
#include <physics/cEnergyBall.h>
#include "cConfigReaderJSON.h"


//Definitions
#define SCREEN_WIDTH 1200.0f
#define SCREEN_HEIGHT 1000.0f

//Globals
glm::vec3 cameraEye = glm::vec3(0.0f, 5.0f, -30.0f);
glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 1.0f);
glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f);			//default upVector for camera

float lastX = SCREEN_WIDTH / 2.0f;
float lastY = SCREEN_HEIGHT / 2.0f;
bool firstMouse = true;
float cameraYaw = 90.0f;
float cameraPitch = 0.0f;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

cVAOManager		gVAOManager;
cShaderManager	gShaderManager;
//vector of meshes in the scene to draw
std::vector<cMesh> g_vecMeshesToDraw;
//vector of string of where the models are located
std::vector<std::string> modelLocations;
//TO-DO: Particle world
nPhysics::cParticleWorld* world;
//Vector of Particles for cannon shots
std::vector<nPhysics::cCannonShot*> cannonShots;

//Cannon model stuff
glm::vec3 cannonPosition = glm::vec3(0.0f, 1.9f, 0.0f);
float cannonYaw = glm::pi<float>();							//Because of the mdoel's orientation, an inital Yaw of pi rotates to face the cannon in positive x
float cannonPitch = 3.0f * glm::pi<float>() / 4.0f;			//And an inital Pitch of pi/2 makes it parallel with the xz-plane (3pi/4 just to give it an interesting inital angle)
//Set by config file
float cannonYawMax;
float cannonYawMin;
float cannonPitchMax;
float cannonPitchMin;

cConfigReaderJSON config;	//global so I can call it it key_callback

//callbacks
static void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error %s\n", description);
}

static void key_callback	(GLFWwindow* window, int key, int scancode, int action, int mods);
static void mouse_callback	(GLFWwindow* window, double xpos, double ypos);

//Used to add models to VAO Manager
bool loadModelsFromFile(GLuint& program);

//Identifiers
void AddCannonShot(int shotType, glm::vec3 position, glm::vec3 velocity);
void AddMeshes(glm::vec3 position, float scale, glm::vec3 colour);

int main(int argc, char** argv)
{
	std::stringstream ss;
	//ss << SOLUTION_DIR << "MidtermPhysicsAndConifgEvanSinasac\\src\\Config.json";	
	ss << "Config.json";			//moved file to project level, now this works
	if (!config.readConfigFile(ss.str()))
	{
		std::cout << "config didn't read good!" << std::endl;
		return false;
	}
	ss.str("");

	//load cannon information from config
	cannonYawMax = config.GetCannonYawMax();
	cannonYawMin = config.GetCannonYawMin();

	cannonPitchMax = config.GetCannonPitchMax();
	cannonPitchMin = config.GetCannonPitchMin();

	cannonPosition = config.GetCannonPosition();
	

	GLFWwindow* window;

	GLuint program = 0;

	GLint mvp_location  = -1;
	GLint vpos_location = -1;
	GLint vcol_location = -1;

	float fpsFrameCount  = 0.0f;
	float fpsTimeElapsed = 0.0f;

	world = new nPhysics::cParticleWorld();

	float timeElapsed = 0.0f;

	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
	{
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Cannon Game!", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glfwSwapInterval(1);

	//OpenGL error checks omitted for brevity

	cShaderManager::cShader vertShader;
	ss << SOLUTION_DIR << "common\\assets\\shaders\\vertShader_01.glsl";
	vertShader.fileName = ss.str().c_str();
	ss.str("");

	cShaderManager::cShader fragShader;
	ss << SOLUTION_DIR << "common\\assets\\shaders\\fragShader_01.glsl";
	fragShader.fileName = ss.str().c_str();
	ss.str("");

	if (gShaderManager.createProgramFromFile("Shader#1", vertShader, fragShader))
	{
		std::cout << "Shader compuled OK" << std::endl;
		program = gShaderManager.getIDFromFriendlyName("Shader#1");
	}
	else
	{
		std::cout << "Error making shader program: " << std::endl;
		std::cout << gShaderManager.getLastError() << std::endl;
	}

	//Getting the locations from the shaders
	GLint matModel_Location = glGetUniformLocation(program, "matModel");
	GLint matView_Location = glGetUniformLocation(program, "matView");
	GLint matProjection_Location = glGetUniformLocation(program, "matProjection");

	GLint matModelInverseTranspose_Location = glGetUniformLocation(program, "matModelInversetranspose");

	//Will remove colour from models, will be assigned by World file
	GLint bUseVertexColour_Location = glGetUniformLocation(program, "bUseVertexColour");
	GLint vertexColourOverride_Location = glGetUniformLocation(program, "vertexColourOverride");

	if (loadModelsFromFile(program))
	{
		std::cout << "loadModelsFromFile finished ok" << std::endl;
	}
	else
	{
		std::cout << "loadModelsFromFile did not finish ok, aborting" << std::endl;
		return -1;
	}

	cMesh platform;
	if (1 < modelLocations.size())
	{
		platform.meshName = modelLocations[1];
		platform.positionXYZ = glm::vec3(5.0f + 25.0f, 0.025f, -5.0f - 25.0f);
		platform.orientationXYZ = glm::vec3(3.0f * glm::pi<float>() / 2.0f, 0.0f, 0.0f);
		platform.scale = 1.675f * 5.0f;
		platform.bOverrideColour = true;
		platform.vertexColourOverride = glm::vec3(96.0f / 255.0f, 96.0f / 255.0f, 96.0f / 255.0f);

		g_vecMeshesToDraw.push_back(platform);		//[0]
	}
	
	cMesh cannon;
	if (2 < modelLocations.size())
	{
		cannon.meshName = modelLocations[2];
		cannon.positionXYZ = cannonPosition;
		cannon.orientationXYZ = glm::vec3(cannonPitch, cannonYaw, 0.0f);
		cannon.scale = 0.3f;
		//cannon.bIsWireframe = true;
		cannon.bOverrideColour = true;
		cannon.vertexColourOverride = glm::vec3(153.0f / 255.0f, 0.0f, 0.0f);

		g_vecMeshesToDraw.push_back(cannon);		//[1]
	}
	//Platform and cannon models, so our vector of meshes will have 2 in it always

	//Face the camera towards the origin
	if (cameraEye.x > 0 && cameraEye.z > 0)
	{
		cameraYaw = 180.f + (atan(cameraEye.z / cameraEye.x) * 180.f / 3.1459f);
	}
	else if (cameraEye.x > 0 && cameraEye.z < 0)
	{
		cameraYaw = 90.f - (atan(cameraEye.z / cameraEye.x) * 180.f / 3.1459f);
	}
	else if (cameraEye.x < 0 && cameraEye.z > 0)
	{
		cameraYaw = (atan(cameraEye.z / cameraEye.x) * 180.f / 3.1459f);
	}
	else if (cameraEye.x < 0 && cameraEye.z < 0)
	{
		cameraYaw = (atan(cameraEye.z / cameraEye.x) * 180.f / 3.1459);
	}
	else if (cameraEye.x == 0.f)
	{
		if (cameraEye.z >= 0.f)
		{
			cameraYaw = 270.f;
		}
		else
		{
			cameraYaw = 90.f;
		}
	}
	else if (cameraEye.z == 0.f)
	{
		if (cameraEye.x <= 0)
		{
			cameraYaw = 0.f;
		}
		else
		{
			cameraYaw = 180.f;
		}
	}
	cameraTarget = glm::vec3(-1.f * cameraEye.x, 0, -1.f * cameraEye.z);
	glm::normalize(cameraTarget);

	//Main loop
	while (!glfwWindowShouldClose(window))
	{
		float ratio;
		int width, height;
		glm::mat4 matModel;
		glm::mat4 p;
		glm::mat4 v;
		glm::mat4 mvp;

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float)height;

		//Turn on the depth buffer
		glEnable(GL_DEPTH);			//Turns on the depth buffer
		glEnable(GL_DEPTH_TEST);	//Check if the pixel is already closer

		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//FPS Title stuff		//Copied from SimpleGameApp
		{
			fpsTimeElapsed += deltaTime;
			fpsFrameCount += 1.0f;
			if (fpsTimeElapsed >= 0.03f)
			{
				std::string fps = std::to_string(fpsFrameCount / fpsTimeElapsed);
				std::string ms = std::to_string(1000.0f * fpsTimeElapsed / fpsFrameCount);
				std::string newTitle = "Cannon Game!   FPS: " + fps + "   MS: " + ms;
				glfwSetWindowTitle(window, newTitle.c_str());
				//reset times and counts
				fpsTimeElapsed = 0.0f;
				fpsFrameCount = 0.0f;
			}
		}

		//Integrate all particles in the world
		world->TimeStep(deltaTime);

		//Update mesh positions after updating particle position
		for (int index = 0; index < cannonShots.size(); index++)
		{
			//Have 2 models (floor and cannon) so when we're adding particles we have to compare with a buffer
			g_vecMeshesToDraw[index + 2].positionXYZ = cannonShots[index]->GetPosition();
			
			//Thought it'd be cewl to have the energy shot get bigger over time, but it just looks a little too confusing
			/*if (cannonShots[index]->GetType() == 4)
			{
				g_vecMeshesToDraw[index + 2].scale += 10.0f * deltaTime;
			}*/

			//If the particle has reached it's death condition we can remove it from the world, the vector and it's corresponding mesh can be removed
			if (!cannonShots[index]->GetIsAlive())
			{
				world->RemoveParticle(cannonShots[index]);
				cannonShots.erase(cannonShots.begin() + index);
				g_vecMeshesToDraw.erase(g_vecMeshesToDraw.begin() + index + 2);
				index--;
			}
		} //end of for


		//*********************************************************************
		//Screen is cleared and we're ready to draw
		//*********************************************************************
		for (unsigned int index = 0; index != g_vecMeshesToDraw.size(); index++)
		{
			cMesh curMesh = g_vecMeshesToDraw[index];

			//         mat4x4_identity(m);
			matModel = glm::mat4(1.0f);     //"Identity" ("do nothing", like x1)
			//*********************************************************************
			//Translate or "move the object
			//*********************************************************************
			glm::mat4 matTranslate = glm::translate(glm::mat4(1.0f),
				curMesh.positionXYZ);

			//matModel = matModel * matTranslate;
			//*********************************************************************
			//
			//*********************************************************************
			//Rotation around the Z-axis
			//mat4x4_rotate_Z(m, m, (float) glfwGetTime());
			glm::mat4 rotateZ = glm::rotate(glm::mat4(1.0f),
				curMesh.orientationXYZ.z,   //(float)glfwGetTime()
				glm::vec3(0.0f, 0.0, 1.0f));

			//matModel = matModel * rotateZ;
			//*********************************************************************
			//
			//*********************************************************************
			//Rotation around the Y-axis
			glm::mat4 rotateY = glm::rotate(glm::mat4(1.0f),
				curMesh.orientationXYZ.y,   //(float)glfwGetTime()
				glm::vec3(0.0f, 1.0, 0.0f));

			//matModel = matModel * rotateY;
			//*********************************************************************
			//
			//*********************************************************************
			//Rotation around the X-axis
			glm::mat4 rotateX = glm::rotate(glm::mat4(1.0f),
				curMesh.orientationXYZ.x,   //(float)glfwGetTime()
				glm::vec3(1.0f, 0.0, 0.0f));

			//matModel = matModel * rotateX;
			//*********************************************************************
			//Scale
			//*********************************************************************
			glm::mat4 matScale = glm::scale(glm::mat4(1.0f),
				glm::vec3(curMesh.scale,  // Scale in X
					curMesh.scale,  // Scale in Y
					curMesh.scale));// Scale in Z

		//matModel = matModel * matScale;
		//*********************************************************************
		// 
		//*********************************************************************
		//mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
			matModel = matModel * matTranslate;
			matModel = matModel * rotateZ;
			matModel = matModel * rotateY;
			matModel = matModel * rotateX;
			matModel = matModel * matScale;


			p = glm::perspective(0.6f,
				ratio,
				0.1f,
				1000.0f);

			v = glm::mat4(1.0f);

			v = glm::lookAt(cameraEye,          // "eye"
				cameraEye + cameraTarget,       // "at" //used to be    cameraTarget
				upVector);

			//mat4x4_mul(mvp, p, m);
			mvp = p * v * matModel;
			glUseProgram(program);


			//glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*) mvp);
			glUniformMatrix4fv(mvp_location, 1, GL_FALSE, glm::value_ptr(mvp));

			//Don't need this anymore since it's being done inside the shader
			//mvp = p * v * matModel;
			glUniformMatrix4fv(matModel_Location, 1, GL_FALSE, glm::value_ptr(matModel));
			glUniformMatrix4fv(matView_Location, 1, GL_FALSE, glm::value_ptr(v));
			glUniformMatrix4fv(matProjection_Location, 1, GL_FALSE, glm::value_ptr(p));

			//Inverse transpose of the mdoel matrix
			//(Used to calculate the normal location in vertex space
			glm::mat4 matInvTransposeModel = glm::inverse(glm::transpose(matModel));
			glUniformMatrix4fv(matModelInverseTranspose_Location, 1, GL_FALSE, glm::value_ptr(matInvTransposeModel));

			//Colour Override HACK (will be removed/changed later)
			if (curMesh.bOverrideColour)
			{
				glUniform1f(bUseVertexColour_Location, (float)GL_TRUE);
				glUniform3f(vertexColourOverride_Location,
					curMesh.vertexColourOverride.r,
					curMesh.vertexColourOverride.g,
					curMesh.vertexColourOverride.b);
			}
			else
			{
				glUniform1f(bUseVertexColour_Location, (float)GL_FALSE);
			}

			//Wireframe
			if (curMesh.bIsWireframe)
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			}
			else
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}
			//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				//GL_POINT, GL_LINE, and GL_FILL


			sModelDrawInfo modelInfo;
			if (gVAOManager.FindDrawInfoByModelName(g_vecMeshesToDraw[index].meshName, modelInfo))
			{
				glBindVertexArray(modelInfo.VAO_ID);
				glDrawElements(GL_TRIANGLES,
					modelInfo.numberOfIndices,
					GL_UNSIGNED_INT,
					(void*)0);
				glBindVertexArray(0);
			}

		} //end of for
		//Scene is drawn

		//"Present" what we've drawn
		glfwSwapBuffers(window);
		glfwPollEvents();
	} //end of while
	glfwDestroyWindow(window);

	glfwTerminate();
	exit(EXIT_SUCCESS);
} //end of main

//Same loadModelsFromFile used in graphics and Project2Physics
bool loadModelsFromFile(GLuint& program)
{
	std::stringstream ss;
	std::stringstream sFile;
	sModelDrawInfo curModel;

	ss << SOLUTION_DIR << "common\\assets\\models\\modelsToLoad.txt";

	std::ifstream theFile(ss.str());
	if (!theFile.is_open())
	{
		fprintf(stderr, "Could not open modelsToLoad.txt");
		return false;
	}
	std::string nextToken;
	ss.str("");

	while (theFile >> nextToken)
	{
		if (nextToken == "end")
		{
			break;
		}

		if (nextToken.find("ply") != std::string::npos)
		{
			sFile << nextToken.c_str();
			ss << SOLUTION_DIR << "common\\assets\\models\\" << sFile.str().c_str();
			modelLocations.push_back(ss.str().c_str());
			if (gVAOManager.LoadModelIntoVAO(ss.str().c_str(), curModel, program))
			{
				std::cout << "Loaded the " << sFile.str().c_str() << " model OK" << std::endl;
			}
			else
			{
				fprintf(stderr, "Error loading model");
			}
			ss.str("");
			sFile.str("");
		}
		else
		{
			sFile << nextToken.c_str() << " ";
		} //end else
	} //end while
	theFile.close();
	return true;
} //end of loadModelsFromFile

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
	//speed of camera/cannon determined by deltaTime for a smoother experience
	float cameraSpeed = 20.0f * deltaTime;
	float cannonSpeed = 1.5f * deltaTime;

	//Basic camera controls
	if (key == GLFW_KEY_A) { cameraEye -= glm::normalize(glm::cross(cameraTarget, upVector)) * cameraSpeed; }				//Go left
	if (key == GLFW_KEY_D) { cameraEye += glm::normalize(glm::cross(cameraTarget, upVector)) * cameraSpeed;	}				//Go right
	if (key == GLFW_KEY_S) { cameraEye -= glm::vec3(cameraTarget.x * cameraSpeed, 0.0f, cameraTarget.z * cameraSpeed); }	//Go back
	if (key == GLFW_KEY_W) { cameraEye += glm::vec3(cameraTarget.x * cameraSpeed, 0.0f, cameraTarget.z * cameraSpeed); }	//Go forward
	if (key == GLFW_KEY_Q) { cameraEye -= upVector * cameraSpeed; }															//Go down
	if (key == GLFW_KEY_E) { cameraEye += upVector * cameraSpeed; }															//Go up

	//Cannon controls to change the direction the cannon faces
	if (key == GLFW_KEY_UP)
	{
		cannonPitch += cannonSpeed;
		//Because of the model, an inital Pitch of pi/2 is required to make the cannon appear "horizontal"
		//So to have a Pitch range of (0, 3pi/8) (just barely not vertical) we start there, 
		//so the angle range is (pi/2, pi/2 + 3pi/8) -> (pi/2, 7pi/8)
		//if (cannonPitch >= 7.0f * glm::pi<float>() / 8)
		//{
		//	cannonPitch = 7.0f * glm::pi<float>() / 8;
		//}
		if (cannonPitch >= glm::radians(cannonPitchMax))
		{
			cannonPitch = glm::radians(cannonPitchMax);
		}
		g_vecMeshesToDraw[1].orientationXYZ = glm::vec3(cannonPitch, cannonYaw, 0.0f);
	}
	if (key == GLFW_KEY_DOWN)
	{
		cannonPitch -= cannonSpeed;
		//Because of the model an inital Pitch of pi/2 is required to make the cannon appear "horizontal"
		//So to have a Pitch range of (0, 3pi/8) we start there,
		//so the angle range is (pi/2, pi/2 + 3pi/8) -> (pi/2, 7pi/8)
		//if (cannonPitch <= glm::pi<float>() / 2.0f)
		//{
		//	cannonPitch = glm::pi<float>() / 2.0f;
		//}
		if (cannonPitch <= glm::radians(cannonPitchMin))
		{
			cannonPitch = glm::radians(cannonPitchMin);
		}
		g_vecMeshesToDraw[1].orientationXYZ = glm::vec3(cannonPitch, cannonYaw, 0.0f);
	}
	if (key == GLFW_KEY_RIGHT)
	{
		cannonYaw -= cannonSpeed;
		//Because of the model, an inital Yaw of pi is required to make the cannon face positive z
		//So to have a Yaw range of (-pi/4, pi/4) we will add/subtract that from the inital
		//So the actual angle range is (-pi/4 + pi, pi/4 + pi) -> (3pi/4, 5pi/4)
		//if (cannonYaw <= 3.0f * glm::pi<float>() / 4.0f)
		//{
		//	cannonYaw = 3.0f * glm::pi<float>() / 4.0f;
		//}
		if (cannonYaw <= glm::radians(cannonYawMin))
		{
			cannonYaw = glm::radians(cannonYawMin);
		}
		g_vecMeshesToDraw[1].orientationXYZ = glm::vec3(cannonPitch, cannonYaw, 0.0f);
	}
	if (key == GLFW_KEY_LEFT)
	{
		cannonYaw += cannonSpeed;
		//Because of the model, an inital Yaw of pi is required to make the cannon face positive z
		//So to have a Yaw range of (-pi/4, pi/4) we will add/subtract that from the inital
		//So the actual angle range is (-pi/4 + pi, pi/4 + pi) -> (3pi/4, 5pi/4)
		if (cannonYaw >= glm::radians(cannonYawMax))
		{
			cannonYaw = glm::radians(cannonYawMax);
		}
		g_vecMeshesToDraw[1].orientationXYZ = glm::vec3(cannonPitch, cannonYaw, 0.0f);
	}

	if (key == GLFW_KEY_1 && action == GLFW_PRESS)
	{
		//Bullet
		glm::vec3 velocity;
		float realCannonPitch = cannonPitch - (glm::pi<float>() / 2.0f);
		float realCannonYaw = cannonYaw - glm::pi<float>();
		velocity.z = cos(realCannonYaw) * cos(realCannonPitch);
		velocity.y = sin(realCannonPitch);
		velocity.x = sin(realCannonYaw) * cos(realCannonPitch);
		velocity = glm::normalize(velocity);
		velocity *= config.GetBulletVelocity();		

		AddCannonShot(1, glm::vec3(cannonPosition.x, cannonPosition.y - 1, cannonPosition.z), velocity);
	}
	if (key == GLFW_KEY_2 && action == GLFW_PRESS)
	{
		//Laser
		glm::vec3 velocity;
		float realCannonPitch = cannonPitch - (glm::pi<float>() / 2.0f);
		float realCannonYaw = cannonYaw - glm::pi<float>();
		velocity.z = cos(realCannonYaw) * cos(realCannonPitch);
		velocity.y = sin(realCannonPitch);
		velocity.x = sin(realCannonYaw) * cos(realCannonPitch);
		velocity = glm::normalize(velocity);
		velocity *= config.GetLaserVelocity();
		AddCannonShot(2, glm::vec3(cannonPosition.x, cannonPosition.y - 1, cannonPosition.z), velocity);
	}
	if (key == GLFW_KEY_3 && action == GLFW_PRESS)
	{
		//Cannon Ball
		glm::vec3 velocity;
		float realCannonPitch = cannonPitch - (glm::pi<float>() / 2.0f);
		float realCannonYaw = cannonYaw - glm::pi<float>();
		velocity.z = cos(realCannonYaw) * cos(realCannonPitch);
		velocity.y = sin(realCannonPitch);
		velocity.x = sin(realCannonYaw) * cos(realCannonPitch);
		velocity = glm::normalize(velocity);
		velocity *= config.GetCannonBallVelocity();
		AddCannonShot(3, glm::vec3(cannonPosition.x, cannonPosition.y - 1, cannonPosition.z), velocity);
	}
	if (key == GLFW_KEY_4 && action == GLFW_PRESS)
	{
		//Energy Ball
		glm::vec3 velocity;
		float realCannonPitch = cannonPitch - (glm::pi<float>() / 2.0f);
		float realCannonYaw = cannonYaw - glm::pi<float>();
		velocity.z = cos(realCannonYaw) * cos(realCannonPitch);
		velocity.y = sin(realCannonPitch);
		velocity.x = sin(realCannonYaw) * cos(realCannonPitch);
		velocity = glm::normalize(velocity);
		velocity *= config.GetEnergyBallVelocity();
		AddCannonShot(4, glm::vec3(cannonPosition.x, cannonPosition.y - 1, cannonPosition.z), velocity);
	}

} //end of key_callback

//Figured out the math for how to do this from https://learnopengl.com/Getting-started/Camera and http://www.opengl-tutorial.org/beginners-tutorials/tutorial-6-keyboard-and-mouse/
//Using the mouse position we calculate the direction that the camera will be facing
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	//if it's the start of the program this smooths out a potentially glitchy jump
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
	//find the offset of where the mouse positions have moved
	float xOffset = xpos - lastX;
	float yOffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;
	//multiply by sensitivity so that it's not potentially crazy fast
	float sensitivity = 0.1f;
	xOffset *= sensitivity;
	yOffset *= sensitivity;

	cameraYaw += xOffset;         // The yaw is the rotation around the camera's y-axis (so we want to add the xOffset to it)
	cameraPitch += yOffset;       // The pitch is the rotation around the camera's x-axis (so we want to add the yOffset to it)
	//This limits the pitch so that we can't just spin the camera under/over itself
	if (cameraPitch > 89.0f)
		cameraPitch = 89.0f;
	if (cameraPitch < -89.0f)
		cameraPitch = -89.0f;
	//calculations for the new direction based on the mouse movements
	glm::vec3 direction;
	direction.x = cos(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch));
	direction.y = sin(glm::radians(cameraPitch));
	direction.z = sin(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch));
	cameraTarget = glm::normalize(direction);
} //fly camera

void AddCannonShot(int shotType, glm::vec3 position, glm::vec3 velocity)
{
	nPhysics::cCannonShot* shot;
	glm::vec3 colour = glm::vec3(1.0f);
	float scale = 7.0f;

	if (shotType == 1)
	{
		nPhysics::cBullet* shotB;
		//From config: size, damping, mass, velocity (magnitude)
		shotB = new nPhysics::cBullet(config.GetBulletMass(), position);
		shotB->SetVelocity(velocity);
		shotB->SetDamping(config.GetBulletDamping());
		shotB->SetAcceleration(glm::vec3(0.0f, -9.8f, 0.0f));		//acceleration could get replaced
		shotB->SetType(1);
		colour = glm::vec3(1.0f, 0.0f, 1.0f);						//colour could get replaced
		scale = config.GetBulletSize();
		shot = shotB;
	}
	else if (shotType == 2)
	{
		nPhysics::cLaser* shotL;
		//From config: size, damping, mass, velocity (magnitude), and distance until death
		shotL = new nPhysics::cLaser(config.GetLaserMass(), position);
		shotL->SetVelocity(velocity);
		shotL->SetDamping(config.GetLaserDamping());
		shotL->SetAcceleration(glm::vec3(0.0f, -3.2f, 0.0f));	//acceleration could get replaced
		shotL->SetDistanceToDeath(config.GetLaserDistanceLimit());
		shotL->SetCannonPosition(cannonPosition);
		shotL->SetType(2);
		colour = glm::vec3(0.0f, 1.0f, 0.3f);					//colour could get replaced
		scale = config.GetLaserSize();
		shot = shotL;
	}
	else if (shotType == 3)
	{
		nPhysics::cCannonBall* shotCB;
		//From config: size, damping, mass, velocity (magnitude)
		shotCB = new nPhysics::cCannonBall(config.GetCannonBallMass(), position);
		shotCB->SetVelocity(velocity);
		shotCB->SetDamping(config.GetCannonBallDamping());
		shotCB->SetAcceleration(glm::vec3(0.0f, -9.8f, 0.0f));	//acceleration could get replaced
		shotCB->SetType(3);
		colour = glm::vec3(0.6f, 0.8f, 1.0f);					//colour could get replaced
		scale = config.GetCannonBallSize();
		shot = shotCB;
	}
	else if (shotType == 4)
	{
		nPhysics::cEnergyBall* shotEB;
		//From config: size, damping, mass, velocity (magnitude), time limit and distance limit
		shotEB = new nPhysics::cEnergyBall(config.GetEnergyBallMass(), position);
		shotEB->SetVelocity(velocity);
		shotEB->SetDamping(config.GetEnergyBallDamping());
		shotEB->SetAcceleration(glm::normalize(velocity) * 2.0f);			//Energy ball accelerates in the same direction it's fired, maybe replace magnitude of acceleration or just use velocity
		shotEB->SetDistanceToDeath(config.GetEnergyBallDistanceLimit());
		shotEB->SetTimeToDie(config.GetEnergyBallTimeLimit());
		shotEB->SetCannonPosition(cannonPosition);
		shotEB->SetType(4);
		colour = glm::vec3(1.0f, 0.4f, 0.0f);								//colour could get replaced
		scale = config.GetEnergyBallSize();;
		shot = shotEB;
	}
	else
	{
		std::cout << "Not a cannon shot!" << std::endl;
		return;
	}

	world->AddParticle(shot);
	cannonShots.push_back(shot);
	AddMeshes(shot->GetPosition(), scale, colour);
	
}
//AddMeshes function I made for Physics Project 2
void AddMeshes(glm::vec3 position, float scale, glm::vec3 colour)
{
	cMesh newBunny;
	newBunny.meshName = modelLocations[0];
	newBunny.positionXYZ = position;
	newBunny.bOverrideColour = true;
	newBunny.vertexColourOverride = colour;
	newBunny.bIsWireframe = true;
	newBunny.scale = scale;
	//Set everything about the bunny model and then push it to the vector that we're drawing
	g_vecMeshesToDraw.push_back(newBunny);
}

