#pragma once
#include <Events/EventBus.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>

namespace Banana
{
	static bool WasGLFWInit = false;

	struct WindowSpecs
	{
		unsigned int Width, Height;
		std::string Title;

		WindowSpecs(unsigned int width, unsigned int height, const std::string& title)
			: Width(width), Height(height), Title(title)
		{}
	};

	class Window
	{
	public:
		Window(const WindowSpecs& specs, EventBus* bus);
		~Window();

		void Update();

		int GetKey(int key);
		int GetMouseButton(int button);
		GLFWwindow* GetNativeWindow() { return m_Window; }
		EventBus* GetEventBus() { return m_EventBus; }
		WindowSpecs& GetSpecs() { return m_Specs; }

	private:
		void SetCallbacks();

	private:
		GLFWwindow* m_Window = nullptr;
		EventBus* m_EventBus = nullptr;
		WindowSpecs m_Specs;
	};
}