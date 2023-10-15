#pragma once
#include <Core/Application.h>

// This function NEEDS to be defined in the game code.
// It returns a pointer to the application that will be run.
/**
 * @brief Returns a pointer to the application that will be run. This function NEEDS to be defined somewhere in the game code or you'll get a sweet unresolved symbol error :)
 * @returns A pointer to the new application
 */
extern Banana::Application* CreateApplication();

/**
 * @brief Entrypoint.
 */
int main(int argc, char* argv[])
{
	Banana::Application* app = CreateApplication();
	app->Run();
	return 0;
}