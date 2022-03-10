Evan Sinasac - 1081418
INFO6019 Physics and Simulation
INFO6025 Configuration and Deployment
Midterm

The purpose of this project is to implement and extend the previous projects to make a cannon game that loads its configuration from a JSON file.  A short demo video explaining the program should be located in the same location as this file.  

Built and compiled using Visual Studios 2019 in Debug and Release in x64.

CONTROLS
W/A/S/D		- Move the camera around the scene
Q/E		- Move the camera up and down in the scene
Mouse		- Moving the mouse moves the camera's target allowing you to look around
1/2/3/4		- Launch different kinds of cannon shots
Arrow Keys	- Change the direction of the cannon

MODELS
Models are loaded from ply files with only vertex coordinates and normals
Models used are our Stanford bunny model and some models from Creepy Cat's 3D Scifi Kit Vol 3 (https://assetstore.unity.com/packages/3d/environments/sci-fi/3d-scifi-kit-vol-3-121447)

CONFIGURATION
Config.json located at PROJECT_DIR/src/ has all the information for the cannon and each type of shot.  cConfigReaderJSON.cpp loads the information and is used to store the values.

PHYSICS
Using the same given cParticleWorld and cParticle for the base.  Implemented a cCannonShot that inherits cParticle and adds variables used for every type of cannon shot.  Then cBullet, cLaser, cCannonBall and cEnergyBall inherit from cCannonShot and implement any additional variables and death conditions for the type of shot.

VIDEO
https://youtu.be/-fKPrv8QURs
