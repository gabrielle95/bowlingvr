#include <Windows.h>
#include "GL/glew.h"
#include <stdio.h>
#include <direct.h> 
#include "ShaderStrings.h"
#include "MainScene.h"
//#include "TestState2.h"

MainScene::MainScene(Application *application) : GameState(application)
{
	//notepad: glFramebufferTextureMultisampleMultiviewOVR
}

bool MainScene::Init()
{
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glClearColor(0.1,0.1,0.1,0.1);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	/* CWD */
	char cCurrentPath[FILENAME_MAX];

	if (!_getcwd(cCurrentPath, sizeof(cCurrentPath)))
	{
		printf("BOWLINGVR:: Can't locate CWD \n");
	}
	cCurrentPath[sizeof(cCurrentPath) - 1] = '\0'; /* not really required */
	printf("BOWLINGVR::The current working directory is %s\n", cCurrentPath);

	/***********************/
	/* SHADER COMPILATION */
	/**********************/

	std::cout << "BOWLING:: Compiling shaders..." << std::endl;

	this->modelShader = new Shader("Model", c_modelShaderVert, c_modelShaderFrag);
	assert(this->modelShader != nullptr);
	modelShader->Use();

	this->depthShader = new Shader("depthshader", c_depthShaderVert, c_depthShaderFrag, c_depthShaderGeom);
	assert(this->depthShader != nullptr);

	this->hdrShader = new Shader("hdr", c_hdrShaderVert, c_hdrShaderFrag);

	this->blurShader = new Shader("blur", c_blurShaderVert, c_blurShaderFrag);

	this->bloomShader = new Shader("bloom", c_bloomShaderVert, c_bloomShaderFrag);

	/***************************/
	/* PHYSICS INITIALISATION */
	/*************************/

	this->dynamicWorld = BulletWorld::Instance()->dynamicWorld;
	assert(this->dynamicWorld != nullptr);

	/*************************/
	/* MODEL INITIALISATION */
	/************************/

	/* ALLEY COLLISION DIRTY FIX */
	btRigidBody * alleyFix = BulletUtils::createInvisibleBoxCollider(0, btVector3(1.35, 0.09, 27), btVector3(-0.35, -0.19999999, -29), glm::mat4(1.0));
	this->dynamicWorld->addRigidBody(alleyFix);

	std::cout << "BOWLING:: Loading models..." << std::endl;

	this->room = new Alley(this->modelShader, std::string(cCurrentPath) + "\\models\\alley.obj");
	this->room->pInit(0, btVector3(0, 0, -17));
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
	this->sphere = new Model(this->modelShader, std::string(cCurrentPath) + "\\models\\ball\\ball_0175.obj");

	this->pin = new Model(this->modelShader, std::string(cCurrentPath) + "\\models\\pin\\bowling_pin_001.obj");

	/*this->alley = new Alley(this->modelShader, std::string(cCurrentPath) + "\\models\\venue.obj");
	this->alley->pInit(0, btVector3(0,0.5,0));
	this->dynamicWorld->addRigidBody(alley->rigidBody);*/

	std::vector<btVector3> pinPositions;
	pinPositions.push_back(btVector3(-1.f, 0.3f, -54.f));
	pinPositions.push_back(btVector3(-0.5f, 0.3f, -54.f));
	pinPositions.push_back(btVector3(0.0f, 0.3f, -54.f));
	pinPositions.push_back(btVector3(0.5f, 0.3f, -54.f));

	pinPositions.push_back(btVector3(-0.75f, 0.3f, -53.5f));
	pinPositions.push_back(btVector3(-0.25f, 0.3f, -53.5f));
	pinPositions.push_back(btVector3(0.25f, 0.3f, -53.5f));

	pinPositions.push_back(btVector3(-0.5f, 0.3f, -52.f));
	pinPositions.push_back(btVector3(0.0f, 0.3f, -52.f));

	pinPositions.push_back(btVector3(-0.25f, 0.3f, -51.5f));
	
	for (int i = 0; i < pinPositions.size(); i++) {
		Pin *tmp = new Pin(this->modelShader, this->pin->meshes, 1.5, 0.085, 0.8, pinPositions[i]);
		pins.push_back(tmp);
		this->dynamicWorld->addRigidBody(tmp->rigidBody);
	}
	pinPositions.clear();
	


	//testball
	Ball *b = new Ball(this->modelShader, this->sphere->meshes, btScalar(10.f), btScalar(0.175), btVector3(2.f, 1.5f, 0.5f));
	dynamicObjects.push_back(b);
	b->rigidBody->setUserPointer(b);
	this->dynamicWorld->addRigidBody(b->rigidBody);
	//testball
	Ball *c = new Ball(this->modelShader, this->sphere->meshes, btScalar(10.f), btScalar(0.175), btVector3(-0.25f, 0.3f, 0.5f));
	dynamicObjects.push_back(c);
	this->dynamicWorld->addRigidBody(c->rigidBody);

	

	/*************************/
	/* LIGHT INITIALISATION */
	/************************/
	std::cout << "BOWLING:: Loading lights..." << std::endl;

	this->Lights.push_back(new Light(this->modelShader, 0, glm::vec4(-2.0f, 3.0f, -32.0f, 1.0)));
	//this->Lights.push_back(new Light(this->modelShader, 1, glm::vec4(-2.0f, 2.5f, 25.0f, 1.0)));
	modelShader->setInt("enabledLights", 1);


	/********************/
	/* SHADOW MAP INIT */
	/*******************/
	std::cout << "BOWLING:: Loading shadow maps..." << std::endl;

	CubeDepthMap = new Shadowmap(2048, 2048);
	assert(CubeDepthMap != nullptr);
	modelShader->setFloat("far_plane", 200.f);
	CubeDepthMap->CreateCubemapMatrices(glm::vec3(-2.0f, 3.0f, -32.0f));

	/*********************/
	/* POST PROCESSING */
	/********************/
	std::cout << "BOWLING:: Loading post processing effects..." << std::endl;

	this->NoEffects = new PostProcessing(application->w(), application->h());
	NoEffects->Init();

	this->BloomEffect = new PostProcessing(application->w(), application->h());
	BloomEffect->Init();

	this->HdrEffect = new Hdr(application->w(), application->h());
	HdrEffect->Init();

	this->msaaEffect = new MSAA(application->w(), application->h());


	hdrShader->Use();
	hdrShader->setInt("hdrBuffer", 1);
	hdrShader->setInt("hdr", 1);
	hdrShader->setFloat("exposure", exposure);
	blurShader->Use();
	blurShader->setInt("image", 1);
	blurShader->setInt("horizontal", 1);
	bloomShader->Use();
	bloomShader->setInt("scene", 0);
	bloomShader->setInt("bloomBlur", 1);
	bloomShader->setInt("bloom", bloom);
	bloomShader->setFloat("exposure", 1.2f); //bloom exposure

	
	// non VR
	/**************************/
	/* CAMERA INITIALISATION */
	/*************************/

	std::cout << "BOWLING:: Initializing camera..." << std::endl;
	this->camera = new Camera(this->modelShader, this->application->w(), this->application->h());
	assert(this->camera != nullptr);

	this->camera->SetTranslation(0,1.7,0);

	//player body
	this->Player = new PlayerBody(btVector3(0, 1.7, 0));
	this->dynamicWorld->addRigidBody(Player->rigidBody);

	this->camVelocity = glm::vec3(this->mouse_speed*5, this->mouse_speed*5, this->mouse_speed*5);
	

	SDL_GetGlobalMouseState(&xPos, &yPos);
	SDL_WarpMouseGlobal(500, 500);

	std::cout << "BOWLING:: READY" << std::endl;

	return true;
}

