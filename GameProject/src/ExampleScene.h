#pragma once
#include <Core/Application.h>
#include <Scene/Scene.h>

using namespace Banana;

/**
 * This is a bare metal example of what a scene may look like in Banana.
 */
class ExampleScene : public Scene
{
public:
	ExampleScene()
	{
		Application::Get()->GetRenderer()->SetProjection(45.0f, 1280.0f / 720.0f, 0.1f, 100.0f);
		Application::Get()->GetEventBus()->Subscribe(this, &ExampleScene::OnWindowResized);
	}

private:
	void OnWindowResized(WindowResizedEvent* event)
	{
		Application::Get()->GetRenderer()->SetProjection(45.0f, (float)event->Width / (float)event->Height, 0.1f, 100.0f);
	}
};
