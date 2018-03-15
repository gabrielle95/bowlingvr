#include <Windows.h>
#include "GL/glew.h"
#include <stdio.h>
#include <direct.h> 
#include "TestState.h"
#include "TestState2.h"

TestState::TestState(Application *application) : GameState(application)
{
}

bool TestState::Init()
{
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glClearColor(0.1,0.1,0.1,0.1);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);


	char cCurrentPath[FILENAME_MAX];

	if (!_getcwd(cCurrentPath, sizeof(cCurrentPath)))
	{
		printf("cwd error \n");
	}
	cCurrentPath[sizeof(cCurrentPath) - 1] = '\0'; /* not really required */
	printf("The current working directory is %s\n", cCurrentPath);

	/* SHADER COMPILATION */
	std::cout << "BOWLING:: Compiling shaders..." << std::endl;

	this->modelShader = new Shader("shader.vert", "shader.frag");
	assert(this->modelShader != nullptr);
	modelShader->Use();
	//modelShader->setInt("depthMap", 10);

	this->depthShader = new Shader("depthshader.vert", "depthshader.frag", "depthshader.geom");
	assert(this->depthShader != nullptr);

	//this->modelShader->Use();
	//modelShader->setInt("depthMap", 0);


	/* PHYSICS INITIALISATION */
	this->dynamicWorld = BulletWorld::Instance()->dynamicWorld;

	assert(this->dynamicWorld != nullptr);

	/* MODEL INITIALISATION */

	/* ALLEY COLLISION DIRTY FIX */
	btRigidBody * alleyFix = BulletUtils::createInvisibleBoxCollider(0, btVector3(1.35, 0.09, 27), btVector3(-0.35, -0.19999999, -12), glm::mat4(1.0));
	this->dynamicWorld->addRigidBody(alleyFix);

	std::cout << "BOWLING:: Loading models..." << std::endl;

	this->room = new Alley(this->modelShader, std::string(cCurrentPath) + "\\models\\room_alley.obj");
	this->room->pInit(0, btVector3(0, 0, 0));
	this->dynamicWorld->addRigidBody(room->rigidBody);

	/*std::vector<btVector3> wallPositions;
	wallPositions.push_back(btVector3(0, -0.1, 0)); //floor
	wallPositions.push_back(btVector3(0, 30, 0)); //ceiling

	wallPositions.push_back(btVector3(0, -0.1, 30)); //back
	wallPositions.push_back(btVector3(0, -0.1, -30)); //front
	wallPositions.push_back(btVector3(30, -0.1, 0)); //right
	wallPositions.push_back(btVector3(-30, -0.1, 0)); //left

	std::vector<btVector3>wallDimensions;
	wallDimensions.push_back(btVector3(30, 0.1, 30));//up and down
	wallDimensions.push_back(btVector3(30, 30, 0.1));//acc to x
	wallDimensions.push_back(btVector3(0.1, 30, 30));//acc to z
	
	int j = 0;
	for (int i = 0; i < wallDimensions.size(); i++) {
		this->dynamicWorld->addRigidBody
		(BulletUtils::createInvisibleBoxCollider(0.0, wallDimensions[i], wallPositions[j], room->modelMatrix));
		this->dynamicWorld->addRigidBody
		(BulletUtils::createInvisibleBoxCollider(0.0, wallDimensions[i], wallPositions[j+1], room->modelMatrix));
		j += 2;
	}
	wallPositions.clear();
	wallDimensions.clear();
	*/
	this->sphere = new Model(this->modelShader, std::string(cCurrentPath) + "\\models\\ball\\sphere-m10-r025.obj");

	this->pin = new Model(this->modelShader, "bowling_pin_000.obj");

	/*this->alley = new Alley(this->modelShader, std::string(cCurrentPath) + "\\models\\venue.obj");
	this->alley->pInit(0, btVector3(0,0.5,0));
	this->dynamicWorld->addRigidBody(alley->rigidBody);*/

	this->Player = new PlayerBody(btVector3(0, 1.7, 10));
	this->dynamicWorld->addRigidBody(Player->rigidBody);

	std::vector<btVector3> pinPositions;
	pinPositions.push_back(btVector3(-1.f, 0.3f, -38.f));
	pinPositions.push_back(btVector3(-0.5f, 0.3f, -38.f));
	pinPositions.push_back(btVector3(0.0f, 0.3f, -38.f));
	pinPositions.push_back(btVector3(0.5f, 0.3f, -38.f));

	pinPositions.push_back(btVector3(-0.75f, 0.3f, -37.5f));
	pinPositions.push_back(btVector3(-0.25f, 0.3f, -37.5f));
	pinPositions.push_back(btVector3(0.25f, 0.3f, -37.5f));

	pinPositions.push_back(btVector3(-0.5f, 0.3f, -37.f));
	pinPositions.push_back(btVector3(0.0f, 0.3f, -37.f));

	pinPositions.push_back(btVector3(-0.25f, 0.3f, -36.5f));
	
	for (int i = 0; i < pinPositions.size(); i++) {
		Pin *tmp = new Pin(this->modelShader, this->pin->meshes, 1.5, 0.075, 0.5, pinPositions[i]);
		pins.push_back(tmp);
		this->dynamicWorld->addRigidBody(tmp->rigidBody);
	}
	pinPositions.clear();

	/* LIGHT INITIALISATION */ //1 for now
	//std::vector<glm::vec4> lightPositions;

	this->Lights.push_back(new Light(this->modelShader, 0, glm::vec4(-2.0f, 2.5f, -15.0f, 1.0)));
	//this->Lights.push_back(new Light(this->modelShader, 1, glm::vec4(-2.0f, 2.5f, 25.0f, 1.0)));
	
	glUniform1i(modelShader->getUniLocation("enabledLights"), 1);
	/* SHADOW MAP INIT */
	oneLight = new Shadowmap(2048, 2048);
	assert(oneLight != nullptr);

	oneLight->CreateCubemapMatrices(glm::vec3(-2.0f, 2.5f, -15.0f));

	/*twoLight = new Shadowmap(1024,1024);
	assert(twoLight != nullptr);
	twoLight->CreateCubemapMatrices(glm::vec3(-2.0f, 2.5f, 25.0f));*/

	/* CAMERA INITIALISATION */
	std::cout << "BOWLING:: Initializing camera..." << std::endl;
	this->camera = new Camera(this->modelShader, this->application->w(), this->application->h());
	assert(this->camera != nullptr);

	this->camera->SetTranslation(0,1.7,10);

	this->camVelocity = glm::vec3(this->mouse_speed*5, this->mouse_speed*5, this->mouse_speed*5);

	SDL_GetGlobalMouseState(&xPos, &yPos);
	SDL_WarpMouseGlobal(500, 500);

	std::cout << "BOWLING:: READY" << std::endl;

	return true;
}

