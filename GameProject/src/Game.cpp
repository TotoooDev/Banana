#include <Core/Entrypoint.h>
#include <ExampleScene.h>

Application* CreateApplication()
{
	// Create a new application
	WindowSpecs specs = WindowSpecs(1280, 720, "Banana Engine");
	Application* app = new Application(specs);

	// Create a new scene
	Ref<ExampleScene> scene = CreateRef<ExampleScene>();

	// Add the scene to the scene pool and launch the game!
	app->GetScenePool()->AddScene(scene, "Scene");
	app->SetCurrentScene(scene);
	return app;
}