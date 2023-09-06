#include <Graphics/Window.h>
#include <Core/Config.h>
#include <Events/Events.h>
#include <Core/Log.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

namespace Banana
{
	Window::Window(const WindowSpecs& specs, EventBus* bus)
		: m_Specs(specs), m_EventBus(bus)
	{
		// Initialize glfw
		if (!WasGLFWInit)
		{
			glfwInit();
			glfwSetErrorCallback([](int error_code, const char* description)
				{
					BANANA_ERROR("GLFW error {0}: {1}", error_code, description);
				});

			// Create the window with OpenGL output if we use OpenGL and the build is in debug mode
			#if defined(BANANA_OPENGL_DEBUG) && defined(BANANA_OPENGL)
				glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
				BANANA_WARN("OpenGL debug context created! The application might be running slower.");
			#endif

			WasGLFWInit = true;
		}

		// Create the glfw window and set it as the current OpenGL context
		m_Window = glfwCreateWindow(m_Specs.Width, m_Specs.Height, m_Specs.Title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_Window);
		glfwSetWindowUserPointer(m_Window, this);

		// Set glfw callbacks 
		SetCallbacks();

		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

		// Setup Dear ImGui style
		 ImGui::StyleColorsDark();
		// ImGui::StyleColorsLight();

		// Setup Platform/Renderer backends
		ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
		ImGui_ImplOpenGL3_Init("#version 330");

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	Window::~Window()
	{
		// Cleaning up
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

		glfwDestroyWindow(m_Window);
		glfwTerminate();
	}

	void Window::Update()
	{
		// Poll all GLFW events and display the graphics on the window
		glfwPollEvents();

		// Render ImGui
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// Swap buffers
		glfwSwapBuffers(m_Window);

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	int Window::GetKey(int key)
	{
		return glfwGetKey(m_Window, key);
	}

	int Window::GetMouseButton(int mouseButton)
	{
		return glfwGetMouseButton(m_Window, mouseButton);
	}

	void Window::ToggleMouseLocking(bool toggle)
	{
		if (toggle)
			glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		else
			glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	void Window::SetCallbacks()
	{
		// Window callbacks
		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* glfwWindow)
			{
				Window* window = (Window*)glfwGetWindowUserPointer(glfwWindow);
				window->GetEventBus()->Publish(new WindowClosedEvent());
			});
		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* glfwWindow)
			{
				Window* window = (Window*)glfwGetWindowUserPointer(glfwWindow);
				window->GetEventBus()->Publish(new WindowClosedEvent());
			});
		glfwSetWindowMaximizeCallback(m_Window, [](GLFWwindow* glfwWindow, int maximized)
			{
				Window* window = (Window*)glfwGetWindowUserPointer(glfwWindow);
				if (maximized)
					window->GetEventBus()->Publish(new WindowMaximizedEvent());
				else
					window->GetEventBus()->Publish(new WindowRestoredEvent());
			});
		glfwSetWindowPosCallback(m_Window, [](GLFWwindow* glfwWindow, int x, int y)
			{
				Window* window = (Window*)glfwGetWindowUserPointer(glfwWindow);
				window->GetEventBus()->Publish(new WindowMovedEvent(x, y));
			});
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* glfwWindow, int width, int height)
			{
				Window* window = (Window*)glfwGetWindowUserPointer(glfwWindow);
				if (width == 0 || height == 0)
					return;
				window->GetSpecs().Width = width;
				window->GetSpecs().Height = height;
				window->GetEventBus()->Publish(new WindowResizedEvent(width, height));
			});

		// Keyboard callbacks
		glfwSetKeyCallback(m_Window, [](GLFWwindow* glfwWindow, int key, int scancode, int action, int mods)
			{
				Window* window = (Window*)glfwGetWindowUserPointer(glfwWindow);
				if (action == GLFW_PRESS)
					window->GetEventBus()->Publish(new KeyDownEvent(key));
				else if (action == GLFW_RELEASE)
					window->GetEventBus()->Publish(new KeyUpEvent(key));
			});

		// Mouse callbacks
		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* glfwWindow, double xpos, double ypos)
			{
				Window* window = (Window*)glfwGetWindowUserPointer(glfwWindow);
				window->GetEventBus()->Publish(new MouseMovedEvent(xpos, ypos));
			});
		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* glfwWindow, int button, int action, int mods)
			{
				Window* window = (Window*)glfwGetWindowUserPointer(glfwWindow);
				if (action == GLFW_PRESS)
					window->GetEventBus()->Publish(new MouseButtonDownEvent(button));
				else if (action == GLFW_RELEASE)
					window->GetEventBus()->Publish(new MouseButtonUpEvent(button));
			});
		glfwSetScrollCallback(m_Window, [](GLFWwindow* glfwWindow, double xoffset, double yoffset)
			{
				Window* window = (Window*)glfwGetWindowUserPointer(glfwWindow);
				window->GetEventBus()->Publish(new MouseScrolledEvent(xoffset, yoffset));
			});
	}
}