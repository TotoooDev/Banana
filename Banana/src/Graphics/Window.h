#pragma once
#include <Events/EventBus.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>

namespace Banana
{
	static bool WasGLFWInit = false;

	/**
	 * Represents the specification for a Window.
	 */
	struct WindowSpecs
	{
		unsigned int Width, Height; /** The size of the window. */
		std::string Title; /** The title of the window. */

		WindowSpecs(unsigned int width, unsigned int height, const std::string& title)
			: Width(width), Height(height), Title(title)
		{}
	};

	class Window
	{
	public:
		/**
		 * @param specs - The window specification to use.
		 * @param bus - A pointer to the event bus to use.
		 */
		Window(const WindowSpecs& specs, EventBus* bus);
		~Window();

		/**
		 * Polls events, swap buffers and performs ImGui renders.
		 */
		void Update();

		/**
		 * @param toggle - Whether to turn mouse locking on or off.
		 */
		void ToggleMouseLocking(bool toggle);

	// Getters
	public:

		/**
		 * @returns The state of the key passed in parameter.
		 * @param key - The key to get the state of.
		 */
		int GetKey(int key);

		/**
		 * @returns The state of the mouse button passed in parameter.
		 * @param button - The mouse button to get the state of.
		 */
		int GetMouseButton(int button);

		/**
		 * @returns The GLFWwindow associated with this window.
		 */
		GLFWwindow* GetNativeWindow() { return m_Window; }

		/**
		 * @returns The event bus of the window.
		 */
		EventBus* GetEventBus() { return m_EventBus; }

		/**
		 * @returns The specification of the window.
		 */
		WindowSpecs& GetSpecs() { return m_Specs; }

	private:
		void SetCallbacks();

	private:
		GLFWwindow* m_Window = nullptr;
		EventBus* m_EventBus = nullptr;
		WindowSpecs m_Specs;
	};
}