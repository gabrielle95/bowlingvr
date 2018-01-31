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
	this->dynamicWorld = BulletWorld::Instance()->dynamicWorld;

	this->testShape = new TestShape(this->shader);
	this->testShape->SetTranslation(0, -2, 5);

	/* ROOM */
	this->room = new Room(this->shader);
	this->room->InitStaticPlanePhysics(btScalar(10.), btVector3(0, -10, 0));
	this->dynamicWorld->addRigidBody(this->room->rigidBody);

	//big ball
	this->sphereObj = new ObjLoader(this->shader, "sphereModel.obj");
	this->sphereObj->InitSpherePhysics(btScalar(20.f), btScalar(1.f), btVector3(0,1,-2));
	this->dynamicWorld->addRigidBody(this->sphereObj->rigidBody);
	dynamicObjects.push_back(this->sphereObj);

	//small ball
	this->smallSp = new ObjLoader(this->shader, "sphereModel_half.obj");
	/*this->smallSp->InitSpherePhysics(btScalar(0.1f), btScalar(0.5f), btVector3(0, 0.5, 0));
	this->dynamicWorld->addRigidBody(this->smallSp->rigidBody);
	dynamicObjects.push_back(this->smallSp);*/

	this->assimpTest = new AssimpModel(this->shader, "sphereModel.obj");


	//cam
	this->camera = new Camera(this->shader, 1600,900);
	this->camera->SetTranslation(0,2,2);

	SDL_WarpMouseGlobal(500, 500);
	SDL_ShowCursor(0);
	this->camVelocity = glm::vec3(this->mouse_speed*5, this->mouse_speed*5, this->mouse_speed*5);
	
	return true;
}

bool TestState::Update()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	this->dynamicWorld->stepSimulation(1.f / 90.f);

	/* DEBUG DRAW */
	if (this->debugMode) {
		BulletWorld::Instance()->debugDraw->SetMatrices(this->camera->getViewMatrix(), this->camera->getProjectionMatrix());
		this->dynamicWorld->debugDrawWorld();
		this->shader->Use();
	}

	/* draw everything that has texture */
	/*glUniform1i(this->hasTextureUniform, true);
	this->testShape->Draw();
	glUniform1i(this->hasTextureUniform, false);

	this->room->Draw();
	for (int j = 0; j < this->dynamicObjects.size(); j++)
	{
		ObjLoader* shape = this->dynamicObjects[j];
		shape->Draw();
	}*/

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
	std::vector<float> fd(this->smallSp->finalData);
	std::vector<unsigned int> el(this->smallSp->elements);
	ObjLoader *shoot = new ObjLoader(this->shader, fd, el);
	shoot->setShader(this->shader);

	btVector3 velocity = direction;
	velocity.normalize();
	velocity *= 10.0f;

	shoot->InitSpherePhysics(btScalar(10.f), btScalar(0.5f), origin);
	this->dynamicWorld->addRigidBody(shoot->rigidBody);
	shoot->rigidBody->setLinearVelocity(velocity);
	dynamicObjects.push_back(shoot);

	/*for (int j = this->dynamicWorld->getNumCollisionObjects() - 1; j >= 0; j--)
	{
		btCollisionObject* obj = this->dynamicWorld->getCollisionObjectArray()[j];
		btRigidBody* body = btRigidBody::upcast(obj);
		btTransform trans;
		if (body && body->getMotionState())
		{
			body->getMotionState()->getWorldTransform(trans);
		}
		else
		{
			trans = obj->getWorldTransform();
		}
		//printf("world pos object %d = %f,%f,%f\n", j, float(trans.getOrigin().getX()), float(trans.getOrigin().getY()), float(trans.getOrigin().getZ()));
	}*/

	fd.clear();
	el.clear();
}

bool TestState::Destroy()
{
	return true;
}

TestState::~TestState()
{
	for (int j = 0; j < this->dynamicObjects.size(); j++)
	{
		ObjLoader* shape = this->dynamicObjects[j];
		delete shape;
	}
	delete this->camera;
	delete this->room;
	delete this->sphereObj;
	delete this->smallSp;
	delete this->shader;
}