//main game loop - per frame operations
bool MainScene::Update()
{

	/* TIME */
	this->deltaNow = SDL_GetTicks();
	this->deltaTime = (this->deltaNow - this->deltaThen) / 1000.0;

	
	/* TRANSFORMATIONS */
	GetInputCallback(); //fron Keyboard

	this->dynamicWorld->stepSimulation(this->deltaTime, 5);
	
	RenderScene();
		
	this->deltaThen = this->deltaNow;

	return true;
}

void MainScene::RenderScene()
{
	glViewport(0, 0, application->w(), application->h());
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//postprocessing off
	if (!PP) {
		//glViewport(0, 0, application->w(), application->h());
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		modelShader->Use();
		modelShader->setVec3("viewPos", this->camera->getPosition());

		//Render the objects
		RenderObjects(modelShader);

		//Render the lights
		RenderLights(modelShader);
	}
	else {
		/* RENDERING SHADOWMAP to FBO */

		this->CubeDepthMap->RenderToDepthmap(depthShader);
		RenderObjects(depthShader);
		this->CubeDepthMap->UnbindFBO();

		/* RENDERING SCENE with textures to MSAA... */

		msaaEffect->BindFBO();
		glViewport(0, 0, application->w(), application->h());
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		modelShader->Use();
		modelShader->setVec3("viewPos", this->camera->getPosition());

		//Render the objects
		RenderObjects(modelShader);

		//Render the lights
		RenderLights(modelShader);

		//Apply shadows -- weird stuff is happening
		this->CubeDepthMap->BindDepthTexture();

		msaaEffect->UnbindFBO();

		msaaEffect->BlitToFBO(NoEffects->fbo);

		//HDR
		HdrEffect->BindFPFramebuffer();
		glViewport(0, 0, application->w(), application->h());
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		hdrShader->Use();
		NoEffects->Bind();
		RenderQuad();
		HdrEffect->UnbindFPFramebuffer();

		glViewport(0, 0, application->w(), application->h());
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//BLUR
		bool horizontal = true, first_iteration = true;
		unsigned int amount = 10;
		blurShader->Use();
		for (unsigned int i = 0; i < amount; i++)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, HdrEffect->pingpongFBO[horizontal]);
			blurShader->setInt("horizontal", horizontal);
			glBindTexture(GL_TEXTURE_2D, first_iteration ? HdrEffect->fbo_textures[1] : HdrEffect->pingpongColorbuffers[!horizontal]);  // bind texture of other framebuffer (or scene if first iteration)
			RenderQuad();
			horizontal = !horizontal;
			if (first_iteration)
				first_iteration = false;
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		//BLOOM
		glViewport(0, 0, application->w(), application->h());
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		bloomShader->Use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, HdrEffect->fbo_textures[0]);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, HdrEffect->pingpongColorbuffers[!horizontal]);

		RenderQuad();
	}
}

