#include <Graphics/Window.h>
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
			#ifdef BANANA_OPENGL_DEBUG
				glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
				BANANA_WARN("OpenGL debug context created! The application might be running slower.");
			#endif
			glfwSetErrorCallback([](int error_code, const char* description)
				{
					BANANA_ERROR("GLFW error {0}: {1}", error_code, description);
				});
			WasGLFWInit = true;
		}

		// Create the glfw window and set it as the current OpenGL context
		m_Window = glfwCreateWindow(m_Specs.Width, m_Specs.Height, m_Specs.Title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_Window);
		glfwSetWindowUserPointer(m_Window, this);

		// Initialize glew
		if (!WasGLEWInit)
		{
			glewInit();
			WasGLEWInit = true;
		}

		// Set glfw callbacks 
		SetCallbacks();
		#ifdef BANANA_OPENGL_DEBUG
			// OpenGL debug context
			InitOpenGLDebugOutput();
		#endif

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

	#ifdef BANANA_DEBUG
	void Window::InitOpenGLDebugOutput()
	{
		int flags;
		glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
		if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
		{
			glEnable(GL_DEBUG_OUTPUT);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

			// Set the callback as a lambda
			// Shamelessly stolen from https://learnopengl.com/In-Practice/Debugging
			glDebugMessageCallback(
				[](GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char* message, const void* userParam)
				{
					// Ignore non-significant error/warning codes
					if (id == 131169 || id == 131185 || id == 131218 || id == 131204)
						return;

					std::stringstream sstream;
					sstream << message << " ";

					switch (severity)
					{
					case GL_DEBUG_SEVERITY_HIGH:         spdlog::error(sstream.str()); break;
					case GL_DEBUG_SEVERITY_MEDIUM:       spdlog::warn(sstream.str()); break;
					case GL_DEBUG_SEVERITY_LOW:          spdlog::warn(sstream.str()); break;
					case GL_DEBUG_SEVERITY_NOTIFICATION: spdlog::info(sstream.str()); break;
					}
				}, nullptr);

			glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
		}
		else
		{
			spdlog::warn("Unable to create a debug context!");
		}
	}
	#endif

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