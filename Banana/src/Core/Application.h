#pragma once
#include <Core/Ref.h>
#include <Graphics/Window.h>
#include <Graphics/RendererAPI.h>
#include <Events/EventBus.h>
#include <Events/Events.h>
#include <Scene/Scene.h>
#include <Scene/ScenePool.h>

namespace Banana
{
	// The application class contains everything related to the currently run application.
	// An application needs to be created along with the `CreateApplication` function that returns a pointer to the application.
	// Usually you will use it as a singleton with the `Get` method.
	// It has a pointer to the current scene that should be updated, you can set it with the `SetCurrentScene` method
	class Application
	{
	public:
		Application(const WindowSpecs& specs);

		// The application should not be clonable or assignable
		Application(Application&) = delete;
		void operator=(const Application&) = delete;
	
		static Application* Get() { return m_Instance; }
		Ref<RendererAPI> GetRenderer() { return m_Renderer; }
		EventBus* GetEventBus() { return &m_EventBus; }
		ScenePool* GetScenePool() { return &m_ScenePool; }
		WindowSpecs GetWindowSpecs() { return m_Window.GetSpecs(); }
	
		void Run();

		void SetCurrentScene(Ref<Scene> scene);

	private:
		void UpdateTimestep();
		void OnWindowClosedEvent(WindowClosedEvent* event);
		void OnWindowResizedEvent(WindowResizedEvent* event);

	private:
		static Application* m_Instance;

		Window m_Window;
		Ref<RendererAPI> m_Renderer;

		Ref<Scene> m_CurrentScene = nullptr;
		ScenePool m_ScenePool;

		EventBus m_EventBus;
		
		bool m_IsRunning = true;
		double m_Timestep = 0.0f;
		double m_LastFrame = 0.0f;
	};
}