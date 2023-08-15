#include <Core/Application.h>
#include <Core/Config.h>
#include <Core/Log.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Banana
{
	Application* Application::m_Instance = nullptr;

	Application::Application(const WindowSpecs& specs)
		: m_Window(specs, &m_EventBus)
	{
		if (m_Instance)
		{
			// An application already exists so we panic
			BANANA_ABORT("An application already exists! You can only create one application at the time.");
			return;
		}
		m_Instance = this;

		// spdlog setup
		auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
		consoleSink->set_pattern("[%D %T] [%^%l%$] %v");
		auto fileSink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>("logs/banana.log", 1048576, 3);
		fileSink->set_pattern("[%D %T] [%^%l%$] %v");
		spdlog::logger logger("Default Logger", { consoleSink, fileSink });
		spdlog::set_default_logger(std::make_shared<spdlog::logger>(logger));
		BANANA_WARN("Booting up Banana!");

		// Sub to the window events
		m_EventBus.Subscribe(this, &Application::OnWindowClosedEvent);
		m_EventBus.Subscribe(this, &Application::OnWindowResizedEvent);

		// Create the renderer
		RendererAPIName api;
		#if defined(BANANA_OPENGL)
			api = RendererAPIName::OpenGL;
		#elif defined(BANANA_VULKAN)
			api = RendererAPIName::Vulkan;
		#endif
		m_Renderer = RendererAPI::Create(api);
	}

	void Application::Run()
	{
		// I mean it can't get simpler than that
		while (m_IsRunning)
		{
			m_Renderer->Clear();
			if (m_CurrentScene)
				m_CurrentScene->UpdateScene(m_Renderer, m_Timestep);
			m_Window.Update();
			UpdateTimestep();
		}
	}

	void Application::SetCurrentScene(Ref<Scene> scene)
	{
		if (m_CurrentScene)
			m_CurrentScene->OnReplaced();
		m_CurrentScene = scene;
		m_CurrentScene->OnSetAsCurrent();
	}

	void Application::UpdateTimestep()
	{
		double currentFrame = glfwGetTime();
		m_Timestep = currentFrame - m_LastFrame;
		m_LastFrame = currentFrame;
	}

	void Application::OnWindowClosedEvent(WindowClosedEvent* event)
	{
		m_IsRunning = false;
	}

	void Application::OnWindowResizedEvent(WindowResizedEvent* event)
	{
		m_Renderer->SetViewport(0, 0, event->Width, event->Height);
	}
}