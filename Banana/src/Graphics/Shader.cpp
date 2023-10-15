#include <Graphics/Log.h>
#include <Graphics/Shader.h>
#include <Graphics/RendererAPI.h>
#include <Graphics/OpenGL/OpenGLShader.h>

namespace Banana
{
    Ref<Shader> Shader::Create(const std::string& vertexPath, const std::string& fragmentPath)
    {
		switch (RendererAPI::GetAPI())
		{
		case RendererAPIName::OpenGL:
			return CreateRef<OpenGLShader>(vertexPath, fragmentPath);
		case RendererAPIName::Vulkan:
			BANANA_ABORT("Vulkan is not supported yet :(");
		case RendererAPIName::None:
		default:
			BANANA_ABORT("Unsupported renderer API {}!", (int)RendererAPI::GetAPI());
		}

		return nullptr;
    }
}