void MainScene::GetInputCallback()
{	
	this->Player->motionstate->getWorldTransform(ptrans);
	this->camera->SetTranslation(ptrans.getOrigin().x(), ptrans.getOrigin().y() + 0.85, ptrans.getOrigin().z());

	const uint8_t *state = SDL_GetKeyboardState(NULL);

	//cos(0) = 1, cos(90) = 0, cos(180) = -1, cos(270) = 0
	//sin(0) = 0, sin(90) = 1, sin(180) = 0, sin(270) = -1

	if (state[SDL_SCANCODE_D])
	{
		/*this->camera->Translate(this->camVelocity.x * this->deltaTime * cos(glm::radians(-this->camRotation.y)),
		0,
		this->camVelocity.z * this->deltaTime * sin(glm::radians(-this->camRotation.y)));*/
		this->Player->rigidBody->setLinearVelocity(btVector3(250 * this->deltaTime, this->Player->rigidBody->getLinearVelocity().y(), this->Player->rigidBody->getLinearVelocity().z()));
	}
	if (state[SDL_SCANCODE_A])
	{
		/*this->camera->Translate(-this->camVelocity.x * this->deltaTime * cos(glm::radians(-this->camRotation.y)),
		0,
		-this->camVelocity.z * this->deltaTime * sin(glm::radians(-this->camRotation.y)));*/
		this->Player->rigidBody->setLinearVelocity(btVector3(-250 * this->deltaTime, this->Player->rigidBody->getLinearVelocity().y(), this->Player->rigidBody->getLinearVelocity().z()));
	}
	if (state[SDL_SCANCODE_W])
	{
		/*this->camera->Translate(-this->camVelocity.x * this->deltaTime * sin(glm::radians(this->camRotation.y)),
		0,
		-this->camVelocity.z * this->deltaTime * cos(glm::radians(this->camRotation.y)));*/
		this->Player->rigidBody->setLinearVelocity(btVector3(this->Player->rigidBody->getLinearVelocity().x(), this->Player->rigidBody->getLinearVelocity().y(), -250 * this->deltaTime));
	}
	if (state[SDL_SCANCODE_S])
	{
		/*this->camera->Translate(this->camVelocity.x * this->deltaTime * sin(glm::radians(this->camRotation.y)),
		0,
		this->camVelocity.z * this->deltaTime * cos(glm::radians(this->camRotation.y)));*/

		this->Player->rigidBody->setLinearVelocity(btVector3(this->Player->rigidBody->getLinearVelocity().x(), this->Player->rigidBody->getLinearVelocity().y(), 250 * this->deltaTime));
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
		//this->debugMode = !this->debugMode;
		this->PP = !this->PP;
	}
	else if (!state[SDL_SCANCODE_P])
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
}