bool TestState::Update()
{
	this->dynamicWorld->stepSimulation(1.f / 60.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/* RENDERING SHADOWMAP - 1st pass */
	///
	this->oneLight->RenderToDepthmap(depthShader);
	RenderObjects(depthShader);
	this->oneLight->UnbindFBO();

	/* RENDERING SCENE - 2nd pass */

	glViewport(0, 0, application->w(), application->h());
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	modelShader->Use();
	modelShader->setFloat("far_plane", 200.f);
	modelShader->setVec3("viewPos", this->camera->getPosition());

	this->oneLight->BindDepthTexture();

	RenderObjects(modelShader);

	//Render the lights
	for (int i = 0; i < Lights.size(); i++)
	{
		Light *light = Lights[i];
		light->Render(modelShader);
	}


	/* TRANSFORMATIONS */

	btTransform ptrans;
	this->Player->motionstate->getWorldTransform(ptrans);
	this->camera->SetTranslation(ptrans.getOrigin().x(), ptrans.getOrigin().y() + 0.85, ptrans.getOrigin().z());


	/* TIME */
	this->deltaNow = SDL_GetTicks();
	/*this->lastTime = currentTime;
	this->currentTime = SDL_GetTicks(); //ms*/

	this->deltaTime = (this->deltaNow - this->deltaThen) / 1000.0;

	const uint8_t *state = SDL_GetKeyboardState(NULL);

	//cos(0) = 1, cos(90) = 0, cos(180) = -1, cos(270) = 0
	//sin(0) = 0, sin(90) = 1, sin(180) = 0, sin(270) = -1

	if (state[SDL_SCANCODE_D])
	{
		/*this->camera->Translate(this->camVelocity.x * this->deltaTime * cos(glm::radians(-this->camRotation.y)),
			0,
			this->camVelocity.z * this->deltaTime * sin(glm::radians(-this->camRotation.y)));*/
		this->Player->rigidBody->setLinearVelocity(btVector3(5, this->Player->rigidBody->getLinearVelocity().y(), this->Player->rigidBody->getLinearVelocity().z()));
	}
	if (state[SDL_SCANCODE_A])
	{
		/*this->camera->Translate(-this->camVelocity.x * this->deltaTime * cos(glm::radians(-this->camRotation.y)),
			0,
			-this->camVelocity.z * this->deltaTime * sin(glm::radians(-this->camRotation.y)));*/
		this->Player->rigidBody->setLinearVelocity(btVector3(-5, this->Player->rigidBody->getLinearVelocity().y(), this->Player->rigidBody->getLinearVelocity().z()));
	}
	if (state[SDL_SCANCODE_W])
	{
		/*this->camera->Translate(-this->camVelocity.x * this->deltaTime * sin(glm::radians(this->camRotation.y)),
			0,
			-this->camVelocity.z * this->deltaTime * cos(glm::radians(this->camRotation.y)));*/
		this->Player->rigidBody->setLinearVelocity(btVector3(this->Player->rigidBody->getLinearVelocity().x(), this->Player->rigidBody->getLinearVelocity().y(),-5));
	}
	if (state[SDL_SCANCODE_S])
	{
		/*this->camera->Translate(this->camVelocity.x * this->deltaTime * sin(glm::radians(this->camRotation.y)),
			0,
			this->camVelocity.z * this->deltaTime * cos(glm::radians(this->camRotation.y)));*/

		this->Player->rigidBody->setLinearVelocity(btVector3(this->Player->rigidBody->getLinearVelocity().x(), this->Player->rigidBody->getLinearVelocity().y(), 5));
	}

	/* JUMPING */
	if (!pressedSpace && state[SDL_SCANCODE_SPACE]) 
	{
		this->pressedSpace = true;
		btScalar yVel = this->Player->rigidBody->getLinearVelocity().y();

		/* you can only jump if you're on the ground */
		if (yVel < 0.05 && yVel > -0.05)
		{
			this->Player->rigidBody->setLinearVelocity(btVector3(this->Player->rigidBody->getLinearVelocity().x(), 3.5, this->Player->rigidBody->getLinearVelocity().z()));
		}
	}
	else if (!state[SDL_SCANCODE_SPACE])
	{
		this->pressedSpace = false;
	}

	//this->Player->rigidBody->setLinearVelocity(btVector3(0,this->Player->rigidBody->getLinearVelocity().y(),0));
	//std::cout << this->Player->rigidBody->getLinearVelocity().y() << std::endl;

	if (!this->pressedC && state[SDL_SCANCODE_C])
	{
		this->pressedC = true;
		glm::mat4 vm = glm::inverse(this->camera->getViewMatrix());
		this->ShootSphere(btVector3(-vm[2][0], -vm[2][1], -vm[2][2]), btVector3(vm[3][0], vm[3][1], vm[3][2] - 0.6));
	}
	else if (!state[SDL_SCANCODE_C])
	{
		this->pressedC = false;
	}

	if (!this->pressedP && state[SDL_SCANCODE_P])
	{
		this->pressedP = true;
		this->debugMode = !this->debugMode;
	}
	else if(!state[SDL_SCANCODE_P])
	{
		this->pressedP = false;
	}
		
	if (this->application->IsWindowActive() == true)
	{
		SDL_GetGlobalMouseState(&xPos, &yPos);
		SDL_WarpMouseGlobal(500, 500);
	}
		
	camRotation.y += deltaTime * mouse_speed * float(500 - xPos);
	camRotation.x += deltaTime * mouse_speed * float(500 - yPos);


	/* to restrict rotating the whole world upside down */
	if (this->camRotation.x > 90)
		this->camRotation.x = 90;
	if (this->camRotation.x < -90)
		this->camRotation.x = -90;


	this->camera->SetRotation(0, 1, 1, 1);
		
	this->camera->Rotate(camRotation.x, 1, 0, 0);
	this->camera->Rotate(camRotation.y, 0, 1, 0);

	this->camera->Update();

	this->deltaThen = this->deltaNow;

	return true;
}

void TestState::RenderObjects(Shader *shader)
{
	/* DEBUG DRAW */
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (this->debugMode) {
		BulletWorld::Instance()->debugDraw->SetMatrices(this->camera->getViewMatrix(), this->camera->getProjectionMatrix());
		this->dynamicWorld->debugDrawWorld();

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	for (int i = 0; i < this->dynamicObjects.size(); i++) {
		Model* shape = this->dynamicObjects[i];
		shape->Render(shader);
	}

	for (int i = 0; i < this->pins.size(); i++) {
		Pin* shape = this->pins[i];
		shape->Render(shader);
	}

	this->room->Render(shader);
}

void TestState::ShootSphere(btVector3 direction, btVector3 origin)
{

	Ball *shoot = new Ball(this->modelShader, this->sphere->meshes, btScalar(10.f), btScalar(0.25f), origin);
	btVector3 velocity = direction;
	velocity.normalize();
	velocity *= 10.0f;

	//shoot->InitPhysicsBody(btBODIES::BALL, btScalar(10.f), btScalar(0.1f), origin, origin);
	this->dynamicWorld->addRigidBody(shoot->rigidBody);
	shoot->rigidBody->setLinearVelocity(velocity);
	dynamicObjects.push_back(shoot);
}

bool TestState::Destroy()
{
	return true;
}

TestState::~TestState()
{
	for (int j = 0; j < this->dynamicObjects.size(); j++)
	{
		Model* shape = this->dynamicObjects[j];
		delete shape;
	}

	for (int j = 0; j < this->pins.size(); j++)
	{
		Pin* shape = this->pins[j];
		delete shape;
	}
	dynamicObjects.clear();
	pins.clear();
	delete this->camera;
	delete this->room;
	delete this->sphere;
	delete this->modelShader;
}
