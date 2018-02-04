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
	glClearColor(0.1,0.1,0.1,0.1);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glEnable(GL_TEXTURE_2D);
	//this->shader = new Shader("test.vert", "test.frag");
	//this->shader->Use();
	//this->hasTextureUniform = this->shader->getUniLocation("hasTexture");

	this->modelShader = new Shader("modelShader.vert", "modelShader.frag");
	this->hasTextureUniform = this->modelShader->getUniLocation("hasTexture");
	this->modelShader->Use();
	/* initialize bullet*/
	this->dynamicWorld = BulletWorld::Instance()->dynamicWorld;

	char cCurrentPath[FILENAME_MAX];

	if (!_getcwd(cCurrentPath, sizeof(cCurrentPath)))
	{
		printf("cwd error \n");
	}

	cCurrentPath[sizeof(cCurrentPath) - 1] = '\0'; /* not really required */

	printf("The current working directory is %s\n", cCurrentPath);
	//this->assimpTest = new AssimpModel(this->modelShader, "sphereModel_half.obj");
	/*assimpTest->InitPhysicsBody(AssimpModel::btBODIES::BALL, btScalar(1.f), btScalar(0.5f));
	this->dynamicWorld->addRigidBody(this->assimpTest->rigidBody);

	this->assimpTestt = new AssimpModel(this->shader, "purple.obj");
	assimpTestt->InitPhysicsBody(AssimpModel::btBODIES::BALL, btScalar(1.f), btScalar(1.f),0, btVector3(0, 10, 0));
	this->dynamicWorld->addRigidBody(this->assimpTestt->rigidBody);*/
	//string path(string(cCurrentPath) + "\\models\\venue\\objects\\Venue01.obj");

	this->room = new Model(this->modelShader, std::string(cCurrentPath) + "\\models\\room\\room.obj");
	room->InitPhysicsBody(btBODIES::PLANE, 0, 0, btVector3(10, 0.1, 10), btVector3(0, -0.1, 0));
	this->dynamicWorld->addRigidBody(this->room->rigidBody);
	
	this->assimpTestt = new Model(this->modelShader, "bowling_pin_000.obj");
	assimpTestt->InitPhysicsBody(btBODIES::PLANE, 1.5, 0, btVector3(0.075, 0.3, 0.075), btVector3(0, 0.3, 0));
	this->dynamicWorld->addRigidBody(this->assimpTestt->rigidBody);

	this->sphere = new Model(this->modelShader, "sphere-m10-r01.obj");
	//cam
	this->camera = new Camera(this->modelShader, 1600,900);
	this->camera->SetTranslation(0,1,10);

	SDL_WarpMouseGlobal(500, 500);
	SDL_ShowCursor(0);
	this->camVelocity = glm::vec3(this->mouse_speed*5, this->mouse_speed*5, this->mouse_speed*5);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE );
	return true;
}

bool TestState::Update()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	this->dynamicWorld->stepSimulation(1.f / 60.f);

	/* DEBUG DRAW */
	if (this->debugMode) {
		BulletWorld::Instance()->debugDraw->SetMatrices(this->camera->getViewMatrix(), this->camera->getProjectionMatrix());
		this->dynamicWorld->debugDrawWorld();
		
		this->modelShader->Use();
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	
	

	/* draw everything that has texture */
	/*glUniform1i(this->hasTextureUniform, true);
	this->room->Draw(this->modelShader);
	glUniform1i(this->hasTextureUniform, false);*/
	//this->assimpTestt->Draw(this->modelShader);
	glm::mat4 p = this->camera->getProjectionMatrix();
	glm::mat4 v = this->camera->getViewMatrix();

	for (int i = 0; i < this->dynamicObjects.size(); i++) {
		Model* shape = this->dynamicObjects[i];
		shape->Render(p,v);
	}
	//this->assimpTest->RenderModel(p, v);
	
	this->assimpTestt->Render(p, v);
	
	this->room->Render(p, v);
	this->camera->Update();

	this->deltaNow = SDL_GetTicks();
	/*this->lastTime = currentTime;
	this->currentTime = SDL_GetTicks(); //ms*/

	this->deltaTime = (this->deltaNow - this->deltaThen) / 1000.0;

	const uint8_t *state = SDL_GetKeyboardState(NULL);

	//cos(0) = 1, cos(90) = 0, cos(180) = -1, cos(270) = 0
	//sin(0) = 0, sin(90) = 1, sin(180) = 0, sin(270) = -1

	if (state[SDL_SCANCODE_D])
	{
		this->camera->Translate(this->camVelocity.x * this->deltaTime * cos(glm::radians(-this->camRotation.y)),
			0,
			this->camVelocity.z * this->deltaTime * sin(glm::radians(-this->camRotation.y)));
	}
	if (state[SDL_SCANCODE_A])
	{
		this->camera->Translate(-this->camVelocity.x * this->deltaTime * cos(glm::radians(-this->camRotation.y)),
			0,
			-this->camVelocity.z * this->deltaTime * sin(glm::radians(-this->camRotation.y)));
	}
	if (state[SDL_SCANCODE_W])
	{
		this->camera->Translate(-this->camVelocity.x * this->deltaTime * sin(glm::radians(this->camRotation.y)),
			0,
			-this->camVelocity.z * this->deltaTime * cos(glm::radians(this->camRotation.y)));
	}
	if (state[SDL_SCANCODE_S])
	{
		this->camera->Translate(this->camVelocity.x * this->deltaTime * sin(glm::radians(this->camRotation.y)),
			0,
			this->camVelocity.z * this->deltaTime * cos(glm::radians(this->camRotation.y)));
	}


	if (!this->pressedC && state[SDL_SCANCODE_C])
	{
		this->pressedC = true;
		glm::mat4 vm = glm::inverse(this->camera->getViewMatrix());
		this->ShootSphere(btVector3(-vm[2][0], -vm[2][1], -vm[2][2]), btVector3(vm[3][0], vm[3][1], vm[3][2]));
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
		

/*	if (state[SDL_SCANCODE_LEFT])
	{	
		this->camRotation.y += 1;
	}
	if (state[SDL_SCANCODE_RIGHT])
	{
		this->camRotation.y -= 1;
	}
	if (state[SDL_SCANCODE_UP])
	{
		this->camRotation.x += 1;
	}
	if (state[SDL_SCANCODE_DOWN])
	{
		this->camRotation.x -= 1;
	}
*/
	int xPos, yPos;

	SDL_GetGlobalMouseState(&xPos, &yPos);
	//SDL_GetMouseState(&xPos, &yPos);
	//SDL_GetRelativeMouseState(&xPos, &yPos);
	//SDL_WarpMouseInWindow(NULL, 500, 500);
	SDL_WarpMouseGlobal(500, 500);

	

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

	
	this->deltaThen = this->deltaNow;
	//printf("Secs per frame: %f\n", deltaTime);

	
	return true;
}

void TestState::ShootSphere(btVector3 direction, btVector3 origin)
{

	Model *shoot = new Model(this->modelShader, this->sphere->meshes);
	btVector3 velocity = direction;
	velocity.normalize();
	velocity *= 10.0f;

	shoot->InitPhysicsBody(btBODIES::BALL, btScalar(10.f), btScalar(0.1f), origin, origin);
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
	delete this->camera;
	delete this->room;
	delete this->sphere;
	delete this->assimpTestt;
	delete this->modelShader;
}