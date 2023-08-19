#include <Core/Entrypoint.h>
#include <ExampleScene.h>

Application* CreateApplication()
{
	WindowSpecs specs = WindowSpecs(1280, 720, "Banana Engine");
	Application* app = new Application(specs);

	Ref<ExampleScene> scene = CreateRef<ExampleScene>();

	app->GetScenePool()->AddScene(scene, "Scene");
	app->SetCurrentScene(scene);
	return app;
}