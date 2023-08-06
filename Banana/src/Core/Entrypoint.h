#pragma once
#include <Core/Application.h>

// This function NEEDS to be defined in the game code.
// It returns a pointer to the application that will be run.
extern Banana::Application* CreateApplication();

int main(int argc, char* argv[])
{
	Banana::Application* app = CreateApplication();
	app->Run();
	return 0;
}