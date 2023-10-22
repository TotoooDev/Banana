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
	/**
	 * The application class contains everything related to the currently run application.
	 * An application needs to be created along with the `CreateApplication` function that returns a pointer to the application.
	 * Usually you will use it as a singleton with the `Get` method.
	 * It has a pointer to the current scene that should be updated, you can set it with the `SetCurrentScene` method
	 */
	class Application
	{
	public:
		/**
		 * Creates a window, sets up the log for debugging (which is disabled in release mode), and creates a renderer.
		 * @param specs - The windows specification that will be used to create the window
		 */
		Application(const WindowSpecs& specs);

	public:
		/**
		 * Runs the application! It runs until the WindowClosedEvent is published or the Stop() method is called.
		 */
		void Run();

		/**
		 * Stops the application.
		 */
		void Stop();

	// Getters
	public:
		/**
		 * Returns a pointer to the instance of the application.
		 * @returns A pointer to the application
		 */
		static Application* Get() { return m_Instance; }

		/**
		 * Returns the timestep, i.e. the time in seconds between frames. It is used to prevent different behaviors depending on framerate.
		 * @returns The timestep in seconds
		 */
		float GetTimestep() { return m_Timestep; }

		/**
		 * Returns a reference to the RendererAPI. Usually you won't need to interact with the RendererAPI directly, the scene manages drawing stuff for you.
		 * @returns A reference to the RendererAPI
		 */
		Ref<RendererAPI> GetRenderer() { return m_Renderer; }

		/**
		 * Returns a pointer to the window's event bus. Use it to subscribe to and publish events.
		 * @returns A pointer to the event bus.
		 */
		EventBus* GetEventBus() { return &m_EventBus; }

		/**
		 * @returns A pointer to the scene pool
		 */
		ScenePool* GetScenePool() { return &m_ScenePool; }

		/**
		 * Returns the window specifications. It gets updated every time you change the window's paramters.
		 * @returns The window specifications.
		 */
		WindowSpecs GetWindowSpecs() { return m_Window.GetSpecs(); }

		/**
		 * Returns a pointer to the window. For now you can only have one window in your application (and I don't really plan on adding multiple windows support lol)
		 * @returns A pointer to the window.
		 */
		Window* GetWindow() { return &m_Window; }

	// Setters
	public:
		/**
		 * Sets the scene that gets updated each frame.
		 * @param scene - The scene that will be set as current
		 */
		void SetCurrentScene(Ref<Scene> scene);

	// Operators
	public:
		// The application should not be clonable or assignable
		Application(Application&) = delete;
		void operator=(const Application&) = delete;

	private:
		void UpdateTimestep();

		/**
		 * Stops the application. Gets called when the WindowClosedEvent is published.
		 */
		void OnWindowClosedEvent(WindowClosedEvent* event);

		/**
		 * Resizes the renderer viewport. Gets called when the WindowResized event is published.
		 */
		void OnWindowResizedEvent(WindowResizedEvent* event);

	private:
		/**
		 * A pointer to the unique instance of the application.
		 */
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