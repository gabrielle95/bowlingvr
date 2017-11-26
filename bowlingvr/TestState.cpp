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
	this->testShape = new TestShape(this->shader);
	this->room = new Room(this->shader);

	/* initialize bullet*/
	this->bwInstance = new BulletWorld();

	//this->box = new Box(this->shader, btVector3(10,10,10));


	this->camera = new Camera(this->shader, 1600,900);
	this->camera->SetTranslation(0,0,2);
	//SDL_WarpMouseInWindow(NULL, 500, 500);
	SDL_WarpMouseGlobal(500, 500);
	SDL_ShowCursor(0);
	this->camVelocity = glm::vec3(.1f,.1f,.1f);
	//std::cout << "caling testCreate" << std::endl;

	this->bwInstance->HelloWorld();
	return true;
}

bool TestState::Update()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	this->room->Draw();
	this->testShape->Draw();
	//this->box->Draw();
	
	this->bwInstance->bwUpdate(1.f / 60.f);
	this->camera->Update();

	const uint8_t *state = SDL_GetKeyboardState(NULL);

	//cos(0) = 1, cos(90) = 0, cos(180) = -1, cos(270) = 0
	//sin(0) = 0, sin(90) = 1, sin(180) = 0, sin(270) = -1

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

	if (state[SDL_SCANCODE_M])
	{
		this->bwInstance->debugDraw->ToggleDebugFlag(btIDebugDraw::DBG_DrawWireframe);
	}

	int xPos, yPos;
	SDL_GetGlobalMouseState(&xPos, &yPos);
	SDL_WarpMouseGlobal(500, 500);
	//SDL_WarpMouseInWindow(NULL, 500, 500);

	camRotation.y += mouse_speed * float(500 - xPos);
	camRotation.x += mouse_speed * float(500 - yPos);
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
	delete this->shader;
}