void MainScene::RenderObjects(Shader *shader)
{
	/* DEBUG DRAW */
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/*if (this->debugMode) {
		BulletWorld::Instance()->debugDraw->SetMatrices(this->camera->getViewMatrix(), this->camera->getProjectionMatrix());
		this->dynamicWorld->debugDrawWorld();

		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}*/

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

void MainScene::RenderLights(Shader * shader)
{
	for (int i = 0; i < Lights.size(); i++)
	{
		Light *light = Lights[i];
		light->Render(shader);
	}
}

void MainScene::ShootSphere(btVector3 direction, btVector3 origin)
{

	Ball *shoot = new Ball(this->modelShader, this->sphere->meshes, btScalar(10.f), btScalar(0.175), origin);
	btVector3 velocity = direction;
	velocity.normalize();
	velocity *= 10.0f;

	//shoot->InitPhysicsBody(btBODIES::BALL, btScalar(10.f), btScalar(0.1f), origin, origin);
	this->dynamicWorld->addRigidBody(shoot->rigidBody);
	shoot->rigidBody->setLinearVelocity(velocity);
	dynamicObjects.push_back(shoot);
}

// renderQuad() renders a 1x1 XY quad in NDC
// -----------------------------------------

void MainScene::RenderQuad()
{
	if (quadVAO == 0)
	{
		float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
			// positions   // texCoords
			-1.0f,  1.0f,  0.0f, 1.0f,
			-1.0f, -1.0f,  0.0f, 0.0f,
			1.0f, -1.0f,  1.0f, 0.0f,

			-1.0f,  1.0f,  0.0f, 1.0f,
			1.0f, -1.0f,  1.0f, 0.0f,
			1.0f,  1.0f,  1.0f, 1.0f
		};
		// setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

bool MainScene::Destroy()
{
	return true;
}

MainScene::~MainScene()
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

	for (int j = 0; j < this->Lights.size(); j++)
	{
		Light* shape = this->Lights[j];
		delete shape;
	}


	Lights.clear();
	dynamicObjects.clear();
	pins.clear();
	delete this->camera;
	delete this->room;
	delete this->sphere;
	delete this->modelShader;

	delete CubeDepthMap;
	delete NoEffects;
	delete BloomEffect;
	delete HdrEffect;

	glDeleteVertexArrays(1, &quadVAO);
	glDeleteBuffers(1, &quadVBO);

}
