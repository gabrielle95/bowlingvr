//#include "Application.h"
#include "MainScene.h"
int main(int argc, char* argv[])
{
	Application *application = new Application();
	SetState<MainScene>(application);
	while (application->Run());
	delete application;
	return 0;
}