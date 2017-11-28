#include <Windows.h>
#include "GL/glew.h"
#include "TestState.h"
#include "TestState2.h"

TestState::TestState(Application *application) : GameState(application)
{
}

bool TestState::Init()
{
	glClearColor(0,0,0,1);
	this->shader = new Shader("test.vert", "test.frag");
	this->shader->Use();
	this->hasTextureUniform = this->shader->getUniLocation("hasTexture");

	/* initialize bullet*/
	this->dynamicWorld = BulletWorld::Instance();

	this->testShape = new TestShape(this->shader);
	//this->testShape->SetTranslation(2, -5, 3);

	//big ball
	this->sphereObj = new ObjLoader(this->shader, "sphereModel.obj");
	this->sphereObj->setShader(this->shader);
	this->sphereObj->SetTranslation(-0.4f, -15, 5);
	this->sphereObj->InitSpherePhysics(btScalar(0.2f), btScalar(1.f));
	this->dynamicWorld->addRigidBody(this->sphereObj->rigidBody);

	//small ball
	this->smallSp = new ObjLoader(this->shader, "sphereModel_half.obj");
	this->smallSp->setShader(this->shader);
	this->smallSp->SetTranslation(0, -0.5, 5);
	this->smallSp->InitSpherePhysics(btScalar(0.5f), btScalar(0.5f));
	this->dynamicWorld->addRigidBody(this->smallSp->rigidBody);


	//plane - nefunguju transformacie
	this->room = new Room(this->shader);
	this->room->InitStaticPlanePhysics(btVector3(0,1,0), btScalar(0));
	this->dynamicWorld->addRigidBody(this->room->rigidBody);


	this->camera = new Camera(this->shader, 1600,900);
	this->camera->SetTranslation(0,0,2);
	//SDL_WarpMouseInWindow(NULL, 500, 500);
	SDL_WarpMouseGlobal(500, 500);
	SDL_ShowCursor(0);
	this->camVelocity = glm::vec3(.1f,.1f,.1f);
	//std::cout << "caling testCreate" << std::endl;

	return true;
}

bool TestState::Update()
{
	//this->currentTime = SDL_GetTicks();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	/* draw everything that has texture */
	glUniform1i(this->hasTextureUniform, true);
	
	//this->testShape->Draw();
	glUniform1i(this->hasTextureUniform, false);

	/*for (int j = 0; j < dynamicObjects.size(); j++)
	{
		DynamicObj* shape = dynamicObjects[j];
		shape->updateDrawable();
	}*/
	
	this->dynamicWorld->stepSimulation(1.f / 60.f);
	
	//this->room->Draw();
	
	this->smallSp->Draw();
	this->sphereObj->Draw();
	
	//this->dynamicWorld->debugDrawWorld();
	
	this->camera->Update();


	/*this->lastTime = currentTime;
	this->currentTime = SDL_GetTicks(); //ms
	
	this->deltaTime = float(currentTime - lastTime) / 1000.f;*/

	const uint8_t *state = SDL_GetKeyboardState(NULL);

	//cos(0) = 1, cos(90) = 0, cos(180) = -1, cos(270) = 0
	//sin(0) = 0, sin(90) = 1, sin(180) = 0, sin(270) = -1
	//std::cout << this->deltaTime << std::endl;

	if (state[SDL_SCANCODE_D])
	{
		this->camera->Translate(this->camVelocity.x * cos(glm::radians(-this->camRotation.y)),
								0,
								this->camVelocity.z * sin(glm::radians(-this->camRotation.y)));
	}
	if (state[SDL_SCANCODE_A])
	{
		this->camera->Translate(-this->camVelocity.x * cos(glm::radians(-this->camRotation.y)),
								0,
								-this->camVelocity.z * sin(glm::radians(-this->camRotation.y)));
	}
	if (state[SDL_SCANCODE_W])
	{
		this->camera->Translate(-this->camVelocity.x * sin(glm::radians(this->camRotation.y)),
								0,
								-this->camVelocity.z * cos(glm::radians(this->camRotation.y)));
	}
	if (state[SDL_SCANCODE_S])
	{
		this->camera->Translate(this->camVelocity.x * sin(glm::radians(this->camRotation.y)),
								0,
								this->camVelocity.z * cos(glm::radians(this->camRotation.y)));
	}

	if (state[SDL_SCANCODE_LEFT])
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

	int xPos, yPos;
	SDL_GetGlobalMouseState(&xPos, &yPos);
	SDL_WarpMouseGlobal(500, 500);
	//SDL_WarpMouseInWindow(NULL, 500, 500);

	camRotation.y += mouse_speed * float(500 - xPos);
	camRotation.x += mouse_speed * float(500 - yPos);

	/* to restrict rotating the whole world upside down */
	if (this->camRotation.x > 90)
		this->camRotation.x = 90;
	if (this->camRotation.x < -90)
		this->camRotation.x = -90;

	this->camera->SetRotation(0,1,1,1);
	this->camera->Rotate(camRotation.x, 1, 0, 0);
	this->camera->Rotate(camRotation.y, 0, 1, 0);

	return true;
}

bool TestState::Destroy()
{
	return true;
}

TestState::~TestState()
{
	delete this->camera;
	delete this->room;
	delete this->sphereObj;
	delete this->smallSp;
	delete this->shader;
}